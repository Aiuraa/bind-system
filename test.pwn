#include <a_samp>
#include <keybind>

new Keybind:enterKeybind; 

main()
{
    // Create keybind N for entering any property command
    enterKeybind = Keybind_Create(KEY_NO, ACTION_COMMAND, "/enter");
    return 1;
}

public OnPlayerConnect(playerid)
{
    // Assign keybind to playerid
    Keybind_AssignToPlayer(enterKeybind, playerid);

    // And then set the trigger state so it can be executed when player is ONFOOT only.
    Keybind_SetTriggerState(enterKeybind, playerid, PLAYER_STATE_ONFOOT);
    return 1;
}
