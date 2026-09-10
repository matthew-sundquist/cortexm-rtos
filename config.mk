# config file for make, loads in the necissary defines for compilation

#CPPFLAGS = -DSTM32L476xx

MCU = STM32F767xx

ifeq ($(MCU), STM32F767xx)
	CPPFLAGS += -DSTM32F767xx
	CFLAGS += -mcpu=cortex-m7
	ASFLAGS += -mcpu=cortex-m7
	LDFLAGS += -T bsp/stm32f767zi/linker.ld -mcpu=cortex-m7
	ARCH_INC += -Iarch/cortex-m7/CMSIS/Device/ST/STM32F767xx -Iarch/cortex-m7/CMSIS/Include -Iarch/cortex-m7/inc
	ARCH_SRC += arch/cortex-m7/src arch/cortex-m7/startup
endif

ifeq ($(MCU), STM32L476xx)
	CPPFLAGS += -DSTM32L476xx
	CFLAGS = -mcpu=cortex-m4
	ASFLAGS = -mcpu=cortex-m4
	LDFLAGS = -T bsp/stm32l476rg/linker.ld -mcpu=cortex-m4
	ARCH_INC = -Iarch/cortex-m4/CMSIS/Device/ST/STM32L4xx/Include -Iarch/cortex-m4/CMSIS/Include -Iarch/cortex-m4/inc
	ARCH_SRC = arch/cortex-m4/src arch/cortex-m4/startup

endif

ARCH_INC += -Iarch
