import serial

ser = serial.Serial('COM5', baudrate=115200, bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE,
                    stopbits=serial.STOPBITS_ONE)

while True:
    print(ser.read())
