#include "Instrument.h"

void Instrument_joyEvent(u16 joy, u16 changed, u16 state){
	if (changed){
		u8 channel = (joy>0) ? 1 : 0;
		u8 bA = (BUTTON_A & state);
		u8 bB = (BUTTON_B & state);
		u8 bC = (BUTTON_C & state);

		//play
		//if any button is pressed and right is held, note will play
		if (BUTTON_RIGHT & state) Instrument_playNote(channel, buttonsToFreq(bA, bB, bC), 2);
		else if (BUTTON_LEFT & state) Instrument_stopNote(channel);
		else if (!sustain[channel]) Instrument_stopNote(channel);

		//control
		if (BUTTON_Y & state & changed) sustain[channel] = ! (sustain[channel]);
		if (BUTTON_X & state & changed){

		}
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
static u16 buttonsToFreq(u8 bA, u8 bB, u8 bC){

	bA = (bA>0) ? 1 : 0;
	bB = (bB>0) ? 1 : 0;
	bC = (bC>0) ? 1 : 0;

	u8 index = bA*4 + bB*2 + bC;

	char debug[20];
	uintToStr((u32) index, debug, 1);
	VDP_drawText(debug, 2, 10);

	//if (index == 0) return 0;
	return PITCHES[index];
}
