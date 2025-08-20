#include "i8080Memory.hpp"

I8080Memory::~I8080Memory()
{
    free(memory);
    // std::cout << "Unload memory i8080" << std::endl;
}

bool I8080Memory::I8080MemoryInitialize(uint16_t memorySize)
{
    this->memorySize = memorySize;
    memory = (uint8_t *)malloc(memorySize * sizeof(uint8_t));
    if (memory == NULL)
    {
        // std::cout << "Error allocating i8080 memory: " << memorySize << std::endl;
        return false;
    }
    for (int i = 0; i < memorySize; i++)
    {
        memory[i] = 0;
    }
    return true;
}

/*bool I8080Memory::LoadRom(const uint8_t *rom, uint16_t size)
{
    if (size > memorySize)
    {
        // std::cout << "Error loading ROM: " << size << " > " << memorySize << std::endl;
        return false;
    }
    for (int i = 0; i < size; i++)
    {
        memory[i] = rom[i];
    }
    return true;
}*/

/*bool I8080Memory::LoadRom(std::string filename)
{
    // std::cout << "i8080 Loading file " << filename << std::endl;
    FILE *pFile;
    long lSize;
    unsigned char *buffer;
    size_t result;

    const char *fname = filename.c_str();
    pFile = fopen(fname, "rb");
    if (pFile == NULL)
    {
        fputs("File error", stderr);
        return false;
        // exit(1);
    }

    // obtain file size:
    fseek(pFile, 0, SEEK_END);
    lSize = ftell(pFile);
    rewind(pFile);
    // std::cout << "Size: " << lSize << std::endl;

    // allocate memory to contain the whole file:
    buffer = (unsigned char *)malloc(sizeof(char) * lSize);
    if (buffer == NULL)
    {
        fclose(pFile);
        fputs("Memory error", stderr);
        return false;
        // exit(2);
    }

    // copy the file into the buffer:
    result = fread(buffer, 1, lSize, pFile);
    if (result != lSize)
    {
        fclose(pFile);
        free(buffer);
        fputs("Reading error", stderr);
        return false;
        // exit(3);
    }

    // the whole file is now loaded in the memory buffer. 

    // transfer the contents of buffer to the memory starting
    // at address 0x0
    for (int i = 0; i < lSize; ++i)
    {
        // if (i < 10)
        //{
        //     printf("%02x \n", buffer[i]);
        // }
        memory[i] = buffer[i];
    }

    // terminate
    fclose(pFile);
    free(buffer);
    return true;
    // std::cout << "Load ok." << std::endl;
}*/


uint8_t I8080Memory::Read(uint16_t address)
{
    return memory[address];
}

void I8080Memory::Write(uint16_t address, uint8_t data)
{
    memory[address] = data;
}
