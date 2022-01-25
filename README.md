# CoronaAiringStation

This Arduino project reminds regular airing by playing the Mario Theme melody.

__Disclaimer:__ This project was developed within the context of a university lecture.

## Project description

Every 20 minutes the opening of the window is reminded by playing the Mario Theme melody. Pressing the button signals that the window has been opened. After 5 minutes or when the humidity difference is exceeded, the melody is played again as a reminder to close the window. Pressing the button again signals that the window is closed.

## Project structure

The structure of the appliance can be viewed as a [Fritzing sketch](project_structure/CoronaAiringStation.pdf) and the program flow in the [state diagram](project_structure/CoronaAiringStation_Statechart).

## Implementation

For playing the Mario Theme melody, the code of [Dipto Pratyaksa](http://www.linuxcircle.com/2013/03/31/playing-mario-bros-tune-with-arduino-and-piezo-buzzer/) was used and modified for own needs.

Moreover, here are the references to the used [pitches](https://docs.arduino.cc/built-in-examples/digital/toneMelody) for the buzzer and the [DHT library](https://github.com/adafruit/DHT-sensor-library) for the used humidity sensor.