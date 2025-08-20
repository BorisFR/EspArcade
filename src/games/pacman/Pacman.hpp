#ifndef PACMAN_HPP
#define PACMAN_HPP

#include <Arduino.h>
#include <SdCard.hpp>

class Pacman
{
public:
    Pacman();
    ~Pacman();

    void Setup(SdCard &sdCard);
    void Loop();

private:

};

#endif