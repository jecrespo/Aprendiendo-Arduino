#!/usr/bin/python
'''
Created on 16/02/2016
@author: jecrespo
@version: 1.0
@description: Blink Arduino mediante Firmata
@url: http://www.aprendiendoarduino.com/
'''


import time
import sys
from PyMata.pymata import PyMata

BOARD_LED = 13
port = input("Puerto Serie Arduino: ")

try:
    arduino = PyMata(port, verbose=True)

    for x in range(10):
        print(x + 1)
        # Set the output to 1 = High
        arduino.digital_write(BOARD_LED, 1)
        # Wait a half second between toggles.
        time.sleep(.5)
        # Set the output to 0 = Low
        arduino.digital_write(BOARD_LED, 0)
        time.sleep(.5)

    # Close PyMata when we are done
    arduino.close()
except: # catch *all* exceptions
    e = sys.exc_info()[0]
    print("Puerto no válido. %s" % e)
