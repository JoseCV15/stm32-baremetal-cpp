CC = gcc
BUILD_DIR = build
UNITY_DIR = ../../external/unity
DRIVERS_DIR = ../../drivers
MOCK_DIR = ../../tests/mock_modules
CMSIS_CORE_DIR := external/cmsis-core/CMSIS/Core/Include
STM32L4_DIR    := external/stm32l4-cmsis/Include

CFLAGS = -Wall -Wextra -Werror -std=c11

INCLUDES = -I$(MOCK_DIR) \
	-I$(CMSIS_CORE_DIR) -I$(STM32L4_DIR) \
	-I$(UNITY_DIR)/src \
	-I$(DRIVERS_DIR)/inc \

COMMON_SOURCES = $(MOCK_DIR)/stm32l476xx.c \
	$(UNITY_DIR)/src/unity.c

RCC_SOURCES = $(DRIVERS_DIR)/src/rcc.c \
	$(DRIVERS_DIR)/src/rcc_clock.c

TARGET_RCC = $(BUILD_DIR)/test_rcc
TARGET_RCC_CLOCK = $(BUILD_DIR)/test_rcc_clock

.PHONY: all test clean

all: test

test: $(TARGET_RCC) $(TARGET_RCC_CLOCK)
	@echo "Running RCC tests"
	@$(TARGET_RCC)
	@echo ""
	@echo "Running RCC clock tests"
	@$(TARGET_RCC_CLOCK)

$(TARGET_RCC): test_rcc.c $(RCC_SOURCES) $(COMMON_SOURCES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

$(TARGET_RCC_CLOCK): test_rcc_clock.c $(DRIVERS_DIR)/src/rcc_clock.c $(COMMON_SOURCES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)