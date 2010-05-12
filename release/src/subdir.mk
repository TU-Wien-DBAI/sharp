# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Global.cpp \
../src/main.cpp

OBJS += \
./src/Global.o \
./src/main.o

CPP_DEPS += \
./src/Global.d \
./src/main.d


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking Compiler'
	$(CC) -O3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


