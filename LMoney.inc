#if defined _included_L_Money
	#endinput
#endif

#define _included_L_Money

#include <a_samp>
#include <script_compatibility>

static
	g_L_PlayerSSidedMoney[MAX_PLAYERS],
	g_L_SSidedMoneyTimer,
	g_L_PlayerExceptionTime[MAX_PLAYERS],
	g_L_PlayerMoneyWarns[MAX_PLAYERS char]
;

public OnScriptInit() {

	EnableStuntBonusForAll(0);
	g_L_SSidedMoneyTimer = SetTimer("LSSidedMoneyTimer", 500, true);
	
	for(new i = GetPlayerPoolSize(); i != -1; i--) {
	
	    if(!IsPlayerConnected(i))
	        continue;
	        
		OnPlayerConnect(i);
	}
	#if defined LSSidedMoney_OnScriptInit
	    return LSSidedMoney_OnScriptInit();
	#else
	    return 1;
	#endif
}

public OnScriptExit() {

	KillTimer(g_L_SSidedMoneyTimer);
	
	#if defined LSSidedMoney_OnScriptExit
	    return LSSidedMoney_OnScriptExit();
	#else
	    return 1;
	#endif
}

public OnPlayerConnect(playerid) {

	ResetPlayerMoney(playerid);
	g_L_PlayerSSidedMoney[playerid] = 0;
	g_L_PlayerMoneyWarns{playerid} = 0;
	g_L_PlayerExceptionTime[playerid] = gettime() - 1;

	EnableStuntBonusForPlayer(playerid, 0);
	
	#if defined LSSidedMoney_OnPlayerConnect
	    return LSSidedMoney_OnPlayerConnect(playerid);
	#else
	    return 1;
	#endif
}


public OnPlayerDeath(playerid, killerid, reason) {

	g_L_PlayerExceptionTime[playerid] = gettime();
	
	#if defined LSSidedMoney_OnPlayerDeath
	    return LSSidedMoney_OnPlayerDeath(playerid, killerid, reason);
	#else
	    return 1;
	#endif
}

public OnPlayerSpawn(playerid) {

	if(g_L_PlayerExceptionTime[playerid]) {
	
		ResetPlayerMoney(playerid);
		GivePlayerMoney(playerid, g_L_PlayerSSidedMoney[playerid]);

		g_L_PlayerExceptionTime[playerid] = 0;
	}
	#if defined LSSidedMoney_OnPlayerSpawn
	    return LSSidedMoney_OnPlayerSpawn(playerid);
	#else
	    return 1;
	#endif
}

forward LSSidedMoneyTimer();
public LSSidedMoneyTimer() {

	new
	    temp_hMoney;
	//replace with foreach or custom array containing online player ids.
	//helps to improve performance since this timer is called every ~500 ms.
	
	for(new i = GetPlayerPoolSize(); i != -1; i--) {
	
	    if(!IsPlayerConnected(i))
	        continue;
	        
		if((gettime() - g_L_PlayerExceptionTime[i]) <= 3)
		    continue;

		temp_hMoney = GetPlayerMoney(i);
		
		if(temp_hMoney != g_L_PlayerSSidedMoney[i]) {
		
		    ResetPlayerMoney(i);
			GivePlayerMoney(i, g_L_PlayerSSidedMoney[i]);
		
		    if(++g_L_PlayerMoneyWarns{i} >= 3) { 
		    
				#if defined OnPlayerMoneyHack
					g_L_PlayerMoneyWarns{i} = 0;
				    return OnPlayerMoneyHack(i, temp_hMoney, g_L_PlayerSSidedMoney[i]);

				#endif
			}
		}
//		else
//			g_L_PlayerMoneyWarns{i} = 0;
	}
	return 1;
}

stock GetPlayerLSSidedMoney(playerid, bool:SAMP_CASH = false) {

	if(SAMP_CASH)
	    return GetPlayerMoney(playerid);
	else
	    return g_L_PlayerSSidedMoney[playerid];
}

stock GivePlayerLSSidedMoney(playerid, amount, bool:SAMP_CASH = false) {

	if(SAMP_CASH)
		GivePlayerMoney(playerid, amount);
	else
	    g_L_PlayerSSidedMoney[playerid] += amount;

	return 1;
}

stock ResetPlayerLSSidedMoney(playerid, bool:SAMP_CASH = false) {

	if(SAMP_CASH)
	    ResetPlayerMoney(playerid);
	else
	    g_L_PlayerSSidedMoney[playerid] = 0;
	    
	return 1;
}


//hooks

#if defined _ALS_OnScriptInit
	#undef OnScriptInit
#else
	#define _ALS_OnScriptInit
#endif

#if defined _ALS_OnScriptExit
	#undef OnScriptExit
#else
	#define _ALS_OnScriptExit
#endif

#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
	#define _ALS_OnPlayerSpawn
#endif

#if defined _ALS_GivePlayerMoney
	#undef GivePlayerMoney
#else
	#define _ALS_GivePlayerMoney
#endif

#if defined _ALS_GetPlayerMoney
	#undef GetPlayerMoney
#else
	#define _ALS_GetPlayerMoney
#endif

#if defined _ALS_ResetPlayerMoney
	#undef ResetPlayerMoney
#else
	#define _ALS_ResetPlayerMoney
#endif


#define     OnScriptInit        LSSidedMoney_OnScriptInit
#define     OnScriptExit        LSSidedMoney_OnScriptExit
#define     OnPlayerConnect     LSSidedMoney_OnPlayerConnect
#define     OnPlayerSpawn       LSSidedMoney_OnPlayerSpawn
#define     OnPlayerDeath       LSSidedMoney_OnPlayerDeath

#define     GivePlayerMoney     GivePlayerLSSidedMoney
#define     GetPlayerMoney      GetPlayerLSSidedMoney
#define     ResetPlayerMoney    ResetPlayerLSSidedMoney


#if defined LSSidedMoney_OnScriptInit
	forward LSSidedMoney_OnScriptInit();
#endif

#if defined LSSidedMoney_OnScriptExit
	forward LSSidedMoney_OnScriptExit();
#endif

#if defined LSSidedMoney_OnPlayerConnect
	forward LSSidedMoney_OnPlayerConnect(playerid);
#endif

#if defined LSSidedMoney_OnPlayerSpawn
	forward LSSidedMoney_OnPlayerSpawn(playerid);
#endif

#if defined LSSidedMoney_OnPlayerDeath
	forward LSSidedMoney_OnPlayerDeath(playerid, killerid, reason);
#endif

#if defined OnPlayerMoneyHack
	forward OnPlayerMoneyHack(playerid, hacked_value, real_value);
#endif

/*public OnPlayerMoneyHack(playerid, hacked_value, real_value) {

	new string[144];
	format(string, sizeof(string), "PlayerID : %d Hacked value : %d | Real value : %d", playerid, hacked_value, real_value);
	return !SendClientMessageToAll(-1, string);
}*/
