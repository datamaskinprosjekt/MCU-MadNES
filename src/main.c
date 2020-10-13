#include "main.h"

void readFPGA(uint16_t *addr, uint16_t *data) {
    *data = *addr;
}

void writeFPGA(uint16_t *addr, uint16_t data) {
    *addr = data;
}

int main(void) {
    
    uint16_t *fpgaAddr;

    CHIP_Init();

    /* Configure core clock to 48 MHz high frequency crystal oscillator*/
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
    CMU_ClockDivSet(cmuClock_HF, cmuClkDiv_512);

    /* Setup SysTick Timer for 1 msec interrupts  */
    //SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000);

    EBI_SetUp();

    /* Memory bank 0: 0x80000000 -> 0x83ffffff */
    fpgaAddr = (uint16_t *) EBI_BankAddress(EBI_BANK0);

    /* Write garbage to FPGA */

    char* garbage = malloc(sizeof(char)*255);

    for(int i = 0; i < 255; i++)
        *(garbage + i) = i;

    int offset = 0;
    int offsetAddr = 0;
    int offsetData = 0;

    while(1) {
        offset ++;
        offsetData = offset % 255;
	offsetAddr = offset % 640;
        writeFPGA(fpgaAddr + offsetAddr, *(garbage + offsetData));
    }


}
