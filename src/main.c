#include <genesis.h>
#include "Instrument.h"
//#include "bad_apple.h"

static void joyEvent(u16 joy, u16 changed, u16 state);
void playMusic();

static void joyEvent(u16 joy, u16 changed, u16 state){
	Instrument_joyEvent(joy, changed, state);
	HUD_joyEvent(joy, changed, state);
}

int main(){
	
	//char debuglog[256];

	JOY_init();
	JOY_setEventHandler(&joyEvent);
	PSG_init();
	Instrument_init();
	HUD_init();

	//VDP_setTileMap(APLAN, TILE_ATTR_FULL(PAL1, 1, 1, 1, 1), 5, 5);

	///SND_startPlay_VGM(bad_apple);
	//main loop
	while(1)
	{
		Instrument_update();
		HUD_update();
		VDP_waitVSync();

		//scan for new controllers
	}

	return (0);
}
