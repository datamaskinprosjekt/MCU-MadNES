#include "fpga_test.h"

char* read_data(char* fileName) {
	FILE* fptr;
	fptr = fopen(fileName, "r");
		
	if (fptr == NULL) {
		exit(0);
	}

    fseek(fptr, 0, SEEK_END);
    unsigned long fileSize = ftell(fptr);

    char* data = malloc(sizeof(char)*(fileSize + 1));
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

Color* create_palette(char* paletteFile, int num_colors) {
	Color* palette;
    char* data;
	char** parsedData;

    int i, j;
	
	data = read_data(paletteFile);
	parsedData = parse_data(data, num_colors * 3);
		
	palette = (Color*) malloc((num_colors + 1) * sizeof(void*));
	i = 0;
	
	for (j = 0; j < (num_colors * 3); j += 3) {
		Color color = {.r = (uint8_t) atoi(parsedData[j]), .g = (uint8_t) atoi(parsedData[j+1]), .b = (uint8_t) atoi(parsedData[j+2])};
		palette[i] = color;
		i++;
	}
	
	free(data);
	free(parsedData);
	
	return palette;
}

char* create_sprite_sheet(char* spriteFile, int num_sprites) {
    char* spriteSheet;
    char* data;
    char** parsedData;

    data = read_data(spriteFile);
	parsedData = parse_data(data, num_sprites * 256);
    spriteSheet = *parsedData;

    free(data);
	free(parsedData);
	
	return spriteSheet;
}

void fpga_test() {

    char* spriteSheet;
    Color* palette;

    char* spriteFile;
    char* paletteFile;

    spriteType st1 = {name: ASTEROID, globalSpriteIdx: 0};
    Object obj1 = {id: 1, type: &st1, localSpriteIdx: 0, xPos: 320, yPos: 240, xFlip: false, yFlip: false, enabled: true, priority: 0};

    spriteFile = "sprite_sheet.txt";
    paletteFile = "color_palette.txt";

    CHIP_Init();

    /* Configure core clock to 48 MHz high frequency crystal oscillator*/
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
    CMU_ClockDivSet(cmuClock_HF, cmuClkDiv_512);

    EBI_SetUp();

    spriteSheet = create_sprite_sheet(spriteFile, NUM_SPRITES);

    palette = create_palette(paletteFile, NUM_COLORS);

    write_sprite_sheet(spriteSheet, NUM_SPRITES * 256);
    write_palette(palette, NUM_COLORS + 1);
    write_object(&obj1);

    free(spriteSheet);
    free(palette);
}
