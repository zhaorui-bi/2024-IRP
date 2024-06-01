import serial.tools.list_ports
import time

plist = list(serial.tools.list_ports.comports())

if len(plist) <= 0:
    print("no port!")
else:
    serialName = "/dev/ttyACM0"
    serialFd = serial.Serial(serialName, 9600, timeout=60)
    print("available port>>>", serialFd.name)
        # op = input()
        # serialFd.write(op.encode())
        # time.sleep(1)   
    op = "i"+'120320'+"o"
    print(op)
    time.sleep(5)
    serialFd.write(op.encode())
    time.sleep(5)
        
