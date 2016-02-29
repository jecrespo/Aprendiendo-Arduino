#!/usr/bin/python
'''
Created on 16/02/2016
@author: jecrespo
@version: 1.0
@description: Medidor de luz mediante Firmata
@url: http://www.aprendiendoarduino.com/
'''

from pyfirmata import Arduino, util
import time
board=Arduino('COM9')
ledpin = board.get_pin('d:13:o') # d=digital 13=pin number o=output

#testeo la conexión
ledpin.write(1)
time.sleep(1)
ledpin.write(0)

#ejecuto el programa

# Pines 2, 3 y 4
p1 = board.get_pin('d:2:o')
p2 = board.get_pin('d:3:o')
p3 = board.get_pin('d:4:o')

#para leer de puertos analógicos inicio un iterador, en caso contrario el Arduino
#sigue mandado datos hasta que llena el buffer del puerto serie
it = util.Iterator(board)
it.start()

#fotoresistencia conectado a entrada analógica A0
l = board.get_pin('a:0:i')
l.enable_reporting()
val = l.read()

#Para parar el programa poner a masa pin A0
while (val > 0):
	
	print (val)
	p1.write(0)
	p2.write(0)
	p3.write(0)
	if (val > 256):
		p1.write(1)
		print("Enciendo 1")
	if (val > 512):
		p2.write(1)
		print("Enciendo 2")
	if (val > 768):
		p3.write(1)
		print("Enciendo 3")
	
	val = l.read()*1000
	time.sleep(1)

print("Fin de programa")
board.exit()
