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
#include "tinyxml2.h"

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
	MEFunctionalTest(const char* suite):m_Case(suite),m_Status(Invalid){};
	MEFunctionalTest(const MEFunctionalTest& rhs)
	{
		m_Case = rhs.m_Case;

		//name of the test case
		m_Class = rhs.m_Class;
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
			//TODO GET SUITE VAL FROM COMMAND LINE MAYBE MAKE ENGINE MEMBER
			std::string path = "../Tests/.json";/*+m_Suite+*/
			json = MESerializer::OpenFileRead(path.c_str());
			m_json_buffer = json.c_str();
			doc.Parse(m_json_buffer);
			//load each name, audioID, etc. into each game object
			const rapidjson::Value& value = doc["TestCases"];

			//NOTE THIS MAY BE FUCKED

			const rapidjson::Value& case_obj = value[m_Case.c_str()];

			m_Description = case_obj["description"].GetString();
			m_IntendedResult = case_obj["intended_result"].GetString();
			

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
	//add static/template function that takes in a test and writes to JUNIT
	void WriteResults(std::string const& vaOutput = "")
	{
		std::string test_namespace = "MayhemEngine_Functional_Testing";

		tinyxml2::XMLDocument doc;
		doc.NewDeclaration(NULL);

		auto root = doc.NewElement("testsuites");
		doc.InsertFirstChild(root);

		auto suite = root->InsertNewChildElement("testsuite");
		suite->SetAttribute("name", test_namespace.c_str());
		root->InsertEndChild(suite);

		auto _case = suite->InsertNewChildElement("testcase");
		_case->SetAttribute("name", m_Case.c_str());
		_case->SetAttribute("classname", (test_namespace + "." + m_Class).c_str());
		_case->SetAttribute("time", m_Duration);
		suite->InsertEndChild(_case);
		if (m_Status == Failed)
		{
			auto fail = _case->InsertNewChildElement("failure");
			fail->SetAttribute("message", (m_Output + vaOutput).c_str());
			fail->SetAttribute("type", "AssertionError");
			_case->InsertEndChild(fail);

		}

		doc.SaveFile("../Tests/test_report.xml");
	};

	std::string m_Class;
	//name of the test case
	std::string m_Case;
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
