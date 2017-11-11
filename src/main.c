#include <genesis.h>
#include "Game.h"
#include "Instrument.h"

// static void joyEvent(u16 joy, u16 changed, u16 state);

// static void joyEvent(u16 joy, u16 changed, u16 state){
// 	Instrument_joyEvent(joy, changed, state);
// 	HUD_joyEvent(joy, changed, state);
// }

int main(){
	
	JOY_setEventHandler(&Game_joyEvent);
	JOY_setSupport(PORT_1, JOY_SUPPORT_6BTN);
	JOY_setSupport(PORT_2, JOY_SUPPORT_6BTN);
	PSG_init();

	Game_addInitCallback(&Instrument_init);
	Game_addInitCallback(&HUD_init);

	Game_addUpdateCallback(&Instrument_update);
	Game_addUpdateCallback(&HUD_update);

	Game_addPauseCallback(&Instrument_pauseSet);
	Game_addPauseCallback(&HUD_pauseSet);

	Game_addJoyEventCallback(&Instrument_joyEvent);
	Game_addJoyEventCallback(&HUD_joyEvent);

	// Instrument_init();
	// HUD_init();

	Game_init();

	//VDP_setTileMap(APLAN, TILE_ATTR_FULL(PAL1, 1, 1, 1, 1), 5, 5);

	//main loop
	while(1)
	{
		// Instrument_update();
		// HUD_update();
		Game_update();

		VDP_waitVSync();
	}

	return 0;
}
