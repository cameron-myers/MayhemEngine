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
	MEFunctionalTest(const char* name):m_Name(name){};
	virtual void Init()
	{
		m_Status = Running;
		m_start_time = MEFramerateController::GetTimeElapsed();
	};
	virtual void Read()
	{
		//do essential reading here, then do extra reading in derived class
		
			std::string json;
			std::string clearData(json);
			std::string path = "../Tests/" + m_Name;
			json = MESerializer::OpenFileRead(path.c_str());
			const char* bufchar = json.c_str();
			m_json_document.Parse(bufchar);
			//load each name, audioID, etc. into each game object
			const rapidjson::Value& value = m_json_document["TestCase"];

			m_Name = value["name"].GetString();
			m_Description = value["description"].GetString();
			m_IntendedResult = value["intended_result"].GetString();
			

			
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
	float m_Duration;

	std::string m_Result;
	std::string m_IntendedResult;

protected:
	rapidjson::Document m_json_document;


private:
	float m_start_time;
};
