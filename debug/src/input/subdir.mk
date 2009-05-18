################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/input/DIMACSParser.cpp \
../src/input/DIMACSLexer.cpp 

OBJS += \
./src/input/DIMACSParser.o \
./src/input/DIMACSLexer.o

CPP_DEPS += \
./src/input/DIMACSParser.d \
./src/input/DIMACSLexer.d

../src/input/DIMACSParser.h: ../src/input/DIMACSParser.cpp

../src/input/DIMACSParser.cpp: ../src/input/DIMACSParser.y
	@echo 'Building file: $<'
	@echo 'Invoking: Bison++ Parser Generator'
	bison++ -d -h"$(@:%.cpp=%.h)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

../src/input/DIMACSLexer.cpp: ../src/input/DIMACSLexer.l ../src/input/DIMACSParser.h
	@echo 'Building file: $<'
	@echo 'Invoking: Flex++ Lexical Analyzer'
	flex++ -d -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

# Each subdirectory must supply rules for building sources it contributes
#src/input/%.o: ../src/input/%.cpp
#	@echo 'Building file: $<'
#	@echo 'Invoking: GCC C++ Compiler'
#	g++ -DDEBUG -O0 -g3 -Wall -ansi -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
#	@echo 'Finished building: $<'
#	@echo ' '

