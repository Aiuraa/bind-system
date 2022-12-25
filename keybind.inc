/*
    SA:MP Pawn Keybind System v2.0
    By: Kirima2nd

    THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, 
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS 
    OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER 
    IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
    WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#if !defined MAX_KEYBIND__
    #if !defined MAX_KEYBIND
        #define MAX_KEYBIND 200
    #endif

    #define MAX_KEYBIND__ Keybind:MAX_KEYBIND
#endif

#if !defined MAX_KEYBIND_ACTION_STRING
    #define MAX_KEYBIND_ACTION_STRING 128
#endif

#define INVALID_KEYBIND_ID Keybind:-1

#define string:

static stock
    Keybind:Keybind_g_sTotalKeybinds,
    bool:Keybind_g_sExists[MAX_KEYBIND__],
    Keybind_g_sAssignKey[MAX_KEYBIND__],
    KeybindAction:Keybind_g_sActionType[MAX_KEYBIND__],
    string:Keybind_g_sActionMessage[MAX_KEYBIND__][MAX_KEYBIND_ACTION_STRING],
    bool:Keybind_g_sAssignToPlayer[MAX_KEYBIND__][MAX_PLAYERS],
    Keybind_g_sTriggerState[MAX_KEYBIND__][MAX_PLAYERS]
;

enum KeybindAction 
{
    ACTION_CHAT,
    ACTION_COMMAND,
    ACTION_CUSTOM
};


static stock Keybind_StrCopy__(dest[], const source[], maxlength = sizeof (dest))
{
    return strcat((dest[0] = EOS, dest), source, maxlength);
}

static stock Keybind:Keybind_GetFreeIndex__()
{
    for (new Keybind:i; i < MAX_KEYBIND__; i ++)
    {
        if (!Keybind_g_sExists[i])
        {
            Keybind_g_sExists[i] = true;
            Keybind_g_sTotalKeybinds ++;
            return i;
        }
    }
    return INVALID_KEYBIND_ID;
}

static stock void:Keybind_ResetVariable__(&Keybind:bind)
{
    Keybind_g_sExists[bind] = false;
    Keybind_g_sAssignKey[bind] = 0;
    Keybind_g_sActionType[bind] = ACTION_NONE;
    Keybind_g_sActionMessage[bind][0] = EOS;

    for (new i; i < MAX_PLAYERS; i ++)
    {
        Keybind_g_sAssignToPlayer[bind][i] = INVALID_PLAYER_ID;
        Keybind_g_sTriggerState[bind][i] = false;
    }

    bind = INVALID_KEYBIND_ID;
}

static stock bool:Keybind_IsKeyPressed__(Keybind:bind, newkeys, oldkeys)
{
    new key = Keybind_g_sAssignKey[bind];
    return ((newkeys & key) && !(oldkeys & key));
}

static stock void:Keybind_CallAction__(Keybind:bind, player)
{
    new KeybindAction:action = Keybind_g_sActionType[bind];
    if (action == ACTION_CHAT)
    {
        CallRemoteFunction("OnPlayerText", "is", player, Keybind_g_sActionMessage[bind]);
    }

    else if (action == ACTION_COMMAND)
    {
        CallRemoteFunction("OnPlayerCommandText", "is", player, Keybind_g_sActionMessage[bind]);
    }

    else if (action == ACTION_CUSTOM)
    {
        CallRemoteFunction("Keybind_OnKeyPressed", "is", player, Keybind_g_sActionMessage[bind]);
    }
}

stock bool:Keybind_IsValid(Keybind:bind)
{
    return ((0 <= _:bind <= MAX_KEYBIND) && Keybind_g_sExists[bind]);
}

stock Keybind:Keybind_Create(key, KeybindAction:action, const string:actionMessage[] = "")
{
    new Keybind:idx = Keybind_GetFreeIndex__();

    if (idx == INVALID_KEYBIND_ID)
    {
        print("[Keybind System Error]: There is no free index left, please edit MAX_KEYBIND to the value more than "#MAX_KEYBIND".");
        return INVALID_KEYBIND_ID;
    }

    if (!(ACTION_CHAT <= action <= ACTION_CUSTOM))
    {
        print("[Keybind System Error]: Invalid action on Keybind_Create");
        return INVALID_KEYBIND_ID;
    }

    Keybind_g_sAssignKey[idx] = key;
    Keybind_g_sActionType[idx] = action;

    strcopy(Keybind_g_sActionMessage[idx], actionMessage, MAX_KEYBIND_ACTION_STRING);

    if (action == ACTION_COMMAND && actionMessage[0] != '/')
    {
        strins(Keybind_g_sActionMessage[idx], "/", 0, MAX_KEYBIND_ACTION_STRING);
    }
    return idx;
}

stock bool:Keybind_SetKey(Keybind:bind, &key)
{
    if (!Keybind_IsValid(bind))
    {
        printf("[Keybind System Error]: Invalid bind %d specified on Keybind_SetKey", _:bind);
        return false;
    }

    Keybind_g_sAssignKey[idx] = key;
    return true;
}

stock bool:Keybind_SetAction(Keybind:bind, KeybindAction:action, string:actionMessage[] = "", {Float, _}:...)
{
    if (!Keybind_IsValid(bind))
    {
        printf("[Keybind System Error]: Invalid bind %d specified on Keybind_SetAction", _:bind);
        return false;
    }

    if (!(ACTION_CHAT <= action <= ACTION_CUSTOM))
    {
        print("[Keybind System Error]: Invalid action on SetKeybindAction");
        return false;
    }

    Keybind_g_sActionType[idx] = action;
    format(Keybind_g_sActionMessage[idx], MAX_KEYBIND_ACTION_STRING, action, ___(3));

    if (action == ACTION_COMMAND && action[0] != '/')
    {
        strins(Keybind_g_sActionMessage[idx], "/", 0, MAX_KEYBIND_ACTION_STRING);
    }
    return true;
}

stock bool:Keybind_AssignToPlayer(Keybind:bind, player)
{
    if (!Keybind_IsValid(bind))
    {
        printf("[Keybind System Error]: Invalid bind %d specified on Keybind_AssignPlayer", _:bind);
        return false;
    }

    if (!IsPlayerConnected(player))
    {
        printf("[Keybind System Error]: Invalid player %d specified on Keybind_AssignPlayer", player);
        return false;
    }

    if (Keybind_g_sAssignToPlayer[bind][player])
    {
        printf("[Keybind System Error]: player %d is already assigned on Keybind_AssignPlayer", player);
        return false;
    }

    Keybind_g_sAssignToPlayer[bind][player] = true;
    return true;
}

stock bool:Keybind_SetTriggerState(Keybind:bind, player, triggerState)
{
    if (!Keybind_IsValid(bind))
    {
        printf("[Keybind System Error]: Invalid bind %d specified on Keybind_SetTriggerState", _:bind);
        return false;
    }

    if (!IsPlayerConnected(player))
    {
        printf("[Keybind System Error]: Invalid player %d specified on Keybind_SetTriggerState", player);
        return false;
    }

    if (!Keybind_g_sAssignToPlayer[bind][player])
    {
        printf("[Keybind System Error]: player %d is not assigned yet on Keybind_SetTriggerState", player);
        return false;
    }

    if (!(PLAYER_STATE_ONFOOT <= triggerState <= PLAYER_STATE_PASSENGER))
    {
        print("[Keybind System Error]: Invalid triggerState on Keybind_SetTriggerState");
        return false;
    }

    Keybind_g_sTriggerState[bind][player]  = triggerState;
    return true;
}

stock bool:Keybind_RemovePlayer(Keybind:bind, player)
{
    if (!Keybind_IsValid(bind))
    {
        printf("[Keybind System Error]: Invalid bind %d specified on Keybind_RemovePlayer", _:bind);
        return false;
    }

    if (!Keybind_g_sAssignToPlayer[bind][player])
    {
        printf("[Keybind System Error]: Player %d is never assigned to bind %d on Keybind_RemovePlayer", player, _:bind);
        return false;
    }

    if (player == INVALID_PLAYER_ID)
    {
        printf("[Keybind System Error]: Invalid player %d specified on Keybind_AssignPlayer", player);
        return false;
    }

    Keybind_g_sAssignToPlayer[bind][player] = false;
    return true;
}

stock bool:Keybind_Remove(Keybind:bind)
{
    if (!Keybind_IsValid(bind))
    {
        printf("[Keybind System Error]: Invalid bind %d specified on Keybind_Remove", _:bind);
        return false;
    }

    Keybind_ResetVariable__(Keybind:bind);
    Keybind_g_sTotalKeybinds --;
    return 1;
}

// Internal Callback
public OnPlayerDisconnect(playerid, reason)
{
    new ret = 1;
    #if defined KeBi_OnPlayerDisconnect
    ret = KeBi_OnPlayerDisconnect(playerid, reason);
    #endif

    for (new Keybind:i; i < Keybind_g_sTotalKeybinds; i ++)
    {
        if (Keybind_g_sAssignToPlayer[i][playerid])
        {
            Keybind_RemovePlayer(i, playerid);
        }
        
    }
    return ret;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    new ret = 1;
    #if defined KeBi_OnPlayerKeyStateChange
    ret = KeBi_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
    #endif

    for (new Keybind:i; i < Keybind_g_sTotalKeybinds; i ++)
    {
        if (!Keybind_g_sAssignToPlayer[i][playerid])
        {
            continue;
        }

        if (!Keybind_IsKeyPressed__(i, newkeys, oldkeys))
        {
            continue;
        }

        if (GetPlayerState(playerid) != Keybind_g_sTriggerState[i][playerid])
        {
            continue;
        }

        Keybind_CallAction__(i, playerid);
    }
    return ret;
}

#if defined _ALS_OnPlayerKeyStateChange
    #undef OnPlayerKeyStateChange
#else
    #define _ALS_OnPlayerKeyStateChange
#endif

#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerKeyStateChange KeBi_OnPlayerKeyStateChange
#define OnPlayerDisconnect KeBi_OnPlayerDisconnect

#if defined KeBi_OnPlayerKeyStateChange
    forward KeBi_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#endif

#if defined KeBi_OnPlayerDisconnect
    forward KeBi_OnPlayerDisconnect(playerid, reason);
#endif

#if defined Keybind_OnKeyPressed
    forward Keybind_OnKeyPressed(playerid, const string:action[]);
#endif
