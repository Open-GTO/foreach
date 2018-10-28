# foreach

foreach standalone include (non y_iterate version). Y_Less dropped support for this version of foreach.

This version of foreach is 19 (0.4.2). This is the standalone version, so it does not require YSI. Use this version if you do not want to include YSI.

This fork based on [that fork](https://github.com/karimcambridge/SAMP-foreach).

# Features
- Actor and Vehicle iterators.
- Multiscripts supported by FOREACH_MULTISCRIPT.
- Optimized ALS hooking process (without CallLocalFunction).
- Changed iterator enable/disable defines.
- Removed npcmodes/ scripts support.
- New functions: Iter_Index

# How to install
If you use foreach only in gamemode, just include foreach like this:
```Pawn
#include <a_samp>
#include <foreach>
```

If you use foreach in filterscript, just include foreach like this:
```Pawn
#define FILTERSCRIPT

#include <a_samp>
#include <foreach>
```

If you use foreach in each of your script, just include foreach like this:
```Pawn
#define FOREACH_MULTISCRIPT

#include <a_samp>
#include <foreach>
```

# Settings

Directive | Iterator | Enabled by default
---|---|---
FOREACH_I_Player | Player | yes
FOREACH_I_Bot | Bot / NPC | yes
FOREACH_I_Character | Character | yes
FOREACH_I_Vehicle | Vehicle | yes
FOREACH_I_Actor | Actor | yes
FOREACH_I_PlayerPlayersStream | PlayerPlayersStream[playerid] | no
FOREACH_I_PlayerVehiclesStream | PlayerVehiclesStream[playerid] | no
FOREACH_I_VehiclePlayersStream | VehiclePlayersStream[vehicleid] | no
FOREACH_I_PlayerActorsStream | PlayerActorsStream[playerid] | no
FOREACH_I_ActorPlayersStream | ActorPlayersStream[actorid] | no
FOREACH_I_PlayerInVehicle | PlayerInVehicle[vehicleid] | no

How to disable any iterator:
```Pawn
#define FOREACH_I_Actor 0
#define FOREACH_I_Vehicle 0
#include <foreach>
```

# How to use
#### Gives $100 to all connected players
```Pawn
foreach (new playerid : Player) {
	GivePlayerMoney(playerid, 100);
}
```

#### Sets 130 and 170 colors to all vehicles with model 400
```Pawn
foreach (new vehicleid : Vehicle) {
	if (GetVehicleModel(vehicleid) == 400) {
		ChangeVehicleColor(vehicleid, 130, 170);
	}
}
```

#### Remove all existed vehicles and actors
For removing `vehicles` or `actors` in foreach loop you should use `Safe` functions:
```Pawn
foreach (new vehicleid : Vehicle) {
	DestroyVehicleSafe(vehicleid);
}

foreach (new actorid : Actor) {
	DestroyActorSafe(actorid);
}
```

#### Custom iterators
Adds two iterators with players in team 1 and 2.
```Pawn
new
	Iterator:Team1<MAX_PLAYERS>,
	Iterator:Team2<MAX_PLAYERS>;

stock Team_SetPlayerTeam(playerid, teamid)
{
	switch (GetPlayerTeam(playerid)) {
		case 1: {
			Iter_Remove(Team1, playerid);
		}
		case 2: {
			Iter_Remove(Team2, playerid);
		}
	}

	switch (teamid) {
		case 1: {
			Iter_Add(Team1, playerid);
		}
		case 2: {
			Iter_Add(Team2, playerid);
		}
	}

	return SetPlayerTeam(playerid, teamid);
}
#if defined _ALS_SetPlayerTeam
	#undef SetPlayerTeam
#else
	#define _ALS_SetPlayerTeam
#endif

#define SetPlayerTeam Team_SetPlayerTeam
```
It can be used like this:
```Pawn
foreach (new i : Team1) {
	GivePlayerMoney(i, 1000);
}

foreach (new i : Team2) {
	SetPlayerHealth(i, 0.0);
}
```
This script gives $1000 to all players from team with id 1, and kills players from team with id 2.

# Custom array of iterators
Adds array of iterators with players in teams:

```Pawn
#define MAX_TEAMS 255

new
	Iterator:Team[MAX_TEAMS]<MAX_PLAYERS>;

public OnGameModeInit()
{
	Iter_Init(Team);

	#if defined Team_OnGameModeInit
		return Team_OnGameModeInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif

#define OnGameModeInit Team_OnGameModeInit
#if defined Team_OnGameModeInit
	forward Team_OnGameModeInit();
#endif



stock Team_SetPlayerTeam(playerid, teamid)
{
	new current_team = GetPlayerTeam(playerid);
	if (current_team != NO_TEAM) {
		Iter_Remove(Team[current_team], playerid);
	}

	if (teamid != NO_TEAM) {
		Iter_Add(Team[teamid], playerid);
	}

	return SetPlayerTeam(playerid, teamid);
}
#if defined _ALS_SetPlayerTeam
	#undef SetPlayerTeam
#else
	#define _ALS_SetPlayerTeam
#endif

#define SetPlayerTeam Team_SetPlayerTeam
```
It can be used like this:
```Pawn
new scores[MAX_TEAMS];
for (new teamid; teamid < MAX_TEAMS; teamid++) {
	foreach (new playerid : Team[teamid]) {
		scores[teamid] += GetPlayerScore(playerid);
	}
}

for (new teamid; teamid < MAX_TEAMS; teamid++) {
	printf("Team %d: %d", teamid, scores[teamid]);
}
```
This script sums up score from each team and prints it.
