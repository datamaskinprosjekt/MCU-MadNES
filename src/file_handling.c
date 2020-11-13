#include "file_handling.h"

///////////////////////////////////////////////////////////////
/// read_data
/// -----------------------------------------------------------
/// Uses the C FILE I/O to read the entire contents of a file
/// and store it as a C-style string
///////////////////////////////////////////////////////////////

char* read_data(char* fileName)
{
	// Declare a FILE handler and open the file
	FILE* fptr;
	fptr = fopen(fileName, "r");
		
	// Abort if there is a form of file I/O error
	if (fptr == NULL) {
		exit(0);
	}

	// Seek to the end of the file and get the cursor position
	// to determine the size of the file.
    fseek(fptr, 0, SEEK_END);
    unsigned long fileSize = ftell(fptr);

	// Allocate enough memory to hold the entire file + a null terminator
    char* data = (char*) malloc(sizeof(char) * (fileSize + 1));
    memset(data, 0, fileSize + 1);

	// Reset the file cursor to the beginning
    fseek(fptr, 0, SEEK_SET);

	// Read the file from start to finish and write the data into
	// `data`
    fread(data, 1, fileSize, fptr);

	// Free the file resource
    fclose(fptr);

    return data;
}


///////////////////////////////////////////////////////////////
/// parse_data
/// -----------------------------------------------------------
/// Separates substrings
///////////////////////////////////////////////////////////////
char** parse_data(char* data, int size)
{
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