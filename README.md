# Hardware Password Manager

A hardware password manager that is plugged in between a keyboard and a computer. In pass-through mode, it does nothing with the keystrokes. In password manager mode, it allows you to type in the login you want to select, and then input a simple password. The password manager hashes the password and inputs it into the computer.

## Features

- Plugs in between a keyboard and a computer
- Pass-through mode and password manager mode
- Account and password input via the keyboard
- LCD screen for displaying account name and masked password
- Password hashing

## Required Hardware

- Arduino board
- PS2 Keyboard
- LiquidCrystal display

## Dependencies

- PS2Keyboard.h
- Keyboard.h
- LiquidCrystal.h
- sha256.c (from https://github.com/amosnier/sha-2)

## Installation

1. Download or clone this repository.
2. Install the required libraries:
   - [PS2Keyboard](https://github.com/PaulStoffregen/PS2Keyboard)
   - [Keyboard](https://github.com/arduino-libraries/Keyboard)
   - [LiquidCrystal](https://github.com/arduino-libraries/LiquidCrystal)
3. Upload the main Arduino code to your Arduino board.

## Usage

1. Connect the hardware password manager between your keyboard and computer.
2. In pass-through mode, the device will simply pass the keystrokes to the computer without processing them.
3. In password manager mode, type the account name (e.g., "Amazon") and press Enter. The account name will be displayed on the LCD screen.
4. Type your simple password for the account and press Enter.
5. The device will hash the password and input it into the computer.
