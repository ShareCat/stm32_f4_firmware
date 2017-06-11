# STM common library directory
STM_COMMON = ../..

# Application Sources
SRCS = leds.c main.c uart.c
SRCS += stm32f4xx_it.c system_stm32f4xx.c

# STM32F4 StdPeriph Driver Sources
SRCS += misc.c stm32f4xx_usart.c stm32f4xx_adc.c stm32f4xx_dma.c
SRCS += stm32f4xx_exti.c stm32f4xx_gpio.c stm32f4xx_rcc.c
SRCS +=  stm32f4xx_syscfg.c stm32f4xx_spi.c stm32f4xx_tim.c
SRCS +=  stm32f4xx_usart.c

# STM32F4 Startup File
SRCS += startup/gcc/startup_stm32f40_41xxx.s

# Project name
PROJ_NAME=blinky
OUTPATH=build

###################################################

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
SIZE=arm-none-eabi-size

# Cortex M4 processor specification
CPU = -mcpu=cortex-m4
FPU = -mfpu=fpv4-sp-d16
FLOAT-ABI = -mfloat-abi=hard
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# Choose debug or release...
CFLAGS = -g -O2
#CFLAGS = -ggdb -O0

CFLAGS += -DHSE_VALUE=8000000
CFLAGS += -DUSE_STDPERIPH_DRIVER
CFLAGS += -DSTM32F40_41xxx -D__CORTEX_M4F -D__FPU_PRESENT
CFLAGS += $(MCU)
CFLAGS += -mlittle-endian
CFLAGS += -Tstm32_flash.ld
CFLAGS += -specs=nano.specs
CFLAGS += -Wl,--no-wchar-size-warning

LIBS = -lc -lm -lnosys

###################################################

vpath %.c \
$(STM_COMMON)/libraries/STM32F4xx_StdPeriph_Driver/src

# Includes
CFLAGS += -I.
CFLAGS += -I$(STM_COMMON)/libraries/CMSIS/Include
CFLAGS += -I$(STM_COMMON)/libraries/CMSIS/Device/ST/STM32F4xx/Include
CFLAGS += -I$(STM_COMMON)/libraries/STM32F4xx_StdPeriph_Driver/inc

OBJS = $(SRCS:.c=.o)

###################################################

.PHONY: lib proj dir_tree

all:  dir_tree proj

dir_tree:
	mkdir -p $(OUTPATH)

proj: $(OUTPATH)/$(PROJ_NAME).elf
	$(SIZE) $(OUTPATH)/$(PROJ_NAME).elf

$(OUTPATH)/$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBPATHS) $(LIBS)
	$(OBJCOPY) -O ihex $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).bin

clean:
	find . -name \*.o -type f -delete
	find . -name \*.lst -type f -delete
	rm -f $(OUTPATH)/$(PROJ_NAME).elf
	rm -f $(OUTPATH)/$(PROJ_NAME).hex
	rm -f $(OUTPATH)/$(PROJ_NAME).bin

# Flash the STM32F4
burn: proj
	st-flash write $(OUTPATH)/$(PROJ_NAME).bin 0x8000000
