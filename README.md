PSGPlay
=======

PSGPlay is a psg-based musical instrument for the sega genesis that supports up to two players.

installation
------------
1. download sgdk (windows only) https://code.google.com/p/sgdk/wiki/Download?tm=2
2. install sgdk using the instructions on the following page, but do not go past step 6: https://code.google.com/p/sgdk/wiki/HowToUseSGDK
3. run sgmake.bat to create out/rom.bin
4. load out/rom.bin as a genesis rom in your emulator of choice

how to play
-----------
six-button controllers required.

use the A,B,C buttons to set the pitch, and hold (or tap, if in sustain mode) d-pad right to sound a note. if in sustain mode, press d-pad left to silence the note. the pitches are set using binary fingering, with c as the lowest significant bit - thus, there are eight possible fingerings.

other controls:
d-pad up+right:    play note with pitch modulated up.
d-pad down+right:  play note with pitch modulated down.
X / Z:             increase/decrease octave.
Y:                 toggle sustain.
start+X / start+Z: transpose to next/previous key.
start+Y:           toggle pitch modulation depth (between semitone and octave).
start+A:           toggle vibrato.
start+B:           toggle arpeggio.
