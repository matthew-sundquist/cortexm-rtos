# config file for make, loads in the necissary defines for compilation

#CPPFLAGS += -DSTM32L476xx

CPPFLAGS += -DSTM32F767xx

ifeq ($(CPPFLAGS),DSTM32F767xx)
	CFLAGS = -mcpu=cortex-m7
	ASFLAGS = -mcpu=cortex-m7
	LDFLAGS = -T bsp/stm32f767zi/linker.ld -mcpu=cortex-m7
endif

ifeq ($(CPPFLAGS), DSTM32L476xx)
	CFLAGS = -mcpu=cortex-m4
	ASFLAGS = -mcpu=cortex-m4
	LDFLAGS = -T bsp/stm32l476rg/linker.ld -mcpu=cortex-m4
endif
