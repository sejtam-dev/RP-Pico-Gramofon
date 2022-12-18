#ifndef GRAMOFON_SH1106_H
#define GRAMOFON_SH1106_H

#include "pico.h"
#include "pico/types.h"

#include "hardware/i2c.h"

#include "FrameBuffer.h"
#include "device.h"

#define SH1106_CONTRAST 0x81
#define SH1106_DISPLAYALL_ON_RESUME 0xA4
#define SH1106_DISPLAYALL_ON 0xA5
#define SH1106_INVERTED_OFF 0xA6
#define SH1106_INVERTED_ON 0xA7
#define SH1106_DISPLAY_OFF 0xAE
#define SH1106_DISPLAY_ON 0xAF
#define SH1106_DISPLAYOFFSET 0xD3
#define SH1106_COMPINS 0xDA
#define SH1106_VCOMDETECT 0xDB
#define SH1106_DISPLAYCLOCKDIV 0xD5
#define SH1106_PRECHARGE 0xD9
#define SH1106_MULTIPLEX 0xA8
#define SH1106_LOWCOLUMN 0x00
#define SH1106_HIGHCOLUMN 0x10
#define SH1106_STARTLINE 0x40
#define SH1106_MEMORYMODE 0x20
#define SH1106_MEMORYMODE_HORZONTAL 0x00
#define SH1106_MEMORYMODE_VERTICAL 0x01
#define SH1106_MEMORYMODE_PAGE 0x10
#define SH1106_COLUMNADDR 0x21
#define SH1106_PAGEADDR 0x22
#define SH1106_COM_REMAP_OFF 0xC0
#define SH1106_COM_REMAP_ON 0xC8
#define SH1106_CLUMN_REMAP_OFF 0xA0
#define SH1106_CLUMN_REMAP_ON 0xA1
#define SH1106_CHARGEPUMP 0x8D
#define SH1106_EXTERNALVCC 0x1
#define SH1106_SWITCHCAPVCC 0x2
#define SH1106_MEMORYMODE 0x20

#define SH1106_SET_PAGE_ADDRESS	0xB0
#define SH1106_MAX_PAGE_COUNT 8

enum class SH1106_SIZE {
    W128xH64,
    W128xH32
};

enum class SH1106_WRITE_MODE : const uint8_t {
    /// sets pixel on regardless of its state
    ADD = 0,

    /// sets pixel off regardless of its state
    SUBTRACT = 1,

    /// inverts pixel, so 1->0 or 0->1
    INVERT = 2,
};

class SH1106 : public Device {
private:
    i2c_inst* inst;
    uint8_t scl;
    uint8_t sda;
    uint8_t address;
    uint32_t baudRate;

    SH1106_SIZE size;

    uint8_t width;
    uint8_t height;

    FrameBuffer frameBuffer;
    bool inverted;

    void sendCommand(uint8_t command);

public:
    SH1106(i2c_inst* inst, uint8_t scl, uint8_t sda, uint8_t address = 0x3C, uint32_t baudRate = 1000000, SH1106_SIZE size = SH1106_SIZE::W128xH64);

    void init();

    void setPixel(int16_t x, int16_t y, SH1106_WRITE_MODE mode = SH1106_WRITE_MODE::ADD);

    void drawBitmapImage(int16_t anchorX, int16_t anchorY, uint8_t image_width, uint8_t image_height, uint8_t *image, SH1106_WRITE_MODE mode = SH1106_WRITE_MODE::ADD);
    void drawChar(const uint8_t* font, char c, uint8_t anchor_x, uint8_t anchor_y, SH1106_WRITE_MODE mode = SH1106_WRITE_MODE::ADD);
    void drawText(const uint8_t* font, const char * text, uint8_t anchor_x, uint8_t anchor_y, SH1106_WRITE_MODE mode = SH1106_WRITE_MODE::ADD);
    void drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, SH1106_WRITE_MODE mode = SH1106_WRITE_MODE::ADD);
    void drawRect(uint8_t x_start, uint8_t y_start, uint8_t x_end, uint8_t y_end, SH1106_WRITE_MODE mode = SH1106_WRITE_MODE::ADD);

    void sendBuffer();
    void clear();
    void clear(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);

    void setOrientation(bool flipped);

    void invertDisplay();
    void setContrast(uint8_t contrast);
};

#endif //GRAMOFON_SH1106_H
