from firmata import *
a = Arduino('COM6', baudrate=57600) # Baudrate must match rate set in sketch
a.pin_mode(13, firmata.OUTPUT)
a.digital_write(13, firmata.HIGH) # LED on pin 13 on
a.digital_write(13, firmata.LOW) # LED on pin 13 off
a.serial.close()