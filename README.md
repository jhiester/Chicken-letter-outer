# Chicken-letter-outer
This is a constantly (and intermittently) evolving project wherein I use an amazon Alexa device along with a HUZZAHESP8266 Feather board from Adafruit to create a simple voice controlled chicken coop door opener. 

I've tried several libraries to get the HUZZAHESP8266 board and Alexa to work together. With most libraries I had trouble getting Alexa to recognize the device. I'm currently using the [Sinric](https://github.com/kakopappa/sinric) library and may incorporte/switch to [aRest](https://github.com/marcoschwartz/aREST). 

Currenlty, Alexa can detect and activate/deactivate the relay to control power to the motor. I'm in the process of building and testing the full circuit.