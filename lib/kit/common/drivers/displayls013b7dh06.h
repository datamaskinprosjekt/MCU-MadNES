/***************************************************************************//**
 * @file
 * @brief Configuration for the display driver for the Sharp Memory LCD
 *        LS013B7DH06
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/
#ifndef _DISPLAY_LS013B7DH06_H_
#define _DISPLAY_LS013B7DH06_H_

#include "emstatus.h"

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 ********************************  DEFINES  ************************************
 ******************************************************************************/

/* Display geometry */
#define LS013B7DH06_WIDTH           (128)
#define LS013B7DH06_HEIGHT          (128)
#define LS013B7DH06_BITS_PER_PIXEL    (3)

/*******************************************************************************
 **************************    FUNCTION PROTOTYPES    **************************
 ******************************************************************************/

/* Initialization function for the LS013B7DH06 device driver. */
EMSTATUS DISPLAY_Ls013b7dh06Init(void);

#ifdef __cplusplus
}
#endif

/** @endcond */

#endif /* _DISPLAY_LS013B7DH06_H_ */
