#include "gpio.h"
#include <lgpio.h>

Gpio::Gpio(QObject *parent) : QObject(parent)
{
    // initialization gpio
    m_handle = lgGpiochipOpen(CHIP);
    for (int pin : LEDS)
        lgGpioClaimOutput(m_handle, LFLAGS, pin, 0);
}

Gpio::~Gpio()
{
    // LED's Ausschalten
    for (int pin : LEDS)
        lgGpioWrite(m_handle, pin, 0);

    lgGpiochipClose(m_handle);
}

void Gpio::set(int value)
{
    for(auto pin : LEDS)
        lgGpioWrite(m_handle, pin, value);
}
