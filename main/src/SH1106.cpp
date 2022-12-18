#include "SH1106.h"

#include "hardware/gpio.h"

#include <cstring>
#include <cmath>
#include <iostream>

SH1106::SH1106(i2c_inst *inst, uint8_t scl, uint8_t sda, uint8_t address, uint32_t baudRate, SH1106_SIZE size)
    : inst(inst), scl(scl), sda(sda), address(address), baudRate(baudRate), inverted(false)
{}

void SH1106::init() {
    i2c_init(inst, baudRate);

    // Initialize I2C pins
    gpio_set_function(scl, GPIO_FUNC_I2C);
    gpio_set_function(sda, GPIO_FUNC_I2C);

    gpio_pull_up(scl);
    gpio_pull_up(sda);

    width = 128;
    if(size == SH1106_SIZE::W128xH32) {
        height = 32;
    } else {
        height = 64;
    }

    frameBuffer = FrameBuffer();

/*
    uint8_t setupCommands[] = {
            SH1106_DISPLAY_OFF,
            SH1106_LOWCOLUMN,
            SH1106_HIGHCOLUMN,
            SH1106_STARTLINE,

            SH1106_MEMORYMODE,
            SH1106_MEMORYMODE_HORZONTAL,

            SH1106_CLUMN_REMAP_OFF | 0x00,
            SH1106_COM_REMAP_OFF | 0x08,

            SH1106_CONTRAST,
            0xFF,

            SH1106_INVERTED_OFF,

            SH1106_MULTIPLEX,
            0x3F,

            SH1106_DISPLAYOFFSET,
            0x00,

            SH1106_DISPLAYCLOCKDIV,
            0x80,

            SH1106_PRECHARGE,
            0x22,

            SH1106_COMPINS,
            0x12,

            SH1106_VCOMDETECT,
            0x40,

            SH1106_CHARGEPUMP,
            0x14,

            SH1106_DISPLAYALL_ON_RESUME,
            SH1106_DISPLAY_ON
    };
*/


    uint8_t setupCommands[] = {
            SH1106_DISPLAY_OFF,

            SH1106_DISPLAYCLOCKDIV,
            0x80,

            SH1106_MULTIPLEX,
            0x3F,

            SH1106_DISPLAYOFFSET,
            0x00,

            SH1106_STARTLINE,

            SH1106_CHARGEPUMP,
            0x14,

            SH1106_MEMORYMODE,
            0x00,

            SH1106_CLUMN_REMAP_OFF,
            SH1106_COM_REMAP_ON,

            SH1106_COMPINS,
            0x12,

            SH1106_CONTRAST,
            0xCF,

            SH1106_PRECHARGE,
            0xF1,

            SH1106_VCOMDETECT,
            0x20,

            SH1106_DISPLAYALL_ON_RESUME,
            SH1106_INVERTED_OFF,

            SH1106_DISPLAY_ON
    };

    for (uint8_t &command: setupCommands) {
        sendCommand(command);
    }

    clear();
    sendBuffer();
}

void SH1106::sendCommand(uint8_t command) {
    uint8_t data[2] = {0x00, command};
    i2c_write_blocking(inst, address, data, 2, false);
}

void SH1106::setPixel(int16_t x, int16_t y, SH1106_WRITE_MODE mode) {
    if (x < 0 || x >= width)
        return;

    if(y < 0 || y >= height)
        return;

    uint8_t byte;
    if (size == SH1106_SIZE::W128xH32) {
        y = (y << 1) + 1;
        byte = 1 << (y & 7);
        char byte_offset = byte >> 1;
        byte = byte | byte_offset;
    } else {
        byte = 1 << (y & 7);
    }

    int n = x + (y / 8) * width;
    if (mode == SH1106_WRITE_MODE::ADD) {
        frameBuffer.byteOR(n, byte);
    } else if (mode == SH1106_WRITE_MODE::SUBTRACT) {
        frameBuffer.byteAND(n, ~byte);
    } else if (mode == SH1106_WRITE_MODE::INVERT) {
        frameBuffer.byteXOR(n, byte);
    }
}

void SH1106::sendBuffer() {
    uint8_t * buffer = frameBuffer.get();

    int i = 0;
    for (int page = 0; page < height / 8; page++) {
        sendCommand(SH1106_SET_PAGE_ADDRESS + page);
        sendCommand(0x02);
        sendCommand(0x10);

        for(int j = 0; j < width; j++) {
            uint8_t data[17];
            data[0] = SH1106_STARTLINE;

            for (uint8_t y = 0; y < 16; y++) {
                data[y + 1] = buffer[i];

                i++;
                j++;
            }

            j--;
            i2c_write_blocking(inst, address, data, 17, false);
        }
    }
}

void SH1106::clear() {
    frameBuffer.clear();
}

void SH1106::clear(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    for(int x = x0; x < x1; x++) {
        for(int y = y0; y < y1; y++) {
            setPixel(x, y, SH1106_WRITE_MODE::SUBTRACT);
        }
    }
}

void SH1106::setOrientation(bool flipped) {
    if (flipped) {
        sendCommand(SH1106_CLUMN_REMAP_OFF);
        sendCommand(SH1106_COM_REMAP_OFF);
    } else {
        sendCommand(SH1106_CLUMN_REMAP_ON);
        sendCommand(SH1106_COM_REMAP_ON);
    }
}

void SH1106::invertDisplay() {
    inverted = !inverted;
    sendCommand(SH1106_INVERTED_OFF | !inverted);
}

void SH1106::setContrast(uint8_t contrast) {
    sendCommand(SH1106_CONTRAST);
    sendCommand(contrast);
}

void SH1106::drawBitmapImage(int16_t anchorX, int16_t anchorY, uint8_t image_width, uint8_t image_height, uint8_t *image, SH1106_WRITE_MODE mode) {
    uint8_t byte;

    for (uint8_t y = 0; y < image_height; y++) {
        for (uint8_t x = 0; x < image_width / 8; x++) {
            byte = image[y * (image_width / 8) + x];
            for (uint8_t z = 0; z < 8; z++) {
                if ((byte >> (7 - z)) & 1) {
                    setPixel(x * 8 + z + anchorX, y + anchorY, mode);
                }
            }
        }
    }
}

void SH1106::drawChar(const uint8_t* font, char c, uint8_t anchor_x, uint8_t anchor_y, SH1106_WRITE_MODE mode) {
    if (c < 32)
        return;

    uint8_t font_width = font[0];
    uint8_t font_height = font[1];

    uint16_t seek = (c - 32) * (font_width * font_height) / 8 + 2;

    uint8_t b_seek = 0;

    for (uint8_t x = 0; x < font_width; x++) {
        for (uint8_t y = 0; y < font_height; y++) {
            if (font[seek] >> b_seek & 0b00000001) {
                setPixel(x + anchor_x, y + anchor_y, mode);
            }
            b_seek++;

            if (b_seek == 8) {
                b_seek = 0;
                seek++;
            }
        }
    }
}

void SH1106::drawText(const uint8_t* font, const char * text, uint8_t anchor_x, uint8_t anchor_y, SH1106_WRITE_MODE mode) {
    uint8_t font_width = font[0];

    uint16_t n = 0;
    while (text[n] != '\0') {
        drawChar(font, text[n], anchor_x + (n * font_width), anchor_y, mode);

        n++;
    }
}

void SH1106::drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, SH1106_WRITE_MODE mode) {
    int x, y, dx, dy, dx0, dy0, px, py, xe, ye, i;
    dx = x1 - x0;
    dy = y1 - y0;
    dx0 = fabs(dx);
    dy0 = fabs(dy);
    px = 2 * dy0 - dx0;
    py = 2 * dx0 - dy0;
    if (dy0 <= dx0) {
        if (dx >= 0) {
            x = x0;
            y = y0;
            xe = x1;
        } else {
            x = x1;
            y = y1;
            xe = x0;
        }

        setPixel(x, y, mode);
        for (i = 0; x < xe; i++) {
            x = x + 1;
            if (px < 0) {
                px = px + 2 * dy0;
            } else {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
                    y = y + 1;
                } else {
                    y = y - 1;
                }
                px = px + 2 * (dy0 - dx0);
            }

            setPixel(x, y, mode);
        }
    } else {
        if (dy >= 0) {
            x = x0;
            y = y0;
            ye = y1;
        } else {
            x = x1;
            y = y1;
            ye = y0;
        }

        setPixel(x, y, mode);
        for (i = 0; y < ye; i++) {
            y = y + 1;
            if (py <= 0) {
                py = py + 2 * dx0;
            } else {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
                    x = x + 1;
                } else {
                    x = x - 1;
                }
                py = py + 2 * (dx0 - dy0);
            }

            setPixel(x, y, mode);
        }
    }
}

void SH1106::drawRect(uint8_t x_start, uint8_t y_start, uint8_t x_end, uint8_t y_end, SH1106_WRITE_MODE mode) {
    drawLine(x_start, y_start, x_end, y_start, mode);
    drawLine(x_start, y_end, x_end, y_end, mode);
    drawLine(x_start, y_start, x_start, y_end, mode);
    drawLine(x_end, y_start, x_end, y_end, mode);
}

