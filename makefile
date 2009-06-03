
RM := rm -rf

all: release

debug:
	@echo "Building for debug..."
	@cd ./debug/ ; make all
	@cp -u ./debug/sharp .
	@echo "Done building."
	@echo " "

release:
	@echo "Building for release..."
	@cd ./release/ ; make all
	@cp -u ./release/sharp .
	@echo "Done building."
	@echo " "

clean:
	@cd ./debug/ ; make clean
	@cd ./release/ ; make clean
	-$(RM) src/input/DIMACSParser.cpp src/input/DIMACSParser.h src/input/DIMACSLexer.cpp sharp

.PHONY: all release debug clean