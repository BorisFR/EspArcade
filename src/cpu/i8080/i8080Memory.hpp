/*
 * File: memory.h
 * Description: Abstracts the memory connected to the 8080 cpu.
 * */

#ifndef I8080_MEMORY_h
#define I8080_MEMORY_h

// #include <iostream>
#include <stdint.h>
#include <string>

class I8080Memory
{
public:
    ~I8080Memory();
    bool I8080MemoryInitialize(uint16_t memorySize);
    //bool LoadRom(const uint8_t *rom, uint16_t size);
    //bool LoadRom(std::string filename);
    uint8_t Read(uint16_t address);
    void Write(uint16_t address, uint8_t data);
    uint8_t *memory;

private:
    uint16_t memorySize;
};

#endif