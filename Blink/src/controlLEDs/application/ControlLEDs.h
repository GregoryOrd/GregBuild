#ifndef CONTROL_LEDS_H
#define CONTROL_LEDS_H

class I_BoardLEDDriver;

class ControlLEDs
{
public:
    ControlLEDs(I_BoardLEDDriver ledDriver);
    ~ControlLEDs();

    void configureBoardLEDForWriting();
    void turnOnBoardLED();
    void turnOffBoardLED();

private:
    I_BoardLEDDriver& ledDriver_;
};

#endif