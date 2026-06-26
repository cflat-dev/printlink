## printlink — a simple G‑code streamer tool
printlink is a lightweight host that connects to a 3D printer over USB and exposes a TCP socket so any client can stream G‑code line‑by‑line.


how to build on raspberry pi os:
```
sudo apt install libserialport-dev
git clone  https://github.com/cflat-dev/printlink
cd printlink
mkdir build/
cd build/
cmake ..
make
```
## Server protocol
1. the server can only have one client at  a time
2. The server streams printer replies immediately
3.  the client can send only one gcode line at a time ending with a new line ("\n")

