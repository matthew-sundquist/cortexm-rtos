
#include config.mk
KERNEL_INC = -Ikernel/core/inc -Ikernel/sync/inc -Ikernel/task/inc -Ikernel/util/inc
ARCH_INC = -Iarch/cortex-m4/CMSIS/Device/ST/STM32L4xx/Include -Iarch/cortex-m4/CMSIS/Include -Iarch/cortex-m4/inc
TESTING_INC = -Itesting/inc
DRIVER_INC = -Idrivers/usart/inc
NET_INC = -Inet/common/inc -Inet/arp/inc -Inet/ethernet/inc
COMMON_INC = -Icommon/ts_queue/inc

KERNEL_SRC = kernel/core/src kernel/sync/src kernel/task/src kernel/util/src
ARCH_SRC = arch/cortex-m4/src arch/cortex-m4/startup
TESTING_SRC = testing/src
DRIVER_SRC = drivers/usart/src
NET_SRC = net/common/src net/ethernet/src net/arp/src
COMMON_SRC = common/ts_queue/src

SRC_DIRS = $(KERNEL_SRC) $(ARCH_SRC) $(TESTING_SRC) $(DRIVER_SRC) $(NET_SRC) $(COMMON_SRC)

BUILD=build

CC = arm-none-eabi-gcc
AS = arm-none-eabi-gcc
CPPFLAGS = $(KERNEL_INC) $(ARCH_INC) $(TESTING_INC) $(DRIVER_INC) $(NET_INC) $(COMMON_INC) -DSTM32L476xx
CFLAGS = -pedantic -Wall -Wextra -mcpu=cortex-m4 -mthumb -ffreestanding
ASFLAGS = -mcpu=cortex-m4 -mthumb

SRCS := $(foreach dir,$(SRC_DIRS),$(shell find $(dir) -type f -name "*.c"))
SRCS_S := $(foreach dir,$(SRC_DIRS),$(shell find $(dir) -type f -name "*.s"))

OBJS := $(patsubst %.c,$(BUILD)/%.o,$(SRCS)) $(patsubst %.s,$(BUILD)/%.o,$(SRCS_S))

.PHONY: release debug clean

release: CPPFLAGS += -DDEBUG=0
release: CFLAGS += -O3 

debug: CPPFLAGS += -DDEBUG=1
debug: CFLAGS += -Og -g 
debug: ASFLAGS += -Og -g 
debug: debug.elf

debug.elf: $(OBJS)
	$(CC) $(OBJS) -T bsp/stm32l476rg/linker.ld -nostdlib -nostartfiles -ffreestanding -Wl,--gc-sections -mcpu=cortex-m4 -mthumb -lgcc -o $@

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: %.S
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(ASFLAGS) -c $< -o $@

$(BUILD)/%.o: %.s
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(ASFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD) debug.elf
