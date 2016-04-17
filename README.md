# arduino-audioMatching
The intention of this project is to isolate specific sounds and create a key. Then
the goal is to use that key and compare against live data to determine if the 
specified sound was present.

This program will collect data from a microphone, run the data through 
an FFT (http://wiki.openmusiclabs.com/wiki/ArduinoFFT) and use cosine similarity
to compare the result against a key.

I am starting from previous code which generates a keyFile from recorded data. And
allows cosine similarity comparison against recorded data. This was written in C++
and needs to be minimized and modified for live data for use on a moteino.
