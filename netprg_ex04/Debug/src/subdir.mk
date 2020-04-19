################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/chat_common.c \
../src/chat_util.c \
../src/netprg_ex04.c 

OBJS += \
./src/chat_common.o \
./src/chat_util.o \
./src/netprg_ex04.o 

C_DEPS += \
./src/chat_common.d \
./src/chat_util.d \
./src/netprg_ex04.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/home/rimikawasaki/eclipse-workspace/mynet -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


