import serial
from serial.tools import list_ports
import time

while(True):
    print("Available COM-Ports:")
    print([comport.device for comport in serial.tools.list_ports.comports()])
    port = input("Choose a port: ")
    try:
        s = serial.Serial(port, baudrate=115200)
    except (OSError, serial.SerialException):
        print("Could not open port: " + str(OSError))
        continue
    print("Success. Sending timestamp... Exit with ctrl-c")

    try:
        while(s.is_open):
            print("Writing string " + str(int(time.time())))
            s.write(str(str(int(time.time())) + '\n').encode('ascii'))
            s.flush()
            retcode = s.read_until().decode('ascii')
            print(retcode)
            if("Success" in retcode):
                print("nice.")
                break
            time.sleep(0.5)
    except KeyboardInterrupt:
        s.close()
        exit()
