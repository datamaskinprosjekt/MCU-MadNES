#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "time.h"
#include "ebi.h"

#include "em_chip.h"
#include "em_cmu.h"
#include "em_ebi.h"

void readFPGA(uint16_t *addr, uint16_t *data);
void writeFPGA(uint16_t *addr, uint16_t data);

int main(void);

#endif
