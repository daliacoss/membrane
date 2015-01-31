#include "HUD.h"

/* clear area from c to c+w and draw text over it
 * (c is relative to x)
 */ 
static void drawText(char *text, u16 x, u16 y, u16 c, u16 w);

static void drawStatusMsg(u8 joy, u8 line);

const u32 circle[8]=
{
	0x00222200,
	0x02222220,
	0x22222222,
	0x22222222,
	0x22222222,
	0x22222222,
	0x02222220,
	0x00222200
};

const u32 circlePressed[8]=
{
	0x00FFFF00,
	0x0FFFFFF0,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0xFFFFFFFF,
	0x0FFFFFF0,
	0x00FFFF00
};

void HUD_init(){
	u16 i, x, y;

	VDP_loadTileData((const u32 *)circle, TILE_USERINDEX, 1, 0);
	VDP_loadTileData((const u32 *)circlePressed, TILE_USERINDEX+8, 1, 0);

	for (i=0; i<2; i++){
		x = X0 + (i * (W_TILES/2));
		if (i) x--;
		y = Y_STATUS;

		VDP_drawText("Tonic", x, y);
		VDP_drawText("Mode", x, y+1);
		VDP_drawText("Octave", x, y+2);
		VDP_drawText("Sustain", x, y+3);
		//hard-coded blank line - check Y_STATUS_BLANK_LINE if this messes things up
		y++;
		VDP_drawText("Mod depth", x, y+4);
		VDP_drawText("Vibrato", x, y+5);
		if (!i) VDP_drawText("Harmony", x, y+6);
		else VDP_drawText("Noise", x, y+6);
		VDP_drawText("Portamento", x, y+7);
		VDP_drawText("Arpeggio", x, y+8);

		if (!i){
			VDP_drawText("PLAYER  1", x+X_PLAYER_NAME, y+11);
		}
		else{
			VDP_drawText("PLAYER  2", x+X_PLAYER_NAME, y+11);
		}

		HUD_updateStatusView(i, 0, 0, 0, 0);
	}
	VDP_drawText("COSSTROPOLIS.COM",23,26);

	// VDP_setTileMap(APLAN, 5, 24);

	// u16 pal[16];
	// VDP_getPalette(PAL0, pal);
	// for(i=0; i<16; i++){
	// 	char text[5];
	// 	intToHex(pal[i], text, 4);
	// 	VDP_drawText(text, 1 + (i/8 * 10), 17+ (i%8));
	// }
}
void HUD_update(){
	// char text[5];
	// uintToStr(currentPitchIndex[0], text, 1);
	// VDP_drawText(text, 10, 20);

	// fix16ToStr(sinFix16(900), text, 2);
	// VDP_drawText(text, 10, 21);
}

void HUD_joyEvent(u16 joy, u16 changed, u16 state){

}

void HUD_updateStatusView(u8 joy, u8 bA, u8 bB, u8 bC, u8 bStart){
	u8 i,j;
	u8 channel;
	char status[20];

	//for each line
	for (j=0;j<9;j++){
		drawStatusMsg(joy,j);		
	}

	u16 x = X0 + (joy * (W_TILES/2));
	if (joy) x--;
	u16 y = Y_STATUS;

	if (bStart){
		bA = bB = bC = 0;
	}
	else{
		if (bA > 0) bA = 1;
		if (bB > 0) bB = 1;
		if (bC > 0) bC = 1;
	}

	//draw either grey or white circles to represent the fingering buttons
	VDP_setTileMapXY(APLAN, TILE_ATTR_FULL(PAL0, 0, 0, 0, TILE_USERINDEX+bA*8), x+X_BUTTONS, y+Y_BUTTONS);
	VDP_setTileMapXY(APLAN, TILE_ATTR_FULL(PAL0, 0, 0, 0, TILE_USERINDEX+bB*8), x+X_BUTTONS+2, y+Y_BUTTONS);
	VDP_setTileMapXY(APLAN, TILE_ATTR_FULL(PAL0, 0, 0, 0, TILE_USERINDEX+bC*8), x+X_BUTTONS+4, y+Y_BUTTONS);
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
			strcpy(text, (sus==SUS_STRUM) ? "STRUM" : (sus==SUS_HAMMER) ? "HAMMER" : "OFF");
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
			strcpy(text, "???");
			break;
	}
	if (line >= Y_STATUS_BLANK_LINE){
		line++;
	}

	//draw in either 1p column or 2p column
	drawText(text, X_STATVAL + ((u16) joy * (W_TILES/2)), Y_STATUS+line, 0, 6);
}
