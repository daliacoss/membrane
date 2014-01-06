#include "Instrument.h"

const u16 PITCHES[] = {
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
u8 sustain[4] = {0,0,0,0};
u16 scale[4][8];

void Instrument_init(){
	int i,j;
	for(i=0; i<4; i++){
		for (j=0; j<8; j++){
			scale[i][j] = SCALE_MAJ[j];		
		}
	}
}

void Instrument_joyEvent(u16 joy, u16 changed, u16 state){
	if (changed){
		u8 channel = (joy>0) ? 1 : 0;
		s8 pitchMod = 0;
		static s8 pitchModAbs = 1;

		u8 bA = (BUTTON_A & state);
		u8 bB = (BUTTON_B & state);
		u8 bC = (BUTTON_C & state);

		//control
		if (BUTTON_Y & state & changed){
			//if start is pressed, y button changes modulation depth
			if (BUTTON_START & state) pitchModAbs = (pitchModAbs==1) ? OCT : 1;
			//if start not pressed, y button toggles sustain
			else sustain[channel] = ! (sustain[channel]);
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
			u16 freq = buttonsToFreq(channel, bA, bB, bC, pitchMod+(4 * OCT));
			Instrument_playNote(channel, freq, 2);
		}
		else if (BUTTON_LEFT & state) Instrument_stopNote(channel);
		else if (!sustain[channel]) Instrument_stopNote(channel);
	}
}

void Instrument_playNote(u8 channel, u16 frequency, u8 envelope){
	PSG_setEnvelope(channel, envelope);
	PSG_setFrequency(channel, frequency);
}

void Instrument_stopNote(u8 channel){
	PSG_setFrequency(channel, 0);
}

/* get pitch from button combination using binary fingering
(bC is lowest significant bit) */
static u16 buttonsToFreq(u8 channel, u8 bA, u8 bB, u8 bC, s16 modifier){

	bA = (bA>0) ? 1 : 0;
	bB = (bB>0) ? 1 : 0;
	bC = (bC>0) ? 1 : 0;

	u16 index = scale[channel][bA*4 + bB*2 + bC] + modifier;

	return PITCHES[(u16) clamp(index, 0, LEN_PITCHES)];
}

static u32 clamp(u32 n, u32 min, u32 max){
	return (n < min) ? min : (n > max) ? max : n;
}
