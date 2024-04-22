/*****************************************************************//**
 * \file         MEtimer.cpp
 * \author       Ethan Mclaughlin
 * \par          Email: ethan.mclaughlin\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#include "MEtimer.h"
#include "Log.h"
std::deque<Timer*> METimerManager::list;


	/**
	 * @brief Makes a new tiemr
	 * 
	 * @param length, how long the timer lasts, mesured in seconds
	 */
	Timer* METimerManager::MakeTimer(float length)
	{
		//adds the timer to the timer manager
		Timer* newtimer = new Timer;
		if (newtimer)
		{
			//sets the values of the timer
			newtimer->time = length;
			newtimer->ended = false;
			newtimer->running = true;
			//PauseStop is for global pause
			newtimer->PauseStop = false;
			//adds it to the list
			addTimer(newtimer);
			return newtimer;
		}
		return NULL;
	}

void METimerManager::DeleteTimer(Timer* timer)
{
		for(auto it = list.begin(); it != list.end(); ++it)
		{
			if(*it == timer)
			{
				list.erase(it);
				delete timer;

			}
		}
}

void METimerManager::Shutdown()
{
		for(auto timer : list)
		{
			delete timer;
		}
		list.clear();
}

/**
	 * @brief Stops the timer from running
	 * 
	 * @param tim, timer to use
	 */
	void METimerManager::stop(Timer* tim)
	{
		tim->running = false;
	}

	/**
	 * @brief starts the timer after it has been stoped
	 * 
	 * @param tim, timer to use
	 */
	void METimerManager::start(Timer* tim)
	{
		tim->running = true;
	}

	/**
	 * @brief Adds or subtracts time from the timer by the amount given
	 * 
	 * @param tim, timer to use
	 * @param amount, how much to change the timer by, use a negative number to subtract time
	 */
	void METimerManager::change(float amount, Timer* tim)
	{
		tim->time += amount;
	}

	/**
	 * @brief Sets the time of the timer to the time given.
	 * 
	 * \param tim, the timer to set the time
	 * \param amount, time to set the timer
	 */
	void METimerManager::set(float amount, Timer* tim)
	{
		tim->time = amount;
		tim->ended = false;
	}

	/**
	 * @brief checks how much time a timer has left.
	 *
	 * \param tim, timer to check
	 * \return bool, if it has ended
	 */
	float METimerManager::checktime(Timer* tim)
	{
		return tim->time;
	}

	/**
	 * @brief checks if a timer has ended.
	 * 
	 * \param tim, timer to check
	 * \return bool, if it has ended
	 */
	bool METimerManager::check(Timer* tim)
	{
		if(tim)
		{
			return tim->ended;
		}
		return false;
	}

	/**
	 * @brief Updates all of the timers
	 * 
	 * \param dt delta time.
	 */
	void METimerManager::Update(float dt)
	{
		//iterator for the timer list
		for(auto& index : list)
		{
			//updates the timer
			//check if its paused
			if (index->running && !index->PauseStop)
			{
				//subtract the time
				index->time -= dt;
				//ME_CORE_INFO(index->time)
			}

			if (index->time <= 0.0f)
			{
				index->ended = true;
			}
		}
	}
	
	/**
	 * @brief Adds a timer to the deque list of timers in the manager
	 * 
	 * \param newtimer timer to add.
	 */
	void METimerManager::addTimer(Timer* newtimer)
	{
		list.push_front(newtimer);
	}

	/**
	 * @brief Pauses all timers.
	 * 
	 */
	void METimerManager::GlobalPause()
	{
		//iterator for the timer list
		for (auto& index : list)
		{
			//Turns on global pause for current timer
			index->PauseStop = true;
		}

	}

	/**
	 * @brief Pauses all timers.
	 *
	 */
	void METimerManager::GlobalUnpause()
	{
		//iterator for the timer list
		for (auto& index : list)
		{
			//Turns off global pause for current timer
			index->PauseStop = false;
		}
	}
