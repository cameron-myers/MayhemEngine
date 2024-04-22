/*****************************************************************//**
 * \file         MEtimer.h
 * \author       Ethan Mclaughlin
 * \par          Email: ethan.mclaughlin\@digipen.edu
 * \par          Course: GAM200
 * \copyright    Copyright (c) 2022 DigiPen, All rights reserved
 * \brief
 * 
 *********************************************************************/
#pragma once
#include <deque>


class METimerManager;


class Timer
{
private:
	float time; //time remaining on the timer
	bool running; //if the timer is running
	bool PauseStop;
	bool ended; //if the timer has ended
	friend class METimerManager;
};

class METimerManager
{
public:
	static Timer* MakeTimer(float length); //makes a new timer
	static void DeleteTimer(Timer* timer); //makes a new timer
	static void Shutdown();
	static void Update(float dt); //updates all of the timers |DO NOT USE, ONLY CALLED BY ENGINE|
	static void stop(Timer*); //stops a timer
	static void start(Timer*); //starts a timer
	static void change(float amount, Timer*); // adds/subtracts time from a timer
	static void set(float amount, Timer*); //sets a timer to a specific time
	static float checktime(Timer*); //check if a timer has finished
	static bool check(Timer*); //check if a timer has finished
	static void GlobalPause(); //pauses all timers
	static void GlobalUnpause(); //unpauses all timers
private:
	static void addTimer(Timer* newtimer); // used to adda a timer to the list
	static std::deque<Timer*> list; //deque of pointers to timers
};