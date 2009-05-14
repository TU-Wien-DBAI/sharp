################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/sharp/SharpEnumASP.cpp \
../src/sharp/SharpEnumMinSAT.cpp \
../src/sharp/SharpSAT.cpp 

OBJS += \
./src/sharp/SharpEnumASP.o \
./src/sharp/SharpEnumMinSAT.o \
./src/sharp/SharpSAT.o 

CPP_DEPS += \
./src/sharp/SharpEnumASP.d \
./src/sharp/SharpEnumMinSAT.d \
./src/sharp/SharpSAT.d 


# Each subdirectory must supply rules for building sources it contributes
src/sharp/%.o: ../src/sharp/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DDEBUG -O0 -g3 -pedantic -Wall -ansi -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


