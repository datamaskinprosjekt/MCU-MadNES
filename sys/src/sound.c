#include "sound.h"


/**************************************************************
 * Sound Support
 * ------------------------------------------------------------
 * Currently not used or tested.
 **************************************************************/


/**************************************************************
 * Enables sound on the board.
 *************************************************************/
void enable_board_sound()
{
    GPIO_PinModeSet(gpioPortC, 2, gpioModePushPull, 0);
    GPIO_PortOutSet(gpioPortC, 2);
}


/**************************************************************
 * Disables sound on the board.
 *************************************************************/
void disable_board_sound()
{
    GPIO_PinModeSet(gpioPortC, 2, gpioModeDisabled, 0);
}