# Jesus Francisco Anaya Gonzalez A00823445

from grove_rgb_lcd import *
from time import sleep
from grovepi import *

# Pin configuration.
buttonPin = 5
sensorPin = 6
pinMode(buttonPin, "INPUT")


def readTemp():
    # Change backlight to green.
    setRGB(0, 255, 0);
    # Read the current temperature and transform it into a string.
    currTemp = str(grovepi.temp(sensorPin, '1.1'))
    setText("Temp = " + currTemp + "C")
    sleep(0.5)


def initialize():
    # Change backlight to red.
    setRGB(255, 0, 0);
    setText("Presiona el boton")
    sleep(0.5)


while True:
    try:
        buttonState = digitalRead(buttonPin)
        if buttonState:
            readTemp()
        else:
            initialize()
    except KeyboardInterrupt:
        print("Keyboard interrupt")
    except (IOError, TypeError) as e:
        print("Somenthing is Wrong")
