#ifndef _INSTRUMENT_H_
#define _INSTRUMENT_H_

#include <genesis.h>
//we need to include HUD so we only do the draw call when we need to
#include "HUD.h"

#define OCT 12
#define OCTAVE_MIN 2
#define OCTAVE_MAX 6
#define VIB_SPEED_DEFAULT 80
#define ENV_DEFAULT 3
#define CHANNEL_DEF_JOY0 0
#define CHANNEL_DEF_JOY1 2

extern u8 octave[4];
extern u8 sustainOn[4];
extern u8 vibratoOn[4];
extern u8 pitchModDepth[4];

void Instrument_init();
void Instrument_update();
void Instrument_joyEvent(u16 joy, u16 changed, u16 state);
void Instrument_playNote(u8 channel, u8 envelope);
void Instrument_stopNote(u8 channel);
//static u16 buttonsToFreq(u8, u8, u8, u8, s16 modifier);
static u8 buttonsToScalePitch(u8, u8, u8);
static void setCPI(u8 channel, u8 scalePitch, s8 pitchMod);
static void updateVibrato(u8 channel);
static u32 clamp(u32 n, u32 min, u32 max);

#endif /* _INSTRUMENT_H_ */
