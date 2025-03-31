/*****************************************************************//**
 * \file         Log.h
 * \author       The Cherno(Youtube)
 * \brief		 credit to: https://www.youtube.com/watch?v=dZr-53LAlOw
 *
 *********************************************************************/
#pragma once

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <memory>

#include "spdlog/spdlog.h"

class Log
{
public:
	static void Init();
	static void Free()
	{
		s_CoreLogger->flush();
		s_ClientLogger->flush();
		spdlog::drop("MAYHEM");
		spdlog::drop("APP");


	};

	inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
private:
	static std::shared_ptr<spdlog::logger> s_CoreLogger;
	static std::shared_ptr<spdlog::logger> s_ClientLogger;
};



//Core Log Macros
#ifndef _DISTRIBUTE
#define ME_CORE_TRACE(...)    ::Log::GetCoreLogger()->trace(__VA_ARGS__); 
#define ME_CORE_INFO(...)     ::Log::GetCoreLogger()->info(__VA_ARGS__);
#define ME_CORE_WARN(...)     ::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define ME_CORE_ERROR(...)    ::Log::GetCoreLogger()->error(__VA_ARGS__);
#define ME_CORE_FATAL(...)    ::Log::GetCoreLogger()->fatal(__VA_ARGS__);

#endif
#ifdef _DISTRIBUTE
#define ME_CORE_TRACE(...);
#define ME_CORE_INFO(...) ;
#define ME_CORE_WARN(...) ;
#define ME_CORE_ERROR(...);
#define ME_CORE_FATAL(...);
#endif


//Client Log Macros
#ifndef _DISTRIBUTE
#define ME_TRACE(...)		  ::Log::GetClientLogger()->trace(__VA_ARGS__); 
#define ME_INFO(...)          ::Log::GetClientLogger()->info(__VA_ARGS__);
#define ME_WARN(...)          ::Log::GetClientLogger()->warn(__VA_ARGS__);
#define ME_ERROR(...)         ::Log::GetClientLogger()->error(__VA_ARGS__);
#define ME_FATAL(...)         ::Log::GetClientLogger()->fatal(__VA_ARGS__);

#endif
#ifdef _DISTRIBUTE
#define ME_CORE_TRACE(...);
#define ME_INFO(...) 
#define ME_WARN(...) 
#define ME_ERROR(...)
#define ME_FATAL(...)
#endif
