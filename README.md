# USB_DSKY

This is a USB and Arduino keyboard heavily inspired by the NASA/Apollo
project's DSKY interface to the Apollo Guidance Computer (AGC)

It's made to look like the keypad of the DSKY, although slightly
smaller due to the components I had while making it.

The authentic "Block II" DSKY has buttons that are roughly 1 inch
between centers.  Mine is about 70 percent smaller, at 21mm between
centers.

It is implemented as matrix of Cherry MX Black keys with diodes as
three rows and 7 columns.  Due to the use of diodes, there's no
ghosting of keys.

# Enclosure

The enclosure was made to closely reproduce the look of the real
DSKY.  Mine was created using laser-cutting acrylic sheets (printable
pdfs included in the "enclosure" directory.) and sandwiching them
all together with M5 hex/allen socket screws.  The specific stack
of sheets and their sandwich ordering is currently TBD, as I'm still
working that out. ;)

I painted the top and bottom layers with a few thin coats of
gray primer, then a few coats of semi-gloss clear coat; all from
"rattle cans".

# Firmware

The included Arduino project is for a "Pro Micro" which is based
on the Arduino Leonardo, running an ATmega 32u4.  The wiring
configuration is also included in the project here.  I did wire up
the diodes wrong compared to what I intended, so I have to scan the
matrix backwards. (7 polls, check 3 rows each time rather than 3
polls, check 7 columns each time.) Oops.

The sketch is a pretty simple matrix scanner, which keeps track of
whether each of the 19 buttons has changed and if it had been
previously pressed or not.  Based on this, it will send key up
and key down USB HID events to the host computer.

# Keyboard map

The current key mapping is:

- "VERB" sends 'v'
- "NOUN" sends 'n'
- "+"/"-" send '+'/'-'
- "0".."9" send '0'..'9'
- "CLR" sends 'c'
- "PRO" sends 'p'
- "KEY REL" sends 'k'
- "ENTR" sends 'e'
- "REST" sends 'r'

# Future plans

I plan on eventually adding reprogrammability to the keyboard.  My
idea is that you press and hold a key (like "VERB") then press "PRO"
to program a key.  Then you press the target key, then a three digit
decimal value for the keycode. Perhaps it will use ASCII or a raw
HID keycode. I'm not sure of that detail yet.  Any remappings would
be stored in the EEPROM.  it might something like:

- Hold "VERB", press "PRO", release both
- Press "3", "ENTR"
- Type "0" "3" "5", "ENTR"

Now, the "3" key will send ascii 35, or '#' instead.

And of course to reset it, Hold "VERB" and press "REST" to restore it.

It would also be possible to store banks of keymaps too... 

- Hold "VERB", press "NOUN", release both
- Press "3", "ENTR"

And now it will be working out of bank 3.  Bank 0 could be the 
pre-canned values above...
