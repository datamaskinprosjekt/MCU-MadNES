# MCU

## Setup

### Required tools
- GNU Arm Embedded Toolchain (https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm)
- Simplicity Commander (https://www.silabs.com/community/wireless/zigbee-and-thread/knowledge-base.entry.html/2016/12/29/simplicity_commander-DqhR)

---

### Building
1. Run `make` from the project root.

The exacutable file can then be found in the folder MCU/exe.

### Flashing the Microcontroller
1. Connect a developement board and the MadNES-board through JTag (J18 on the MadNES-board and the JLink Debug Connector on the EFM32GG)
2. Open Simplicity Commander.
    1. Set Debug Mode to Out in the Kit-tab and connect to the board.
    2. Enter the path to the executable file in the field Binary File in the Flash-tab.
    3. Press Flash.

---

## Project Structure

The system files include all files that are not game-specific.

---

## Creating your own game for MadNES
There is an 
If you want to create your own game you will need to follow some basic guidelines.
- Create a sprite sheet where each sprite is 16 x 16 bytes and place it in `sprite_data` in data.c.
- Create a palette where each color is 3 bytes (RGB) and place it in `palette_data`in data.c.
- Provide information about your sprite sheet and palette by setting the macros `NUM_SPRITES` and `NUM_COLORS` to the number of sprites in your sprite sheet and the number of colors in your palette respectively.
- Make use of the Oject structure, see ?.c


Initialize your game by adding the objects you will need. One object will correspond to one element on screen. 