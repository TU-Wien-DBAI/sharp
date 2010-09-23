# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Global.cpp \
../src/Problem.cpp \
../src/DatalogProblem.cpp \
../src/ArgumentationProblem.cpp \
../src/main.cpp

OBJS += \
./src/Global.o \
./src/Problem.o \
./src/DatalogProblem.o \
./src/ArgumentationProblem.o \
./src/main.o

CPP_DEPS += \
./src/Global.d \
./src/Problem.d \
./src/DatalogProblem.d \
./src/ArgumentationProblem.d \
./src/main.d


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking Compiler'
	$(CC) $(COMP_FLAGS) -DDEBUG -O0 -g3 -pedantic -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


