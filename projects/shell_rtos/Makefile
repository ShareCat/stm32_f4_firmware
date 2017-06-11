# STM common library directory
STM_COMMON = ../..

# Application Sources
SRCS = main.c event.c leds.c blink.c shell.c uart.c
SRCS += stm32f4xx_it.c system_stm32f4xx.c

# RTOS Config Sources
SRCS += RTX_Conf_CM.c

# RTOS Kernel Sources
SRCS += rt_CMSIS.c rt_Event.c rt_List.c rt_Mailbox.c rt_MemBox.c
SRCS += rt_Memory.c rt_Mutex.c rt_Robin.c rt_Semaphore.c
SRCS += rt_System.c rt_Task.c rt_Time.c

# RTOS HAL Sources
SRCS += HAL_CM.c HAL_CM4.s SVC_Table.s

# STM32F4 StdPeriph Driver Sources
SRCS += misc.c stm32f4xx_usart.c stm32f4xx_adc.c stm32f4xx_dma.c
SRCS += stm32f4xx_exti.c stm32f4xx_gpio.c stm32f4xx_rcc.c
SRCS +=  stm32f4xx_syscfg.c stm32f4xx_spi.c stm32f4xx_tim.c
SRCS +=  stm32f4xx_usart.c

# STM32F4 Startup File
SRCS += startup/gcc/startup_stm32f40_41xxx.s

# Project name
PROJ_NAME=shell_rtos
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
CFLAGS += -DSTM32F40_41xxx -D__CORTEX_M4F -D__FPU_PRESENT -D__CMSIS_RTOS
CFLAGS += $(MCU)
CFLAGS += -ffunction-sections
CFLAGS += -mlittle-endian
CFLAGS += -Tstm32_flash.ld
CFLAGS += -specs=nano.specs
CFLAGS += -Wl,--no-wchar-size-warning

LIBS = -lc -lm -lnosys

###################################################

vpath %.c \
$(STM_COMMON)/libraries/RTX-v4.79/SRC \
$(STM_COMMON)/libraries/STM32F4xx_StdPeriph_Driver/src \

vpath %.s \
$(STM_COMMON)/libraries/RTX-v4.79/SRC/GCC

# Includes
CFLAGS += -I.
CFLAGS += -I$(STM_COMMON)/libraries/CMSIS/Include
CFLAGS += -I$(STM_COMMON)/libraries/CMSIS/Device/ST/STM32F4xx/Include
CFLAGS += -I$(STM_COMMON)/libraries/STM32F4xx_StdPeriph_Driver/inc
CFLAGS += -I$(STM_COMMON)/libraries/RTX-v4.79/INC
CFLAGS += -I$(STM_COMMON)/libraries/RTX-v4.79/SRC

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
