################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../init_tcpclient.c \
../init_tcpserver.c \
../init_udpclient.c \
../init_udpserver.c \
../other.c 

OBJS += \
./init_tcpclient.o \
./init_tcpserver.o \
./init_udpclient.o \
./init_udpserver.o \
./other.o 

C_DEPS += \
./init_tcpclient.d \
./init_tcpserver.d \
./init_udpclient.d \
./init_udpserver.d \
./other.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


