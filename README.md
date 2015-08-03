Membrane
=======

Membrane is a musical instrument for the Sega Genesis that supports up to two players.

Run
-----------

Load Membrane.bin using your favourite Genesis emulator or flash cartridge. If you acquired Membrane through a source distribution, you will need to build Membrane.bin yourself using the below instructions.

See manual.pdf for help on playing the game itself.

Build (Windows only)
------------

**Dependencies**  
sgdk: https://code.google.com/p/sgdk/wiki/Download?tm=2  
sgmake: https://github.com/deckman/sgmake

1. Compile and install SGDK using the instructions on the following page (ignore the step about "compiling your own project"): https://github.com/Stephane-D/SGDK/wiki/Setup-SGDK-basic
2. Build and install sgmake using the instructions in sgmake's README file
3. Run sgmake from this project to create out/Membrane.bin
