import serial
import requests
import json
import time
from enum import Enum

baseUrl = "http://raspi.local/"
serialPort = "/dev/cu.usbmodem1421"
updateInterval = 5


class Command(Enum):
    Error = 1
    Request = 2
    Send = 3
    Ping = 4

commandIDs = {
    Command.Error:   0x0,
    Command.Request: 0x1,
    Command.Send:    0x5,
    Command.Ping:    0x4
}


def create_header(deviceID, command):
    commandID = commandIDs[command]

    header = deviceID << 3
    header = header | commandID

    return header


def send_message(device, command, datatype, value):
    header = create_header(device["deviceID"], command)
    message = bytes([header, datatype, value])
    ser.write(message)
    return message


def receive_message():
    i = ser.read(4)
    return i

ser = serial.Serial(
    port=serialPort,
    baudrate=9600,
    parity=serial.PARITY_ODD,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1)

device = {"deviceID": 2, "productID": 0x10}

send_message(device, Command.Send, 0x1, 0x1)

#devices = []
#availableIDs = [1 .. 31]
#lastUpdateTime = time.time()

#r = requests.get(baseUrl + "api/devices/")
#devices = r.json()

#while True:
#    if (time.time - lastUpdateTime >= updateInterval):
#        r = requests.get(baseUrl + "api/commands/")

# for device in devices:
#     send_message(device, Command.Ping)
#     back = receive_message()
#     if (back[0]=0x0):
#         devices.remove(device)
