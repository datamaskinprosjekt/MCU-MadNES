#include "ebi.h"

void EBI_SetUp(void) {

	/******************************
	 * Turn on EBI and GPIO clock *
	 ******************************/
	CMU_ClockEnable(cmuClock_EBI, true);
	CMU_ClockEnable(cmuClock_GPIO, true);

	/***********************************
	 * Set GPIO pin modes to push pull *
	 ***********************************/

	/* Address and Data */
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

	/* Chip select */
	GPIO_PinModeSet(gpioPortD, 9, gpioModePushPull, 1);		// EBI_CS0
	//GPIO_PinModeSet(gpioPortD, 10, gpioModePushPull, 1);	// EBI_CS1
	//GPIO_PinModeSet(gpioPortD, 11, gpioModePushPull, 1);	// EBI_CS2
	//GPIO_PinModeSet(gpioPortD, 12, gpioModePushPull, 1);	// EBI_CS3

	/* Write and read enable */
	GPIO_PinModeSet(gpioPortF, 8, gpioModePushPull, 0);		// EBI_WEn
	GPIO_PinModeSet(gpioPortF, 9, gpioModePushPull, 0);		// EBI_REn

	/* Address latch enable */
	GPIO_PinModeSet(gpioPortC, 11, gpioModePushPull, 0); 	//EBI_ALE

	/* Configure board for EBI mode on PB15 MCU_EBI_CONNECT */
	GPIO_PinModeSet(gpioPortB, 15, gpioModePushPull, 0);
	/* Configure board for EBI mode on PD13 MCU_SPI_CONNECT */
	GPIO_PinModeSet(gpioPortD, 13, gpioModePushPull, 1);

	/**************************
	 * Set EBI configurations *
	 **************************/

	// EBI_Init_TypeDef = EBI_INIT_DEFAULT {                                                            
	// 	ebiModeD8A8,    /* mode */           
	// 	ebiActiveLow,   /* ardyPolarity */
	// 	ebiActiveLow,   /* alePolarity */ 
	// 	ebiActiveLow,   /* wePolarity */                       
	// 	ebiActiveLow,   /* rePolarity */                         
	// 	ebiActiveLow,   /* csPolarity */
	// 	ebiActiveLow,   /* blPolarity */
	// 	false,          /* blEnable */
	// 	false,          /* noIdle */
	// 	false,          /* ardyEnable */
	// 	false,          /* ardyDisableTimeout */
	// 	EBI_BANK0,      /* banks */
	// 	EBI_CS0,        /* csLines */
	// 	0,              /* addrSetUpCycles */
	// 	1,              /* addrHoldCycles */
	// 	false,          /* addrHalfALE */
	// 	0,              /* readSetupCycles */
	// 	0,              /* readStrobeCycles */
	// 	0,              /* readHoldCycles */
	// 	false,          /* readPageMode */
	// 	false,          /* readPrefetch */
	// 	false,          /* readHalfRE */
	// 	0,              /* writeSetupCycles */
	// 	0,              /* writeStrobeCycles */
	// 	1,              /* writeHoldCycles */
	// 	false,          /* writeBufferDisable */
	// 	false,          /* writeHalfWE */
	// 	ebiALowA0,      /* aLow */
	// 	ebiAHighA0,     /* aHigh */
	// 	ebiLocation0,   /* location */
	// 	true,           /* enable */
  	// }

	EBI_Init_TypeDef ebiConfig = EBI_INIT_DEFAULT;

	/* FPGA */
	ebiConfig.mode = ebiModeD16A16ALE;
	ebiConfig.banks = EBI_BANK0;
	ebiConfig.csLines = EBI_CS0;

	ebiConfig.addrHoldCycles  = 3;
	ebiConfig.addrSetupCycles = 3;

	ebiConfig.readStrobeCycles = 7;
	ebiConfig.readHoldCycles   = 3;
	ebiConfig.readSetupCycles  = 3;

	ebiConfig.writeStrobeCycles = 7;
	ebiConfig.writeHoldCycles   = 3;
	ebiConfig.writeSetupCycles  = 3;

	ebiConfig.alePolarity = ebiActiveLow;
	ebiConfig.wePolarity = ebiActiveHigh;
	ebiConfig.rePolarity = ebiActiveLow;
	ebiConfig.csPolarity = ebiActiveLow;

	ebiConfig.addrHalfALE = true;
	ebiConfig.noIdle = true;

	ebiConfig.location = ebiLocation0;
	
	/**************************************
	 * Initialize EBI with configurations *
	 **************************************/

	EBI_Init(&ebiConfig);

}

void EBI_TearDown(void) {

	/******************************
	 * Disable GPIO pins *
	 ******************************/

	/* Address and Data */
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

	/* Chip select */
	GPIO_PinModeSet(gpioPortD, 9, gpioModeDisabled, 0);		// EBI_CS0
	GPIO_PinModeSet(gpioPortD, 10, gpioModeDisabled, 0);	// EBI_CS1
	GPIO_PinModeSet(gpioPortD, 11, gpioModeDisabled, 0);	// EBI_CS2
	GPIO_PinModeSet(gpioPortD, 12, gpioModeDisabled, 0);	// EBI_CS3

	/* Write and read enable */
	GPIO_PinModeSet(gpioPortF, 8, gpioModeDisabled, 0);		// EBI_WEn
	GPIO_PinModeSet(gpioPortF, 9, gpioModeDisabled, 0);		// EBI_REn

	/* Address latch enable */
	GPIO_PinModeSet(gpioPortC, 11, gpioModeDisabled, 0); 	//EBI_ALE

	/* Configure board for OFF mode on PB15 MCU_EBI_CONNECT */
    GPIO_PinModeSet(gpioPortB, 15, gpioModePushPull, 1);
	/* Configure board for OFF mode on PD13 MCU_SPI_CONNECT */
	GPIO_PinModeSet(gpioPortD, 13, gpioModePushPull, 1);

	/****************************
	 * Reset EBI configurations *
	 ****************************/

	EBI_Disable();

	/**********************
	 * Turn off EBI clock *
	 **********************/

	CMU_ClockEnable(cmuClock_EBI, false);

}
