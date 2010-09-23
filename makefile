
CC := g++

all: release

debug:
	@echo "Building for debug..."
	@cd ./debug/ ; make all CC="$(CC)"
	@cp -u ./debug/sharp .
	@echo "Done building."
	@echo " "

debug-verbose:
	@echo "Building for debug..."
	@cd ./debug/ ; make verbose CC="$(CC)"
	@cp -u ./debug/sharp .
	@echo "Done building."
	@echo " "

release:
	@echo "Building for release..."
	@cd ./release/ ; make all CC="$(CC)"
	@cp -u ./release/sharp .
	@echo "Done building."
	@echo " "

clean:
	@cd ./debug/ ; make clean
	@cd ./release/ ; make clean
	-$(RM) src/input/DIMACSParser.cpp src/input/DIMACSParser.h src/input/DIMACSLexer.cpp src/input/DatalogParser.cpp src/input/DatalogParser.h src/input/DatalogLexer.cpp src/input/ArgumentationParser.cpp src/input/ArgumentationParser.h src/input/ArgumentationLexer.cpp sharp

.PHONY: all release debug debug-verbose clean
