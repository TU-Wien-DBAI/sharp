# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/sharp/SharpEnumASP.cpp \
../src/sharp/SharpEnumMinSAT.cpp \
../src/sharp/SharpSAT.cpp \
../src/sharp/ExtendedHypertree.cpp \
../src/sharp/LazySolutionSet.cpp

OBJS += \
./src/sharp/SharpEnumASP.o \
./src/sharp/SharpEnumMinSAT.o \
./src/sharp/SharpSAT.o \
./src/sharp/ExtendedHypertree.o \
./src/sharp/LazySolutionSet.o

CPP_DEPS += \
./src/sharp/SharpEnumASP.d \
./src/sharp/SharpEnumMinSAT.d \
./src/sharp/SharpSAT.d \
./src/sharp/ExtendedHypertree.d \
./src/sharp/LazySolutionSet.d


# Each subdirectory must supply rules for building sources it contributes
src/sharp/%.o: ../src/sharp/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -pedantic -Wall -c -ansi -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


