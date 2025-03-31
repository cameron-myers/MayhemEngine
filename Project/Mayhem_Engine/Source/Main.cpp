#pragma once
/*********************************************************************************************************************/
/*!
  \file              Main.cpp
  \author            Cameron Myers // Nate White
  \par               Email: cameron.myers\@digipen.edu, nate.white\@digipen.edu
  \par               Course: GAM200
  \par               Copyright � 2022 DigiPen, All rights reserved.

  \brief
	
    Joey de Vries LearnOpenGL:
    https://learnopengl.com/.

    The following libraries were not created by DigiPen but are licensed for free use:
    - GLAD: https://github.com/Dav1dde/glad/tree/glad2
    - GLFW: https://www.glfw.org/

 */
 /*********************************************************************************************************************/


/*********************************************************************************************************************/
/* Includes */
/*********************************************************************************************************************/
// Makes sure Windows.h brings less forward declarations than necessary really.
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Engine.h>
#undef CreateWindow
#include <glad/glad.h>
#include <glfw/glfw3.h> 
#include "MERand.h"
#include <iostream>             // std::cerr, std:::endl
#include <utility>
#include "OpenGLError.h"
#include <glm/gtx/transform.hpp>
#include "MEFramerateController.h"
#include "Log.h"
#include "MERendering.h"
#include "MEWindow.h"
#include "MEInput.h"
#include "shellapi.h"
#include <vector>

typedef struct GLFWwindow GLFWwindow;
typedef GLFWwindow* GLFWwindowPtr;
STICKYKEYS g_StartupStickyKeys = { sizeof(STICKYKEYS), 0 };
void AllowAccessibilityShortcutKeys(bool bAllowKeys);
void parse_args(PWSTR pCmdLine, int argCount);
std::string from_Wchar_to_Str(wchar_t* in);
bool headless = false;

std::vector<std::string> arguments;
std::vector<std::string> arg_values;
/*!********************************************************************************************************************
	\par this is from the graphics quickstart
  \brief
    Marks a variable as unreferenced for the compiler.
    Avoids warnings if they are turned on as errors.
  \param x
    The variable to marked as unreferenced.
**********************************************************************************************************************/

#define UNREFERENCED(x) (void)x




/*!
  \brief
    Defines the entry point for the application.
  \param instance
    The OS-specific handle to the executable.
  \param prevInstance
    Essentially unused value that was used during the Windows 16-bit days (according to official documentation).
  \param pCmdLine
    Contains the command line arguments in Unicode.
  \param cmdShow
    A flag indicating the window will be minimized, maximized, or shown as normal.
  \returns
    A return code used by the operating system.
*/
int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, PWSTR pCmdLine, int cmdShow)
{
	UNREFERENCED(instance);
    UNREFERENCED(prevInstance);
	UNREFERENCED(cmdShow);

#ifdef _DEBUG
    //THIS opens a console window to print to
    //prolly the bad way but it works for now
    FILE* fpstdin = stdin, * fpstdout = stdout, * fpstderr = stderr;

    AllocConsole();
    freopen_s(&fpstdin, "CONIN$", "r", stdin);
    freopen_s(&fpstdout, "CONOUT$", "w", stdout);
    freopen_s(&fpstderr, "CONOUT$", "w", stderr);
    std::cout << "Welcome to Mayhem Engine" << std::endl;

#endif

    // Save the current sticky/toggle/filter key settings so they can be restored them later
    SystemParametersInfo(SPI_GETSTICKYKEYS, sizeof(STICKYKEYS), &g_StartupStickyKeys, 0);

    //disable sticky keys
    AllowAccessibilityShortcutKeys(false);
    int argCount = 0;

	parse_args(pCmdLine, argCount);


    //create a new window
    MEWindow* Window = new MEWindow((char*)"Mayhem_Engine");
    if(!Window)
    {
        assert("Failed to Create a window");
        return -1;
    }
	Input::GrabWindow(Window);

    //create the engine
    Engine* engine = new Engine(Window);

    if (arguments.size() > 1)
    {
        //get args into map
        for (size_t i = 0; i < arguments.size(); ++i)
        {
            std::string temp = arguments[i].substr(1, arguments[i].length() - 1);

            Engine::s_TestingArgs[temp] = arg_values[i].c_str();
        }

        //functesting == "1"
        if (strcmp(Engine::s_TestingArgs["FuncTesting"], "1") == 0)
        {
            std::cout << "The Engine will now boot in testing mode" << std::endl;
            headless = true;
            engine->isHeadless = true;

        }
    }
    
    if (!engine)
    {
        assert("Failed to initialize the Engine");
        return -1;
    }
    engine->Initialize();


    
    //main render loop
    int framecount = 0;
    float deetee = 0;
    float deetee2 = 0;

	while (!glfwWindowShouldClose(Window->GetWindow()))
	{
        deetee = MEFramerateController::GetFrameTime();
        int width, height;
        glfwGetWindowSize(Window->GetWindow(), &width, &height);
        Window->SetSize(glm::vec2(width, height));
        //update the engine
        engine->Update(deetee);
        //render the engine
        engine->Render();

        glfwPollEvents();
        //temp code for frame count
        framecount += 1;
        deetee2 += deetee;
        if (deetee2 >= 0.11)
        {
            int fps = framecount * 10;
            
            char buffer[256];
            memset(buffer, '\0', sizeof(buffer));
            int x = 
                (buffer, sizeof(buffer), "FPS:%d", fps);
            MEFramerateController::SetFPS(fps);
            glfwSetWindowTitle(Window->GetWindow(), "Zeppelin Rush");
            deetee2 = 0;
            framecount = 0;
        }
	}
    Log::Free();
    delete engine;
    delete Window;
    //re-enable sticky keys
    AllowAccessibilityShortcutKeys(true);

#ifdef _DEBUG
	FreeConsole();
#endif
    return 0;
}


/**
 * \brief FROM MICROSOFT DOC
 * \link https://learn.microsoft.com/en-us/windows/win32/dxtecharts/disabling-shortcut-keys-in-games
 * \param bAllowKeys enable/disable
 */
void AllowAccessibilityShortcutKeys(bool bAllowKeys)
{
    if (bAllowKeys)
    {
        // Restore StickyKeys/etc to original state and enable Windows key      
        STICKYKEYS sk = g_StartupStickyKeys;


        SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &g_StartupStickyKeys, 0);

    }
    else
    {
        // Disable StickyKeys/etc shortcuts but if the accessibility feature is on, 
        // then leave the settings alone as its probably being usefully used

        STICKYKEYS skOff = g_StartupStickyKeys;
        if ((skOff.dwFlags & SKF_STICKYKEYSON) == 0)
        {
            // Disable the hotkey and the confirmation
            skOff.dwFlags &= ~SKF_HOTKEYACTIVE;
            skOff.dwFlags &= ~SKF_CONFIRMHOTKEY;

            SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &skOff, 0);
        }

        
    }
}

std::string from_Wchar_to_Str(wchar_t* in)
{
    std::wstring ws(in);
    std::string str(ws.begin(), ws.end());
    return  str;
}

void parse_args(PWSTR pCmdLine, int argCount)
{
    LPWSTR* args = CommandLineToArgvW(pCmdLine, &argCount);

    for (int i = 0; i < argCount; ++i)
    {
        //even
        if (i % 2 == 0)
        {
            arguments.push_back(from_Wchar_to_Str(args[i]));
        }
        else
        {
            arg_values.push_back(from_Wchar_to_Str(args[i]));
        }
    }

}