/*********************************************************************************************************************/
/*!
  \file              Log.cpp
  \author            Nicholas Leben
  \par               Email: nick.leben\@digipen.edu
  \par               Course: GAM200
  \par               Copyright © 2022 DigiPen, All rights reserved.
  \par				 credit to: https://www.youtube.com/watch?v=dZr-53LAlOw (thecherno on youtube)

  \brief
	this implements the spdlog library 
 */
 /*********************************************************************************************************************/

/*********************************************************************************************************************/
/* Includes */
/*********************************************************************************************************************/
#include <iostream>
#include <Log.h>
#include <ostream>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

/*!********************************************************************************************************************
	\brief
		this function initializes variables to be used for logging
**********************************************************************************************************************/

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
#ifdef _DEBUG
		//set text pattern, wiki page: https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
		//we use: [date and time] "name of system" (log level): text/variable to be logged

		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Mayhem.log", true));

		logSinks[0]->set_pattern("%^[%r] %n: %v%$");
		logSinks[1]->set_pattern("%^[%r] %n: %v%$");

		s_CoreLogger = std::make_shared<spdlog::logger>("MAYHEM", begin(logSinks), end(logSinks));
		std::cout << "core init" << std::endl;

		spdlog::register_logger(s_CoreLogger);
		std::cout << "core reg" << std::endl;

		//s_CoreLogger = spdlog::stdout_color_mt("MAYHEM");
		s_CoreLogger->set_level(spdlog::level::trace);
		s_CoreLogger->flush_on(spdlog::level::trace);

		s_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
		std::cout << "client init" << std::endl;

		spdlog::register_logger(s_ClientLogger);
		std::cout << "client reg" << std::endl;
		//s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
		s_ClientLogger->flush_on(spdlog::level::trace);

		spdlog::flush_every(std::chrono::milliseconds(100));

#endif // _DEBUG
	}
