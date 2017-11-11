#include "HUD.h"

#define leftEdge(col) (col * ((WIDTH_TILES-(HORIZONTAL_MARGIN*2)) / 2)) + (HORIZONTAL_MARGIN)

/* clear area from c to c+w and draw text over it
 * (c is relative to x)
 */ 
static void drawText(char *text, u16 x, u16 y, u16 c, u16 w);

static void drawStatusMsg(u8 joy, u8 line);

static void createHUD(u8 player);
static void createHUDAll();
static void clearPlayerSide(u8 player);
static void createPauseMenu(u8 player);

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
const u32 note[16]=
{
	0x0000FF00,
	0x0000FFF0,
	0x0000F00F,
	0x0000F000,
	0x0000F000,
	0x0000F000,
	0x0000F000,
	0x0000F000,

	0x0000F000,
	0x0FFFF000,
	0xFFFFF000,
	0x0FFF0000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000
};

const u32 overscore[8]=
{
	0xFFFFFFFF,
	// 0xFFFFFFFF,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
};

const u8 statusCodes[9] = {
	STATUS_CODE_KEY,
	STATUS_CODE_KEY, //store key twice, because it's written on two lines
	STATUS_CODE_OCTAVE,
	STATUS_CODE_SUSTAIN,
	STATUS_CODE_MOD,
	STATUS_CODE_VIBRATO,
	STATUS_CODE_ARP,
	STATUS_CODE_HARMONY,
	STATUS_CODE_PORTA,
};

u8 test = 0;

void HUD_init(){
	VDP_loadTileData((const u32 *)circle, TILE_USERINDEX, 1, 0);
	VDP_loadTileData((const u32 *)circlePressed, TILE_USERINDEX+1, 1, 0);
	VDP_loadTileData((const u32 *)note, TILE_USERINDEX+2, 2, 0);
	VDP_loadTileData((const u32 *)overscore, TILE_USERINDEX+4, 2, 0);

	createHUDAll();

	// VDP_setTileMap(APLAN, 5, 24);

	// u16 pal[16];
	// VDP_getPalette(PAL0, pal);
	// for(i=0; i<16; i++){
	// 	char text[5];
	// 	intToHex(pal[i], text, 4);
	// 	VDP_drawText(text, 1 + (i/8 * 10), 17+ (i%8));
	// }
}

static void clearPlayerSide(u8 player){

	u8 i = 0;
	u8 x = (WIDTH_TILES/2) * player;

	char blankString[(WIDTH_TILES/2)+1];

	for (i=0; i<WIDTH_TILES/2; i++){
		blankString[i] = ' ';
	}
	blankString[WIDTH_TILES/2] = '\0';

	for (i=0; i<HEIGHT_TILES; i++){
		VDP_drawText(blankString, x, i);
	}
}

static void createHUD(u8 player){
	u16 i, x, y;

	// for (i=0; i<2; i++){
	// x = X0 + (player * (WIDTH_TILES/2));
	// if (player) x--;
	x = leftEdge(player);
	y = Y_STATUS;

	VDP_drawText("Tonic", x, y);
	VDP_drawText("Mode", x, y+1);
	VDP_drawText("Octave", x, y+2);
	VDP_drawText("Sustain", x, y+3);
	//hard-coded blank line - check Y_STATUS_BLANK_LINE if this messes things up
	y++;
	VDP_drawText("Mod depth", x, y+4);
	VDP_drawText("Vibrato", x, y+5);
	VDP_drawText("Arpeggio", x, y+6);
	if (!player) VDP_drawText("Harmony", x, y+7);
	else VDP_drawText("Noise", x, y+7);
	VDP_drawText("Portamento", x, y+8);

	if (!player){
		VDP_drawText("PLAYER  1", x+X_PLAYER_NAME, y+11);
	}
	else{
		VDP_drawText("PLAYER  2", x+X_PLAYER_NAME, y+11);
	}

	HUD_updateStatusView(player, 0, 0, 0, 0, 255);
	// }

	if (player){
		VDP_drawText("v0.2dev",32,25);
		VDP_drawText("COSSTROPOLIS.COM",23,26);
	}
}

static void createPauseMenu(u8 player){

	u8 x = leftEdge(player), y = Y_PAUSE;
	u8 i;

	VDP_drawText("      OPTIONS", x, y);
	//draw underline
	for (i=x+6; i<x+13; i++){
		VDP_setTileMapXY(APLAN, TILE_ATTR_FULL(PAL0, 0, 0, 0, TILE_USERINDEX+4), i, y+1);
	}

	VDP_drawText("Coming soon...", x, y+2);
	VDP_drawText("Double-tap START", x, y+5);
	VDP_drawText("to resume", x, y+6);
}

static void createHUDAll(){
	u8 i;
	for (i=0; i<2; i++){
		createHUD(i);
	}
}

void HUD_update(){

}

void HUD_pauseSet(u8 joy, u8 pauseState){
	clearPlayerSide(joy);

	if (! pauseState){
		createHUD(joy);
	}
	else {
		createPauseMenu(joy);
	}
}

void HUD_joyEvent(u16 joy, u16 changed, u16 state){

	// u8 bA = (BUTTON_A & state);
	// u8 bB = (BUTTON_B & state);
	// u8 bC = (BUTTON_C & state);
	// u8 bStart = BUTTON_START & state;

	// if (state || changed) HUD_updateStatusView(joy, bA, bB, bC, bStart);
}

void HUD_updateStatusView(u8 joy, u8 bA, u8 bB, u8 bC, u8 bStart, u8 statusCode){
	u8 i,j;
	u8 channel;
	char status[20];

	//for each line
	for (j=0;j<9;j++){
		if (statusCode & statusCodes[j]){
			drawStatusMsg(joy,j);
		}
	}

	// u16 x = X0 + (joy * (WIDTH_TILES/2));
	// if (joy) x--;
	// u16 x = (joy * ((WIDTH_TILES-(HORIZONTAL_MARGIN*2)) / 2)) + (HORIZONTAL_MARGIN);
	u16 x = leftEdge(joy);
	u16 y = Y_STATUS;

	if (bStart && statusCode){
		bA = bB = bC = 0;
	}
	else{
		if (bA > 0) bA = 1;
		if (bB > 0) bB = 1;
		if (bC > 0) bC = 1;
	}

	//draw either grey or white circles to represent the fingering buttons
	VDP_setTileMapXY(APLAN, TILE_ATTR_FULL(PAL0, 0, 0, 0, TILE_USERINDEX+bA), x+X_BUTTONS, y+Y_BUTTONS);
	VDP_setTileMapXY(APLAN, TILE_ATTR_FULL(PAL0, 0, 0, 0, TILE_USERINDEX+bB), x+X_BUTTONS+2, y+Y_BUTTONS);
	VDP_setTileMapXY(APLAN, TILE_ATTR_FULL(PAL0, 0, 0, 0, TILE_USERINDEX+bC), x+X_BUTTONS+4, y+Y_BUTTONS);

	char solfege[3];
	u8 mode = modeList[joy][keyIndex[joy]];
	u8 scalePitch = Instrument_buttonsToScalePitch(bA, bB, bC) % 7;
	u8 truePitch = SCALES[mode][scalePitch];
	switch (scalePitch){
		case 0:
			strcpy(solfege, "Do");
			break;
		case 1:
			strcpy(solfege, (truePitch == 1) ? "Ra" : "Re");
			break;
		case 2:
			strcpy(solfege, (truePitch == 3) ? "Me" : "Mi");
			break;
		case 3:
			strcpy(solfege, (truePitch == 6) ? "Fi" : "Fa");
			break;
		case 4:
			strcpy(solfege, (truePitch == 8) ? "Si" : (truePitch == 6) ? "Se" : "Sol");
			break;
		case 5:
			strcpy(solfege, (truePitch == 10) ? "Li" : (truePitch == 8) ? "Le" : "La");
			break;
		case 6:
			strcpy(solfege, (truePitch == 10) ? "Te" : "Ti");
			break;
		default:
			strcpy(solfege, "");
			break;
	}
	drawText(solfege, x+X_BUTTONS+6, y+Y_BUTTONS, 0, 3);

	// VDP_fillTileMapRectInc(APLAN, TILE_ATTR_FULL(PAL0, 0, 0, 0, TILE_USERINDEX+2), x+X_NOTE, 25, 1, 2);

	//draw musical note
	if (playing[joy]){
		VDP_fillTileMapRectInc(APLAN, TILE_ATTR_FULL(PAL0, 0, 0, 0, TILE_USERINDEX+2), x+X_NOTE, Y_NOTE, 1, 2);
	}
	else {
		VDP_clearTileMapRect(APLAN, x+X_NOTE, Y_NOTE, 1, 2);
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
			strcpy(text, (sus==SUS_STRUM) ? "STRUM" : (sus==SUS_HAMMER) ? "HAMMER" : "OFF");
			break;
		case 4:
			uintToStr(pitchModDepth[channel], text, 1);
			break;
		case 5:
			strcpy(text, (vibratoOn[channel]) ? "ON" : "OFF");
			break;
		case 7:
			if (!joy){
				strcpy(text, (harmonyOn) ? "ON" : "OFF");
			}
			else{
				// strcpy(text, (noiseOn) ? "ON" : "OFF");
				// if (noiseOn){
				// 	uintToStr(noiseOn, text, 1);
				// }
				// else{
				// 	strcpy(text, "OFF");
				// }
				strcpy(text, (noiseOn==NOISE_MODE_CHAN3PERIOD) ? "PERIOD" :
					(noiseOn==NOISE_MODE_CHAN3WHITE) ? "WHITE" :
					(noiseOn==NOISE_MODE_CLOCKWHITE) ? "CLOCK" :
					"OFF");
			}
			break;
		case 8:
			strcpy(text, (portamentoOn[channel]) ? "ON" : "OFF");
			break;
		case 6:
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

	//the `- joy` means subtract 1 from x if on 2p column
	// u8 x = X_STATVAL + (joy * (WIDTH_TILES/2)) - (joy);
	u8 x = X_STATVAL + leftEdge(joy);
	drawText(text, x, Y_STATUS+line, 0, 6);
}
