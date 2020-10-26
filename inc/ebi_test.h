#ifndef _EBI_TEST_H_
#define _EBI_TEST_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "ebi.h"

#include "em_chip.h"
#include "em_cmu.h"
#include "em_ebi.h"

void readFPGA(uint16_t *addr, uint16_t *data);
void writeFPGA(uint16_t *addr, uint16_t data);

int ebi_test(void);

#endif
