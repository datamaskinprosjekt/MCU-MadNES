#ifndef _FILE_HANDLING_H_
#define _FILE_HANDLING_H_

#include <stdio.h>
#include <stdlib.h>

char* read_data(char* fileName);

char** parse_data(char* data, int size);

#endif // _FILE_HANDLING_H_