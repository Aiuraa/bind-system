# SA:MP Pawn Keybind

[![sampctl](https://img.shields.io/badge/sampctl-samp--pawn--keybind-2f2f2f.svg?style=for-the-badge)](https://github.com/Kirima2nd/samp-pawn-keybind)

Simple keybind system with several types, making it more dynamic and easy to use.

## Installation

Simply install to your project:

```bash
sampctl package install Aiuraa/samp-pawn-keybind
```

Include in your code and begin using the library:

```pawn
#include <keybind>
```

## Function Lists

Callbacks:
```pawn
// Called when you set the key press
forward Bind_OnKeyPressed(playerid, const string:action[]);
```

Functions:
```pawn
// Check if keybind is valid
forward Keybind_IsValid(Keybind:bind);

// Create keybind for certain keys.
forward Keybind:Keybind_Create(key, BindAction:action, string:actionMessage[] = "");

// Assign keybind to player (Will automatically removed when disconnect)
forward Keybind_AssignToPlayer(Keybind:bind, player);

// Set trigger state for player that already assigned to the keybind
forward bool:Keybind_SetTriggerState(Keybind:bind, player, triggerState)

// Set/Change Keybind keys
// More info, see: https://sampwiki.blast.hk/wiki/Keys
forward Keybind_SetKey(Keybind:bind, key);

// Set/Change Keybind action
forward Keybind_SetAction(Keybind:bind, KeybindAction:action, string:actionMessage[] = "");

// Remove player & trigger state from keybind manually
forward Keybind_RemovePlayer(Keybind:bind, player);

// Remove Keybind from the server
forward Keybind_Remove(Keybind:bind);
```

## Usage

```pawn
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
```

## Testing

To test, simply run the package:

```bash
sampctl package run
```
