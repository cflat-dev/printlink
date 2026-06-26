#!/bin/sh

# Install libusb headers (still needs sudo)
sudo apt update
sudo apt install -y libusb-1.0-0-dev

# Clone MoonUSB if missing
if [ ! -d "moonusb" ]; then
    git clone https://github.com/stetre/moonusb/
fi

cd moonusb || exit 1

# Build for Lua 5.4
make LUAVER=5.4

# Install locally (no sudo)
make LUAVER=5.4 PREFIX="$HOME/.local" install

