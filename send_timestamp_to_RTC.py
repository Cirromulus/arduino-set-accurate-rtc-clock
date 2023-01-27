#!/usr/bin/python3

import serial
from serial.tools import list_ports
import datetime
from datetime import timezone
import time

seconds_from_1970_to_2000 = datetime.datetime(2000,1,1,0,0,0,tzinfo=timezone.utc).timestamp()

while(True):
    print("Available COM-Ports:")
    print([comport.device for comport in serial.tools.list_ports.comports()])
    if(len(serial.tools.list_ports.comports()) == 0):
        time.sleep(.5)
        break
    port = serial.tools.list_ports.comports()[0].device

    if(len(serial.tools.list_ports.comports()) > 1):
        port = input("Choose a port: ")
    try:
        s = serial.Serial(port, baudrate=115200,timeout=.5)
    except (OSError, serial.SerialException):
        print("Could not open port: " + str(OSError))
        time.sleep(.5)
        continue

    print("Sending timestamp... Exit with ctrl-c")

    try:
        while(s.is_open):
            unix_timestamp = datetime.datetime.now(tz=timezone.utc).timestamp()
            seconds_since_2000 = unix_timestamp - seconds_from_1970_to_2000
            timestamp_to_send = (str(int(seconds_since_2000)) + '\r\n')
            print("Writing string " + timestamp_to_send)
            s.write(timestamp_to_send.encode('ascii'))
            s.flush()
            retcode = s.readline().decode('ascii')
            print(retcode)
            if("Success" in retcode):
                print("nice.")
                exit()
            time.sleep(1)
    except KeyboardInterrupt:
        print("stop")
    s.close()
    exit()