This is a constantly (and intermittently) evolving project.

I use an amazon echo dot along with a HUZZAHESP8266 Feather board from Adafruit to create a simple voice controlled chicken coop door opener. The motor can also be controlled via a web interface. I'm not very happy with the implementation of the web interface and controls. I'm looking into using my old raspberry-pi computer to serve node-red and a MQTT broker as an alternative. The initial tests are promising.

I've tried several libraries to get the HUZZAHESP8266 board and Alexa to work together. With most libraries I had trouble getting Alexa to recognize the device. I'm currently using the Sinric library and may incorporte/switch to aRest.
