#include "ebi.h"


/**************************************************************
 * EBI Functionality
 * ------------------------------------------------------------
 * Here are all the functions used for utilizing the EBI bus
 * to the FPGA on the EFM32GG.
 **************************************************************/


/******************************************************************
 * @brief Sets up the EBI for the FPGA.
 * 
 * Enables EBI and GPIO clock.
 * Sets GPIO pin modes to PushPull for AD, CS, WE, RE, and ALE pins.
 * Configures and initializes EBI Bank 0 for the FPGA.
 * Disables automatic chip select for Bank 0.
 ******************************************************************/ 
void setup_EBI()
{
	// Enable EBI and GPIO clocks
	CMU_ClockEnable(cmuClock_EBI, true);
	CMU_ClockEnable(cmuClock_GPIO, true);

	// Set GPIO pin modes PushPull

	// Address and data pins
	GPIO_PinModeSet(gpioPortE, 8, gpioModePushPull, 0);		// EBI_AD0
	GPIO_PinModeSet(gpioPortE, 9, gpioModePushPull, 0);		// EBI_AD1
	GPIO_PinModeSet(gpioPortE, 10, gpioModePushPull, 0); 	// EBI_AD2
	GPIO_PinModeSet(gpioPortE, 11, gpioModePushPull, 0);	// EBI_AD3
	GPIO_PinModeSet(gpioPortE, 12, gpioModePushPull, 0);	// EBI_AD4
	GPIO_PinModeSet(gpioPortE, 13, gpioModePushPull, 0);	// EBI_AD5
	GPIO_PinModeSet(gpioPortE, 14, gpioModePushPull, 0);	// EBI_AD6
	GPIO_PinModeSet(gpioPortE, 15, gpioModePushPull, 0);	// EBI_AD7
	GPIO_PinModeSet(gpioPortA, 15, gpioModePushPull, 0);	// EBI_AD8
	GPIO_PinModeSet(gpioPortA, 0, gpioModePushPull, 0);		// EBI_AD9
	GPIO_PinModeSet(gpioPortA, 1, gpioModePushPull, 0);		// EBI_AD10
	GPIO_PinModeSet(gpioPortA, 2, gpioModePushPull, 0);		// EBI_AD11
	GPIO_PinModeSet(gpioPortA, 3, gpioModePushPull, 0);		// EBI_AD12
	GPIO_PinModeSet(gpioPortA, 4, gpioModePushPull, 0);		// EBI_AD13
	GPIO_PinModeSet(gpioPortA, 5, gpioModePushPull, 0);		// EBI_AD14
	GPIO_PinModeSet(gpioPortA, 6, gpioModePushPull, 0);		// EBI_AD15

	// Chip select pins
	GPIO_PinModeSet(gpioPortD, 9, gpioModePushPull, 1);		// EBI_CS0
	GPIO_PinModeSet(gpioPortD, 10, gpioModePushPull, 1);	// EBI_CS1
	GPIO_PinModeSet(gpioPortD, 11, gpioModePushPull, 1);	// EBI_CS2
	GPIO_PinModeSet(gpioPortD, 12, gpioModePushPull, 1);	// EBI_CS3

	// Write and read enable pins
	GPIO_PinModeSet(gpioPortF, 8, gpioModePushPull, 0);		// EBI_WEn
	GPIO_PinModeSet(gpioPortF, 9, gpioModePushPull, 0);		// EBI_REn

	// Address latch enable pin
	GPIO_PinModeSet(gpioPortC, 11, gpioModePushPull, 0); 	//EBI_ALE

	// Set EBI configurations for FPGA
	EBI_Init_TypeDef ebiConfig = EBI_INIT_DEFAULT;

	ebiConfig.mode = ebiModeD16A16ALE;

	ebiConfig.addrHoldCycles  = 1;
	ebiConfig.addrSetupCycles = 1;

	ebiConfig.readStrobeCycles = 1;
	ebiConfig.readHoldCycles   = 1;
	ebiConfig.readSetupCycles  = 1;

	ebiConfig.writeStrobeCycles = 1;
	ebiConfig.writeHoldCycles   = 1;
	ebiConfig.writeSetupCycles  = 1;

	ebiConfig.alePolarity = ebiActiveLow;
	ebiConfig.wePolarity = ebiActiveLow;
	ebiConfig.rePolarity = ebiActiveLow;
	ebiConfig.csPolarity = ebiActiveHigh;

	ebiConfig.location = ebiLocation1;
	
	// Initialize EBI with configurations
	EBI_Init(&ebiConfig);

	// Disable CS0 and clear all chip select lines
	EBI_ChipSelectEnable(EBI_CS0, false);

	GPIO_PinOutClear(gpioPortD, 9);
	GPIO_PinOutClear(gpioPortD, 10);
	GPIO_PinOutClear(gpioPortD, 11);
	GPIO_PinOutClear(gpioPortD, 12);
}


/******************************************************************
 * @brief Tears down the EBI bus for the FPGA.
 * 
 * Sets GPIO pin modes to Disabled for AD, CS, WE, RE, and ALE pins.
 * Disables EBI.
 * Disables the EBI clock.
 ******************************************************************/ 
void teardown_EBI()
{
	// Set GPIO pin modes Disabled

	// Address and data pins
	GPIO_PinModeSet(gpioPortE, 8, gpioModeDisabled, 0);		// EBI_AD0
	GPIO_PinModeSet(gpioPortE, 9, gpioModeDisabled, 0);		// EBI_AD1
	GPIO_PinModeSet(gpioPortE, 10, gpioModeDisabled, 0); 	// EBI_AD2
	GPIO_PinModeSet(gpioPortE, 11, gpioModeDisabled, 0);	// EBI_AD3
	GPIO_PinModeSet(gpioPortE, 12, gpioModeDisabled, 0);	// EBI_AD4
	GPIO_PinModeSet(gpioPortE, 13, gpioModeDisabled, 0);	// EBI_AD5
	GPIO_PinModeSet(gpioPortE, 14, gpioModeDisabled, 0);	// EBI_AD6
	GPIO_PinModeSet(gpioPortE, 15, gpioModeDisabled, 0);	// EBI_AD7
	GPIO_PinModeSet(gpioPortA, 15, gpioModeDisabled, 0);	// EBI_AD8
	GPIO_PinModeSet(gpioPortA, 0, gpioModeDisabled, 0);		// EBI_AD9
	GPIO_PinModeSet(gpioPortA, 1, gpioModeDisabled, 0);		// EBI_AD10
	GPIO_PinModeSet(gpioPortA, 2, gpioModeDisabled, 0);		// EBI_AD11
	GPIO_PinModeSet(gpioPortA, 3, gpioModeDisabled, 0);		// EBI_AD12
	GPIO_PinModeSet(gpioPortA, 4, gpioModeDisabled, 0);		// EBI_AD13
	GPIO_PinModeSet(gpioPortA, 5, gpioModeDisabled, 0);		// EBI_AD14
	GPIO_PinModeSet(gpioPortA, 6, gpioModeDisabled, 0);		// EBI_AD15

	// Chip select
	GPIO_PinModeSet(gpioPortD, 9, gpioModeDisabled, 0);		// EBI_CS0
	GPIO_PinModeSet(gpioPortD, 10, gpioModeDisabled, 0);	// EBI_CS1
	GPIO_PinModeSet(gpioPortD, 11, gpioModeDisabled, 0);	// EBI_CS2
	GPIO_PinModeSet(gpioPortD, 12, gpioModeDisabled, 0);	// EBI_CS3

	// Write and read enable pins
	GPIO_PinModeSet(gpioPortF, 8, gpioModeDisabled, 0);		// EBI_WEn
	GPIO_PinModeSet(gpioPortF, 9, gpioModeDisabled, 0);		// EBI_REn

	// Address latch enable pin
	GPIO_PinModeSet(gpioPortC, 11, gpioModeDisabled, 0); 	//EBI_ALE

	// Reset EBI configuration
	EBI_Disable();

	// Disable EBI clock
	CMU_ClockEnable(cmuClock_EBI, false);
}


/******************************************************************
 * Sets the chip select port corresponding to the desired
 * FPGA memory bank.
 * 
 * @param bank The bank enum corresponding to the FPGA memory bank.
 ******************************************************************/ 
void set_bank(BANKSELECT bank)
{
	switch (bank) {
		case OAM:
			GPIO_PortOutSetVal(gpioPortD, OAM_BANK_VAL, CS_MASK);
			break;
		case SPRITE:
			GPIO_PortOutSetVal(gpioPortD, SPRITE_BANK_VAL, CS_MASK);
			break;
		case TILE:
			GPIO_PortOutSetVal(gpioPortD, TILE_BANK_VAL, CS_MASK);
			break;
		case PALETTE:
			GPIO_PortOutSetVal(gpioPortD, PALETTE_BANK_VAL, CS_MASK);
			break;
		case TAM:
			GPIO_PortOutSetVal(gpioPortD, TAM_BANK_VAL, CS_MASK);
			break;
	}
}


/******************************************************************
 * Clears the chip select port for the FPGA.
 ******************************************************************/ 
void clear_bank()
{
	GPIO_PortOutClear(gpioPortD, CS_MASK);
}
