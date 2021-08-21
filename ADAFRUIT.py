
from Adafruit_IO import Client, RequestError, Feed
from time import sleep
import serial
import time

def enviar():
    time.sleep(0.01)
    ent.write(centenas1)
    time.sleep(0.01)
    ent.write(decenas1)
    time.sleep(0.01)
    ent.write(unidades1)

ADAFRUIT_IO_USERNAME = "guillelam"
ADAFRUIT_IO_KEY = "aio_CKSq58qpqSSBGe49J4XSXAT24xpu"
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

ent = serial.Serial("COM3",9600)
ent.timeout = 3
time.sleep(1)

Sensor1_feed = aio.feeds('sensor1')
env_feed = aio.feeds('sensor2')

ent.write(b'a')
print('enviado')
centenas = ent.readline(1).decode("ascii")
decenas = ent.readline(1).decode("ascii")
unidades = ent.readline(1).decode("ascii")
contador_b = int(centenas+decenas+unidades)

aio.env_data(Sensor1_feed.key, contador_b)

env_data = aio.receive(env_feed.key)

bitcount = env_data.value
centenas1 = bitcount[0:1].encode('ascii', 'ignore')
decenas1 = bitcount[1:2].encode('ascii', 'ignore')
unidades1 = bitcount[2:3].encode('ascii', 'ignore')
enviar()    

