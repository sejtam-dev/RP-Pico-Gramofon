#include "FrameBuffer.h"

#include <cstring>

FrameBuffer::FrameBuffer() {
    this->buffer = new uint8_t[FRAMEBUFFER_SIZE];
}

void FrameBuffer::byteOR(int n, uint8_t byte) {
    if (n > (FRAMEBUFFER_SIZE-1)) return;
    this->buffer[n] |= byte;
}

void FrameBuffer::byteAND(int n, uint8_t byte) {
    if (n > (FRAMEBUFFER_SIZE-1)) return;
    this->buffer[n] &= byte;
}

void FrameBuffer::byteXOR(int n, uint8_t byte) {
    if (n > (FRAMEBUFFER_SIZE-1)) return;
    this->buffer[n] ^= byte;
}


void FrameBuffer::setBuffer(uint8_t *new_buffer) {
    this->buffer = new_buffer;
}

void FrameBuffer::clear() {
    memset(this->buffer, 0, FRAMEBUFFER_SIZE);
}

unsigned char *FrameBuffer::get() {
    return this->buffer;
}