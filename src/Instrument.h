#ifndef _INSTRUMENT_H_
#define _INSTRUMENT_H_

#include <genesis.h>
// we need to include HUD so we only do the draw call when we need to
#include "HUD.h"

#define OCT 12
#define OCTAVE_MIN 2
#define OCTAVE_MAX 6
#define VIB_SPEED_DEFAULT 80
#define VIB_X_MAX 512
#define PORTA_X_MAX 512
#define ENV_DEFAULT 3
#define SUS_OFF 0
#define SUS_STRUM 1
#define SUS_HAMMER 2
#define CHANNEL_DEF_JOY0 0
#define CHANNEL_DEF_JOY1 2
#define MAX_KEYS 10
#define SCALE_LENGTH 8

// modes
#define MAJOR 0
#define DORIAN 1
#define PHRYGIAN 2
#define LYDIAN 3
#define MIXOLYDIAN 4
#define MINOR 5
#define LOCRIAN 6
#define MINOR_MEL 7
#define MINOR_HARM 8
// keys
#define C 0
#define C_S 1
#define D 2
#define D_S 3
#define E 4
#define F 5
#define F_S 6
#define G 7
#define G_S 8
#define A 9
#define A_S 10
#define B 11

// channel settings
extern u16 currentPitchIndex[4];
extern u8 octave[4];
extern u8 sustainOn[4];
extern u8 vibratoOn[4];
extern u8 pitchModDepth[4];
extern u8 arpeggioOn[4];
extern u8 portamentoOn[4];
extern u16 portamentoSpeed[4];
//extern u8 tonic[4];
extern u16 scale[4][8];
// player settings
extern u8 keyIndex[2];
extern u8 tonicList[2][MAX_KEYS];
extern u8 modeList[2][MAX_KEYS];
extern u8 paused[2];

void Instrument_init();
void Instrument_update();
void Instrument_joyEvent(u16 joy, u16 changed, u16 state);
void Instrument_playNote(u8 channel, u8 envelope);
void Instrument_stopNote(u8 channel);

#endif /* _INSTRUMENT_H_ */
