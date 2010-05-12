# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/support/support.cpp 

OBJS += \
./src/support/support.o 

CPP_DEPS += \
./src/support/support.d 


# Each subdirectory must supply rules for building sources it contributes
src/support/%.o: ../src/support/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking Compiler'
	$(CC) -O3 -pedantic -Wall -c -ansi -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


