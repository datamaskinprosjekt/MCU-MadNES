#include "ebi_main.h"

void readFPGA(uint16_t *addr, uint16_t *data) {
    *data = *addr;
}

void writeFPGA(uint16_t *addr, uint16_t data) {
    *addr = data;
}

int ebi_main(void) {
    
    uint16_t *fpgaAddr0;
    uint16_t *fpgaAddr1;
    uint16_t *fpgaAddr2;
    uint16_t *fpgaAddr3;

    CHIP_Init();

    /* Configure core clock to 48 MHz high frequency crystal oscillator*/
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
    CMU_ClockDivSet(cmuClock_HF, cmuClkDiv_256);

    EBI_SetUp();

    /* Memory bank 0: 0x80000000 -> 0x83ffffff */
    fpgaAddr0 = (uint16_t *) EBI_BankAddress(EBI_BANK0);
    /* Memory bank 1: 0x84000000 -> 0x87ffffff */
    fpgaAddr1 = (uint16_t *) EBI_BankAddress(EBI_BANK1);
    /* Memory bank 2: 0x88000000 -> 0x8bffffff */
    fpgaAddr2 = (uint16_t *) EBI_BankAddress(EBI_BANK2);
    /* Memory bank 3: 0x8c000000 -> 0x8fffffff */
    fpgaAddr3 = (uint16_t *) EBI_BankAddress(EBI_BANK3);


    /* Write garbage to FPGA */
    char* garbage = malloc(sizeof(char) * 255);

    for(int i = 0; i < 255; i++)
        *(garbage + i) = i;

    int offset = 0;
    int offsetAddr = 0;
    int offsetData = 0;

    while(1) {
        offset ++;
        offsetData = offset % 255;
	    offsetAddr = offset % 640;
        set_bank(OAM);
        writeFPGA(fpgaAddr0 + offsetAddr, *(garbage + offsetData));
        clear_bank();
    }
}
