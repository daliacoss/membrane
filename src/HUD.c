#include "HUD.h"

void HUD_init(){
	u16 i, x;
	for (i=0; i<2; i++){
		x = X0 + (i * (W_TILES/2));
		VDP_drawText("Octave", x, Y_STATUS);
		VDP_drawText("Sustain", x, Y_STATUS+1);
		VDP_drawText("Mod depth", x, Y_STATUS+2);
		VDP_drawText("Vibrato", x, Y_STATUS+3);
		if (!i) VDP_drawText("Harmony", x, Y_STATUS+4);
		else VDP_drawText("Noise", x, Y_STATUS+4);
		VDP_drawText("Portamento", x, Y_STATUS+5);

		HUD_updateStatusView(i);

	}

}
void HUD_update(){
	//updateStatusView();
}
void HUD_joyEvent(u16 joy, u16 changed, u16 state){

}

void HUD_updateStatusView(u8 joy){
	u8 i,j;
	u8 channel;
	char status[20];

	//for each line
	for (j=0;j<4;j++){
		drawStatusMsg(joy,j);		
	}
}

static void drawText(char *text, u16 x, u16 y, u16 c, u16 w){
	VDP_clearText(x+c, y, w);
	VDP_drawText(text, x, y);
}

static void drawStatusMsg(u8 joy, u8 line){
	u8 channel = (joy>0) ? CHANNEL_DEF_JOY1 : CHANNEL_DEF_JOY0;
	char text[20];
	char ntext[5];

	switch(line){
		case 0:
			uintToStr(octave[channel], text, 1);
			break;
		case 1:
			strcpy(text, (sustainOn[channel]) ? "ON" : "OFF");
			break;
		case 2:
			uintToStr(pitchModDepth[channel], text, 1);
			break;
		case 3:
			strcpy(text, (vibratoOn[channel]) ? "ON" : "OFF");
			break;
	}
	//draw in either 1p column or 2p column
	drawText(text, X_STATVAL + ((u16) joy * (W_TILES/2)), Y_STATUS+line, 0, 4);
}
