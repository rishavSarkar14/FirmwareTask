from tqdm import tqdm
import time
from serial import Serial
import os

#Opening the file
filename = "C:/Users/risha/Documents/Finance.txt"
file1 = open(filename, 'r')

#Opening the serial port COM3
ser = Serial('COM6', baudrate=2400)

#Defining a string to concatenate the received characters
string = bytearray()


for _ in tqdm(range(os.path.getsize(filename)), unit="bits", unit_scale=8):
    char = file1.read(1)

    if not char:
        ser.write(b'\x00')
        break
    
    else:
        ser.write(char.encode('utf-8'))


for _ in tqdm(range(os.path.getsize(filename)), unit="bits", unit_scale=8):
    char = ser.read(1)

    if not char:
        break
    else:
        string.append(int.from_bytes(char, "big"))

print("Exchange complete! Here is the received text: ")
print(string)
