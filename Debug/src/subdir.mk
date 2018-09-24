################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ArmOS_lab4_ex1.cpp \
../src/DigitalIoPin.cpp \
../src/blinky.cpp \
../src/cr_cpp_config.cpp \
../src/cr_startup_lpc15xx.cpp 

C_SRCS += \
../src/crp.c \
../src/sysinit.c 

OBJS += \
./src/ArmOS_lab4_ex1.o \
./src/DigitalIoPin.o \
./src/blinky.o \
./src/cr_cpp_config.o \
./src/cr_startup_lpc15xx.o \
./src/crp.o \
./src/sysinit.o 

CPP_DEPS += \
./src/ArmOS_lab4_ex1.d \
./src/DigitalIoPin.d \
./src/blinky.d \
./src/cr_cpp_config.d \
./src/cr_startup_lpc15xx.d 

C_DEPS += \
./src/crp.d \
./src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -std=c++11 -D__NEWLIB__ -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -DCPP_USE_HEAP -D__LPC15XX__ -I"D:\1.Project\NPCXpresso10.2\lpc_board_nxp_lpcxpresso_1549\inc" -I"D:\1.Project\NPCXpresso10.2\lpc_chip_15xx\inc" -I"D:\1.Project\NPCXpresso10.2\FreeRTOS\inc" -I"D:\1.Project\NPCXpresso10.2\FreeRTOS\src\include" -I"D:\1.Project\NPCXpresso10.2\FreeRTOS\src\portable\GCC\ARM_CM3" -I"D:\1.Project\NPCXpresso10.2\lib_itm\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=c11 -D__NEWLIB__ -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -DCPP_USE_HEAP -D__LPC15XX__ -I"D:\1.Project\NPCXpresso10.2\lpc_board_nxp_lpcxpresso_1549\inc" -I"D:\1.Project\NPCXpresso10.2\lpc_chip_15xx\inc" -I"D:\1.Project\NPCXpresso10.2\FreeRTOS\inc" -I"D:\1.Project\NPCXpresso10.2\FreeRTOS\src\include" -I"D:\1.Project\NPCXpresso10.2\FreeRTOS\src\portable\GCC\ARM_CM3" -I"D:\1.Project\NPCXpresso10.2\lib_itm\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


