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