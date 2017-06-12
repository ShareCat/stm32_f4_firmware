# Put your stlink folder here so make burn will work.
STLINK=~/stlink.git

# STM common library directory
STM_COMMON = ../..

# Application Sources
SRCS = main.c event.c leds.c blink.c netconf.c telnet.c shell.c uart.c newlib_stubs.c
SRCS += stm32f4xx_it.c system_stm32f4xx.c stm32f4x7_eth_bsp.c

# LwIP API Sources
SRCS += api_lib.c api_msg.c err.c netbuf.c netdb.c
SRCS += netifapi.c sockets.c tcpip.c

# LwIP Netif Sources
SRCS += etharp.c slipif.c 

# LwIP Core Sources
SRCS += autoip.c def.c dhcp.c dns.c icmp.c igmp.c inet.c
SRCS += inet_chksum.c init.c ip.c ip_addr.c ip_frag.c
SRCS += mem.c memp.c pbuf.c netif.c raw.c stats.c sys.c
SRCS += tcp.c tcp_in.c tcp_out.c timers.c udp.c

# LwIP Port Sources
SRCS += $(STM_COMMON)/libraries/LWIP-1.4.1/port/STM32F4x7/arch/ethernetif.c 
SRCS += $(STM_COMMON)/libraries/LWIP-1.4.1/port/STM32F4x7/arch/sys_arch.c

# RTOS Config Sources
SRCS += RTX_Conf_CM.c

# RTOS Kernel Sources
SRCS += rt_CMSIS.c rt_Event.c rt_List.c rt_Mailbox.c rt_MemBox.c
SRCS += rt_Memory.c rt_Mutex.c rt_Robin.c rt_Semaphore.c
SRCS += rt_System.c rt_Task.c rt_Time.c

# RTOS HAL Sources
SRCS += HAL_CM.c  HAL_CM4.s SVC_Table.s

# STM32F4 Discovery Sources
SRCS += stm32f4_discovery_lcd.c stm32f4_discovery.c

# STM32F4 StdPeriph Driver Sources
SRCS += misc.c stm32f4xx_usart.c stm32f4xx_adc.c stm32f4xx_dma.c
SRCS += stm32f4xx_exti.c stm32f4xx_fsmc.c stm32f4xx_gpio.c
SRCS += stm32f4xx_rcc.c stm32f4xx_syscfg.c stm32f4xx_sdio.c

# STM32F4 Ethernet Driver Sources
SRCS += stm32f4x7_eth.c

# STM32F4 Startup File
SRCS += startup/gcc/startup_stm32f40_41xxx.s

# Project name
PROJ_NAME=telnet_rtos
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

###################################################

vpath %.c \
$(STM_COMMON)/libraries/RTX-v4.79/SRC \
$(STM_COMMON)/libraries/STM32F4-Discovery \
$(STM_COMMON)/libraries/STM32F4xx_StdPeriph_Driver/src \
$(STM_COMMON)/libraries/STM32F4x7_ETH_Driver/src \
$(STM_COMMON)/libraries/LWIP-1.4.1/src/api \
$(STM_COMMON)/libraries/LWIP-1.4.1/src/netif \
$(STM_COMMON)/libraries/LWIP-1.4.1/src/core \
$(STM_COMMON)/libraries/LWIP-1.4.1/src/core/ipv4 \
$(STM_COMMON)/libraries/LWIP-1.4.1/port/STM32F4x7/arch

vpath %.s \
$(STM_COMMON)/libraries/RTX-v4.79/SRC/GCC

# Includes
CFLAGS += -I.
CFLAGS += -I$(STM_COMMON)/libraries/CMSIS/Include
CFLAGS += -I$(STM_COMMON)/libraries/CMSIS/Device/ST/STM32F4xx/Include
CFLAGS += -I$(STM_COMMON)/libraries/STM32F4x7_ETH_Driver/inc
CFLAGS += -I$(STM_COMMON)/libraries/STM32F4xx_StdPeriph_Driver/inc
CFLAGS += -I$(STM_COMMON)/libraries/STM32F4-Discovery
CFLAGS += -I$(STM_COMMON)/libraries/LWIP-1.4.1/port/STM32F4x7
CFLAGS += -I$(STM_COMMON)/libraries/LWIP-1.4.1/src/include
CFLAGS += -I$(STM_COMMON)/libraries/LWIP-1.4.1/src/include/ipv4
CFLAGS += -I$(STM_COMMON)/libraries/LWIP-1.4.1/src/include/lwip
CFLAGS += -I$(STM_COMMON)/libraries/LWIP-1.4.1/src/include/netif
CFLAGS += -I$(STM_COMMON)/libraries/LWIP-1.4.1/port/STM32F4x7/arch
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
	$(STLINK)/st-flash write $(OUTPATH)/$(PROJ_NAME).bin 0x8000000
