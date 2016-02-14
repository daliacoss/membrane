#include <genesis.h>
#include "Instrument.h"

#define X0 1
#define Y0 1
#define Y_STATUS 1
#define Y_STATUS_BLANK_LINE 4
#define X_STATVAL X0 + 11
#define WIDTH_TILES 40
#define HEIGHT_TILES 28
#define HORIZONTAL_MARGIN 1
#define X_PLAYER_NAME 5
#define X_BUTTONS 5
#define Y_BUTTONS 14
#define X_NOTE 9
#define Y_NOTE 19

#define Y_PAUSE 3

#define STATUS_CODE_KEY 128
#define STATUS_CODE_OCTAVE 64
#define STATUS_CODE_SUSTAIN 32
#define STATUS_CODE_MOD 16
#define STATUS_CODE_VIBRATO 8
#define STATUS_CODE_HARMONY 4
#define STATUS_CODE_PORTA 2
#define STATUS_CODE_ARP 1

void HUD_init();
void HUD_update();
void HUD_joyEvent(u16 joy, u16 changed, u16 state);
void HUD_pauseSet(u8 joy, u8 state);
void HUD_updateStatusView(u8 joy, u8 bA, u8 bB, u8 bC, u8 bStart, u8 statusCode);
