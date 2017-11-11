#ifndef _GAME_H_
#define _GAME_H_

#include <genesis.h>

typedef void (* VoidCallback)();
typedef void (* JoyEventCallback)(u16 joy, u16 changed, u16 state);
typedef void (* PauseCallback)(u16 joy, u8 state);

void Game_init();
void Game_update();
void Game_joyEvent(u16 joy, u16 changed, u16 state);
void Game_setPaused(u8 joy, u8 state);
u8 Game_getPaused(u8 joy);

void Game_addInitCallback(VoidCallback callback);
void Game_addUpdateCallback(VoidCallback callback);
void Game_addJoyEventCallback(JoyEventCallback callback);
void Game_addPauseCallback(PauseCallback callback);

#endif