/*______________________________________________________________________________

					Lordz's Timers - L_Timers.inc <In Development>
					    By Lordz AKA Lordzy
					    - 2014

					Version : 1.0 BETA
					+ Supports player-timers only for now!
					
	"L_Timers" is an include created by me, Lordz, which supports some easy task
	of managing timers. This include for now, supports only player-timers as its
	in BETA stage. While using player timers, it don't really use the timer
	functions such as "SetTimer" or "SetTimerEx" and uses "OnPlayerUpdate" to
	catch up with the timings. A maximum of 25 timers can be run on a player
	continuously. To run further more, a timer has to be stopped.
	
	  This Source Code Form is subject to the
      terms of the Mozilla Public License, v.
      2.0. If a copy of the MPL was not
      distributed with this file, You can
      obtain one at
      http://mozilla.org/MPL/2.0/.

Author : Lordz AKA Lordzy

Functions:

native SetPlayerTimer(playerid, intervals); //Timer:SetPlayerTimer(playerid, intervals);
native GetPlayerTimer(playerid, Timer:timerid, TIMER_DATA_TYPE);
	TIMER_DATA_TYPE (0) = returns the tick it got started!
	TIMER_DATA_TYPE (1) = returns the intervals the timer was set to run
	TIMER_DATA_TYPE (2) = returns the remaining millisecond to complete the task!

native IsPlayerTimerOn(playerid, Timer:timerid);

Callbacks:
OnPlayerTimerEnd(playerid, Timer:timerid);


PLEASE DEFINE 'FILTERSCRIPT' IF USING THIS AS A FILTERSCRIPT!

______________________________________________________________________________*/

#if defined _included_ltimers
	#endinput
#endif

#define _included_ltimers

#tryinclude <a_samp>

#define MAX_PLAYER_TIMERS   25
#define INVALID_LTIMER      0xFF

//Maximum timers which could run continously for a player.

enum LT_pTimENUM {
	bool:LT_pTimer,
	LT_pTimerIntervals,
	LT_pTimerAIntervals,
	LT_pTimerStarted,
}

static
	LTimer_pData[MAX_PLAYERS][LT_pTimENUM][MAX_PLAYER_TIMERS];

stock bool:IsPlayerTimerOn(playerid, Timer:timerid)
{
	if(LTimer_pData[playerid][LT_pTimer][_:timerid]) return true;
	else return false;
}

stock Timer:SetPlayerTimer(playerid, intervals)
{
	if(!IsPlayerConnected(playerid)) return Timer:INVALID_LTIMER;
	if(intervals < 20) return Timer:INVALID_LTIMER;
	for(new i = 0; i< MAX_PLAYER_TIMERS; i++) {
	    if(IsPlayerTimerOn(playerid, Timer:i)) continue;
	    LTimer_pData[playerid][LT_pTimer][i] = true;
	    LTimer_pData[playerid][LT_pTimerStarted][i] = GetTickCount();
	    LTimer_pData[playerid][LT_pTimerIntervals][i] = LTimer_pData[playerid][LT_pTimerStarted][i]+intervals;
	    LTimer_pData[playerid][LT_pTimerAIntervals][i] = intervals;
	    return Timer:i;
	}
	return Timer:INVALID_LTIMER;
}

stock GetPlayerTimer(playerid, Timer:timerid, TIMER_DATA_TYPE)
{
	if(!IsPlayerTimerOn(playerid, Timer:timerid)) return INVALID_LTIMER;
	switch(TIMER_DATA_TYPE) {
	    case 0 : return LTimer_pData[playerid][LT_pTimerStarted][_:timerid];
	    case 1 : return LTimer_pData[playerid][LT_pTimerAIntervals][_:timerid];
	    case 2 : {
			new
				LT_tempticks = GetTickCount();
			if(LT_tempticks <= LTimer_pData[playerid][LT_pTimerIntervals][_:timerid])
   			return LTimer_pData[playerid][LT_pTimerIntervals][_:timerid] - LT_tempticks;
		}
	}
	return INVALID_LTIMER;
}

public OnPlayerUpdate(playerid)
{
	new
	    LTTicks;
	for(new i = 0; i< MAX_PLAYER_TIMERS; i++) {
	    if(!IsPlayerTimerOn(playerid, Timer:i)) continue;
	    LTTicks = GetTickCount(); //Tick count should be assigned in each loop as the value could possibly change.
	    if(LTimer_pData[playerid][LT_pTimerIntervals][_:i] <= LTTicks) {
	        //Calling out the callback first so that the coder could gain info
	        //about the timer if required and then only it gets reset!
	        CallLocalFunction("OnPlayerTimerEnd", "ii", playerid, _:i);
			LTimer_pData[playerid][LT_pTimer][_:i] = false;
			LTimer_pData[playerid][LT_pTimerAIntervals][_:i] = -1;
			LTimer_pData[playerid][LT_pTimerIntervals][_:i] = -1;
			LTimer_pData[playerid][LT_pTimerStarted][_:i] = -1;
		}
	}
	#if defined LTIMER_OnPlayerUpdate
	LTIMER_OnPlayerUpdate(playerid);
	#endif
	return 1;
}

#if defined _ALS_OnPlayerUpdate
	#undef OnPlayerUpdate
#else
	#define _ALS_OnPlayerUpdate
#endif

#define OnPlayerUpdate LTIMER_OnPlayerUpdate

#if defined LTIMER_OnPlayerUpdate
forward LTIMER_OnPlayerUpdate(playerid);
#endif


static stock LTimer_pInit()
{
	for(new i; i< GetMaxPlayers(); i++) {
	    if(!IsPlayerConnected(i)) continue;
	    for(new Timer:a; _:a< MAX_PLAYER_TIMERS; _:a++) {
			LTimer_pData[i][LT_pTimer][_:a] = false;
			LTimer_pData[i][LT_pTimerAIntervals][_:a] = -1;
			LTimer_pData[i][LT_pTimerIntervals][_:a] = -1;
			LTimer_pData[i][LT_pTimerStarted][_:a] = -1;
		}
	}
	return 1;
}

#if defined FILTERSCRIPT
public OnFilterScriptInit()
{
	LTimer_pInit();
	#if defined LTIMER_OnFilterScriptInit
	LTIMER_OnFilterScriptInit();
	#endif
	return 1;
}

#if defined _ALS_OnFilterScriptInit
	#undef OnFilterScriptInit
#else
	#define _ALS_OnFilterScriptInit
#endif

#define OnFilterScriptInit LTIMER_OnFilterScriptInit

#if defined LTIMER_OnFilterScriptInit
forward LTIMER_OnFilterScriptInit();
#endif


#else

public OnGameModeInit()
{
	LTimer_pInit();
	#if defined LTIMER_OnGameModeInit
	LTIMER_OnGameModeInit();
	#endif
	return 1;
}

#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif

#define OnGameModeInit LTIMER_OnGameModeInit

#if defined LTIMER_OnGameModeInit
forward LTIMER_OnGameModeInit();
#endif

#endif

forward OnPlayerTimerEnd(playerid, Timer:timerid);

/*
	
new Timer:test;
public OnPlayerSpawn(playerid)
{
 test = SetPlayerTimer(playerid, 1000);
 new shit = GetPlayerTimer(playerid, test, 1);
 printf("%d", shit);
 return 1;
}

public OnPlayerTimer(playerid, Timer:timerid)
{
	if(timer == test) {
		printf("TESTED!");
	}
	return 1;
}


*/
