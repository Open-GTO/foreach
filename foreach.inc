/*----------------------------------------------------------------------------*\
					===========================
					 foreach efficient looping
					===========================
Description:
	Provides efficient looping through sparse data sets, such as connected
	players. Significantly improved from the original version to be a generic
	loop system, rather then purely a player loop system. When used for
	players this has constant time O(n) for number of connected players (n),
	unlike standard player loops which are O(MAX_PLAYERS), regardless of the
	actual number of connected players. Even when n is MAX_PLAYERS this is
	still faster.
Legal:
	Version: MPL 1.1

	The contents of this file are subject to the Mozilla Public License Version
	1.1 (the "License"); you may not use this file except in compliance with
	the License. You may obtain a copy of the License at
	http://www.mozilla.org/MPL/

	Software distributed under the License is distributed on an "AS IS" basis,
	WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
	for the specific language governing rights and limitations under the
	License.

	The Original Code is the YSI foreach include.

	The Initial Developer of the Original Code is Alex "Y_Less" Cole.
	Portions created by the Initial Developer are Copyright (C) 2011
	the Initial Developer. All Rights Reserved.

	Contributors:
		ZeeX, koolk, JoeBullet/Google63, g_aSlice/Slice, Kar

	Thanks:
		JoeBullet/Google63 - Handy arbitrary ASM jump code using SCTRL.
		ZeeX - Very productive conversations.
		koolk - IsPlayerinAreaEx code.
		TheAlpha - Danish translation.
		breadfish - German translation.
		Fireburn - Dutch translation.
		yom - French translation.
		50p - Polish translation.
		Zamaroht - Spanish translation.
		Dracoblue, sintax, mabako, Xtreme, other coders - Producing other modes
			for me to strive to better.
		Pixels^ - Running XScripters where the idea was born.
		Matite - Pestering me to release it and using it.

	Very special thanks to:
		Thiadmer - PAWN, whose limits continue to amaze me!
		Kye/Kalcor - SA:MP.
		SA:MP Team past, present and future - SA:MP.

Version:
	19 (0.4.2)
Changelog:
	14/05/15:
		Cleaned up the entire include.
	16/07/12:
		Fixed a bug with "loadfs" blocking callbacks.
		Slightly reduced the memory consumption of constant strings.
	25/05/12:
		Added a delay timer to remove players for "Kick" crashes.
	05/01/12:
		Fixed multi-dimensional iterators.
		Fixed "FOREACH_NO_BOTS".
		Made "Iterator:" support multi-dimensional arrays.
	07/12/11:
		Underlying implementation tweak for more consistent code.
		Added Iter_Contains.
	06/12/11:
		Minor fixes.
	31/10/11:
		Changed the underlying loop code to be slightly faster.
		Added support for Iter_SafeRemove, prompting refactoring.
	17/09/11:
		Fixed arrays under the new syntax.
	28/04/11:
		Moved iterator identifiers to end of variables.
		Rewrote "foreach" to accept two syntaxes for "foreach (new i : Iter)".
	16/08/10:
		Removed all the "2" versions of the functions.
	14/08/10:
		Added Iter_Clear to reset an array.
	06/08/10:
		Added special array declaration format.
	18/12/09:
		Added Iter_Func2 functions for multi-dimensional iterators.
		Renamed foreact et al as keywords in the documentation.
		Changed licensing from GPL to MPL.
	02/09/09:
		Fixed (again) for 0.3.
		Added free slot finding.
	21/08/09:
		Updated to include random functions.
		Made entirely stand alone.
		Ported to 0.3 (separate version).
		Added automatic callback hook code.
		Removed debug information from stand alone version.
	06/01/08:
		Added debug information.
	09/10/07:
		Moved to system.
	16/09/07:
		Added list sorting.
		Made this part of Y SeRver Includes, not Y Sever Includes.
		Made list sorting optional.
		Fixed version number.
	08/09/07:
		First version.
Functions:
	Public:
		OnPlayerDisconnect - Called when a player leaves to remove them.
		OnPlayerConnect - Called when a player connects to add them.
	Core:
		-
	Stock:
		Iter_AddInternal - Add a value to an iterator.
		Iter_RemoveInternal - Remove a value from an iterator.
		Iter_RandomInternal - Get a random item from an iterator.
		Iter_FreeInternal - Gets the first free slot in the iterator.
		Iter_InitInternal - Initialises a multi-dimensional iterator.
		Iter_ContainsInternal - Checks if a value is in an iterator.
	Static:
		-
	Inline:
		Iter_Create - Create a new iterator value set.
		Iter_Add - Wraps Iter_AddInternal.
		Iter_SafeRemove - Wraps Iter_SafeRemoveInternal.
		Iter_Remove - Wraps Iter_RemoveInternal.
		Iter_Random - Wraps Iter_RandomInternal.
		Iter_Count - Gets the number of items in an iterator.
		Iter_Free - Wraps around Iter_FreeInternal.
		Iter_Contains - Wraps around Iter_ContainsInternal.
	API:
		-
Callbacks:
	-
Hooks:
	Iter_OnPlayerConnect - Hook for the OnPlayerConnect callback.
	Iter_OnPlayerDisconnect - Hook for the OnPlayerDisconnect callback.
	Iter_OnGameModeInit - Only exists to make the code compile correctly...
Definitions:
	-
Enums:
	-
Macros:
	-
Keywords:
	foreach - Function to loop an iterator.
Tags:
	Iterator - Declare an iterator.
Variables:
	Global:
		-
	Static:
		-
Commands:
	-
Operators:
	-
Iterators:
	Player - List of all players connected.
	Bot - List of all bots (npcs) connected.
	NPC - Alias of Bot.
	Character - All players and bots.
\*----------------------------------------------------------------------------*/

#if !defined _samp_included
	#error "Please include a_samp or a_npc before foreach"
#endif

#if defined _foreach_included
	#endinput
#endif

#define _foreach_included

/*----------------------------------------------------------------------------*\

\*----------------------------------------------------------------------------*/

#define ITER_NONE -1

#define _Y_ITER_ARRAY: _:_Y_ITER_C0:
#define _Y_ITER_ARRAY_SIZE(%1) _:_Y_ITER_C1:_Y_ITER_C2:sizeof %1@YSII_Ag-1

#define _Y_ITER_C0:%0[%1]@YSII_%4g%3) %0@YSII_%4g[%1]%3)
#define _Y_ITER_C1:_Y_ITER_C2:%0[%1]@YSII_Ag%3) %0@YSII_Ag[]%3)
#define _Y_ITER_C2:sizeof%0(%1)@YSII_Ag-1;_:(%2=_Y_ITER_ARRAY:%3(%4)@YSII_Ag[%5])!=_Y_ITER_ARRAY_SIZE(%6);) -1;_:(%2=%3@YSII_Ag(%4,%5))!=-1;)
#define _Y_ITER_C3:%0[%1]@YSII_Cg,%2[%3]@YSII_Ag[%4]={%5} _Y_ITER_C3:%0@YSII_Cg[%1],%0@YSII_Ag[%1][%4]

forward Iter_OPDCInternal(playerid);

/*----------------------------------------------------------------------------*\
Array:
	IteratorArray
Notes:
	Creates a new iterator array start/array pair.
\*----------------------------------------------------------------------------*/

#define IteratorArray:%1[%2]<%3> %1@YSII_Cg[%2],%1@YSII_Ag[%2][%3+1]

/*----------------------------------------------------------------------------*\
Array:
	Iterator
Notes:
	Creates a new iterator start/array pair.
\*----------------------------------------------------------------------------*/

#define Iterator:%1<%2> _Y_ITER_C3:%1@YSII_Cg,%1@YSII_Ag[(%2)+1]={(%2)*2,(%2)*2-1,...}
#define iterator%0<%1> new Iterator:%0<%1>

/*----------------------------------------------------------------------------*\
Function:
	Iter_Init
Params:
	iter - Name of the iterator array to initialise.
Return:
	-
Notes:
	Wrapper for Iter_InitInternal.

native Iter_Init(IteratorArray:Name[]<>);

\*----------------------------------------------------------------------------*/

#define Iter_Init(%1) \
	Iter_InitInternal(%1@YSII_Ag,sizeof %1@YSII_Ag,sizeof %1@YSII_Ag[]-1)

/*----------------------------------------------------------------------------*\
Function:
	Iter_Add
Params:
	iter - Name of the iterator to add the data to.
	value - Value to add to the iterator.
Return:
	-
Notes:
	Wrapper for Iter_AddInternal.

native Iter_Add(Iterator:Name<>, value);

\*----------------------------------------------------------------------------*/

#define Iter_Add(%1,%2) Iter_AddInternal(_Y_ITER_ARRAY:%1@YSII_Cg,_Y_ITER_ARRAY:%1@YSII_Ag,%2,_Y_ITER_ARRAY_SIZE(%1))

/*----------------------------------------------------------------------------*\
Function:
	Iter_Free
Params:
	iter - Name of the iterator to get the first free slot in.
Return:
	-
Notes:
	Wrapper for Iter_FreeInternal.

native Iter_Free(Iterator:Name<>);

\*----------------------------------------------------------------------------*/

#define Iter_Free(%1) Iter_FreeInternal(_Y_ITER_ARRAY:%1@YSII_Ag,_Y_ITER_ARRAY_SIZE(%1))

/*----------------------------------------------------------------------------*\
Function:
	Iter_Remove
Params:
	iter - Name of the iterator to remove data from.
	value - Data to remove.
Return:
	-
Notes:
	Wrapper for Iter_RemoveInternal.

native Iter_Remove(Iterator:Name<>, value);

\*----------------------------------------------------------------------------*/

#define Iter_Remove(%1,%2) Iter_RemoveInternal(_Y_ITER_ARRAY:%1@YSII_Cg,_Y_ITER_ARRAY:%1@YSII_Ag,%2,_Y_ITER_ARRAY_SIZE(%1))

/*----------------------------------------------------------------------------*\
Function:
	Iter_Contains
Params:
	iter - Name of the iterator to check membership of.
	value - Value to check.
Return:
	-
Notes:
	Checks if the given value is in the given iterator.

native Iter_Remove(Iterator:Name<>, value);

\*----------------------------------------------------------------------------*/

#define Iter_Contains(%1,%2) Iter_ContainsInternal(_Y_ITER_ARRAY:%1@YSII_Ag,%2,_Y_ITER_ARRAY_SIZE(%1))

/*----------------------------------------------------------------------------*\
Function:
	Iter_SafeRemove
Params:
	iter - Name of the iterator to remove data from.
	value - Data to remove.
	next - Container for the pointer to the next element.
Return:
	-
Notes:
	Wrapper for Iter_SafeRemoveInternal. Common use:

	Iter_SafeRemove(iter, i, i);

native Iter_SafeRemove(Iterator:Name<>, value, &next);

\*----------------------------------------------------------------------------*/

#define Iter_SafeRemove(%1,%2,%3) Iter_SafeRemoveInternal(_Y_ITER_ARRAY:%1@YSII_Cg,_Y_ITER_ARRAY:%1@YSII_Ag,%2,%3,_Y_ITER_ARRAY_SIZE(%1))

/*----------------------------------------------------------------------------*\
Function:
	Iter_Random
Params:
	iter - Name of the iterator to get a random slot from.
Return:
	-
Notes:
	Wrapper for Iter_RandomInternal.

native Iter_Random(Iterator:Name<>);

\*----------------------------------------------------------------------------*/

#define Iter_Random(%1) Iter_RandomInternal(_Y_ITER_ARRAY:%1@YSII_Cg,_Y_ITER_ARRAY:%1@YSII_Ag,_Y_ITER_ARRAY_SIZE(%1))

/*----------------------------------------------------------------------------*\
Function:
	Iter_Count
Params:
	iter - Name of the iterator to get the number of items from.
Return:
	-
Notes:
	Returns the number of items in this iterator.

native Iter_Count(Iterator:Name<>);

\*----------------------------------------------------------------------------*/

#define Iter_Count(%1) (_Y_ITER_ARRAY:%1@YSII_Cg)

/*----------------------------------------------------------------------------*\
Function:
	Iter_Clear
Params:
	iter - Name of the iterator empty.
Return:
	-
Notes:
	Wrapper for Iter_ClearInternal.

native Iter_Clear(IteratorArray:Name[]<>);

\*----------------------------------------------------------------------------*/

#define Iter_Clear(%1) Iter_ClearInternal(_Y_ITER_ARRAY:%1@YSII_Cg,_Y_ITER_ARRAY:%1@YSII_Ag,_Y_ITER_ARRAY_SIZE(%1))

/*----------------------------------------------------------------------------*\
Create the internal iterators.
\*----------------------------------------------------------------------------*/

new
	Iterator:Player<MAX_PLAYERS>,
	Iterator:Bot<MAX_PLAYERS>,
	Iterator:Character<MAX_PLAYERS>,
	Iterator:Vehicle<MAX_PLAYERS>,
	Iterator:Actor<MAX_PLAYERS>;

#define NPC@YSII_Cg Bot@YSII_Cg
#define NPC@YSII_Ag Bot@YSII_Ag

/*----------------------------------------------------------------------------*\
Function:
	foreach
Params:
	data - Data to iterate through.
	as - Variable to set value to.
Return:
	-
Notes:
	Not exactly the same as PHP foreach, just iterates through a list and
	returns the value of the current slot but uses that slot as the next index
	too. Variables must be in the form @YSII_<gname>S for the start index and
	@YSII_<gname>A for the data array where <name> is what's entered in data.
\*----------------------------------------------------------------------------*/

#define foreach%1(%0) for(new Y_FOREACH_SECOND|||Y_FOREACH_THIRD|||%0|||)
// This allows us to use "new" multiple times - stripping off ONLY whole words.
#define new%0|||%9|||%1:%2||| %9|||%0|||%1|||%2|||

// This one is called if the new syntax is required, but the state of "new" is
// as-yet unknown. This attempts to call "%1" as a macro, if it starts with
// "new" as a whole word then it will (and will also helpfully strip off the
// "new" keyword for us).
#define Y_FOREACH_THIRD|||%0|||%1|||%2||| %1=Y_FOREACH_FIFTH|||Y_FOREACH_FOURTH|||%1:%2|||

// This is called if the "new" macro is called for a second time.
#define Y_FOREACH_FOURTH|||%0=Y_FOREACH_FIFTH|||%1|||%2||| new Y_FOREACH_SIXTH;%0|||Y_FOREACH_SEVENTH|||%2|||

// This is called when there are tags on the "new" declaration.
#define Y_FOREACH_SEVENTH|||%9Y_FOREACH_SIXTH;%0|||%1|||%2||| new %0:%1=%0:(_Y_ITER_ARRAY_SIZE(%2));_:(%1=%0:_Y_ITER_ARRAY:%2@YSII_Ag[_:%1])!=_Y_ITER_ARRAY_SIZE(%2);

// This is called when there aren't.
#define Y_FOREACH_SIXTH;%0|||Y_FOREACH_SEVENTH|||%2||| %0=_Y_ITER_ARRAY_SIZE(%2);_:(%0=_Y_ITER_ARRAY:%2@YSII_Ag[%0])!=_Y_ITER_ARRAY_SIZE(%2);

// This is called if "%1" didn't have "new" at the start.
#define Y_FOREACH_FIFTH|||Y_FOREACH_FOURTH|||%1:%2||| _Y_ITER_ARRAY_SIZE(%2);_:(%1=_Y_ITER_ARRAY:%2@YSII_Ag[%1])!=_Y_ITER_ARRAY_SIZE(%2);

// This is the old version, but DON'T add "new" because that already exists from
// the failed "new" macro call above.
#define Y_FOREACH_SECOND|||Y_FOREACH_THIRD|||%1,%2||| %2=_Y_ITER_ARRAY_SIZE(%1);_:(%2=_Y_ITER_ARRAY:%1@YSII_Ag[%2])!=_Y_ITER_ARRAY_SIZE(%1);

/*----------------------------------------------------------------------------*\
Function:
	Iter_OnFilterScriptInit
Params:
	-
Return:
	-
Notes:
	Fixes a bug where callbacks are not detected when "loadfs" is used after the
	GM has already started. If this is a GM this is just never used called.
\*----------------------------------------------------------------------------*/

public OnFilterScriptInit()
{
	// Player
	Iter_Clear(Player);
	Iter_Clear(Bot);
	Iter_Clear(Character);

	for (new i = 0; i != MAX_PLAYERS; ++i) {
		if (IsPlayerConnected(i)) {
			if (!IsPlayerNPC(i)) {
				Iter_Add(Player, i);
			} else {
				Iter_Add(Bot, i);
			}
			Iter_Add(Character, i);
		}
	}

	// Vehicle
	Iter_Clear(Vehicle);
	for (new i = 1; i != MAX_VEHICLES; ++i) {
		if (GetVehicleModel(i)) {
			Iter_Add(Vehicle, i);
		}
	}

	// Actor
	Iter_Clear(Actor);
	for (new i = 0; i != MAX_ACTORS; ++i) {
		if (IsValidActor(i)) {
			Iter_Add(Actor, i);
		}
	}

	#if defined Iter_OnFilterScriptInit
		return Iter_OnFilterScriptInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnFilterScriptInit
	#undef OnFilterScriptInit
#else
	#define _ALS_OnFilterScriptInit
#endif

#define OnFilterScriptInit Iter_OnFilterScriptInit
#if defined Iter_OnFilterScriptInit
	forward Iter_OnFilterScriptInit();
#endif

/*----------------------------------------------------------------------------*\
Function:
	Iter_OnGameModeInit
Params:
	-
Return:
	-
Notes:
	There are WIERD bugs in this script, seemingly caused by the compiler, so
	this hopefully fixes them. The OnGameModeInit code is written to be
	very fast by utilising the internal array structure instead of the regular
	Add functions.
\*----------------------------------------------------------------------------*/

public OnGameModeInit()
{
	// Player
	Iter_Clear(Player);
	Iter_Clear(Bot);
	Iter_Clear(Character);

	for (new i = 0; i != MAX_PLAYERS; ++i) {
		if (!IsPlayerConnected(i)) {
			continue;
		}

		if (!IsPlayerNPC(i)) {
			Iter_Add(Player, i);
		} else {
			Iter_Add(Bot, i);
		}
		Iter_Add(Character, i);
	}

	// Vehicle
	Iter_Clear(Vehicle);
	for (new i = 1; i != MAX_VEHICLES; ++i) {
		if (GetVehicleModel(i)) {
			Iter_Add(Vehicle, i);
		}
	}

	// Actor
	Iter_Clear(Actor);
	for (new i = 0; i != MAX_ACTORS; ++i) {
		if (IsValidActor(i)) {
			Iter_Add(Actor, i);
		}
	}

	#if defined Iter_OnGameModeInit
		return Iter_OnGameModeInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif

#define OnGameModeInit Iter_OnGameModeInit
#if defined Iter_OnGameModeInit
	forward Iter_OnGameModeInit();
#endif

/*----------------------------------------------------------------------------*\
Function:
	Iter_OnPlayerConnect
Params:
	playerid - Player who joined.
Return:
	-
Notes:
	Adds a player to the loop data. Now sorts the list too. Note that I found
	the most bizzare bug ever (I *think* it may be a compiler but, but it
	requires further investigation), basically it seems that multiple variables
	were being treated as the same variable (namely @YSII_EgotS and
	@YSII_CgharacterS were the same and @YSII_EgotC and @YSII_CgharacterC were the
	same). Adding print statements which reference these variables seem to fix
	the problem, and I have tried to make sure that the values will never actually
	get printed.
\*----------------------------------------------------------------------------*/

public OnPlayerConnect(playerid)
{
	if (!IsPlayerNPC(playerid)) {
		Iter_Add(Player, playerid);
	} else {
		Iter_Add(Bot, playerid);
	}
	Iter_Add(Character, playerid);

	#if defined Iter_OnPlayerConnect
		return Iter_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect Iter_OnPlayerConnect
#if defined Iter_OnPlayerConnect
	forward Iter_OnPlayerConnect(playerid);
#endif

/*----------------------------------------------------------------------------*\
Function:
	Iter_OnPlayerDisconnect
Params:
	playerid - Player who left.
Return:
	-
Notes:
	Removes a player from the loop data. No longer uses "hook" to ENSURE that
	this is always last. Previously I think that the order of evaluation in
	y_hooks meant that this got called before the user "OnPlayerDisconnect".
\*----------------------------------------------------------------------------*/

public OnPlayerDisconnect(playerid, reason)
{
	#if defined Iter_OnPlayerDisconnect
		Iter_OnPlayerDisconnect(playerid, reason);
	#else

	SetTimerEx("Iter_OPDCInternal", 0, false, "i", playerid);
	return 1;
}
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect Iter_OnPlayerDisconnect
#if defined Iter_OnPlayerDisconnect
	forward Iter_OnPlayerDisconnect(playerid, reason);
#endif

/*----------------------------------------------------------------------------*\
Function:
	Iter_CreateVehicle
Params:
	-
Return:
	-
Notes:
	-
\*----------------------------------------------------------------------------*/

stock Iter_CreateVehicle(modelid, Float:x, Float:y, Float:z, Float:angle, color1, color2, respawn_delay, addsiren = 0)
{
	new
		ret = CreateVehicle(modelid, x, y, z, angle, color1, color2, respawn_delay, addsiren);
	if (ret != INVALID_VEHICLE_ID) {
		Iter_Add(Vehicle, ret);
	}
	return ret;
}
#if defined _ALS_CreateVehicle
	#undef CreateVehicle
#else
	#define _ALS_CreateVehicle
#endif

#define CreateVehicle Iter_CreateVehicle

/*----------------------------------------------------------------------------*\
Function:
	Iter_AddStaticVehicle
Params:
	-
Return:
	-
Notes:
	-
\*----------------------------------------------------------------------------*/

stock Iter_AddStaticVehicle(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:angle, color1, color2)
{
	new
		ret = AddStaticVehicle(modelid, spawn_x, spawn_y, spawn_z, angle, color1, color2);
	if (ret != INVALID_VEHICLE_ID) {
		Iter_Add(Vehicle, ret);
	}
	return ret;
}
#if defined _ALS_AddStaticVehicle
	#undef AddStaticVehicle
#else
	#define _ALS_AddStaticVehicle
#endif

#define AddStaticVehicle Iter_AddStaticVehicle

/*----------------------------------------------------------------------------*\
Function:
	Iter_AddStaticVehicleEx
Params:
	-
Return:
	-
Notes:
	-
\*----------------------------------------------------------------------------*/

stock Iter_AddStaticVehicleEx(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:angle, color1, color2, respawn_delay, addsiren = 0)
{
	new
		ret = AddStaticVehicleEx(modelid, spawn_x, spawn_y, spawn_z, angle, color1, color2, respawn_delay, addsiren);
	if (ret != INVALID_VEHICLE_ID) {
		Iter_Add(Vehicle, ret);
	}
	return ret;
}
#if defined _ALS_AddStaticVehicleEx
	#undef AddStaticVehicleEx
#else
	#define _ALS_AddStaticVehicleEx
#endif

#define AddStaticVehicleEx Iter_AddStaticVehicleEx

/*----------------------------------------------------------------------------*\
Function:
	Iter_DestroyVehicle
Params:
	-
Return:
	-
Notes:
	-
\*----------------------------------------------------------------------------*/

stock Iter_DestroyVehicle(vehicleid)
{
	Iter_Remove(Vehicle, vehicleid);
	return DestroyVehicle(vehicleid);
}
#if defined _ALS_DestroyVehicle
	#undef DestroyVehicle
#else
	#define _ALS_DestroyVehicle
#endif

#define DestroyVehicle Iter_DestroyVehicle

/*----------------------------------------------------------------------------*\
Function:
	Iter_CreateActor
Params:
	-
Return:
	-
Notes:
	-
\*----------------------------------------------------------------------------*/

stock Iter_CreateActor(modelid, Float:X, Float:Y, Float:Z, Float:Rotation)
{
	new
		ret = CreateActor(modelid, X, Y, Z, Rotation);
	if (ret != INVALID_ACTOR_ID) {
		Iter_Add(Actor, ret);
	}
	return ret;
}
#if defined _ALS_CreateActor
	#undef CreateActor
#else
	#define _ALS_CreateActor
#endif

#define CreateActor Iter_CreateActor

/*----------------------------------------------------------------------------*\
Function:
	Iter_DestroyActor
Params:
	-
Return:
	-
Notes:
	-
\*----------------------------------------------------------------------------*/

stock Iter_DestroyActor(actorid)
{
	Iter_Remove(Actor, actorid);
	return DestroyActor(actorid);
}
#if defined _ALS_DestroyActor
	#undef DestroyActor
#else
	#define _ALS_DestroyActor
#endif

#define DestroyActor Iter_DestroyActor

/*----------------------------------------------------------------------------*\
Function:
	Iter_OPDCInternal
Params:
	playerid - Player who left.
Return:
	-
Notes:
	Called AFTER "OnPlayerDisconnect" so that using "Kick" inside a "foreach"
	loop does not crash the server due to an OOB error.
\*----------------------------------------------------------------------------*/

public Iter_OPDCInternal(playerid)
{
	if (IsPlayerConnected(playerid)) {
		return;
	}
	if (!IsPlayerNPC(playerid)) {
		Iter_Remove(Player, playerid);
	} else {
		Iter_Remove(Bot, playerid);
	}
	Iter_Remove(Character, playerid);
}

/*----------------------------------------------------------------------------*\
Function:
	Iter_RandomInternal
Params:
	count - Number of items in the iterator.
	array[] - Iterator data.
	size - Size of the iterator.
Return:
	-
Notes:
	Returns a random value from an iterator.
\*----------------------------------------------------------------------------*/

stock Iter_RandomInternal(count, array[], size)
{
	if (count == 0) {
		return ITER_NONE;
	}
	new
		rnd = random(count),
		cur = array[size];
	while (cur != size) {
		if (rnd-- == 0) {
			return cur;
		}
		cur = array[cur];
	}
	return ITER_NONE;
}

/*----------------------------------------------------------------------------*\
Function:
	Iter_FreeInternal
Params:
	array[] - Iterator data.
	size - Size of the iterator.
Return:
	-
Notes:
	Finds the first free slot in the iterator. Iterators now HAVE to be
	sorted for this function to work correctly as it uses that fact to decide
	whether a slot is unused or the last one. If you want to use the slot
	straight after finding it the iterator will need to re-find it to add in
	the data.
\*----------------------------------------------------------------------------*/

stock Iter_FreeInternal(array[], size)
{
	for (new i = 0; i != size; ++i) {
		if (array[i] > size) {
			return i;
		}
	}
	return ITER_NONE;
}

/*----------------------------------------------------------------------------*\
Function:
	Iter_AddInternal
Params:
	&count - Number of items in the iterator.
	array[] - Iterator data.
	value - Item to add.
	size - Size of the iterator.
Return:
	-
Notes:
	Adds a value to a given iterator set. Now detects when you try and add the
	last item multiple times, as well as all the other items. Now simplified even
	further with the new internal representation.
\*----------------------------------------------------------------------------*/

stock Iter_AddInternal(&count, array[], value, size)
{
	if (0 <= value < size && array[value] > size) {
		new
			last = size,
			next = array[last];
		while (next < value) {
			last = next;
			next = array[last];
		}
		array[last] = value;
		array[value] = next;
		++count;
		return 1;
	}
	return 0;
}

/*----------------------------------------------------------------------------*\
Function:
	Iter_RemoveInternal
Params:
	&count - Number of items in the iterator.
	array[] - Iterator data.
	value - Item to remove.
	size - Size of the iterator.
Return:
	-
Notes:
	Removes a value from an iterator.
\*----------------------------------------------------------------------------*/

stock Iter_RemoveInternal(&count, array[], value, size)
{
	new
		last;
	return Iter_SafeRemoveInternal(count, array, value, last, size);
}

/*----------------------------------------------------------------------------*\
Function:
	Iter_SafeRemoveInternal
Params:
	&count - Number of items in the iterator.
	array[] - Iterator data.
	value - Item to remove.
	&last - Pointer in which to store the last pointer.
	size - Size of the iterator.
Return:
	-
Notes:
	Removes a value from an iterator safely.
\*----------------------------------------------------------------------------*/

stock Iter_SafeRemoveInternal(&count, array[], value, &last, size)
{
	if (0 <= value < size && array[value] <= size) {
		last = size;
		new
			next = array[last];
		while (next != value) {
			last = next;
			next = array[last];
		}
		array[last] = array[value];
		array[value] = size + 1;
		--count;
		return 1;
	}
	return 0;
}

/*----------------------------------------------------------------------------*\
Function:
	Iter_ContainsInternal
Params:
	array[] - Iterator data.
	value - Item to check.
	size - Size of the iterator.
Return:
	-
Notes:
	Checks if this item is in the iterator.
\*----------------------------------------------------------------------------*/

stock Iter_ContainsInternal(array[], value, size)
{
	return 0 <= value < size && array[value] <= size;
}

/*----------------------------------------------------------------------------*\
Function:
	Iter_ClearInternal
Params:
	&count - Number of items in the iterator.
	array[] - Iterator data.
	size - Size of the iterator.
Return:
	-
Notes:
	Resets an iterator.
\*----------------------------------------------------------------------------*/

stock Iter_ClearInternal(&count, array[], size)
{
	for (new i = 0, t = size + 1; i < size; ++i) {
		array[i] = t;
	}
	array[size] = size;
	count = 0;
}

/*----------------------------------------------------------------------------*\
Function:
	Iter_InitInternal
Params:
	array[][] - Iterator array to initialise.
	s0 - Size of first dimension.
	s1 - Size of second dimension.
Return:
	-
Notes:
	Multi-dimensional arrays can not be initialised at compile time, so need to be
	done at run time, which is slightly annoying.
\*----------------------------------------------------------------------------*/

stock Iter_InitInternal(arr[][], s0, s1)
{
	for (new i = 0, t = s1 + 1; i < s0; ++i) {
		for (new j = 0; j < s1; ++j) {
			arr[i][j] = t;
		}
		arr[i][s1] = s1;
	}
}

/*----------------------------------------------------------------------------*\
Function:
	Iter_PrevInternal
Params:
	array[] - Iterator data.
	size - Size of the iterator.
	slot - The current slot.
Return:
	-
Notes:
	Gets the element in an iterator that points to the current element.
\*----------------------------------------------------------------------------*/

stock Iter_PrevInternal(array[], size, slot)
{
	if (0 <= slot <= size && array[slot] <= size) {
		for (new last = slot; last--; ) {
			if (array[last] == slot) {
				return last;
			}
		}
	}
	return size;
}

/*----------------------------------------------------------------------------*\
Function:
	Iter_Begin
Params:
	iter - Name of the iterator to get the start of.
Return:
	-
Notes:
	Gets a point BEFORE the start of the iterator (the theoretical beginning).
\*----------------------------------------------------------------------------*/

#define Iter_Begin(%1) (_Y_ITER_ARRAY_SIZE(%1))
#define Iter_Start Iter_Begin

/*----------------------------------------------------------------------------*\
Function:
	Iter_End
Params:
	iter - Name of the iterator to
Return:
	-
Notes:
	Gets a point AFTER the end of the iterator (think "MAX_PLAYERS").
\*----------------------------------------------------------------------------*/

#define Iter_End(%1) (_Y_ITER_ARRAY_SIZE(%1))
#define Iter_Finish Iter_End

/*----------------------------------------------------------------------------*\
Function:
	Iter_First
Params:
	iter - Name of the iterator to
Return:
	-
Notes:
	Gets the first element in an iterator.
\*----------------------------------------------------------------------------*/

#define Iter_First(%1) (_Y_ITER_ARRAY:%1@YSII_Ag[_Y_ITER_ARRAY_SIZE(%1)])

/*----------------------------------------------------------------------------*\
Function:
	Iter_Last
Params:
	iter - Name of the iterator to
Return:
	-
Notes:
	Gets the last element in an iterator.
\*----------------------------------------------------------------------------*/

#define Iter_Last(%1) Iter_PrevInternal(_Y_ITER_ARRAY:%1@YSII_Ag,_Y_ITER_ARRAY_SIZE(%1),_Y_ITER_ARRAY_SIZE(%1))

/*----------------------------------------------------------------------------*\
Function:
	Iter_Next
Params:
	iter - Name of the iterator to
Return:
	-
Notes:
	Gets the element in an interator after the current one.
\*----------------------------------------------------------------------------*/

#define Iter_Next(%1,%2) (_Y_ITER_ARRAY:%1@YSII_Ag[(%2)])

/*----------------------------------------------------------------------------*\
Function:
	Iter_Prev
Params:
	iter - Name of the iterator to
Return:
	-
Notes:
	Gets the element in an iterator before the current one. Slow.
\*----------------------------------------------------------------------------*/

#define Iter_Prev(%1,%2) Iter_PrevInternal(_Y_ITER_ARRAY:%1@YSII_Ag,_Y_ITER_ARRAY_SIZE(%1),(%2))
#define Iter_Previous Iter_Prev
