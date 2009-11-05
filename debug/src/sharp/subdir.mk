# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/sharp/Helper.cpp \
../src/sharp/ExtendedHypertree.cpp \
../src/sharp/AbstractAlgorithm.cpp \
../src/sharp/AnswerSet.cpp \
../src/sharp/Circumscription.cpp \
../src/sharp/Satisfiability.cpp

OBJS += \
./src/sharp/Helper.o \
./src/sharp/ExtendedHypertree.o \
./src/sharp/AbstractAlgorithm.o \
./src/sharp/AnswerSet.o \
./src/sharp/Circumscription.o \
./src/sharp/Satisfiability.o

CPP_DEPS += \
./src/sharp/Helper.d \
./src/sharp/ExtendedHypertree.d \
./src/sharp/AbstractAlgorithm.d \
./src/sharp/AnswerSet.d \
./src/sharp/Circumscription.d \
./src/sharp/Satisfiability.d \

# Each subdirectory must supply rules for building sources it contributes
src/sharp/%.o: ../src/sharp/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ $(COMP_FLAGS) -DDEBUG -O0 -g3 -pedantic -Wall -ansi -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


