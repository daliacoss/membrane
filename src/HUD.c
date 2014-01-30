#include "HUD.h"

void HUD_init(){
	u16 i, x;
	for (i=0; i<2; i++){
		x = X0 + (i * (W_TILES/2));
		VDP_drawText("Tonic", x, Y_STATUS);
		VDP_drawText("Mode", x, Y_STATUS+1);
		VDP_drawText("Octave", x, Y_STATUS+2);
		VDP_drawText("Sustain", x, Y_STATUS+3);
		VDP_drawText("Mod depth", x, Y_STATUS+4);
		VDP_drawText("Vibrato", x, Y_STATUS+5);
		if (!i) VDP_drawText("Harmony", x, Y_STATUS+6);
		else VDP_drawText("Noise", x, Y_STATUS+6);
		VDP_drawText("Portamento", x, Y_STATUS+7);
		VDP_drawText("Arpeggio", x, Y_STATUS+8);

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
	for (j=0;j<9;j++){
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
	u8 sus = sustainOn[channel];
	u8 ki = keyIndex[joy];

	switch(line){
		case 0:
			strcpy(text, (tonicList[joy][ki]==C) ? "C" : 
				(tonicList[joy][ki]==C_S) ? "C#/Db" : 
				(tonicList[joy][ki]==D) ? "D" : 
				(tonicList[joy][ki]==D_S) ? "D#/Eb" : 
				(tonicList[joy][ki]==E) ? "E" : 
				(tonicList[joy][ki]==F) ? "F" : 
				(tonicList[joy][ki]==F_S) ? "F#/Gb" : 
				(tonicList[joy][ki]==G) ? "G" : 
				(tonicList[joy][ki]==G_S) ? "G#/Ab" : 
				(tonicList[joy][ki]==A) ? "A" : 
				(tonicList[joy][ki]==A_S) ? "A#/Bb" : 
				(tonicList[joy][ki]==B) ? "B" : 
				"N/A");
			break;
		case 1:
			strcpy(text, (modeList[joy][ki]==MAJOR) ? "MAJOR" : 
				(modeList[joy][ki]==DORIAN) ? "DORIAN" : 
				(modeList[joy][ki]==PHRYGIAN) ? "PHRYG" : 
				(modeList[joy][ki]==LYDIAN) ? "LYDIAN" : 
				(modeList[joy][ki]==MIXOLYDIAN) ? "MXLYD" : 
				(modeList[joy][ki]==MINOR) ? "MINOR" : 
				(modeList[joy][ki]==LOCRIAN) ? "LOCRI" : 
				(modeList[joy][ki]==MINOR_MEL) ? "M MEL" : 
				(modeList[joy][ki]==MINOR_HARM) ? "M HARM" : 
				"N/A");
			break;
		case 2:
			uintToStr(octave[channel], text, 1);
			break;
		case 3:
			strcpy(text, (sus==SUS_STRUM) ? "STRUM" : (sus==SUS_FREE) ? "FREE" : "OFF");
			break;
		case 4:
			uintToStr(pitchModDepth[channel], text, 1);
			break;
		case 5:
			strcpy(text, (vibratoOn[channel]) ? "ON" : "OFF");
			break;
		case 8:
			strcpy(text, (arpeggioOn[channel]) ? "ON" : "OFF");
			break;
		default:
			strcpy(text, "");
			break;
	}
	//draw in either 1p column or 2p column
	drawText(text, X_STATVAL + ((u16) joy * (W_TILES/2)), Y_STATUS+line, 0, 6);
}
