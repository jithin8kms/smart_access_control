
####################################
# Target and Toolchain Setup
####################################
TARGET      = bootloader
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
SRC_DIRS    = source system startup config Drivers/STM32F4xx_HAL_Driver/Src
BUILD_DIR   = build
LINKER      = linker/STM32F412ZGTX_FLASH.ld
MBEDTLS_DIR = mbedtls/library

####################################
# Includes
####################################
INCLUDES = \
	-Imbedtls/include \
  -IDrivers/CMSIS/Device/ST/STM32F4xx/Include \
  -IDrivers/CMSIS/Include \
  -IDrivers/STM32F4xx_HAL_Driver/Inc \
  -IDrivers/STM32F4xx_HAL_Driver/Inc/Legacy \
  -Iconfig \
  -Isource \
  -Iinclude \

MBEDTLS_SRCS = \
  $(MBEDTLS_DIR)/asn1parse.c \
  $(MBEDTLS_DIR)/asn1write.c \
  $(MBEDTLS_DIR)/bignum.c \
  $(MBEDTLS_DIR)/ecp.c \
  $(MBEDTLS_DIR)/ecp_curves.c \
  $(MBEDTLS_DIR)/ecdsa.c \
  $(MBEDTLS_DIR)/oid.c \
  $(MBEDTLS_DIR)/pk.c \
  $(MBEDTLS_DIR)/pkparse.c \
  $(MBEDTLS_DIR)/pk_wrap.c \
  $(MBEDTLS_DIR)/sha256.c	\
	$(MBEDTLS_DIR)/md.c \
  $(MBEDTLS_DIR)/platform_util.c \
  $(MBEDTLS_DIR)/constant_time.c \
#	$(MBEDTLS_DIR)/pem.c \
#  $(MBEDTLS_DIR)/platform.c \
#  $(MBEDTLS_DIR)/base64.c \

####################################
# Source Files & Object Mapping
####################################
C_SRCS  := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c)) $(MBEDTLS_SRCS)
CPP_SRCS:= $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))
ASM_SRCS:= $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.s))

SRCS    := $(C_SRCS) $(CPP_SRCS) $(ASM_SRCS) 
OBJS    := $(patsubst %.c,$(BUILD_DIR)/%.o,$(C_SRCS))
OBJS    += $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(CPP_SRCS))
OBJS    += $(patsubst %.s,$(BUILD_DIR)/%.o,$(ASM_SRCS))

####################################
# Flags
####################################
CFLAGS   = $(MCUFLAGS) -Wall -Os -std=c11 -ffreestanding -fdata-sections -ffunction-sections -flto
CXXFLAGS = $(MCUFLAGS) -Wall -Os -std=c++17 -fno-exceptions -fno-rtti -ffreestanding -fdata-sections -ffunction-sections -flto
LDFLAGS  = $(MCUFLAGS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--gc-sections -T$(LINKER) -flto -specs=nano.specs
MBEDTLS_DEFS := -DMBEDTLS_CONFIG_FILE=\"mbedtls/mbedtls_config.h\"

PUBLIC_KEY = ../keys/public.pem
PUBLIC_KEY_H = $(BUILD_DIR)/public_pem.h

####################################
# Build Targets
####################################
.PHONY: all
all: $(BUILD_DIR)/$(TARGET).elf


# Convert PEM to C header
$(PUBLIC_KEY_H): $(PUBLIC_KEY)
	@echo "🧾 Generating public_key.h from PEM..."
	@mkdir -p $(dir $@)
	xxd -i $< > $@


$(BUILD_DIR)/$(TARGET).elf: $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(LD) $(LDFLAGS) -o $@ $^
	$(OBJCOPY) -O binary $@ $(BUILD_DIR)/$(TARGET).bin
	$(OBJCOPY) -O ihex $@ $(BUILD_DIR)/$(TARGET).hex
	$(SIZE) $@

OBJ_DEPS = $(PUBLIC_KEY_H)

$(BUILD_DIR)/%.o: %.c | $(OBJ_DEPS)
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $(INCLUDES) $(MBEDTLS_DEFS) $< -o $@

$(BUILD_DIR)/%.o: %.cpp | $(OBJ_DEPS)
	@mkdir -p $(dir $@)
	$(CXX) -c $(CXXFLAGS) $(INCLUDES) $(MBEDTLS_DEFS) $< -o $@

$(BUILD_DIR)/%.o: %.s | $(OBJ_DEPS)
	@mkdir -p $(dir $@)
	$(CC) -c $(MCUFLAGS) $< -o $@


####################################
# Cleanup
####################################
.PHONY: clean
clean:
	powershell -Command "Remove-Item -Recurse -Force build -ErrorAction SilentlyContinue"
	powershell -Command "Remove-Item -Force *.elf, *.bin, *.hex -ErrorAction SilentlyContinue"
	powershell -Command "Remove-Item -Force $(PUBLIC_KEY_H) -ErrorAction SilentlyContinue"
