# Trans-Lux-LED-Ticker
Quick Communication demo for a translux LED ticker

This is a program to interface with a circa 2002 Trans-Lux Graph Jet LED stock ticker. Bicolour LEDS (red, green, and orange). The commands below were deduced by spamming the sign with random data and figuring out what did what.

###Hardware notes 
We used a Particle Photon to send data to the sign over WIFI, but anything that can do TTL serial should work. Communication is 9600 baud standard 8 bit. We bypassed the signal in opticoupler and connected directly to the hardware UART on the Z8 microcontroler (pin 25). 

###Theory of Operation
The sign works by sending data to the vertical column of pixels on the far right hand side of the sign. It then shift registers this to the left as new data comes in. 
The data packet consists of a single colour byte, up to 5 data bytes and a 5 to write the buffer to the display. 
To calculate the data bytes, divide the vertical line buffer into 5 rows of 5 leds. Figure out which leds you want to light in each section, treat a lit led as a 1 and off as a 0 and convert that binary to an int. So for example 11011 is 27, 00100 is 4, etc. Add 32 to that number for the first section, 64 for the second etc. 

The sign doesn't apear to have a built in character map, so you have to render the text/graphics yourself.

###Protocol Command List
I am going to use decimal equivalents for the byte values. 

* 4: clears the screen
* 5: writes the buffer to the screen - send a colour and up to 5 data bytes first
colour modes: The sign can only do two colours at once, with off being a colour 
* 6: red pixel 
* 7: green
* 8: orange
* 9: interlaced red and green 
* 10: red and orange
* 11: green and red
* 12: green and orange
* 13: orange and red
* 14: orange and green

* 15: makes the sign blink at ~1hz
* 16: stops the blinking

data blocks
* 32-63 first block of 5 leds
* 64-95 second block of 5
* 96-127: third block of 5
* 128-159: forth block of 5
* 160-191: fifth block - bottom row ignored (screen is 24 pixels high not 25).

