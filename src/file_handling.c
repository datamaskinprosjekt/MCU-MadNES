#include "file_handling.h"

char* read_data(char* fileName) {
	FILE* fptr;
	fptr = fopen(fileName, "r");
		
	if (fptr == NULL) {
		exit(0);
	}

    fseek(fptr, 0, SEEK_END);
    unsigned long fileSize = ftell(fptr);

    char* data = (char*) malloc(sizeof(char) * (fileSize + 1));
    memset(data, 0, fileSize + 1);

    fseek(fptr, 0, SEEK_SET);

    fread(data, 1, fileSize, fptr);

    fclose(fptr);

    return data;
}

char** parse_data(char* data, int size) {
	char** parsedData;
	
	int i;
	
	parsedData = (char**) malloc(size * sizeof(char*));
	char* next = strtok(data, " \n");
	
	i = 0;
		
	while(next != NULL) {
		parsedData[i] = next;
		next = strtok(NULL, " \n");
		i++;
	}
	
	return parsedData;
}