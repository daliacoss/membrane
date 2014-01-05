#include <genesis.h>
#include "Instrument.h"
//#include "music_tfd.h"

static void joyEvent(u16 joy, u16 changed, u16 state);
void playMusic();

static void joyEvent(u16 joy, u16 changed, u16 state){

	// char debug[20];
	// uintToStr((u32) joy, debug, 1);
	// VDP_drawText(debug, 2, 20);

	// // must be bitwise AND - maybe because it's u16?
	// if (joy == 0){
	// 	if (changed & BUTTON_START & state) playMusic();
	// }

	Instrument_joyEvent(joy, changed, state);
}

void playMusic(){
	static int playing = 0;
	static int frequency = 440;

	if (!playing || playing){
		PSG_setEnvelope(0, 2);
		PSG_setTone(0, 1000);
		PSG_setFrequency(0, frequency);
	}
	else{
		PSG_setFrequency(0, 0);
	}
	playing = ! playing;
	frequency += 30;
}

int main(){
	
	char debuglog[256];

	JOY_init();
	JOY_setEventHandler(&joyEvent);
	PSG_init();

	//VDP_setTileMap(APLAN, TILE_ATTR_FULL(PAL1, 1, 1, 1, 1), 5, 5);

	VDP_drawText("hello gumersindo", 2, 2);

	//main loop
	while(1)
	{

		VDP_waitVSync();
	}

	return (0);
}
