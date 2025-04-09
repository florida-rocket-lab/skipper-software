build:
	python3 Builder/build/generate_build_directories.py

flight-computer:
	@echo off
	echo Beginning to build the flight computer, a.k.a. the Teensy 4.0 ...
	arduino-cli version  # Check if arduino-cli is installed

	PORT := $(shell arduino-cli board list | awk 'NR==2 {print $$1}')  # Automatically grabs the first connected board.
	echo Detected an Arduino on port $(PORT)...
	BOARD_FQBN := $(shell arduino-cli board list | awk 'NR==2 {print $$5}')  # Should be: "teensy:avr:teensy41"; just for posterity or if we decide to change boards
	echo Detected the Board FQBN, a.k.a. the make of the board, as $(BOARD_FQBN), which should be teensy:avr:teensy41
	SKETCH_LOC := ./Builder/build/flight-computer  # Where the python script dumps the files

	mkdir -p ./Builder/binaries
	echo Compiling...
	arduino-cli compile --fqbn $(BOARD_FQBN) --build-path ./Builder/binaries ./ $(SKETCH_LOC)
	echo Uploading...
	arduino upload -p $(PORT) --fqbn $(BOARD_FQBN) --input-file ./Builder/binaries/flight-computer.ino.hex
	echo Finished!

flight-computer-test:
	@echo off
	echo Beginning to build the flight computer testing suite, meaning you should be uploading to the Teensy 4.0 ...
	arduino-cli version  # Check if arduino-cli is installed

	PORT := $(shell arduino-cli board list | awk 'NR==2 {print $$1}')  # Automatically grabs the first connected board.
	echo Detected an Arduino on port $(PORT)...
	BOARD_FQBN := $(shell arduino-cli board list | awk 'NR==2 {print $$5}')  # Should be: "teensy:avr:teensy41"; just for posterity or if we decide to change boards
	echo Detected the Board FQBN, a.k.a. the make of the board, as $(BOARD_FQBN), which should be teensy:avr:teensy41
	SKETCH_LOC := ./Builder/build/flight-computer-test  # Where the python script dumps the files

	mkdir -p ./Builder/binaries
	echo Compiling...
	arduino-cli compile --fqbn $(BOARD_FQBN) --build-path ./Builder/binaries ./ $(SKETCH_LOC)
	echo Uploading...
	arduino upload -p $(PORT) --fqbn $(BOARD_FQBN) --input-file ./Builder/binaries/flight-computer.ino.hex
	echo Finished!

skipper-transceiver:
	@echo off
	echo Beginning to build the skipper transceiver, a.k.a. the Nano ...
	arduino-cli version  # Check if arduino-cli is installed

	PORT := $(shell arduino-cli board list | awk 'NR==2 {print $$1}')  # Automatically grabs the first connected board.
	echo Detected an Arduino on port $(PORT)...
	BOARD_FQBN := $(shell arduino-cli board list | awk 'NR==2 {print $$5}')  # Should be: "arduino:avr:nano"; just for posterity or if we decide to change boards
	echo Detected the Board FQBN, a.k.a. the make of the board, as $(BOARD_FQBN), which should be arduino:avr:nano
	SKETCH_LOC := ./Builder/build/skipper-transceiver  # Where the python script dumps the files

	mkdir -p ./Builder/binaries
	echo Compiling...
	arduino-cli compile --fqbn $(BOARD_FQBN) --build-path ./Builder/binaries ./ $(SKETCH_LOC)
	echo Uploading...
	arduino upload -p $(PORT) --fqbn $(BOARD_FQBN) --input-file ./Builder/binaries/skipper-transceiver.ino.hex
	echo Finished!

skipper-transceiver-test:
	@echo off
	echo Beginning to build the skipper transceiver testing suite, meaning you should be uploading to the Nano ...
	arduino-cli version  # Check if arduino-cli is installed

	PORT := $(shell arduino-cli board list | awk 'NR==2 {print $$1}')  # Automatically grabs the first connected board.
	echo Detected an Arduino on port $(PORT)...
	BOARD_FQBN := $(shell arduino-cli board list | awk 'NR==2 {print $$5}')  # Should be: "arduino:avr:nano"; just for posterity or if we decide to change boards
	echo Detected the Board FQBN, a.k.a. the make of the board, as $(BOARD_FQBN), which should be arduino:avr:nano
	SKETCH_LOC := ./Builder/build/skipper-transceiver-test  # Where the python script dumps the files

	mkdir -p ./Builder/binaries
	echo Compiling...
	arduino-cli compile --fqbn $(BOARD_FQBN) --build-path ./Builder/binaries ./ $(SKETCH_LOC)
	echo Uploading...
	arduino upload -p $(PORT) --fqbn $(BOARD_FQBN) --input-file ./Builder/binaries/skipper-transceiver.ino.hex
	echo Finished!

ground-transceiver:
	@echo off
	echo Beginning to build the skipper transceiver, a.k.a. the Uno ...
	arduino-cli version  # Check if arduino-cli is installed

	PORT := $(shell arduino-cli board list | awk 'NR==2 {print $$1}')  # Automatically grabs the first connected board.
	echo Detected an Arduino on port $(PORT)...
	BOARD_FQBN := $(shell arduino-cli board list | awk 'NR==2 {print $$5}')  # Should be: "arduino:avr:uno"; just for posterity or if we decide to change boards
	echo Detected the Board FQBN, a.k.a. the make of the board, as $(BOARD_FQBN), which should be arduino:avr:uno
	SKETCH_LOC := ./Builder/build/ground-transceiver  # Where the python script dumps the files

	mkdir -p ./Builder/binaries
	echo Compiling...
	arduino-cli compile --fqbn $(BOARD_FQBN) --build-path ./Builder/binaries ./ $(SKETCH_LOC)
	echo Uploading...
	arduino upload -p $(PORT) --fqbn $(BOARD_FQBN) --input-file ./Builder/binaries/ground-transceiver.ino.hex
	echo Finished!

ground-transceiver-test:
	@echo off
	echo Beginning to build the ground transceiver testing suite, meaning you should be uploading to the Uno ...
	arduino-cli version  # Check if arduino-cli is installed

	PORT := $(shell arduino-cli board list | awk 'NR==2 {print $$1}')  # Automatically grabs the first connected board.
	echo Detected an Arduino on port $(PORT)...
	BOARD_FQBN := $(shell arduino-cli board list | awk 'NR==2 {print $$5}')  # Should be: "arduino:avr:uno"; just for posterity or if we decide to change boards
	echo Detected the Board FQBN, a.k.a. the make of the board, as $(BOARD_FQBN), which should be arduino:avr:uno
	SKETCH_LOC := ./Builder/build/ground-transceiver-test  # Where the python script dumps the files

	mkdir -p ./Builder/binaries
	echo Compiling...
	arduino-cli compile --fqbn $(BOARD_FQBN) --build-path ./Builder/binaries ./ $(SKETCH_LOC)
	echo Uploading...
	arduino upload -p $(PORT) --fqbn $(BOARD_FQBN) --input-file ./Builder/binaries/ground-transceiver.ino.hex
	echo Finished!

clean:
	@echo off

	echo Removing flight-computer binary...
	rm -f ./Builder/binaries/flight-computer.ino.hex
	echo Removing skipper-transceiver binary...
	rm -f ./Builder/binaries/skipper-transceiver.ino.hex
	echo Removing ground-transceiver binary...
	rm -f ./Builder/binaries/ground-transceiver.ino.hex
