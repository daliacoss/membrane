#ifndef _INSTRUMENT_H_
#define _INSTRUMENT_H_

#include <genesis.h>

#define OCT 12
#define VIB_SPEED_DEFAULT 80

void Instrument_init();
void Instrument_update();
void Instrument_joyEvent(u16 joy, u16 changed, u16 state);
void Instrument_playNote(u8 channel);
void Instrument_stopNote(u8 channel);
//static u16 buttonsToFreq(u8, u8, u8, u8, s16 modifier);
static u8 buttonsToScalePitch(u8, u8, u8);
static void setCPI(u8 channel, u8 scalePitch, s8 pitchMod);
static void updateVibrato(u8 channel);
static u32 clamp(u32 n, u32 min, u32 max);

#endif /* _INSTRUMENT_H_ */
