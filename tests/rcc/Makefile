CC := gcc

BUILD_DIR := build

UNITY_DIR := ../../external/unity
CMSIS_CORE_DIR := ../../external/cmsis-core
STM32_CMSIS_DIR := ../../external/stm32l4-cmsis
DRIVERS_DIR := ../../drivers
CFLAGS := -Wall -Wextra -Werror -std=c11

INCLUDES := -I$(UNITY_DIR)/src \
	-I$(DRIVERS_DIR)/inc \
	-I$(CMSIS_CORE_DIR)/CMSIS/Core/Include \
	-I$(STM32_CMSIS_DIR)/Include

SOURCES := test_rcc_clock.c \
	$(UNITY_DIR)/src/unity.c \
	$(DRIVERS_DIR)/src/rcc_clock.c

TARGET := $(BUILD_DIR)/test_rcc_clock

.PHONY: all test clean

all: test

test: $(TARGET)
	./$(TARGET)

$(TARGET): $(SOURCES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $(SOURCES) -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)