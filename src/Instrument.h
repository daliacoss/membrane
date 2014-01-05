#ifndef _INSTRUMENT_H_
#define _INSTRUMENT_H_

#include <genesis.h>

const u16 PITCHES[] = {
	262,
//	278,
	294,
//	311,
	330,
	349,
//	370,
	392,
//	415,
	440,
	494,
	523
};
u8 sustain[4] = {0,0,0,0};

void Instrument_joyEvent(u16 joy, u16 changed, u16 state);
void Instrument_playNote(u8 channel, u16 frequency, u8 envelope);
void Instrument_stopNote(u8 channel);
static u16 buttonsToFreq(u8, u8, u8);

#endif /* _INSTRUMENT_H_ */
