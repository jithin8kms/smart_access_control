####################################
# Target and Toolchain Setup
####################################
TARGET      = smart_access_control
MCU         = cortex-m4
FPU         = -mfpu=fpv4-sp-d16
FLOAT-ABI   = -mfloat-abi=hard
MCUFLAGS    = -mcpu=$(MCU) -mthumb $(FPU) $(FLOAT-ABI)

CC          = arm-none-eabi-gcc
CXX         = arm-none-eabi-g++ 
LD          = arm-none-eabi-gcc
OBJCOPY     = arm-none-eabi-objcopy
SIZE        = arm-none-eabi-size

####################################
# Directories
####################################
SRC_DIRS    = source system startup config Drivers/STM32F4xx_HAL_Driver/Src Middlewares/Third_Party/FreeRTOS Middlewares/Third_Party/FreeRTOS/portable/GCC/ARM_CM4F Middlewares/Third_Party/FreeRTOS/portable/MemMang
BUILD_DIR   = build
LINKER      = linker/STM32F412ZGTX_FLASH.ld

####################################
# Includes
####################################
INCLUDES = \
  -IDrivers/CMSIS/Device/ST/STM32F4xx/Include \
  -IDrivers/CMSIS/Include \
  -IDrivers/STM32F4xx_HAL_Driver/Inc \
  -IDrivers/STM32F4xx_HAL_Driver/Inc/Legacy \
  -IMiddlewares/Third_Party/FreeRTOS/include \
  -IMiddlewares/Third_Party/FreeRTOS/portable/GCC/ARM_CM4F \
  -Iconfig \
  -Isource \
  -Iinclude

####################################
# Source Files & Object Mapping
####################################
C_SRCS  := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
CPP_SRCS:= $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))
ASM_SRCS:= $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.s))

SRCS    := $(C_SRCS) $(CPP_SRCS) $(ASM_SRCS)
OBJS    := $(patsubst %.c,$(BUILD_DIR)/%.o,$(C_SRCS))
OBJS    += $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(CPP_SRCS))
OBJS    += $(patsubst %.s,$(BUILD_DIR)/%.o,$(ASM_SRCS))

####################################
# Flags
####################################
CFLAGS   = $(MCUFLAGS) -Wall -O0 -g3 -std=c11 -ffreestanding -fdata-sections -ffunction-sections
CXXFLAGS = $(MCUFLAGS) -Wall -O0 -g3 -std=c++17 -fno-exceptions -fno-rtti -ffreestanding -fdata-sections -ffunction-sections
LDFLAGS  = $(MCUFLAGS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--gc-sections -T$(LINKER)

####################################
# Files for Signing
####################################
PRIVATE_KEY = ../keys/private.pem
BIN_FILE = $(BUILD_DIR)/$(TARGET).bin
SIG_FILE = $(BUILD_DIR)/$(TARGET).sig

BIN_HEADER = $(BUILD_DIR)/$(TARGET)_bin.h
SIG_HEADER = $(BUILD_DIR)/$(TARGET)_sig.h

####################################
# Build Targets
####################################
.PHONY: all clean

all: $(BUILD_DIR)/$(TARGET).elf $(BIN_HEADER) $(SIG_FILE)

# Final ELF + BIN + HEX
$(BUILD_DIR)/$(TARGET).elf: $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(LD) $(LDFLAGS) -o $@ $^
	$(OBJCOPY) -O binary $@ $(BIN_FILE)
	$(OBJCOPY) -O ihex $@ $(BUILD_DIR)/$(TARGET).hex
	$(SIZE) $@

# Convert .elf -> .bin
$(BIN_FILE): $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

# Compilation Rules
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c $(CXXFLAGS) $(INCLUDES) $< -o $@

$(BUILD_DIR)/%.o: %.s
	@mkdir -p $(dir $@)
	$(CC) -c $(MCUFLAGS) $< -o $@

####################################
# Post-build Actions
####################################

# Signature generation
$(SIG_FILE): $(BIN_FILE)
	@echo "🔐 Signing firmware..."
	openssl dgst -sha256 -sign $(PRIVATE_KEY) -out $@ $<
	@echo "✅ Signature saved as $@"

# Create .h from .bin
$(BIN_HEADER): $(BIN_FILE)
	@echo "🧾 Generating bin header..."
	xxd -i $< > $@

####################################
# Cleanup
####################################
clean:
	powershell -Command "Remove-Item -Recurse -Force build -ErrorAction SilentlyContinue"
	powershell -Command "Remove-Item -Force *.elf, *.bin, *.hex -ErrorAction SilentlyContinue"
