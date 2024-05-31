/*********************************************************************
 * \file         MEFunctionalTesting.h
 * \author       Cameron Myers
 * \brief		 
 *
 *********************************************************************/

#pragma once
#include <map>
#include <stack>
#include <string>
#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>

#include "MEFramerateController.h"
#include "MESerializer.h"

class MEFunctionalTest;

enum TestStatus
{
	Invalid = -1,
	Passed = 0,
	Failed,
	Running,
	Aborted,
};

class MEFunctionalTest
{

public:
	MEFunctionalTest() = default;
	MEFunctionalTest(const char* name):m_Name(name),m_Status(Invalid){};
	MEFunctionalTest(const MEFunctionalTest& rhs)
	{
		//name of the test case
		m_Name = rhs.m_Name;
		//description of the test [OPTIONAL]
		m_Description = rhs.m_Description;
		//test status
		m_Status = rhs.m_Status;
		//test output, errors, results
		m_Output = rhs.m_Output;
		//duration of the test
		m_Duration = rhs.m_Duration;

		m_Result = rhs.m_Result;
		m_IntendedResult = rhs.m_IntendedResult;

		m_start_time = rhs.m_start_time;

		m_json_buffer = rhs.m_json_buffer;

	}
	virtual void Init()
	{
		m_Status = Running;
		m_start_time = MEFramerateController::GetTimeElapsed();
	};
	virtual std::string Read()
	{
		//do essential reading here, then do extra reading in derived class
		
			std::string json;
			rapidjson::Document doc;
			std::string clearData(json);
			std::string path = "../Tests/" + m_Name + +".json";
			json = MESerializer::OpenFileRead(path.c_str());
			m_json_buffer = json.c_str();
			doc.Parse(m_json_buffer);
			//load each name, audioID, etc. into each game object
			const rapidjson::Value& value = doc["TestCase"];

			m_Name = value["name"].GetString();
			m_Description = value["description"].GetString();
			m_IntendedResult = value["intended_result"].GetString();

			return json;

			
	};
	virtual void Update(float dt)
	{
		m_Duration += dt;
	};
	virtual void Shutdown()
	{
		m_Duration = MEFramerateController::GetTimeElapsed() - m_start_time;



	};

	//name of the test case
	std::string m_Name;
	//description of the test [OPTIONAL]
	std::string m_Description;
	//test status
	TestStatus m_Status;
	//test output, errors, results
	std::string m_Output;
	//duration of the test
	float m_Duration = -1.f;

	std::string m_Result;
	std::string m_IntendedResult;

protected:
	const char* m_json_buffer = nullptr;


private:
	float m_start_time = -1.f;
};
