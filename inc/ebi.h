#ifndef _EBI_H_
#define _EBI_H_

#include <stdint.h>
#include <stdbool.h>
#include "em_cmu.h"
#include "em_ebi.h"
#include "em_gpio.h"

void EBI_Setup(void);
void EBI_TearDown(void);

#endif
