CC = gcc
BUILD_DIR = build
UNITY_DIR = ../../external/unity
DRIVERS_DIR = ../../drivers
MOCK_DIR = ../../tests/mock_modules
CMSIS_CORE_DIR := external/cmsis-core/CMSIS/Core/Include
STM32L4_DIR    := external/stm32l4-cmsis/Include

CFLAGS = -Wall -Wextra

INCLUDES = -I$(MOCK_DIR) \
	-I$(CMSIS_CORE_DIR) -I$(STM32L4_DIR) \
	-I$(UNITY_DIR)/src \
	-I$(DRIVERS_DIR)/inc \

COMMON_SOURCES = $(MOCK_DIR)/stm32l476xx.c \
	$(UNITY_DIR)/src/unity.c

GPIO_SOURCES = $(DRIVERS_DIR)/src/gpio.c
TARGET_GPIO = $(BUILD_DIR)/test_gpio

.PHONY: all test clean

all: test

test: $(TARGET_GPIO)
	@echo "Running GPIO tests"
	@$(TARGET_GPIO)
	@echo ""

$(TARGET_GPIO): test_gpio.c $(GPIO_SOURCES) $(COMMON_SOURCES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)