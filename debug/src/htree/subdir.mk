# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/htree/CompSet.cpp \
../src/htree/Component.cpp \
../src/htree/Globals.cpp \
../src/htree/H_BucketElim.cpp \
../src/htree/Hyperedge.cpp \
../src/htree/Hypergraph.cpp \
../src/htree/Hypertree.cpp \
../src/htree/Node.cpp \
../src/htree/Parser.cpp \
../src/htree/SetCover.cpp \
../src/htree/SpecialHEdge.cpp 

OBJS += \
./src/htree/CompSet.o \
./src/htree/Component.o \
./src/htree/Globals.o \
./src/htree/H_BucketElim.o \
./src/htree/Hyperedge.o \
./src/htree/Hypergraph.o \
./src/htree/Hypertree.o \
./src/htree/Node.o \
./src/htree/Parser.o \
./src/htree/SetCover.o \
./src/htree/SpecialHEdge.o 

CPP_DEPS += \
./src/htree/CompSet.d \
./src/htree/Component.d \
./src/htree/Globals.d \
./src/htree/H_BucketElim.d \
./src/htree/Hyperedge.d \
./src/htree/Hypergraph.d \
./src/htree/Hypertree.d \
./src/htree/Node.d \
./src/htree/Parser.d \
./src/htree/SetCover.d \
./src/htree/SpecialHEdge.d 


# Each subdirectory must supply rules for building sources it contributes
src/htree/%.o: ../src/htree/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ $(COMP_FLAGS) -DDEBUG -O0 -g3 -pedantic -Wall -ansi -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


