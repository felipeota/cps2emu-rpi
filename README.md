CPS2Emu port for Raspberry PI
=============================

This is a port of CPS2Emu for the Raspberry PI.

How to compile
--------------

### On the Raspbery Pi

You need sdl development packages. For Raspbian:

    sudo apt-get install libsdl-dev

Then

    make

### Cross compile

To cross compile you have to set the environment variables CROSS_COMPILE to the prefix of the toolchain and SYS_ROOT to the root of a Raspbian image. 
I recommend using [this](https://github.com/raspberrypi/tools) toolchain and to
follow [this](http://www.raspberrypi.org/phpBB3/viewtopic.php?f=31&t=8478) to get a Raspbian image as SYS_ROOT.
After that simply run

    make

How to run
----------

After compiling you'll get two executables: cps2emu and cps2romcnv. The first one is the emulator and the latter is a tool to cache gfx from roms (I don't know what it does in full detail).
Create a cps2emu directory with the following structure:
- config/ -- This is the config directory from this repository, copy it.
- roms/ -- This will hold the roms.
- cps2emu
- cps2romcnv

Place the roms zip in the roms/ directory and from the csp2emu directory run

    ./cps2romcnv <rom>

where <rom> is the rom name you want to run (without the extension).
This will create the cache file for the rom, which is mandatory. It can take a while to run, so be patient.
After that you can run

    ./cps2emu <rom> #without the extension

And that's it.

Keys
----

Arrows: stick

ASDZCX: buttons

T: insert coin

U: start

L: menu

Issues
------

The sound is not working
It doesn't refresh the whole when going from a menu to the game.
