# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/sharp/Helper.cpp \
../src/sharp/AbstractAlgorithm.cpp \
../src/sharp/AnswerSet.cpp \
../src/sharp/Argumentation.cpp \
../src/sharp/HeadCycleFreeAnswerSet.cpp \
../src/sharp/Circumscription.cpp \
../src/sharp/Satisfiability.cpp

OBJS += \
./src/sharp/Helper.o \
./src/sharp/AbstractAlgorithm.o \
./src/sharp/AnswerSet.o \
./src/sharp/Argumentationnnnnnnnnnnn.o \
./src/sharp/HeadCycleFreeAnswerSet.o \
./src/sharp/Circumscription.o \
./src/sharp/Satisfiability.o

CPP_DEPS += \
./src/sharp/Helper.d \
./src/sharp/AbstractAlgorithm.d \
./src/sharp/AnswerSet.d \
./src/sharp/Argumentation.d \
./src/sharp/HeadCycleFreeAnswerSet.d \
./src/sharp/Circumscription.d \
./src/sharp/Satisfiability.d

# Each subdirectory must supply rules for building sources it contributes
src/sharp/%.o: ../src/sharp/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking Compiler'
	$(CC) -O3 -pedantic -Wall -c -ansi -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


