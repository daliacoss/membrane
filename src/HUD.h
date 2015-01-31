#include <genesis.h>
#include "Instrument.h"

#define X0 1
#define Y0 1
#define Y_STATUS 1
#define Y_STATUS_BLANK_LINE 4
#define X_STATVAL X0 + 11
#define W_TILES 40
#define X_PLAYER_NAME 5
#define X_BUTTONS 5
#define Y_BUTTONS 14
#define X_NOTE 9
#define Y_NOTE 19

void HUD_init();
void HUD_update();
void HUD_joyEvent(u16 joy, u16 changed, u16 state);
void HUD_updateStatusView(u8 joy, u8 bA, u8 bB, u8 bC, u8 bStart);
