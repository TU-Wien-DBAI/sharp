# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/input/AbstractHypergraph.cpp \
../src/input/DatalogParser.cpp \
../src/input/DatalogLexer.cpp \
../src/input/DatalogHypergraph.cpp \
../src/input/DIMACSParser.cpp \
../src/input/DIMACSLexer.cpp \
../src/input/DIMACSHypergraph.cpp 

OBJS += \
./src/input/AbstractHypergraph.o \
./src/input/DatalogParser.o \
./src/input/DatalogLexer.o \
./src/input/DatalogHypergraph.o \
./src/input/DIMACSParser.o \
./src/input/DIMACSLexer.o \
./src/input/DIMACSHypergraph.o

CPP_DEPS += \
./src/input/AbstractHypergraph.d \
./src/input/DatalogParser.d \
./src/input/DatalogLexer.d \
./src/input/DatalogHypergraph.d \
./src/input/DIMACSParser.d \
./src/input/DIMACSLexer.d \
./src/input/DIMACSHypergraph.d

../src/input/%Parser.h: ../src/input/%Parser.cpp

../src/input/%Parser.cpp: ../src/input/%Parser.y
	@echo 'Building file: $<'
	@echo 'Invoking: Bison++ Parser Generator'
	bison++ -d -h"$(@:%.cpp=%.h)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

../src/input/%Lexer.cpp: ../src/input/%Lexer.l ../src/input/%Parser.h
	@echo 'Building file: $<'
	@echo 'Invoking: Flex++ Lexical Analyzer'
	flex++ -d -o"$(@:%.cpp=%.yy)" "$<"
	@echo 'Renaming lexer class in file: $(@:%.cpp=%.yy)'
	@echo 'Invoking: sed - stream editor'
	sed 's/yyFlexLexer/$*FlexLexer/g' "$(@:%.cpp=%.yy)" | sed 's/define $*FlexLexer/define yyFlexLexer/g' > "$@"
	@echo 'Finished renaming lexer class in file: $(@:%.cpp=%.yy)'
	@echo 'Finished building: $<'
	@echo ' '

src/input/%.o: ../src/input/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ $(COMP_FLAGS) -DDEBUG -O0 -g3 -Wall -ansi -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

