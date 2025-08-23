#include "Driver8080bw.hpp"

static unsigned char palette[] = /* V.V */ /* Smoothed pure colors, overlays are not so contrasted */
    {
        0x00, 0x00, 0x00, /* BLACK */
        0xff, 0x20, 0x20, /* RED */
        0x20, 0xff, 0x20, /* GREEN */
        0xff, 0xff, 0x20, /* YELLOW */
        0xff, 0xff, 0xff, /* WHITE */
        0x20, 0xff, 0xff, /* CYAN */
        0xff, 0x20, 0xff  /* PURPLE */
};

enum { BLACK,RED,GREEN,YELLOW,WHITE,CYAN,PURPLE }; /* V.V */

// ********************************************************

Driver8080bw::Driver8080bw()
{
    Serial.println("Driver8080bw created");
}

Driver8080bw::~Driver8080bw()
{
    Serial.println("Driver8080bw destroyed");
}
