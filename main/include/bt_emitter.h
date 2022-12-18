#ifndef GRAMOFON_BT_EMITTER_H
#define GRAMOFON_BT_EMITTER_H

#include "pico.h"
#include "pico/types.h"

#include "hardware/uart.h"

#include <string>
#include <vector>

#include "device.h"

#define KCX_TEST_COMMAND            "AT+"
#define KCX_RESET_COMMAND           "AT+REST"
#define KCX_STATUS_COMMAND          "AT+STATUS"
#define KCX_CONNECTION_ADD_COMMAND  "AT+CONADD="
#define KCX_DISCONNECT_COMMAND      "AT+DISCON"
#define KCX_SCAN_COMMAND            "AT+SCAN"
#define KCX_LINK_ADD_COMMAND        "AT+ADDLINKADD="
#define KCX_LINK_DELETE             "AT+DELVMLINK"

struct BT_Device {
    std::string name;
    std::string macAddress;
};

class BT_Emitter : public Device {
private:
    static BT_Emitter* instance;

    static uart_inst* inst;
    uint8_t rx;
    uint8_t tx;
    uint32_t baudRate;

    bool connected = false;
    BT_Device _connectedDevice;

    static std::vector<std::string> irqLines;
    static void uartIRQ();

    void sendCommand(std::string& command);
    std::vector<std::string> sendIrqCommand(std::string& command, uint32_t sleepTime);

public:
    BT_Emitter(uart_inst* uartInst, uint8_t rx, uint8_t tx, uint32_t baudRate = 9600);
    static BT_Emitter* getInstance() { return instance; }

    void init();

    bool isReady();
    std::vector<BT_Device> search();

    bool isConnected() { return connected; }
    BT_Device connectedDevice() { return _connectedDevice; }

    bool connect(BT_Device& device);
    bool disconnect();

};

#endif //GRAMOFON_BT_EMITTER_H
