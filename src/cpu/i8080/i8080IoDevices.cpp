#include "i8080IoDevices.hpp"

void I8080IoDevices::IoDevicesInitialize()
{
    port0 = 0x00;
    port1 = 0x00;
    port2 = 0x00;
    portout3 = 0x00;
    portout5 = 0x00;
    shift_register_value = 0x0000;
}

uint8_t I8080IoDevices::Read_device(uint8_t device_number)
{
    switch (device_number)
    {
    case 0x0:
        return port0;
    case 0x1:
        return port1;
    case 0x2:
        return port2;
    case 0x3:
        return Get_shift_register_result();
    default:
        return 0u;
    }
}

void I8080IoDevices::Write_device(uint8_t device_number, uint8_t data)
{
    switch (device_number)
    {
    case 0x2:
        Set_shift_register_result_offset(data);
        break;
    case 0x3:
        portout3 = data;
        break;
    case 0x5:
        portout5 = data;
        break;
    case 0x4:
        Fill_shift_register(data);
        break;
    default:
        break;
    }
}

void I8080IoDevices::UpdateSounds()
{
}

void I8080IoDevices::SetupSounds()
{
}

uint8_t I8080IoDevices::Get_shift_register_result()
{
    // return ((shift_register_value >> (8 - shift_register_offset)) & 0xff);
    return (shift_register_value << shift_register_offset) >> 8;
}

void I8080IoDevices::Set_shift_register_result_offset(uint8_t amount)
{
    shift_register_offset = amount & 0x7;
    //if (amount < 8)
    //    shift_register_offset = amount;
}

void I8080IoDevices::Fill_shift_register(uint8_t data)
{
    shift_register_value = (shift_register_value >> 8) | (data << 8);
}
