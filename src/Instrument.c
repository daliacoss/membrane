#include "Instrument.h"

const u16 PITCHES[] = {0,
	16, 17, 18, 19, 21, 22, 23, 25, 26, 28, 29, 31,
	33, 35, 37, 39, 41, 44, 46, 49, 52, 55, 58, 62,
	66, 70, 74, 78, 83, 87, 93, 98, 104, 110, 117, 124,
	131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247,
	262, 278, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494,
	524, 556, 588, 622, 660, 698, 740, 784, 830, 880, 932, 988,
	1048, 1112, 1176, 1244, 1320, 1396, 1480, 1568, 1660, 1760, 1864, 1976,
	2096, 2224, 2352, 2488, 2640, 2792, 2960, 3136, 3320, 3520, 3728, 3952
};
const u16 LEN_PITCHES = sizeof(PITCHES) / sizeof(PITCHES[0]);
const u16 SCALE_MAJ[] = {0,2,4,5,7,9,11,12};
u16 currentPitchIndex[4] = {0,0,0,0};
u8 envelope[4] = {0,0,0,0};
u8 sustainOn[4] = {0,0,0,0};
u16 scale[4][8];
u8 vibratoOn[4] = {0,0,0,0};
s16 vibratoY[4] = {0,0,0,0};
u16 vibratoX[4] = {0,0,0,0};
u16 vibratoDepth[4] = {5,20,20,20};
u16 vibratoSpeed[4] = {60,80,80,80};

void Instrument_init(){
	int i,j;
	for(i=0; i<3; i++){
		//PSG_setEnvelope(i, envelope[i]);
		for (j=0; j<8; j++){
			scale[i][j] = SCALE_MAJ[j];
		}
	}
}

void Instrument_update(){
	int i;
	//for each non-noise channel
	for (i=0; i<3; i++){
		updateVibrato(i);
		if (envelope[i]) Instrument_playNote(i, envelope[i]);
	}
	/*char debugLog[20];
	intToStr(vibratoY[0], debugLog, 1);
	VDP_clearText(2, 20, 20);
	VDP_drawText(debugLog, 2, 20);*/
}

void Instrument_joyEvent(u16 joy, u16 changed, u16 state){
	if (changed){
		u8 channel = (joy>0) ? CHANNEL_DEF_JOY1 : CHANNEL_DEF_JOY0;
		s8 pitchMod = 0;
		static u8 pitchModAbs = 1;

		u8 bA = (BUTTON_A & state);
		u8 bB = (BUTTON_B & state);
		u8 bC = (BUTTON_C & state);

		//control
		//s (doubletap): menu
		//As: vibrato
		//Bs: harmony (noise if 2p)
		//Cs: arpeggio or portamento
		//X: octave shift down
		//Y: sustain
		//Z: octave shift up
		//Xs: key shift down
		//Ys: pitchmod depth
		//Zs: key shift up

		if (BUTTON_X & state & changed){
			vibratoOn[channel] = ! (vibratoOn[channel]);
		}
		if (BUTTON_Y & state & changed){
			//if start is pressed, y button changes modulation depth
			if (BUTTON_START & state) pitchModAbs = (pitchModAbs==1) ? OCT : 1;
			//if start not pressed, y button toggles sustainOn
			else sustainOn[channel] = ! (sustainOn[channel]);
		}
		if (BUTTON_UP & state){
			pitchMod = pitchModAbs;
		}
		else if (BUTTON_DOWN & state){
			pitchMod = -pitchModAbs;
		}

		//play
		//if any button is pressed and right is held, note will play
		if (BUTTON_RIGHT & state){
			envelope[channel] = ENV_DEFAULT;
			setCPI(channel, scale[channel][buttonsToScalePitch(bA, bB, bC)], pitchMod);
		}
		else if (BUTTON_LEFT & state) Instrument_stopNote(channel);
		else if (!sustainOn[channel]) Instrument_stopNote(channel);
	}
}

void Instrument_playNote(u8 channel, u8 envelope){
	u16 freq = PITCHES[currentPitchIndex[channel]];
	if (freq && vibratoOn[channel]){
		freq += vibratoY[channel] - vibratoDepth[channel];
	}
	PSG_setEnvelope(channel, envelope);
	PSG_setFrequency(channel, freq);
}

void Instrument_stopNote(u8 channel){
	envelope[channel] = 0;
	//this will init for all channels, so playNote must be called every frame
	PSG_init();
}

/* get unmodified pitch from button combination using binary fingering
(bC is lowest significant bit) */
static u8 buttonsToScalePitch(u8 bA, u8 bB, u8 bC){
	bA = (bA>0) ? 1 : 0;
 	bB = (bB>0) ? 1 : 0;
  	bC = (bC>0) ? 1 : 0;

  	return bA*4 + bB*2 + bC;
}

static void setCPI(u8 channel, u8 scalePitch, s8 pitchMod){
	//replace 4 with currentOctave[channel]
	//+1 is neccessary as the first element of PITCHES is 0 Hz
	currentPitchIndex[channel] = 1 + scalePitch + pitchMod + (4 * OCT);
}

static void updateVibrato(u8 i){
	fix16 fVibY;
	if (vibratoOn[i]){
		//calculate amplitude from sine of vibratoX
		fVibY = sinFix16(vibratoX[i]);
		//convert amplitude to u16 fraction of vibratoDepth
		vibratoY[i] = fix16ToInt(fix16Mul(fVibY, intToFix16(vibratoDepth[i])));

		//increment vibratoX
		vibratoX[i] += vibratoSpeed[i];
		if (vibratoX[i] > 512) vibratoX[i] = 0;
	}
	else{
		vibratoY[i] = 0;
	}
}

static u32 clamp(u32 n, u32 min, u32 max){
	return (n < min) ? min : (n > max) ? max : n;
}
