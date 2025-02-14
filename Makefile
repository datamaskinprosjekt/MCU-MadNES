####################################################################
# Makefile                                                         #
####################################################################

.SUFFIXES: # ignore builtin rules
.PHONY: all debug release clean

####################################################################
# Definitions                                                      #
####################################################################

DEVICE = EFM32GG980F1024
PROJECTNAME = MadNES

OBJ_DIR = build
EXE_DIR = exe
LST_DIR = lst

LIB = lib
SYS_SRC = sys/src
SYS_INC = sys/inc
GAME_SRC = game/src
GAME_INC = game/inc
MAIN = game/src/main.c
DIR = .

####################################################################
# Definitions of toolchain                                         #
# You might need to do changes to match your system setup          #
####################################################################

# Change path to the tools according to your system configuration
# DO NOT add trailing whitespace chars, they do matter !
# WINDOWSCS  ?= GNU Tools ARM Embedded\4.8 2013q4
LINUXCS    ?= /gcc-arm-none-eabi-9-2020-q2-update

RMDIRS	   := rm -rf
RMFILES	   := rm -rf
ALLFILES   := /*.*
NULLDEVICE := /dev/null
SHELLNAMES := $(ComSpec)$(COMSPEC)

# Try autodetecting the environment
ifeq ($(SHELLNAMES),)
  # Assume we are making on a Linux platform
  TOOLDIR := $(LINUXCS)
else
  QUOTE :="
  ifneq ($(COMSPEC),)
    # Assume we are making on a mingw/msys/cygwin platform running on Windows
    # This is a convenient place to override TOOLDIR, DO NOT add trailing
    # whitespace chars, they do matter !
    TOOLDIR := $(PROGRAMFILES)/$(WINDOWSCS)
    ifeq ($(findstring cygdrive,$(shell set)),)
      # We were not on a cygwin platform
      NULLDEVICE := NUL
    endif
  else
    # Assume we are making on a Windows platform
    # This is a convenient place to override TOOLDIR, DO NOT add trailing
    # whitespace chars, they do matter !
    SHELL      := $(SHELLNAMES)
    TOOLDIR    := $(ProgramFiles)/$(WINDOWSCS)
    RMDIRS     := rd /s /q
    RMFILES    := del /s /q
    ALLFILES   := \*.*
    NULLDEVICE := NUL
  endif
endif

# Create directories and do a clean which is compatible with parallell make
$(shell mkdir $(OBJ_DIR)>$(NULLDEVICE) 2>&1)
$(shell mkdir $(EXE_DIR)>$(NULLDEVICE) 2>&1)
$(shell mkdir $(LST_DIR)>$(NULLDEVICE) 2>&1)
ifeq (clean,$(findstring clean, $(MAKECMDGOALS)))
  ifneq ($(filter $(MAKECMDGOALS),all debug release),)
    $(shell $(RMFILES) $(OBJ_DIR)$(ALLFILES)>$(NULLDEVICE) 2>&1)
    $(shell $(RMFILES) $(EXE_DIR)$(ALLFILES)>$(NULLDEVICE) 2>&1)
    $(shell $(RMFILES) $(LST_DIR)$(ALLFILES)>$(NULLDEVICE) 2>&1)
  endif
endif
#$(QUOTE)$(TOOLDIR)/bin/
CC	= arm-none-eabi-gcc
LD	= arm-none-eabi-ld
AR	= arm-none-eabi-ar
OBJCOPY = arm-none-eabi-objcopy
DUMP	= arm-none-eabi-objdump

####################################################################
# Flags                                                            #
####################################################################

# -MMD : Don't generate dependencies on system header files.
# -MP  : Add phony targets, useful when a h-file is removed from a project.
# -MF  : Specify a file to write the dependencies to.
DEPFLAGS = -MMD -MP -MF $(@:.o=.d)

#
# Add -Wa,-ahld=$(LST_DIR)/$(@F:.o=.lst) to CFLAGS to produce assembly list files
#
override CFLAGS += -D$(DEVICE) -std=c99 -Wall -Wextra -mcpu=cortex-m3 -mthumb \
 -ffunction-sections -fdata-sections -fomit-frame-pointer -g \
$(DEPFLAGS)

override ASMFLAGS += -x assembler-with-cpp -D$(DEVICE) -Wall -Wextra -mcpu=cortex-m3 -mthumb 

#
# NOTE: The -Wl,--gc-sections flag may interfere with debugging using gdb.
#

override LDFLAGS += -Xlinker -Map=$(LST_DIR)/$(PROJECTNAME).map -mcpu=cortex-m3 \
-mthumb -T$(LIB)/EFM32GG/Source/GCC/efm32gg.ld --specs=nano.specs \
-Wl,--gc-sections

LIBS = -Wl,--start-group -lgcc -lm -lc -lnosys -Wl,--end-group

INCLUDEPATHS += \
-I. \
-I$(LIB)/CMSIS/Include \
-I$(LIB)/EFM32GG/Include \
-I$(LIB)/emlib/inc \
-I$(SYS_INC) \
-I$(GAME_INC) \


####################################################################
# Files                                                            #
####################################################################

C_SRC += \
$(SYS_SRC)/sys_time.c \
$(SYS_SRC)/debug_output.c \
$(SYS_SRC)/ebi.c \
$(SYS_SRC)/fpga.c \
$(SYS_SRC)/spi.c \
$(SYS_SRC)/interrupt_handlers.c \
$(SYS_SRC)/controllers.c \
$(GAME_SRC)/main.c \
$(GAME_SRC)/object.c \
$(GAME_SRC)/game_logic.c \
$(GAME_SRC)/data.c \
$(LIB)/EFM32GG/Source/system_efm32gg.c \
$(LIB)/emlib/src/em_assert.c \
$(LIB)/emlib/src/em_cmu.c \
$(LIB)/emlib/src/em_core.c \
$(LIB)/emlib/src/em_ebi.c \
$(LIB)/emlib/src/em_emu.c \
$(LIB)/emlib/src/em_gpio.c \
$(LIB)/emlib/src/em_system.c \
$(LIB)/emlib/src/em_usart.c \

s_SRC +=

S_SRC += \
$(LIB)/EFM32GG/Source/GCC/startup_efm32gg.S \

####################################################################
# Rules                                                            #
####################################################################

C_FILES = $(notdir $(C_SRC) )
S_FILES = $(notdir $(S_SRC) $(s_SRC) )
#make list of source paths, sort also removes duplicates
C_PATHS = $(sort $(dir $(C_SRC) ) )
S_PATHS = $(sort $(dir $(S_SRC) $(s_SRC) ) )

C_OBJS = $(addprefix $(OBJ_DIR)/, $(C_FILES:.c=.o))
S_OBJS = $(if $(S_SRC), $(addprefix $(OBJ_DIR)/, $(S_FILES:.S=.o)))
s_OBJS = $(if $(s_SRC), $(addprefix $(OBJ_DIR)/, $(S_FILES:.s=.o)))
C_DEPS = $(addprefix $(OBJ_DIR)/, $(C_FILES:.c=.d))
OBJS = $(C_OBJS) $(S_OBJS) $(s_OBJS)

vpath %.c $(C_PATHS)
vpath %.s $(S_PATHS)
vpath %.S $(S_PATHS)


# Default build is release build
all:      release

debug:    CFLAGS += -DDEBUG -O0 -g
debug:    $(EXE_DIR)/$(PROJECTNAME).bin

release:  CFLAGS += -DNDEBUG -O0 -g3
release:  $(EXE_DIR)/$(PROJECTNAME).bin

# Create objects from C SRC files
$(OBJ_DIR)/%.o: %.c
	@echo "Building file: $<"
	$(CC) $(CFLAGS) $(INCLUDEPATHS) -c -o $@ $<

# Assemble .s/.S files
$(OBJ_DIR)/%.o: %.s
	@echo "Assembling $<"
	$(CC) $(ASMFLAGS) $(INCLUDEPATHS) -c -o $@ $<

$(OBJ_DIR)/%.o: %.S
	@echo "Assembling $<"
	$(CC) $(ASMFLAGS) $(INCLUDEPATHS) -c -o $@ $<

# Link
$(EXE_DIR)/$(PROJECTNAME).out: $(OBJS) 
	@echo "Linking target: $@"
	$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $(EXE_DIR)/$(PROJECTNAME).out

# Create binary file
$(EXE_DIR)/$(PROJECTNAME).bin: $(EXE_DIR)/$(PROJECTNAME).out
	@echo "Creating binary file"
	$(OBJCOPY) -O binary $(EXE_DIR)/$(PROJECTNAME).out $(EXE_DIR)/$(PROJECTNAME).bin
# Uncomment next line to produce assembly listing of entire program
#	$(DUMP) -h -S -C $(EXE_DIR)/$(PROJECTNAME).out>$(LST_DIR)/$(PROJECTNAME)out.lst

.PHONY : clean
clean:
ifeq ($(filter $(MAKECMDGOALS),all debug release),)
	$(RMDIRS) $(OBJ_DIR) $(LST_DIR) $(EXE_DIR)
endif

# include auto-generated dependency files (explicit rules)
ifneq (clean,$(findstring clean, $(MAKECMDGOALS)))
-include $(C_DEPS)
endif