/*****************************************************************//**
 * \file         MEEditor.cpp
 * \author       Cameron Myers
 * \par          Email: cameron.myers\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "MEWindow.h"
#include "MEEditor.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glm/gtx/transform.hpp>

#include <filesystem>
#include <iostream>
#include <fstream>
#include <winsock.h>
#include <string>

#include "Collider.h"
#include "GameObject.h"
#include "MEInput.h"
#include "MEResources.h"
#include "MESpace.h"
#include "MESpaceManager.h"
#include "SpriteSource.h"
#include "Stats.h"
#include "Transform.h"
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include "MERendering.h"

#include "Windows.h"
#include <shellapi.h>
#include "Mesh.h"
#include "Animation.h"
#include "Audio.h"
#include "imgui_internal.h"
#include "Sprite.h"
#include "SpriteSourceManager.h"
#include "MeshManager.h"
#include "Physics.h"


bool isOpen;

std::vector<MESpace*> spaceList;
std::deque<int> list;
MEWindow* window_;
GameObject* selectedObject;
GameObject* object_preview;
std::string dropped_file;
METexture dropped_asset;


METexture anim_draw = 0;
float preview_scale = 1.0f;
glm::mat4 anim_transform;

bool spritesheet_check = false;
int spritesheet_list_index = 0;
std::vector<std::string> spritesheet_list;


//boost::uuids::random_generator_pure gen;

typedef struct tool
{
	bool display;
	int flags;
	std::string name;
	ImVec2 start_pos = ImVec2(50,50);
} tool;


/**
 * \brief this is where tools are defined
 */
tool mousePos = { true, ImGuiWindowFlags_None,  "mouse tool", ImVec2(20.f,5.f)};
tool colliderDraw = { false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoInputs |
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground,"collider" };
tool statsDraw = { false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings,"stats" };
tool objectList = { true, ImGuiWindowFlags_AlwaysVerticalScrollbar|ImGuiWindowFlags_MenuBar, "ObjectListTool", ImVec2(20.f, 75.f)};
tool transformTool = { false, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar, "Transform Editor", ImVec2(1400.f, 20.f) };
tool statsTool = { true, ImGuiWindowFlags_None | ImGuiWindowFlags_AlwaysAutoResize, "Stats Editor" };
tool spriteSourceTool = { true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings, "SpriteSource Importer", ImVec2(850, 400.f) };
tool componentTool = { true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings, "Component Manager", ImVec2(850, 400.f) };
tool spriteTool = { true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar|
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings, "Sprite Editor" , ImVec2(1400.f, 150.f) };
tool animationTool = { true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar|
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings, "Animation Editor" , ImVec2(1400.f, 620.f) };
tool colliderTool = { false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar|
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings, "Collider Editor" , ImVec2(1400.f, 750.f) };

ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
SpriteSource* boxDraw;
SpriteSource* areaDraw;
SpriteSource source;
Mesh mesh;

glm::vec2 preview_pos;
bool preview_toggle = true;
//private functions

void GameObjectListTool(tool* tool, std::vector<MESpace*> spaceList);
void StatsDraw(GameObject* obj);
void CollisionDraw(GameObject* obj);
void displayNode(std::string name, GameObjectManager* objects, ImGuiTreeNodeFlags flags);
void TransformTool(tool* tool);
void StatsTool(tool* tool);
void SpriteSourceImporter(tool* tool);
void ComponentManager(tool* tool);
void SpriteTool(tool* tool);
void AnimationTool(tool* tool);
void ColliderTool(tool* tool);



void get_image_width_height(const char* img, int* width, int* height);
std::string get_image_name(std::string path);
std::string get_image_name_ext(std::string path);
std::vector<std::string> get_all_spritesources_json();
ImVec2 GetConvertedPos(ImVec2 oldPos);

void MEEditor::ResetSelected()
{
	selectedObject = nullptr;
	selectedObject = new GameObject();
	if (object_preview)
	{
		delete object_preview;
	}
	object_preview = new GameObject();
	FreeTexture(&anim_draw);
}

void MEEditor::SetSelectedObject(GameObject* obj)
{
	selectedObject = obj;
	if(object_preview)
	{
		delete object_preview;
	}
	object_preview = new GameObject(obj);
	if(object_preview->HasComponent(cAnimation))
	{
		object_preview->Has(Animation)->SetisLooping(true);
	}
	FreeTexture(&anim_draw);
	anim_draw = 0;
}

/// <summary>
/// initialize the editor
/// </summary>
/// <param name="_window"></param>
void MEEditor::Initialize(MEWindow* _window)
{
	isOpen = false;
	
	ImGuiContext* ctx = ImGui::CreateContext();
	ImGui::SetCurrentContext(ctx);

	ImGuiIO& io = ImGui::GetIO();
	//fill optional fields here
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.DisplaySize = ImVec2((float)MEWindow::GetWidth(), (float)MEWindow::GetHeight());
	io.FontGlobalScale = (float)MEWindow::GetHeight() / 1080.f;

	io.Fonts->AddFontDefault();

	ImFontConfig config;
	config.MergeMode = true;
	config.GlyphMinAdvanceX = 13.0f;
	config.GlyphOffset = ImVec2(0.f, 2.f);
	config.GlyphExtraSpacing = ImVec2(3.f, 0.f);


	window_ = _window;
	//init renderer and platform
	ImGui_ImplGlfw_InitForOpenGL(_window->GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");
	boxDraw = new SpriteSource("BoxColliderDraw");
	areaDraw = new SpriteSource("AreaColliderDraw");

	selectedObject = new GameObject();
	object_preview = new GameObject();
	source = SpriteSource();
	//mesh = Mesh("MainMesh");
	spritesheet_list = get_all_spritesources_json();

	//texture to draw animation to
	//CreateTexture(&anim_draw, 200, 200);

	//gen = boost::uuids::random_generator();
}
/// <summary>
/// update the editor
/// </summary>
/// <param name="dt"></param>
void MEEditor::Update(float dt)
{

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::NewFrame();

	//make sure sandbox object is updated
	object_preview->Update(dt);
	//keep the animation running

	std::vector<std::string> spaces = MESpaceManager::GetSpaceNames();

	std::vector<MESpace*> space_list;

	for(auto space : spaces)
	{
		space_list.push_back(MESpaceManager::GetSpace(space));
	}

	//only runs if editor is open
	if (MEEditor::IsOpen())
	{
		ToolManager(space_list);


		ImGui::SetNextWindowPos(mousePos.start_pos, ImGuiCond_FirstUseEver);
		ImGui::Begin("MousePos", &mousePos.display, mousePos.flags);

		//realtive to glfw not imgui
		if (ImGui::IsMousePosValid())
			ImGui::Text("Mouse pos: (%g, %g)", Input::GetMouseX(), Input::GetMouseY());
		ImGui::Text("ImGui Mouse pos: (%g, %g)", ImGui::GetMousePos().x, ImGui::GetMousePos().y);
		ImGui::End();

	}

	DebugDrawManager(space_list);

	ImGui::EndFrame();

}
/// <summary>
/// render the imgui windows?
/// </summary>
void MEEditor::Render()
{
	if(selectedObject->HasComponent(cAnimation) && IsOpen())
	{
		anim_draw = *MEDrawToTexture(*selectedObject->Has(Sprite)->GetSpriteSource(), *MeshManager::Find("MainMesh"), object_preview->Has(Animation)->getFrameIndex(), anim_transform);

	}
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}
/// <summary>
/// shutdown imgui
/// </summary>
void MEEditor::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	ImGui::DestroyContext();
	spaceList.clear();
	list.clear();

	delete boxDraw;
	delete areaDraw;
	FreeTexture(&dropped_asset);
	FreeTexture(&anim_draw);
	


	boxDraw = nullptr;
	areaDraw = nullptr;
	dropped_asset = NULL;
	anim_draw = NULL;
}

/**
 * \brief returns the current selected object
 * \return a pointer to the selected object
 */
GameObject* MEEditor::GetSelectedObject()
{
	return selectedObject;
}

/// <summary>
/// manage the tools used in the imgui windows
/// </summary>
/// <param name="space_list"></param>
void MEEditor::ToolManager(std::vector<MESpace*> space_list)
{
	GameObjectListTool(&objectList, space_list);
	TransformTool(&transformTool);
	StatsTool(&statsTool);
	SpriteSourceImporter(&spriteSourceTool);
	ComponentManager(&componentTool);
	SpriteTool(&spriteTool);
	AnimationTool(&animationTool);
	ColliderTool(&colliderTool);
}

/**
 * \brief Handles any debug draw features
 * \param spaceList 
 */
void MEEditor::DebugDrawManager(std::vector<MESpace*> spaceList)
{
	for (auto space : spaceList)
	{
		if (space->IsActive())
		{
			GameObjectManager* active_list = space->GetActive();

			list = active_list->GetObjectIDs();

			int i = 0;
			for (auto id : list)
			{
				GameObjectPtr obj = active_list->GetObjectByID(id);
				if (obj)
				{
					StatsDraw(obj);
					CollisionDraw(obj);
				}
				++i;

			}
		}
	}
}

/// <summary>
/// set the open param
/// </summary>
/// <param name="_isOpen"></param>
void MEEditor::SetOpen(bool _isOpen)
{
	isOpen = _isOpen;
}
/// <summary>
/// check if imgui is open
/// </summary>
/// <returns></returns>
bool MEEditor::IsOpen()
{
	return isOpen;
}

void MEEditor::ToggleOpen()
{
	//take this opp to reload the sprite sheets
	//prolly move to a better place eventually
	spritesheet_list = get_all_spritesources_json();

	//ResizeEditor();

	if (IsOpen())
	{
		SetOpen(false);
		//glfwRestoreWindow(window->GetWindow());

	}
	else
	{
		SetOpen(true);
	}
}

void MEEditor::ReceiveDrop(const char** paths, int count)
{
	//get the first path with a valid file format
	//loop through path array
	for (int i = 0; i < count; ++i)
	{
		//check if last 4 is .png
		if (strstr(paths[i], ".png"))
		{
			// if yes, set dropped_file
			dropped_file = std::string(paths[i]);
			//free old asset
			if (dropped_asset != NULL)
			{
				FreeTexture(&dropped_asset);
			}
			
			//load asset
			CreateTextureImage(&dropped_asset, paths[i], source.GetWidth(), source.GetHeight());

			break;
		}
	}

}

/// <summary>
/// Set the window size that ImGui stores
/// </summary>
/// <param name="_size">viewport size from glfw</param>
void MEEditor::SetWindowSize(glm::vec2 _size)
{
	if(ImGui::GetCurrentContext())
	{
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.DisplaySize = ImVec2(_size.x, _size.y);
		io.FontGlobalScale = (_size.y / 1080);
	}
}

/// <summary>
/// helper function to get the position converted to 0,0 at center
/// </summary>
/// <param name="oldPos"></param>
/// <returns></returns>
ImVec2 GetConvertedPos(ImVec2 oldPos)
{
	
	float x, y;
	float winX, winY;

	winX = (float)MEWindow::GetWidth();
	winY = (float)MEWindow::GetHeight();


	winX /= 2.f;
	winY /= 2.f;
	x = oldPos.x + winX;
	y = -oldPos.y + winY;

	ImVec2 vector(x, y);
	return vector;/*returns the mouse x and y position*/
}


//////DEBUG DRAW////////////////////////////////////////

/**
 * \brief draws certain stats for objects with a stats component
 * \param obj 
 */
void StatsDraw(GameObject* obj)
{
	if (obj->HasComponent(cStats))
	{
		if(statsDraw.display)
		{
			if (ImGui::Begin(("stats" + std::to_string(obj->GetID())).c_str(), &statsDraw.display, statsDraw.flags))
			{
				auto transform = obj->Has(Transform);
				auto stats = obj->Has(Stats);
				if (stats)
				{
					glm::vec2 trans = transform->GetTranslation();
					glm::vec2 scale = transform->GetScale();
					ImGui::SetWindowPos(GetConvertedPos(ImVec2(trans.x - (scale.x / 2.f), trans.y + (scale.y / 2.f) + 30.f)));
					ImGui::TextColored(ImVec4(1, 0, 0, 1), "Health: %i", stats->GetHealth());
					ImGui::TextColored(ImVec4(0, 0, 0, 1), "Name: %s", obj->GetName().c_str());

				}
				ImGui::End();
			}
		}
		
	}

}

/**
 * \brief Draws collider boxes for objects with a collider component
 * \param obj
 * \TODO Good for now
 */
void CollisionDraw(GameObject* obj)
{
	if( colliderDraw.display == true)
	{
			
		//does collision draw for children
		for(auto object : obj->GetChildren())
		{
			CollisionDraw(object);
		}

		if (obj->HasComponent(cCollider))
		{
			auto collider = obj->Has(Collider);

			if (collider)
			{

				//box collider draw
				if (collider->GetType() == TypeBox)
				{
					//Set the location of the debug draw
					//auto id = gen();
					ImGui::SetNextWindowPos(GetConvertedPos(ImVec2(collider->getTopLeft().x - 5.f, collider->getTopLeft().y)));

					if (ImGui::Begin(("collider" + std::to_string(obj->GetID()) + obj->GetName()).c_str(), &colliderDraw.display, colliderDraw.flags))
					{
						ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
						ImGui::Image((void*)*boxDraw->GetTexture(), ImVec2(collider->GetBoxWidth(), collider->GetBoxHeight()), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0, 0.2f, 0.2f, 0.3f));
						ImGui::PopItemFlag();
						ImGui::End();
					}
				}
				//Radius/Area Collision draw
				else if (collider->GetType() == TypeArea)
				{
					//get the center of the radius
					//calc top left of the circle as if it is in a square
					auto trans = obj->Has(Transform)->GetTranslation();
					ImGui::SetNextWindowPos(GetConvertedPos(ImVec2(trans.x - collider->GetRadius() - 5.f, trans.y + collider->GetRadius())));

					if (ImGui::Begin(("collider" + obj->GetName()).c_str(), &colliderDraw.display, colliderDraw.flags))
					{
						ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
						ImGui::Image((void*)*areaDraw->GetTexture(), ImVec2(collider->GetRadius() * 2, collider->GetRadius() * 2), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 0.3f));
						ImGui::PopItemFlag();

						ImGui::End();
					}
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////



//////EDITORS////////////////////////////////////////////

/// <summary>
/// helper function for game object list tool
/// </summary>
/// <param name="tool"></param>
/// <param name="space_list"></param>
void GameObjectListTool(tool* tool, std::vector<MESpace*> space_list)
{
	int current = 0;
	//ImGui::ShowDemoWindow(&tool->isActive);
	ImGui::SetNextWindowPos(tool->start_pos, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSizeConstraints(ImVec2(300, 450), ImVec2(500, window_->GetHeight()));
	ImGui::Begin(tool->name.c_str(), &tool->display, tool->flags);
	ImGui::BeginMenuBar();

	//new asset button
	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0, 0));

	//FILE OP MENU
	if(ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Import Asset"))
		{
			spriteSourceTool.display = true;
		}


		//save button
		if (ImGui::MenuItem( "Save"))
		{
			selectedObject->Save();
		}

		//open in json button
		if (ImGui::MenuItem("Open JSON"))
		{
			wchar_t pathName[FILENAME_MAX];
			TCHAR newpath[4096];
			std::string name = selectedObject->GetName();
			std::wstring obj = std::wstring(name.begin(), name.end());
			if (!selectedObject->HasParent())
			{
				wsprintf(pathName, L"../Assets/GameObjects/%s.json", obj.c_str());

			}
			else
			{
				wsprintf(pathName, L"../Assets/GameObjects/Children/%s.json", obj.c_str());

			}

			GetFullPathName(pathName, 4096, newpath, NULL);
			HINSTANCE test = ShellExecute(NULL, NULL, newpath, 0, 0, SW_SHOW);
			std::cout << test << std::endl;
		}
		ImGui::EndMenu();

	}

	//WINDOW MENU DROPDOWN
	if (ImGui::BeginMenu("Window"))
	{
		if (ImGui::MenuItem("Sprite Source Importer"))
		{
			spriteSourceTool.display = true;
		}

		if(ImGui::MenuItem( "Component Manager"))
		{
			componentTool.display = true;
		}

		if (ImGui::MenuItem("Animation Editor"))
		{
			animationTool.display = true;
		}

		if (ImGui::MenuItem("Sprite Editor"))
		{
			spriteTool.display = true;
		}

		if (ImGui::MenuItem("Transform Editor"))
		{
			transformTool.display = true;
		}

		if (ImGui::MenuItem("Collider Editor"))
		{
			colliderTool.display = true;
		}
		ImGui::EndMenu();

	}

	//DEBUG MENU
	if (ImGui::BeginMenu("Debug"))
	{
		ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup, true);



		ImGui::MenuItem("Collider Draw", NULL, &colliderDraw.display);
		ImGui::MenuItem("Stats Draw", NULL, &statsDraw.display);


		ImGui::PopItemFlag();
		ImGui::EndMenu();

	}

	
	/*if (ImGui::Button(ICON_FA_FILE"New", ImVec2(50, 35)))
	{
		spriteSourceTool.display = true;
	}


	//save button
	if (ImGui::Button(ICON_FA_FLOPPY_DISK "Save", ImVec2(60, 35)))
	{
		selectedObject->Save();
	}

	//open in json button
	if (ImGui::Button(ICON_FA_FILE_CODE"Open JSON", ImVec2(115, 35)))
	{
		wchar_t pathName[FILENAME_MAX];
		TCHAR newpath[4096];
#ifdef _DEBUG
		std::string name = selectedObject->GetName();
		std::wstring obj = std::wstring(name.begin(), name.end());
		if (!selectedObject->HasParent())
		{
			wsprintf(pathName, L"../Assets/GameObjects/%s.json", obj.c_str());

		}
		else
		{
			wsprintf(pathName, L"../Assets/GameObjects/Children/%s.json", obj.c_str());

		}

#endif // _DEBUG
		GetFullPathName(pathName, 4096, newpath, NULL);
		HINSTANCE test = ShellExecute(NULL, NULL, newpath, 0, 0, SW_SHOW);
		std::cout << test << std::endl;

	}*/

	ImGui::PopStyleVar();

	ImGui::EndMenuBar();

	ImGuiTreeNodeFlags active_flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	ImGuiTreeNodeFlags inactive_flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick;


	for (auto space : space_list)
	{

		std::string name = *space->GetName();
		name.push_back('>');

		displayNode("Active:" + name, space->GetActive(), active_flags);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
		displayNode("Incactive:" + name, space->GetInactive(), inactive_flags);
		ImGui::PopStyleVar();

	}

	ImGui::End();
}

/// <summary>
/// helper function for transform editing tool
/// </summary>
/// <param name="tool"></param>
void TransformTool(tool* tool)
{
	if(tool->display && selectedObject->HasComponent(cTransform))
	{
		ImGui::SetNextWindowPos(tool->start_pos, ImGuiCond_FirstUseEver);
		if(ImGui::Begin(tool->name.c_str(), &tool->display, tool->flags ))
		{
				
			if(selectedObject->GetName() != "\0")
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0, 0));
				ImGui::BeginMenuBar();
				if (ImGui::Button( "Save", ImVec2(60, 25)))
				{
					selectedObject->Has(Transform)->Save();
				}
				ImGui::EndMenuBar();
				ImGui::PopStyleVar();
				//Get all the transform stuff
				TransformPtr transform = selectedObject->Has(Transform);
				glm::vec3 trans = transform->GetRelativeTran();
				glm::vec3 scale_ = transform->GetScale();
				float rotation = transform->GetRotation();

				//make imgui friendly
				float translation[3] = { trans.x, trans.y, trans.z };
				float scale[3] = { scale_.x, scale_.y, scale_.z };
				float rot = rotation;


				//translation
				ImGui::NewLine();
				ImGui::SameLine(120.f);
				ImGui::PushItemWidth(350.f);
				ImGui::DragFloat3("##1", translation, 0.5f , -2000.f, 2000.f,"%.2f");
				ImGui::SameLine(-350.f, 20.f );
				ImGui::LabelText("Translation", "");

				ImGui::NewLine();

				//scale
				ImGui::SameLine(120.f);
				ImGui::PushItemWidth(350.f);
				ImGui::DragFloat2("##2", scale ,0.5f, -2000, 2000);
				ImGui::SameLine(-350.f, 20.f);
				ImGui::LabelText("Scale", "");

				ImGui::NewLine();

				//rotation
				ImGui::SameLine(120.f);
				ImGui::PushItemWidth(350.f);
				ImGui::DragFloat("##", &rot, 0.5f, -360, 360);
				ImGui::SameLine(-350.f, 20.f);
				ImGui::LabelText("Rotation", "");

				//set everything
				transform->SetTranslation(glm::vec3(translation[0], translation[1], translation[2]));
				transform->SetScale(glm::vec3(scale[0], scale[1], scale[2]));
				transform->SetRotation(rot);
			}
			ImGui::End();
		}

	}

}
/// <summary>
/// helper function for stats editor tool
/// </summary>
/// <param name="tool"></param>
void StatsTool(tool* tool)
{
	if(selectedObject->HasComponent(cStats))
	{
		ImGui::Begin(tool->name.c_str(), &tool->display, tool->flags );
		Stats* stats = selectedObject->Has(Stats);
		ImGui::PushItemWidth(100.f);
		ImGui::LabelText("Health ", "%i", stats->GetHealth());
		ImGui::LabelText("ReloadTimer ", "%f", stats->GetReloadTimer());
		ImGui::LabelText("RespawnTimer ", "%f", stats->GetCurrentRespawnTimer());
		ImGui::LabelText("IsHurt ", stats->GetIsHurt() ? "true" : "false");
		ImGui::LabelText("IsAttacking ", stats->GetIsAttacking() ? "true" : "false");
		ImGui::LabelText("IsDead ", stats->GetIsDead() ? "true" : "false");
		ImGui::LabelText("IsReloading", stats->GetIsReloading() ? "true" : "false");
		ImGui::LabelText("Object Type ", stats->GetObjectType() );
		ImGui::LabelText("Upgrade Level ", "%i", stats->GetLevel());


		ImGui::End();
	}

}

/**
 * \brief This tool imports new sprite sources and auto generates SpriteSource JSON
 * \param tool 
 */
void SpriteSourceImporter(tool* tool)
{
	if(tool->display )
	{

		ImGui::SetNextWindowPos(tool->start_pos, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSizeConstraints(ImVec2(350, 350), ImVec2(600, window_->GetHeight()));
		//base imgui window
		if(!ImGui::Begin(tool->name.c_str(), &tool->display, tool->flags))
		{
			ImGui::End();
		}
		else
		{
			//if a valid file has been grabbed
			if(!dropped_file.empty())
			{

				int x_init = 4 , y_init = 4;

				int width, height;
				get_image_width_height(dropped_file.c_str(), &width, &height);

				//STYLE: Display asset
				ImGui::BeginChildFrame(ImGuiID("src_prev"), ImVec2(590, 500), ImGuiWindowFlags_AlwaysHorizontalScrollbar);
				ImGui::Image((void*)dropped_asset, ImVec2((width/x_init), (height/ y_init)), ImVec2(0.f, 0.f), ImVec2(1.f, 1.f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
				//STYLE: Below asset show the resolution
				ImGui::EndChildFrame();

				ImGui::Text("%i x %i", width, height);
				ImGui::NewLine();


				//Name input
				//show text field

				ImGui::Text("Select from existing names, or type a new one");
				ImGui::NewLine();

				ImGui::SameLine(120.f);
				ImGui::PushItemWidth(350.f);
				if(ImGui::BeginCombo("##2", spritesheet_list[spritesheet_list_index].c_str()))
				{
					


					for(int i = 0; i < spritesheet_list.size(); ++i)
					{
						bool is_selected = (spritesheet_list[spritesheet_list_index] == spritesheet_list[i]);
						if(ImGui::Selectable(spritesheet_list[i].c_str(), is_selected))
						{
							spritesheet_list_index = i;
							source.SetName(spritesheet_list[i]);
						}
						if(is_selected)
						{
							ImGui::SetItemDefaultFocus();
						}

					}

					ImGui::EndCombo();
				}
				if (ImGui::IsItemHovered()) ImGui::SetTooltip("Names of already generated JSON");
				ImGui::SameLine(-350.f, 20.f);
				ImGui::LabelText("Existing Name", "");


				ImGui::NewLine();

				std::string buf = source.GetName();

				ImGui::SameLine(120.f);
				ImGui::PushItemWidth(350.f);
				if(ImGui::InputText("##3", &buf))
				{
					spritesheet_list_index = 0;
				}
				if (ImGui::IsItemHovered()) ImGui::SetTooltip("Name that would be used in the Sprite Component");
				ImGui::SameLine(-350.f, 20.f);
				ImGui::LabelText("Name", "");

				source.SetName(buf);

				ImGui::NewLine();

				//check if it is a sprite sheet
				if (source.GetNumRows() >= 2 || source.GetNumCols() >= 2) spritesheet_check = true;

				//Check box for sprite sheet or single asset
				ImGui::SameLine(120.f);
				ImGui::PushItemWidth(350.f);
				ImGui::Checkbox("##4", &spritesheet_check);
				if (ImGui::IsItemHovered()) ImGui::SetTooltip("Select if the asset you are importing is a Sprite Sheet");
				ImGui::SameLine(-350.f, 20.f);
				ImGui::LabelText("SpriteSheet", "");



				//if check show fields for sprite data
				if(spritesheet_check)
				{
					//STYLE: Fill in below fields
					ImGui::Text("Fill in below fields");
					ImGui::NewLine();

					//STYLE: Rows int field
					int spritesheet_rows = source.GetNumRows();
					ImGui::SameLine(120.f);
					ImGui::PushItemWidth(150.f);
					ImGui::InputInt("##5", &spritesheet_rows);
					ImGui::SameLine(-150.f, 20.f);
					ImGui::LabelText("Rows", "");
					source.SetNumRows(spritesheet_rows);

					ImGui::NewLine();


					//STYLE: Columns int field
					int spritesheet_cols = source.GetNumCols();
					ImGui::SameLine(120.f);
					ImGui::PushItemWidth(150.f);
					ImGui::InputInt("##6", &spritesheet_cols);
					ImGui::SameLine(-150.f, 20.f);
					ImGui::LabelText("Columns", "");
					source.SetNumCols(spritesheet_cols);


				}

				ImGui::NewLine();
				ImGui::NewLine();

				ImGui::SameLine(120.f);

				if(ImGui::Button("Clear", ImVec2(70, 20)))
				{
					//the texture will be free'd when a new one is created and at shutdown

					//clear the path string
					dropped_file.clear();

				}
				
				ImGui::SameLine(220.f);
				if(ImGui::Button("Import", ImVec2(70, 20)))
				{
					//Generate the Sprite Source
					
					//get the path of the file
					source.SetPath(get_image_name_ext(dropped_file));

					//generate the json
					source.Save();

					//check for edit of existing json
					//if yes reload source
					if(spritesheet_list_index != 0)
					{
						SpriteSourceManager::ReBuild(source.GetName());

					}
					//update the list
					spritesheet_list = get_all_spritesources_json();

				}
				if (ImGui::IsItemHovered()) ImGui::SetTooltip("Generate the Sprite Source JSON");


			}
			//no file :(
			else
			{
				//STYLE: Drag asset here to import
				ImGui::Text("Drag Asset here to Import");

			}

			ImGui::End();
		}

	}
	
}

/**
 * \brief Tool to allow the user to manage what components exist on a game object
 * \param tool 
 */
void ComponentManager(tool* tool)
{
	if (tool->display)
	{

		ImGui::SetNextWindowPos(tool->start_pos, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSizeConstraints(ImVec2(350, 350), ImVec2(600.f, (float)window_->GetHeight()));
		//base imgui window
		if (ImGui::Begin(tool->name.c_str(), &tool->display, tool->flags))
		{
			ImGui::Text((selectedObject->GetName() + "'s Components").c_str());
			ImGui::NewLine();

			//display a list of existing components
			if(ImGui::BeginListBox("##1",ImVec2(300, 200)))
			{
				

				if (selectedObject->HasComponent(cTransform))
				{
					ImGui::Selectable("Transform Component");
				}
				if (selectedObject->HasComponent(cSprite))
				{
					ImGui::Selectable("Sprite Component");
				}
				if (selectedObject->HasComponent(cAnimation))
				{
					ImGui::Selectable("Animation Component");
				}
				if (selectedObject->HasComponent(cPhysics))
				{
					ImGui::Selectable("Physics Component");
				}
				if (selectedObject->HasComponent(cCollider))
				{
					ImGui::Selectable("Collider Component");
				}

				if (selectedObject->HasComponent(cAudio))
				{
					ImGui::Selectable("Audio Component");
				}

				if (selectedObject->HasComponent(cBehavior))
				{
					ImGui::Selectable("Behavior Component");

				}
				if (selectedObject->HasComponent(cStats))
				{
					ImGui::Selectable("Stats Component");
				}
				if (selectedObject->HasComponent(cEmitter))
				{
					ImGui::Selectable("Emitter Component");

				}
				if (selectedObject->HasComponent(cEmitterBehavior))
				{
					ImGui::Selectable("Emitter Behavior Component");

				}
				ImGui::EndListBox();
			}



			/*
			const char* components[10];
			components[0] = "Transform";
			components[1] = "Sprite";
			components[2] = "Animation";
			components[3] = "Physics";
			components[4] = "Collider";
			components[5] = "Audio";
			components[6] = "Behavior";
			components[7] = "Stats";
			components[8] = "Emitter";
			components[9] = "EmitterBehavior";

			std::vector<TypeEnum> supported_components = {cSprite, cAnimation, cPhysics, cCollider, cAudio};

			//combo that list all add-able and removeable components
			ImGui::Combo("Add/Remove Components", &selected_component, components, 10);

			//plus button for add   //minus button for remove
			//on add/remove, check if the component already exists

			if(ImGui::Button(ICON_FA_PLUS, ImVec2(30, 30)))
			{
				//check if supported
				if(std::find(supported_components.begin(), supported_components.end(), (TypeEnum)selected_component) != supported_components.end())
				{
					switch (selected_component)
					{
					case cSprite:
						selectedObject->Add(new Sprite());

						break;
					case cAnimation:
						selectedObject->Add(new Animation());
						break;

					case cPhysics:
						selectedObject->Add(new Physics());
						selectedObject->Has(Physics)->Save();
						break;

					case cCollider:
						selectedObject->Add(new Collider());
						break;

					case cAudio:
						selectedObject->Add(new Audio());
						selectedObject->Has(Audio)->Save();
						break;
					}
					delete object_preview;
					object_preview = new GameObject(selectedObject);

				}
				//not supported
				else
				{
					ImGui::BeginPopupModal(ICON_FA_TRIANGLE_EXCLAMATION "NOT SUPPORTED", NULL, ImGuiWindowFlags_AlwaysAutoResize);
					ImGui::Text("This component cannot be added/removed from this editor\n Please open in JSON");
					ImGui::Separator();

					//OPEN IN JSON
					if (ImGui::Button(ICON_FA_FILE_CODE"Open JSON", ImVec2(50, 30)))
					{
						wchar_t pathName[FILENAME_MAX];
						TCHAR newpath[4096];
						std::string name = selectedObject->GetName();
						std::wstring obj = std::wstring(name.begin(), name.end());
						if (!selectedObject->HasParent())
						{
							wsprintf(pathName, L"../Assets/GameObjects/%s.json", obj.c_str());

						}
						else
						{
							wsprintf(pathName, L"../Assets/GameObjects/Children/%s.json", obj.c_str());

						}

						GetFullPathName(pathName, 4096, newpath, NULL);
						HINSTANCE test = ShellExecute(NULL, NULL, newpath, 0, 0, SW_SHOW);
						std::cout << test << std::endl;
					}
					ImGui::SetItemDefaultFocus();
					if (ImGui::Button(ICON_FA_XMARK"Cancel", ImVec2(50,30)))
					{
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
			}

			if(ImGui::Button(ICON_FA_MINUS, ImVec2(30, 30)))
			{
				//check if supported
				if (std::find(supported_components.begin(), supported_components.end(), (TypeEnum)selected_component) != supported_components.end())
				{

				}
				//not supported
				else
				{
					ImGui::BeginPopupModal(ICON_FA_TRIANGLE_EXCLAMATION "NOT SUPPORTED", NULL, ImGuiWindowFlags_AlwaysAutoResize);
					ImGui::Text("This component cannot be added/removed from this editor\n Please open in JSON");
					ImGui::Separator();

					//OPEN IN JSON
					if (ImGui::Button(ICON_FA_FILE_CODE"Open JSON", ImVec2(50, 30)))
					{
						wchar_t pathName[FILENAME_MAX];
						TCHAR newpath[4096];
						std::string name = selectedObject->GetName();
						std::wstring obj = std::wstring(name.begin(), name.end());
						if (!selectedObject->HasParent())
						{
							wsprintf(pathName, L"../Assets/GameObjects/%s.json", obj.c_str());

						}
						else
						{
							wsprintf(pathName, L"../Assets/GameObjects/Children/%s.json", obj.c_str());

						}

						GetFullPathName(pathName, 4096, newpath, NULL);
						HINSTANCE test = ShellExecute(NULL, NULL, newpath, 0, 0, SW_SHOW);
						std::cout << test << std::endl;
					}
					ImGui::SetItemDefaultFocus();
					if (ImGui::Button(ICON_FA_XMARK"Cancel", ImVec2(50, 30)))
					{
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
			}
			*/


			ImGui::End();

		}

	}
}

/**
 * \brief This tool allows you to edit the Sprite component of an object
 * \param tool 
 */
void SpriteTool(tool* tool)
{
	if (tool->display && selectedObject->HasComponent(cSprite))
	{
		ImGui::SetNextWindowPos(tool->start_pos, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSizeConstraints(ImVec2(220, 350), ImVec2(500, window_->GetHeight()));
		//base imgui window
		if (!ImGui::Begin(tool->name.c_str(), &tool->display, tool->flags))
		{
			ImGui::End();
		}
		else
		{
			ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0, 0));
			ImGui::BeginMenuBar();
			if (ImGui::Button("Save", ImVec2(60, 25)))
			{
				selectedObject->Has(Sprite)->Save();
			}
			ImGui::EndMenuBar();
			ImGui::PopStyleVar();

			Sprite* sprite = selectedObject->Has(Sprite);
			SpriteSource* src = sprite->GetSpriteSource();
			if (src != NULL)
			{
				//render the current sprite
				ImGui::Image((void*)*src->GetTexture(), ImVec2(300, 300), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.f, 1.f, 1.f, 1.f));
				//show fields for the sprite properties

			}
			
			ImGui::NewLine();


			//ALPHA
			float alpha = sprite->GetAlpha();

			ImGui::SameLine(120.f);
			ImGui::PushItemWidth(350.f);
			ImGui::DragFloat("##1", &alpha, 0.01f, 0.0f, 1.0f);
			ImGui::SameLine(-350.f, 20.f);
			ImGui::LabelText("Alpha", "");
			sprite->SetAlpha(alpha);

			ImGui::NewLine();


			//RGB
			glm::vec3 rgb = sprite->GetRGB();
			float _rgb[3] = { rgb.x, rgb.y, rgb.z };

			ImGui::SameLine(120.f);
			ImGui::PushItemWidth(350.f);
			ImGui::DragFloat3("##2", _rgb, 0.2f,0.0f,10.0f);
			if (ImGui::IsItemHovered())ImGui::SetTooltip("Doesn't save");
			ImGui::SameLine(-350.f, 20.f);
			ImGui::LabelText("RGB Values", "");
			sprite->SetRGB(glm::vec3(_rgb[0], _rgb[1], _rgb[2]));

			ImGui::NewLine();


			//FRAME INDEX
			int frame = sprite->GetFrame();

			ImGui::SameLine(120.f);
			ImGui::PushItemWidth(350.f);
			ImGui::Text("%i", frame);
			ImGui::SameLine(-350.f, 20.f);
			ImGui::LabelText("Frame Index", "");

			ImGui::NewLine();
			//change sprite source button

			ImGui::SameLine(170.f);
			ImGui::PushItemWidth(300.f);
			if (ImGui::BeginCombo("##6", spritesheet_list[spritesheet_list_index].c_str()))
			{



				for (int i = 0; i < spritesheet_list.size(); ++i)
				{
					bool is_selected = (spritesheet_list[spritesheet_list_index] == spritesheet_list[i]);
					if (ImGui::Selectable(spritesheet_list[i].c_str(), is_selected))
					{
						spritesheet_list_index = i;
						if (i != 0)
						{
							sprite->SetSpriteSource(SpriteSourceManager::ReBuild(spritesheet_list[i]));
						}
					}
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}

				}

				ImGui::EndCombo();
			}
			ImGui::SameLine(-300.f, 20.f);
			ImGui::LabelText("Sprite Source", "");

			ImGui::NewLine();


			//TEXT MODE
			//get the current mode
			char* mode = sprite->GetTextMode();
			char* modes[3];
			//options array
			modes[0] = (char*)"None";
			modes[1] = (char*)"Right";
			modes[2] = (char*)"Center";

			//find which mode we are currently in
			int index = 0;
			for (int i = 0; i < 3; ++i)
			{
				if (!strcmp(mode, modes[i]))
				{
					index = i;
					break;
				}
			}

			//display
			ImGui::SameLine(370.f);
			ImGui::PushItemWidth(100.f);
			ImGui::Combo("##4",&index,modes, 3);
			ImGui::SameLine(-100.f, 20.f);
			ImGui::LabelText("TextMode", "");

			//update the mode
			sprite->SetTextMode(modes[index]);

			ImGui::NewLine();

			//SPRITE TEXT
			//if text mode
			if(index != 0)
			{
				//show text field
				std::string buf = sprite->GetText();

				ImGui::SameLine(120.f);
				ImGui::PushItemWidth(350.f);
				ImGui::InputText("##5", &buf);
				ImGui::SameLine(-350.f, 20.f);
				ImGui::LabelText("Sprite Text", "");

				std::string newtext = buf;
				sprite->SetText(newtext);
			}



			
			//make dropdown that lists all avaliable
			//HARD PART

			//either use drag drop again but with a json
			//or populate the dropdown with all json in the art folder probably using filesystem
			ImGui::End();

		}
	
	}

}

/**
 * \brief This tool allows you to edit the Animation component of an object
 * \param tool
 */
void AnimationTool(tool* tool)
{

	if (tool->display && selectedObject->Has(Animation))
	{

		Animation* animation = object_preview->Has(Animation);

		ImGui::SetNextWindowPos(tool->start_pos, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSizeConstraints(ImVec2(220, 350), ImVec2(600.f, (float)window_->GetHeight()));
		//base imgui window
		if (!ImGui::Begin(tool->name.c_str(), &tool->display, tool->flags))
		{
			ImGui::End();
		}
		else
		{
			ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0, 0));
			ImGui::BeginMenuBar();
			if (ImGui::Button( "Save", ImVec2(60, 25)))
			{
				selectedObject->Has(Animation)->Save();
			}
			ImGui::EndMenuBar();
			ImGui::PopStyleVar();

			//only for properties not for preview
			Animation* anim = selectedObject->Has(Animation);

			SpriteSource* sprite_src = selectedObject->Has(Sprite)->GetSpriteSource();

			//should be the same size as the temp buffer's texture
			float frame_width = (float)*sprite_src->GetWidth() / (float)sprite_src->GetNumCols();
			float frame_height = (float)*sprite_src->GetHeight() / (float)sprite_src->GetNumRows();
			/*float uSize = 1.0f / sprite_src->GetNumCols();
			float vSize = 1.0f / sprite_src->GetNumRows();*/

			ImGui::Text("x: %f", frame_width);
			ImGui::Text("y: %f", frame_height);
		





			/////////////////////
			///check for 3 different frame sizes
			
			//256x256
			//-835,-385
			if (glm::distance(256.f, frame_width) <= 25.f)
			{
				preview_pos = glm::vec2(-835.f, -385.f);
				preview_toggle = true;

			}
			//512x512
			//pos: -785,-285
			else if (glm::distance(512.f, frame_width) <= 25.f)
			{
				preview_pos = glm::vec2(-710.f, -285.f);
				preview_toggle = true;

			}
			//2048x2048
			//0,0
			else if (glm::distance(2048.f, frame_width) <= 25.f)
			{
				preview_pos = glm::vec2(0.f, -85.f);
				preview_toggle = true;

			}
			//not supported
			else
			{
				preview_pos = glm::vec2(0.f, 0.f);
				preview_toggle = false;
			}
			/////////////////////


			glm::mat4 scaleMatrix = glm::scale(glm::vec3(frame_width, -frame_height, 1.f));
			glm::mat4 rotationMatrix = glm::rotate(glm::radians(0.f), glm::vec3(0, 0, 1));
			//create translation matrix from translate.x and translate.y
			glm::mat4 translateMatrix = glm::translate(glm::vec3(preview_pos.x, preview_pos.y, 1.f));
			anim_transform = translateMatrix * rotationMatrix * scaleMatrix;

			//check if preview is supported
			if (preview_toggle)
			{

				//draw preview
			ImGui::BeginChild("AnimationPreview", ImVec2(512.f, 512.f), true, ImGuiWindowFlags_AlwaysHorizontalScrollbar);
			ImGui::Image((void*)anim_draw, ImVec2(frame_width * preview_scale, frame_height * preview_scale));
			ImGui::EndChild();

			//draw scale for the preview
			ImGui::NewLine();
			ImGui::SameLine(135.f);
			ImGui::PushItemWidth(200.f);
			ImGui::DragFloat("##1", &preview_scale, 0.025f, 0.0f);
			ImGui::SameLine(-200.f, 20.f);
			ImGui::LabelText("Preview Scale", "");
			}
			else
			{
				ImGui::Text("NO PREVIEW - FRAME SIZE NOT SUPPORTED");

			}
			ImGui::NewLine();


			//Play Pause Button
			ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.55f, 0.55f));

			ImGui::SameLine(25.f);
			ImGui::Text("Play/Pause");

			 
			if(animation->getIsRunning() == false)
			{
				//show pause button
				ImGui::SameLine(135.f);

				if(ImGui::Button("PLAY", ImVec2(30, 30)))
				{
					animation->SetIsRunning(true);
				}

			}
			else
			{
				//show play button
				ImGui::SameLine(135.f);
				if(ImGui::Button("PAUSE", ImVec2(30, 30)))
				{
					animation->SetIsRunning(false);
				}
				

			}
			ImGui::PopStyleVar();

			ImGui::NewLine();

			//frameindex
			ImGui::Text("Frame Index: %d", animation->getFrameIndex());

			ImGui::NewLine();

			//Frame Count
			int frame_count = anim->getFrameCount();

			ImGui::SameLine(135.f);
			ImGui::PushItemWidth(200.f);
			ImGui::DragInt("##3", &frame_count, 1.0f, 20000);
			ImGui::SameLine(-200.f, 20.f);
			ImGui::LabelText("Frame Count", "");
			anim->SetFrameCount(frame_count);
			animation->SetFrameCount(frame_count);
			ImGui::NewLine();
			

			//Frame Duration
			float durr = anim->getFrameDuration();

			ImGui::SameLine(135.f);
			ImGui::PushItemWidth(200.f);
			ImGui::DragFloat("##4", &durr, 0.005f, 0.0f, 10.0f);
			if (ImGui::IsItemHovered())ImGui::SetTooltip("How long before the next frame");
			ImGui::SameLine(-200.f, 20.f);
			ImGui::LabelText("Frame Duration", "");
			anim->SetFrameDuration(durr);
			animation->SetFrameDuration(durr);

			ImGui::NewLine();


			//Looping
			bool loop = anim->isLooping();

			ImGui::SameLine(135.f);
			ImGui::Checkbox("##5", &loop);
			if (ImGui::IsItemHovered())ImGui::SetTooltip("Does the animation loop");
			ImGui::SameLine(-200.f, 20.f);
			ImGui::LabelText("Loop Animation", "");
			anim->SetisLooping(loop);

			ImGui::NewLine();

			bool diff = anim->isDiffAnimations();

			ImGui::SameLine(135.f);
			ImGui::Checkbox("##6", &diff);
			if (ImGui::IsItemHovered())ImGui::SetTooltip("Are animations grouped by row");
			ImGui::SameLine(-200.f, 20.f);
			ImGui::LabelText("Diff Animations", "");
			anim->SetisDiffAnimations(diff);
			animation->SetisDiffAnimations(diff);

			ImGui::End();
		}

	}

}

/**
 * \brief This tool allows you to edit the Collider component of an object
 * \param tool
 */
void ColliderTool(tool* tool)
{
	if (tool->display && selectedObject->HasComponent(cCollider))
	{
		ImGui::SetNextWindowPos(tool->start_pos, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSizeConstraints(ImVec2(220, 130), ImVec2(500, window_->GetHeight()));
		//base imgui window
		if (!ImGui::Begin(tool->name.c_str(), &tool->display, tool->flags))
		{
			ImGui::End();
		}
		else
		{
			auto collider = selectedObject->Has(Collider);

			ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0, 0));
			ImGui::BeginMenuBar();
			if (ImGui::Button("Save", ImVec2(60, 25)))
			{
				selectedObject->Has(Collider)->Save();
			}
			ImGui::EndMenuBar();
			ImGui::PopStyleVar();

			//collider type to string

			char* collider_types[3];
			//options array
			collider_types[0] = (char*)"None";
			collider_types[1] = (char*)"Area";
			collider_types[2] = (char*)"Box";

			int collider_type = collider->GetType();

			

			//Collider type dropdown
			ImGui::Text("Collider Type:");
			ImGui::SameLine(120.f);
			ImGui::PushItemWidth(150.f);
			ImGui::Combo("##1", &collider_type, collider_types, 3);

			collider->SetType((ColliderType)collider_type);

			ImGui::NewLine();
			if(collider_type == TypeArea)
			{
				float rad = collider->GetRadiusMod();

				ImGui::Text("Radius:");
				ImGui::SameLine(120.f);
				ImGui::PushItemWidth(150.f);
				ImGui::DragFloat("##2", &rad, 0.05f, 0.f);

				collider->SetRadiusMod(rad);
			}
			else if(collider_type == TypeBox)
			{
				float xy[2];
				xy[0] = collider->GetWidthMod();
				xy[1] = collider->GetHeightMod();

				ImGui::Text("Box Size:");
				ImGui::SameLine(120.f);
				ImGui::PushItemWidth(150.f);
				ImGui::DragFloat2("##2", xy , 0.05f, 0.f);

				collider->SetBoxSize(glm::vec2(xy[0], xy[1]));
			}

			ImGui::End();
		}

	}
}


/////////////////////////////////////////////////////////



//////HELPERS////////////////////////////////////////////
/**
 * \brief creates a treeNode for the gameobject list
 * \param name Name of the node
 * \param objects the object manager for the tree
 * \param flags any imgui flags to associate
 */
void displayNode(std::string name, GameObjectManager* objects, ImGuiTreeNodeFlags flags)
{
	//check if space
	if (name.at(name.size() - 1) == '>')
	{
		//remove flag
		name.pop_back();

		bool open = ImGui::TreeNodeEx(name.c_str(), flags);
		if (open)
		{
			for (int child_n = 0; child_n < objects->GetObjectCount(); child_n++)
			{
				displayNode((*objects)[child_n]->GetName(), objects, flags);
			}
			ImGui::TreePop();
		}
	}
	//game object
	else
	{
		//does this gameobject have children
		if (objects->GetObjectByName(name)->IsParent())
		{
			ImGuiTreeNodeFlags temp = flags;
			if (selectedObject->IsNamed(name)) flags |= ImGuiTreeNodeFlags_Selected;
			bool open = ImGui::TreeNodeEx(name.c_str(), flags);
			if (open)
			{
				if (ImGui::IsItemClicked())
				{
					MEEditor::SetSelectedObject(objects->GetObjectByName(name));


					//update the index to current src
					std::string src_name = selectedObject->Has(Sprite)->GetSpriteSource()->GetName();

					for(int i = 0; i < spritesheet_list.size(); ++i)
					{
						if (spritesheet_list[i] == src_name)
						{
							spritesheet_list_index = i;
							break;
						}
					}

					//update the animation
					/*if (selectedObject->HasComponent(cAnimation))
					{
						animation_tool_sequence = Animation(*selectedObject->Has(Animation));


					}*/
				}
				int childrenCount = objects->GetObjectByName(name)->GetChildCount();
				//object_manager = temp.get
				for (int child_n = 0; child_n < childrenCount; child_n++)
				{
					GameObjectManager children = GameObjectManager(objects->GetObjectByName(name)->GetChildren());
					displayNode(children[child_n]->GetName(), &children, temp);
				}
				ImGui::TreePop();
			}

		}
		else
		{

			if (selectedObject->IsNamed(name)) flags |= ImGuiTreeNodeFlags_Selected;
			bool open = ImGui::TreeNodeEx(name.c_str(), flags);
			if (ImGui::IsItemClicked())
			{
				MEEditor::SetSelectedObject(objects->GetObjectByName(name));
				if(selectedObject->HasComponent(cSprite))
				{
					//update the index to current src
					std::string src_name = selectedObject->Has(Sprite)->GetSpriteSource()->GetName();

					for (int i = 0; i < spritesheet_list.size(); ++i)
					{
						if (spritesheet_list[i] == src_name)
						{
							spritesheet_list_index = i;
							break;
						}
					}
				}


			}
			if (open)
			{
				ImGui::TreePop();
			}


		}

	}

}

void get_image_width_height(const char* img, int* width, int* height)
{
	std::ifstream in(img);

	in.seekg(16);
	in.read((char*)width, 4);
	in.read((char*)height, 4);
#ifdef _DEBUG 
	*width = ntohl(*width);
	*height = ntohl(*height);
#endif

}

std::string get_image_name(std::string path)
{
	std::string filenameext = get_image_name_ext(path);
	std::string::size_type const p(filenameext.find_last_of('.'));
	return filenameext.substr(0, p);
	
}

std::string get_image_name_ext(std::string path)
{
	return path.substr(path.find_last_of("/\\") + 1);
}

std::vector<std::string> get_all_spritesources_json()
{
	std::string path = "../Assets/Art";
	std::vector<std::string> source_list;
	source_list.push_back(" ");
	
	std::filesystem::directory_iterator it = std::filesystem::directory_iterator(path);

	for (std::filesystem::directory_iterator i = begin(it); i != end(it); ++i )
	{
		//check if its a JSON
		if(i->path().extension() == ".json")
		{
			std::string temp = i->path().filename().string();
			source_list.push_back(temp.substr(0, temp.length() - 5));
			//add the json to the list
		}
	}
	return source_list;

}
/////////////////////////////////////////////////////////
