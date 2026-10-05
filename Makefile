#======================================================================
# STM32L476 Bare-Metal C++
#======================================================================

APP_TARGET  := firmware
BOOT_TARGET := bootloader

.DEFAULT_GOAL := all

#----------------------------------------------------------------------
# Toolchain
#----------------------------------------------------------------------

CC      := arm-none-eabi-gcc
CXX     := arm-none-eabi-g++
OBJCOPY := arm-none-eabi-objcopy
SIZE    := arm-none-eabi-size

#----------------------------------------------------------------------
# MCU
#----------------------------------------------------------------------

MCU := cortex-m4

CPU_FLAGS := \
	-mcpu=$(MCU) \
	-mthumb \
	-mfpu=fpv4-sp-d16 \
	-mfloat-abi=hard

#----------------------------------------------------------------------
# Directories
#----------------------------------------------------------------------
BUILD_DIR := build

CMSIS_CORE_DIR := external/cmsis-core/CMSIS/Core/Include
STM32L4_DIR    := external/stm32l4-cmsis/Include

#----------------------------------------------------------------------
# Source files
#----------------------------------------------------------------------
BOOT_C_SOURCES := startup/bootloader_startup.c 

BOOT_CPP_SOURCES := \
	bootloader/main.cpp \
	bootloader/BootManager.cpp

APP_C_SOURCES := \
	startup/startup.c \
	drivers/src/gpio.c \
	drivers/src/led.c \
	drivers/src/rcc_clock.c \
	drivers/src/rcc.c \
	drivers/src/uart.c \
	drivers/src/systick.c \
	drivers/src/button.c

APP_CPP_SOURCES := application/main.cpp

#----------------------------------------------------------------------
# Include paths
#----------------------------------------------------------------------

INCLUDES := \
	-I$(CMSIS_CORE_DIR) \
	-I$(STM32L4_DIR) \
	-Istartup \
	-Iapplication \
	-Ibootloader \
	-Idrivers/inc 

#----------------------------------------------------------------------
# Defines
#----------------------------------------------------------------------

DEFINES := \
	-DSTM32L476xx

#----------------------------------------------------------------------
# Compiler flags
#----------------------------------------------------------------------

COMMON_FLAGS := \
	$(CPU_FLAGS) \
	$(INCLUDES) \
	$(DEFINES) \
	-ffunction-sections \
	-fdata-sections \
	-Wall \
	-Wextra

CFLAGS := \
	$(COMMON_FLAGS) \
	-std=c11

CXXFLAGS := \
	$(COMMON_FLAGS) \
	-std=c++17 \
	-fno-exceptions \
	-fno-rtti

#----------------------------------------------------------------------
# Linker
#----------------------------------------------------------------------

BOOT_LDSCRIPT := linker/stm32l476rg_bootloader.ld
APP_LDSCRIPT  := linker/stm32l476rg.ld

LDFLAGS := \
	$(CPU_FLAGS) \
	-Wl,--gc-sections \
	-nodefaultlibs \
	-nostdlib

LDLIBS := \
	-lgcc

#----------------------------------------------------------------------
# Bootloader objects
#----------------------------------------------------------------------


BOOT_C_OBJECTS := $(patsubst %.c, $(BUILD_DIR)/bootloader/obj/%.o, $(BOOT_C_SOURCES))
BOOT_CPP_OBJECTS := $(patsubst %.cpp, $(BUILD_DIR)/bootloader/obj/%.o, $(BOOT_CPP_SOURCES))
BOOT_OBJECTS := $(BOOT_C_OBJECTS) $(BOOT_CPP_OBJECTS)

BOOT_ELF := $(BUILD_DIR)/bootloader/bootloader.elf
BOOT_BIN := $(BUILD_DIR)/bootloader/bootloader.bin
BOOT_MAP := $(BUILD_DIR)/bootloader/bootloader.map

#----------------------------------------------------------------------
# Application objects
#----------------------------------------------------------------------

APP_C_OBJECTS := $(patsubst %.c, $(BUILD_DIR)/application/obj/%.o, $(APP_C_SOURCES))
APP_CPP_OBJECTS := $(patsubst %.cpp, $(BUILD_DIR)/application/obj/%.o, $(APP_CPP_SOURCES))

APP_OBJECTS := $(APP_C_OBJECTS) $(APP_CPP_OBJECTS)

APP_ELF := $(BUILD_DIR)/application/$(APP_TARGET).elf
APP_BIN := $(BUILD_DIR)/application/$(APP_TARGET).bin
APP_MAP := $(BUILD_DIR)/application/$(APP_TARGET).map

#----------------------------------------------------------------------
# Bootloader build
#----------------------------------------------------------------------

$(BOOT_ELF): $(BOOT_OBJECTS)
	@mkdir -p $(dir $@)
	$(CXX) $(LDFLAGS) -T$(BOOT_LDSCRIPT) -Wl,-Map=$(BOOT_MAP) $^ $(LDLIBS) -o $@
	$(SIZE) $@

$(BOOT_BIN): $(BOOT_ELF)
	@mkdir -p $(dir $@)
	$(OBJCOPY) -O binary $< $@

#----------------------------------------------------------------------
# Application build
#----------------------------------------------------------------------

$(APP_ELF): $(APP_OBJECTS)
	@mkdir -p $(dir $@)
	$(CXX) $(LDFLAGS) -T$(APP_LDSCRIPT) -Wl,-Map=$(APP_MAP) $^ $(LDLIBS) -o $@
	$(SIZE) $@

$(APP_BIN): $(APP_ELF)
	@mkdir -p $(dir $@)
	$(OBJCOPY) -O binary $< $@

# ----------------------------------------------------------------------
# Compile C sources
# ----------------------------------------------------------------------
$(BUILD_DIR)/bootloader/obj/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/application/obj/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# ----------------------------------------------------------------------
# Compile C++ sources
# ----------------------------------------------------------------------
$(BUILD_DIR)/bootloader/obj/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR)/application/obj/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

#----------------------------------------------------------------------
# Targets
#----------------------------------------------------------------------
.PHONY: all bootloader application clean \
	flash-bootloader flash-application flash info

all: bootloader application

bootloader: $(BOOT_BIN)
application: $(APP_BIN)

clean:
	rm -rf $(BUILD_DIR)

#----------------------------------------------------------------------
# Flash
#----------------------------------------------------------------------
OPENOCD = openocd
OPENOCD_CFG = -f interface/stlink.cfg -f target/stm32l4x.cfg

flash-bootloader: $(BOOT_BIN)
	$(OPENOCD) $(OPENOCD_CFG) -c "program $(BOOT_BIN) 0x08000000 verify" -c "reset run" -c "shutdown"

flash-application: $(APP_BIN)
	$(OPENOCD) $(OPENOCD_CFG) -c "program $(APP_BIN) 0x08008000 verify" -c "reset run" -c "shutdown"

flash: all
	$(OPENOCD) $(OPENOCD_CFG) -c "program $(BOOT_BIN) 0x08000000 verify" -c "program $(APP_BIN) 0x08008000 verify" \
		-c "reset run" -c "shutdown"


#----------------------------------------------------------------------
# Information
#----------------------------------------------------------------------

info:
	@echo "App target:      $(APP_TARGET)"
	@echo "Boot target:     $(BOOT_TARGET)"
	@echo "MCU:             STM32L476RG"
	@echo "Core:            $(MCU)"
	@echo "CMSIS Core:      $(CMSIS_CORE_DIR)"
	@echo "STM32 CMSIS:     $(STM32L4_DIR)"
	@echo "Bootloader LD:   $(BOOT_LDSCRIPT)"
	@echo "Application LD:  $(APP_LDSCRIPT)"