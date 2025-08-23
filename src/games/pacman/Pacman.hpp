#ifndef PACMAN_HPP
#define PACMAN_HPP

#include <Game.hpp>

class Pacman : public Game
{
public:
    Pacman();
    ~Pacman();

    void Setup(SdCard &sdCard) override;
    void Loop() override;

private:

};

#endif