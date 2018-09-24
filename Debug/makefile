################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: ArmOS_lab4_ex1.axf

# Tool invocations
ArmOS_lab4_ex1.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU C++ Linker'
	arm-none-eabi-c++ -nostdlib -L"D:\1.Project\NPCXpresso10.2\lpc_board_nxp_lpcxpresso_1549\Debug" -L"D:\1.Project\NPCXpresso10.2\lpc_chip_15xx\Debug" -L"D:\1.Project\NPCXpresso10.2\FreeRTOS\Debug" -L"D:\1.Project\NPCXpresso10.2\lib_itm\Debug" -Xlinker -Map="ArmOS_lab4_ex1.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m3 -mthumb -T "ArmOS_lab3_ex2_Debug.ld" -o "ArmOS_lab4_ex1.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS)$(CXX_DEPS)$(OBJS)$(CPP_DEPS)$(C_DEPS) ArmOS_lab4_ex1.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "ArmOS_lab4_ex1.axf"; # arm-none-eabi-objcopy -v -O binary "ArmOS_lab4_ex1.axf" "ArmOS_lab4_ex1.bin" ; # checksum -p LPC1549 -d "ArmOS_lab4_ex1.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
