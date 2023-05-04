from time import time
import serial
ser = serial.Serial(port = 'COM3',baudrate = 9600, timeout = 1)
f = open("Serial"+str(time())+".txt", 'w')

velocity = 0
acceleration = 0

while 1:
    line = ser.readline()
    if line.find("A:") != -1:          # filter out incomplete (invalid) lines
        line = line.replace("A:","")   # Delete "!ANG...
        velocity, acceleration = [float(e) for e in line.split(",")]   # Convert text to float
        print("Velocity:", velocity, "Acceleration: ", acceleration)
        f.write("{0}, {1}\n".format(velocity, acceleration))  # Write to the output log file
