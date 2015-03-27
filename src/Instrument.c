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
// const u16 SCALE_MAJ[] = {0,2,4,5,7,9,11,12};
const u16 SCALES[9][SCALE_LENGTH] = {
	{0,2,4,5,7,9,11,12},
	{0,2,3,5,7,9,10,12},
	{0,1,3,5,7,8,10,12},
	{0,2,4,6,7,9,11,12},
	{0,2,4,5,7,9,10,12},
	{0,2,3,5,7,8,10,12},
	{0,1,3,5,6,8,10,12},
	{0,2,3,5,7,9,11,12}, //melodic minor
	{0,2,3,5,7,8,11,12}, //harmonic minor
};
//channel settings
u16 scale[4][SCALE_LENGTH];
u16 currentPitchIndex[4] = {0,0,0,0};
u16 lastPitchIndex[4] = {0,0,0,0};
u8 harmonyInterval = 2;	//harmony only available for player 1
u8 octave[4] = {4,4,4,4};
u8 pitchModDepth[4] = {1,1,1,1};
s8 pitchModState[4] = {0,0,0,0};
u8 envelope[4] = {0,0,0,0};
u8 sustainOn[4] = {0,0,0,0};
u8 arpeggioOn[4] = {0};
u8 arpeggioIndex[4] = {0,0,0,0};
u16 arpeggioX[4] = {0,0,0,0};
u8 arpeggioNoteLen[4] = {2,2,7,7};
u8 arpeggioPattLen[4] = {2,2,8,8};
s16 arpeggioPattern[4][16] = {
	{0,12},
	{0,12},
	{0,0,12,7,0,5,7,0},
	{0,0,12,7,0,5,7,0}
};
u8 portamentoOn[4] = {0,0,0,0};
u16 portamentoX[4] = {0,0,0,0};
s32 portamentoY[4] = {0,0,0,0};
u16 portamentoSpeed[4] = {100,100,100,100};
u8 vibratoOn[4] = {0,0,0,0};
s16 vibratoY[4] = {0,0,0,0};
u16 vibratoX[4] = {0,0,0,0};
fix16 vibratoDepth[4] = {FIX16(.4),FIX16(.4),FIX16(.4),FIX16(.4)};
u16 vibratoSpeed[4] = {60,80,80,80};
u8 harmonyOn = 0;
u8 noiseOn = 0;
// u8 noiseType;
// u8 noiseFreq;

//player settings
u8 keyIndex[2];
u8 debugvar = 0;
u8 tonicList[2][MAX_KEYS] = {
	{C,A,E,G,D,D_S,F,A_S,B,C},
	{C,A,E,G,D,D_S,F,A_S,B,C},
};
u8 modeList[2][MAX_KEYS] = {
	{MAJOR,MINOR,PHRYGIAN,MINOR_HARM,DORIAN,MINOR,MINOR_MEL,LOCRIAN,MAJOR,MINOR},
	{MAJOR,MINOR,PHRYGIAN,MINOR_HARM,DORIAN,MINOR,MINOR_MEL,LOCRIAN,MAJOR,MINOR},
};
u8 noiseModes[MAX_NOISE_MODES][2] = {
	{PSG_NOISE_TYPE_PERIODIC, PSG_NOISE_FREQ_TONE3},
	{PSG_NOISE_TYPE_WHITE, PSG_NOISE_FREQ_TONE3},
	{PSG_NOISE_TYPE_WHITE, PSG_NOISE_FREQ_CLOCK2},
	{PSG_NOISE_TYPE_WHITE, PSG_NOISE_FREQ_CLOCK4},
	{PSG_NOISE_TYPE_WHITE, PSG_NOISE_FREQ_CLOCK8},
};
u8 paused[2] = {0};
u8 playing[2] = {0,0};


//static u16 buttonsToFreq(u8, u8, u8, u8, s16 modifier);
static void setCPI(u8 channel, u8 scalePitch, s8 pitchMod);
static void updateVibrato(u8 channel);
static void updateArpeggio(u8 channel);
static void updatePortamento(u8 channel);
static u32 clamp(u32 n, u32 min, u32 max);
static void setKey(u8 joy, u8 ki);
static void restartPortamento(u8 channel);
static void changeNoiseMode();

void Instrument_init(){
	int i,j;
	for(i=0; i<2; i++){
		setKey(i, 0);
	}
}

void Instrument_update(){
	u16 i;
	//for each channel
	for (i=0; i<4; i++){
		if (i != CHANNEL_DEF_NOISE){
			updateVibrato(i);
			updateArpeggio(i);
			updatePortamento(i);
		}
		if (envelope[i]) Instrument_playNote(i, envelope[i]);
	}

	octave[CHANNEL_DEF_HARMONY] = octave[CHANNEL_DEF_JOY0];
	// char debugLog[20];
	// intToStr(keyIndex[0], debugLog, 1);
	// VDP_clearText(2, 20, 20);
	// VDP_drawText(debugLog, 2, 20);
}

void Instrument_joyEvent(u16 joy, u16 changed, u16 state){

	u8 bA = (BUTTON_A & state);
	u8 bB = (BUTTON_B & state);
	u8 bC = (BUTTON_C & state);
	u8 bStart = BUTTON_START & state;

	u8 statusCode = 0;

	if (changed){
		u8 channel = (joy>0) ? CHANNEL_DEF_JOY1 : CHANNEL_DEF_JOY0;
		s8 pitchMod = 0;
		static u8 pitchModAbs = 1;

		// u8 bA = (BUTTON_A & state);
		// u8 bB = (BUTTON_B & state);
		// u8 bC = (BUTTON_C & state);
		// u8 bStart = BUTTON_START & state;

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
		//UPs: portamento

		if (BUTTON_X & state & changed){
			//if start is pressed, go to previous key
			if (bStart){
				//happy accident: b/c keyIndex is unsigned, this will wrap around
				setKey(joy, clamp((keyIndex[joy]-1), 0, MAX_KEYS-1));
				statusCode += STATUS_CODE_KEY;
			}
			//else, decrease octave
			else {
				octave[channel] = clamp(octave[channel]-1, OCTAVE_MIN, OCTAVE_MAX);
				if (!joy){
					octave[CHANNEL_DEF_HARMONY] = octave[channel];
				}
				statusCode += STATUS_CODE_OCTAVE;
			}
		}
		else if (BUTTON_Z & state & changed){
			if (bStart){
				setKey(joy, (keyIndex[joy]+1) % MAX_KEYS);
				statusCode += STATUS_CODE_KEY;
			}
			else {
				octave[channel] = clamp(octave[channel]+1, OCTAVE_MIN, OCTAVE_MAX);
				if (!joy){
					octave[CHANNEL_DEF_HARMONY] = octave[channel];
				}
				statusCode += STATUS_CODE_OCTAVE;
			}
		}

		if (BUTTON_Y & state & changed){
			//if start is pressed, y button changes modulation depth
			if (bStart){
				pitchModDepth[channel] = (pitchModDepth[channel]==1) ? OCT : 1;
				statusCode += STATUS_CODE_MOD;
			}
			//if start not pressed, y button toggles sustainOn
			else {
				sustainOn[channel] = ! (sustainOn[channel]);
				statusCode += STATUS_CODE_SUSTAIN;
			}
		}


		if (BUTTON_UP & state){
			if (BUTTON_UP & changed && bStart){
				portamentoOn[channel] = ! portamentoOn[channel];
				if (!joy){
					portamentoOn[CHANNEL_DEF_HARMONY] = ! portamentoOn[CHANNEL_DEF_HARMONY];
				}
				statusCode += STATUS_CODE_PORTA;
			}
			else {
				pitchModState[channel] = pitchModDepth[channel];
				pitchMod = pitchModDepth[channel];

				if (envelope[channel] != PSG_ENVELOPE_MIN){
					Instrument_setPitch(joy, Instrument_buttonsToScalePitch(bA, bB, bC), pitchMod);
				}
			}
		}
		else if (BUTTON_DOWN & state){
			pitchModState[channel] = -pitchModDepth[channel];
			pitchMod = -pitchModDepth[channel];

			if (envelope[channel] != PSG_ENVELOPE_MIN){
				Instrument_setPitch(joy, Instrument_buttonsToScalePitch(bA, bB, bC), pitchMod);
			}
		}
		//if pitch modulator has just gone neutral, re-sound note
		else if (pitchModState[channel]){
			pitchModState[channel] = 0;
			Instrument_setPitch(joy, Instrument_buttonsToScalePitch(bA, bB, bC), pitchMod);
			// setCPI(channel, scale[channel][Instrument_buttonsToScalePitch(bA, bB, bC)], pitchMod);
		}

		if (bA & changed && bStart){
			vibratoOn[channel] = ! (vibratoOn[channel]);
			if (! joy){
				vibratoOn[CHANNEL_DEF_HARMONY] = ! vibratoOn[CHANNEL_DEF_HARMONY];
			}
			statusCode += STATUS_CODE_VIBRATO;
		}
		else if (bB & changed && bStart){
			arpeggioOn[channel] = ! (arpeggioOn[channel]);
			if (! joy){
				arpeggioOn[CHANNEL_DEF_HARMONY] = ! arpeggioOn[CHANNEL_DEF_HARMONY];
			}
			else {
				arpeggioOn[CHANNEL_DEF_NOISE] = ! arpeggioOn[CHANNEL_DEF_NOISE];
			}
			statusCode += STATUS_CODE_ARP;
		}
		else if (bC & changed && bStart){
			//harmony for player 1
			if (! joy){
				harmonyOn = ! harmonyOn;

				//silence harmony channel if we're turning harmmony off
				if (! harmonyOn){
					Instrument_stopNote(CHANNEL_DEF_HARMONY);
				}
				else if (envelope[channel] != PSG_ENVELOPE_MIN){
					envelope[CHANNEL_DEF_HARMONY] = ENV_DEFAULT;
				}
			}
			//noise for player 2
			else {
				// noiseOn = ! noiseOn;
				changeNoiseMode();
			}

			//harmony status code is used for both harmony and noise
			statusCode += STATUS_CODE_HARMONY;
		}
		//play
		//if any button is pressed and right is held, note will play
		else if (BUTTON_RIGHT & state){

			if (envelope[channel] != PSG_ENVELOPE_MIN && portamentoOn[channel]){
				restartPortamento(channel);
			}

			// setCPI(channel, scale[channel][Instrument_buttonsToScalePitch(bA, bB, bC)], pitchMod);
			Instrument_setPitch(joy, Instrument_buttonsToScalePitch(bA, bB, bC), pitchMod);

			if (!joy && harmonyOn){
				envelope[CHANNEL_DEF_HARMONY] = ENV_DEFAULT;
				envelope[channel] = ENV_DEFAULT;
			}
			else if (joy && noiseOn){
				envelope[CHANNEL_DEF_NOISE] = ENV_DEFAULT;
			}
			else {
				envelope[channel] = ENV_DEFAULT;
			}

			playing[joy] = 1;
		}
		else if ((BUTTON_LEFT & state) || !sustainOn[channel]){
			Instrument_stopNote(channel);
			Instrument_stopNote(CHANNEL_DEF_NOISE);
			Instrument_stopNote(CHANNEL_DEF_HARMONY);
		}

		HUD_updateStatusView(joy, bA, bB, bC, bStart, statusCode);
	}
	// else if (state) HUD_updateStatusView(joy, bA, bB, bC, bStart);
}

void Instrument_playNote(u8 channel, u8 envelope){
	u16 cpi = currentPitchIndex[channel];
	if (arpeggioOn[channel])
		cpi += arpeggioPattern[channel][arpeggioIndex[channel]];
	u16 freq = PITCHES[cpi];

	if (freq && vibratoOn[channel]){
		//remove the "- vibratoDepth" to change the direction of the vibrato
		freq -= fix16ToInt(vibratoY[channel]);// - vibratoDepth[channel];
	}
	if (portamentoOn[channel]){
		freq -= portamentoY[channel];
	}
	PSG_setEnvelope(channel, envelope);
	if (channel != CHANNEL_DEF_NOISE){
		PSG_setFrequency(channel, freq);
	}
}

void Instrument_stopNote(u8 channel){
	vu8 *pb;

	envelope[channel] = PSG_ENVELOPE_MIN;
	PSG_setEnvelope(channel, PSG_ENVELOPE_MIN);

	if (channel <= 1){
		playing[0] = 0;
	}
	else if (channel <= 3){
		playing[1] = 0;
	}
	/*pb = (u8*) PSG_PORT;

	//this part comes from psg.c
	// set tone to 0
	*pb = 0x80 | (channel << 5) | 0x00;
	*pb = 0x00;
	// set envelope to silent
	*pb = 0x90 | (channel << 5) | 0x0F;*/
}

void Instrument_setPitch(u8 joy, u8 scalePitch, u8 pitchMod){

	u8 channel = (joy>0) ? CHANNEL_DEF_JOY1 : CHANNEL_DEF_JOY0;

	setCPI(channel, scale[channel][scalePitch], pitchMod);
	if (channel == CHANNEL_DEF_JOY0){
		u8 harmonyPitch = (scalePitch + harmonyInterval) % (SCALE_LENGTH-1);

		u8 octave = (scalePitch + harmonyInterval) / (SCALE_LENGTH - 1);
		setCPI(CHANNEL_DEF_HARMONY, scale[channel][harmonyPitch], pitchMod + octave * OCT);

		// char text[5];
		// intToStr(octave * OCT, text, 4);
		// VDP_drawText(text, 10, 26);
	}
	else if (channel == CHANNEL_DEF_JOY1){
		setCPI(CHANNEL_DEF_NOISE, scale[channel][scalePitch], pitchMod);
	}
}

/* get unmodified pitch from button combination using binary fingering
(bC is lowest significant bit) */
u8 Instrument_buttonsToScalePitch(u8 bA, u8 bB, u8 bC){
	bA = (bA>0) ? 1 : 0;
 	bB = (bB>0) ? 1 : 0;
  	bC = (bC>0) ? 1 : 0;

  	return bA*4 + bB*2 + bC;
}

static void setCPI(u8 channel, u8 scalePitch, s8 pitchMod){

	u8 joy = (channel < 2) ? 0 : 1;

	lastPitchIndex[channel] = currentPitchIndex[channel];
	//+1 is neccessary as the first element of PITCHES is 0 Hz
	currentPitchIndex[channel] = 1 + scalePitch + pitchMod + (octave[channel] * OCT);
	//add tonic
	currentPitchIndex[channel] += tonicList[joy][keyIndex[joy]];
	
}

static void updateVibrato(u8 i){

	fix16 fVibY;
	u16 cpi = currentPitchIndex[i];

	if (arpeggioOn[i]){
		cpi += arpeggioPattern[i][arpeggioIndex[i]];
	}
	if (cpi <= 0){
		return;
	}

	u16 currentPitch = PITCHES[currentPitchIndex[i]];
	u16 lowestPitch = PITCHES[currentPitchIndex[i]-1];

	if (vibratoOn[i]){
		fix16 depthInHz = fix16Mul(intToFix16(currentPitch - lowestPitch), vibratoDepth[i]);

		//we want the original pitch to be perceived more than the min/max pitches.
		//thus, we want the peaks of the sine wave to be 0 and the origin to be 1.
		//to accomplish this, we rectify the wave, then subtract it from 1
		fVibY = sinFix16(vibratoX[i]);
		if (fVibY < 0){
			fVibY = -fVibY;
		}
		fVibY = FIX16(1) - fVibY;

		//convert amplitude to u16 fraction of vibratoDepth
		// vibratoY[i] = fix16ToInt(fix16Mul(fVibY, intToFix16(vibratoDepth[i])));

		//convert amplitude to Hz
		vibratoY[i] = fix16Mul(depthInHz, fVibY);

		//increment vibratoX
		vibratoX[i] += vibratoSpeed[i];
		if (vibratoX[i] > VIB_X_MAX) vibratoX[i] = 0;
	}
	else{
		vibratoY[i] = FIX16(0);
	}
}

static void updateArpeggio(u8 chan){
	if (arpeggioOn[chan] && arpeggioNoteLen[chan] && arpeggioPattLen[chan]){
		arpeggioX[chan]++;
		if (arpeggioX[chan] % arpeggioNoteLen[chan] == 0){
			arpeggioIndex[chan] = (1 + arpeggioIndex[chan]) % arpeggioPattLen[chan];
		}
	}
}

static void restartPortamento(u8 channel){
	portamentoX[channel] = PORTA_X_MAX;
}

static void updatePortamento(u8 channel){
	if (portamentoOn[channel]){

		u16 px = portamentoX[channel];
		s32 py;

		if (lastPitchIndex[channel] == currentPitchIndex[channel]){
			portamentoX[channel] = 0;
		}
		else if (portamentoX[channel] >= portamentoSpeed[channel]){
			if (px > PORTA_X_MAX){
				portamentoX[channel] = 0;
			}
			else {
				portamentoX[channel] -= portamentoSpeed[channel];
			}
		}
		else if (portamentoX[channel] < portamentoSpeed[channel]){
			portamentoX[channel] = 0;
		}
		//if portamento is at max, do nothing
		else{
			return;
		}

		s32 delta = PITCHES[currentPitchIndex[channel]] - PITCHES[lastPitchIndex[channel]];
		py = fix32ToRoundedInt(fix32Mul(fix32Div(intToFix32(px), intToFix32(PORTA_X_MAX)), intToFix32(delta)));
		portamentoY[channel] = py;

		// char text[5];
		// intToStr(PITCHES[currentPitchIndex[channel]], text, 4);
		// VDP_drawText(text, 10, 26);

		// intToStr(PITCHES[lastPitchIndex[channel]], text, 4);
		// VDP_drawText(text, 15, 26);

		// intToStr(delta, text, 4);
		// VDP_drawText(text, 10, 27);
	}
}

static void changeNoiseMode(){
	//there are actually (MAX_NOISE_MODES + 1) noise modes, if you count noise being off
	if (noiseOn + 1 > MAX_NOISE_MODES){
		noiseOn = 0;
	}
	else{
		noiseOn++;
	}

	if (noiseOn){
		PSG_setNoise(noiseModes[noiseOn-1][0], noiseModes[noiseOn-1][1]);
		// PSG_setNoise(0, 3);
	}
}

static void setKey(u8 joy, u8 ki){
	int i,j;
	//for each channel controlled by joypad
	keyIndex[joy] = ki;
	for (j=0;j<2;j++){
		for (i=0;i<SCALE_LENGTH;i++){
			//0,1 if joy0; 2,3 if joy1
			scale[j + (joy * 2)][i] = SCALES[modeList[joy][ki]][i];
		}	
	}
}

static u32 clamp(u32 n, u32 min, u32 max){
	return (n < min) ? min : (n > max) ? max : n;
}
