import serial.tools.list_ports


ports = list(serial.tools.list_ports.comports())
for p in ports:
    print p


ser = serial.Serial('/dev/ttyACM0', 9600, timeout=3)
ser.write(shortcoords.txt)


while(7):
	var = True;
