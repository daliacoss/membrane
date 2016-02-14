#include "Game.h"

// void (* initCallbacks[16])();
// void (* updateCallbacks[16])();
// void (* joyEventCallbacks[16])();
// void (* pauseCallbacks[16])();
#define MAX_CALLBACKS 16

typedef struct ListVoidCallback {
	u8 length;
	VoidCallback list[MAX_CALLBACKS];
} ListVoidCallback;

typedef struct ListJoyEventCallback {
	u8 length;
	JoyEventCallback list[MAX_CALLBACKS];
} ListJoyEventCallback;

typedef struct ListPauseCallback {
	u8 length;
	PauseCallback list[MAX_CALLBACKS];
} ListPauseCallback;

ListVoidCallback initCallbacks;
ListVoidCallback updateCallbacks;
ListJoyEventCallback joyEventCallbacks;
ListPauseCallback pauseCallbacks;

u8 paused[2];

void Game_init(){

	u8 i;
	for (i=0; i<initCallbacks.length; i++){
		initCallbacks.list[i]();
	}
}

void Game_update(){

	u8 i;
	for (i=0; i<updateCallbacks.length; i++){
		updateCallbacks.list[i]();
	}
}

void Game_joyEvent(u16 joy, u16 changed, u16 state){

	u8 i;
	for (i=0; i<joyEventCallbacks.length; i++){
		joyEventCallbacks.list[i](joy, changed, state);
	}
}

void Game_setPaused(u8 joy, u8 state){

	paused[joy] = state;

	u8 i;
	for (i=0; i<pauseCallbacks.length; i++){
		pauseCallbacks.list[i](joy, state);
	}
}

u8 Game_getPaused(u8 joy){

	return paused[joy];
}

void Game_addInitCallback(VoidCallback callback){

	if (initCallbacks.length > MAX_CALLBACKS){
		return;
	}

	initCallbacks.list[initCallbacks.length] = callback;
	initCallbacks.length++;
}

void Game_addUpdateCallback(VoidCallback callback){

	if (updateCallbacks.length > MAX_CALLBACKS){
		return;
	}

	updateCallbacks.list[updateCallbacks.length] = callback;
	updateCallbacks.length++;
}
void Game_addJoyEventCallback(JoyEventCallback callback){

	if (joyEventCallbacks.length > MAX_CALLBACKS){
		return;
	}

	joyEventCallbacks.list[joyEventCallbacks.length] = callback;
	joyEventCallbacks.length++;
}
void Game_addPauseCallback(PauseCallback callback){

	if (pauseCallbacks.length > MAX_CALLBACKS){
		return;
	}

	pauseCallbacks.list[pauseCallbacks.length] = callback;
	pauseCallbacks.length++;
}
