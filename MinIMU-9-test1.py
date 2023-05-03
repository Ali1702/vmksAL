from time import time
import serial
ser = serial.Serial(port = 'COM3',baudrate = 9600, timeout = 1)
f = open("Serial"+str(time())+".txt", 'w')

roll=0
pitch=0
yaw=0
while 1:
    line = ser.readline()
    if line.find("!ANG:") != -1:          # filter out incomplete (invalid) lines
        line = line.replace("!ANG:","")   # Delete "!ANG...
        roll,pitch,yaw = [float(e) for e in line.split(",")]   # Convert text to float
        print("Roll:", roll, "Pitch:", pitch, "Yaw:", yaw)
        f.write("{0},{1},{2}\n".format(roll,pitch,yaw))  # Write to the output log file
