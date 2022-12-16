#ifndef GRAMOFON_FRAMEBUFFER_H
#define GRAMOFON_FRAMEBUFFER_H

#include "pico.h"
#include "pico/types.h"

#define FRAMEBUFFER_SIZE 1024

class FrameBuffer {
    uint8_t * buffer;

public:
    FrameBuffer();

    void byteOR(int n, uint8_t byte);
    void byteAND(int n, uint8_t byte);
    void byteXOR(int n, uint8_t byte);

    void setBuffer(uint8_t * new_buffer);

    void clear();

    unsigned char * get();
};

#endif //GRAMOFON_FRAMEBUFFER_H
