#include "fpga_test.h"

char* read_file(char* fileName) {
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

void fpga_test() {

    char* spriteSheet;
    Color* palette[NUM_COLORS];

    char* spriteFile;

    char* paletteBuffer;
    char* paletteFile;

    spriteType st1 = {name: ASTEROID, globalSpriteIdx: 0};
    Object obj1 = {id: 1, type: &st1, localSpriteIdx: 0, xPos: 320, yPos: 240, xFlip: false, yFlip: false, enabled: true, priority: 1};

    char i, j;

    spriteFile = "sprite_sheet.txt";
    paletteFile = "color_palette.txt";

    CHIP_Init();

    /* Configure core clock to 48 MHz high frequency crystal oscillator*/
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
    CMU_ClockDivSet(cmuClock_HF, cmuClkDiv_512);

    EBI_SetUp();

    spriteSheet = read_file(spriteFile);
    paletteBuffer = read_file(paletteFile);

    *palette = (Color*) malloc((NUM_COLORS + 1) * sizeof(void*));
    i = 0;

    for (j = 0; j < SIZE_PALETTE; j += 3) {
        Color color = {.r = *paletteBuffer[j], .g = *paletteBuffer[j+1], .b = *paletteBuffer[j+2]};
        palette[i] = &color;
        i++;
    }

    write_sprite_sheet(spriteSheet);
    write_palette(*palette, NUM_COLORS);
    write_object(&obj1);

    free(spriteSheet);
    free(paletteBuffer);
}
