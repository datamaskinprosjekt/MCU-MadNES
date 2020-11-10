#ifndef _EBI_H_
#define _EBI_H_

#include <stdint.h>
#include <stdbool.h>
#include "em_cmu.h"
#include "em_ebi.h"
#include "em_gpio.h"

/* Bits 9-12 */
#define CS_MASK 0b111 << 9

/* Bank Selects */
#define OAM_BANK_VAL     0 << 9
#define SPRITE_BANK_VAL  1 << 9
#define TILE_BANK_VAL    2 << 9
#define PALETTE_BANK_VAL 3 << 9
#define TAM_BANK_VAL     4 << 9

typedef enum BANKSELECT {
    OAM,
    SPRITE,
    TILE,
    PALETTE,
    TAM
} BANKSELECT;

void setup_EBI(void);
void teardown_EBI(void);

void set_bank(BANKSELECT bank);
void clear_bank();

#endif
