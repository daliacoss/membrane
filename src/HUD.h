#include <genesis.h>
#include "Instrument.h"

#define X0 1
#define Y0 1
#define Y_STATUS 3
#define X_STATVAL X0 + 11
#define W_TILES 40

void HUD_init();
void HUD_update();
void HUD_joyEvent(u16 joy, u16 changed, u16 state);
void HUD_updateStatusView(u8 joy);

/* clear area from c to c+w and draw text over it
 * (c is relative to x)
 */ 
static void drawText(char *text, u16 x, u16 y, u16 c, u16 w);

static void drawStatusMsg(u8 joy, u8 line);
