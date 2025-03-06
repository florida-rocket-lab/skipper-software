build:
	python3 Builder/build/generate_build_directories.py

flight-computer:
	@echo off
	arduino-cli version  # Check if arduino-cli is installed

	PORT := $(shell arduino-cli board list | awk 'NR==2 {print $$1}')  # Automatically grabs the first connected board.
	BOARD_FQBN := $(shell arduino-cli board list | awk 'NR==2 {print $$5}')  # Should be: "teensy:avr:teensy41"; just for posterity or if we decide to change boards
	SKETCH_LOC := ./Builder/build/flight-computer  # Where the python script dumps the files

	mkdir -p ./Builder/binaries
	arduino-cli compile --fqbn $(BOARD_FQBN) --build-path ./Builder/binaries ./ $(SKETCH_LOC)
	arduino upload -p $(PORT) --fqbn $(BOARD_FQBN) --input-file ./Builder/binaries/flight-computer.ino.hex

skipper-transceiver:
	@echo off
	arduino-cli version  # Check if arduino-cli is installed

	PORT := $(shell arduino-cli board list | awk 'NR==2 {print $$1}')  # Automatically grabs the first connected board.
	BOARD_FQBN := $(shell arduino-cli board list | awk 'NR==2 {print $$5}')  # Should be: "teensy:avr:teensy41"; just for posterity or if we decide to change boards
	SKETCH_LOC := ./Builder/build/skipper-transceiver  # Where the python script dumps the files

	mkdir -p ./Builder/binaries
	arduino-cli compile --fqbn $(BOARD_FQBN) --build-path ./Builder/binaries ./ $(SKETCH_LOC)
	arduino upload -p $(PORT) --fqbn $(BOARD_FQBN) --input-file ./Builder/binaries/skipper-transceiver.ino.hex

ground-transceiver:
	@echo off
	arduino-cli version  # Check if arduino-cli is installed

	PORT := $(shell arduino-cli board list | awk 'NR==2 {print $$1}')  # Automatically grabs the first connected board.
	BOARD_FQBN := $(shell arduino-cli board list | awk 'NR==2 {print $$5}')  # Should be: "teensy:avr:teensy41"; just for posterity or if we decide to change boards
	SKETCH_LOC := ./Builder/build/ground-transceiver  # Where the python script dumps the files

	mkdir -p ./Builder/binaries
	arduino-cli compile --fqbn $(BOARD_FQBN) --build-path ./Builder/binaries ./ $(SKETCH_LOC)
	arduino upload -p $(PORT) --fqbn $(BOARD_FQBN) --input-file ./Builder/binaries/ground-transceiver.ino.hex

clean:
	rm -f ./Builder/binaries/flight-computer.ino.hex
	rm -f ./Builder/binaries/skipper-transceiver.ino.hex
	rm -f ./Builder/binaries/ground-transceiver.ino.hex
