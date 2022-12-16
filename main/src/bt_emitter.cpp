#include <pico/time.h>

#include "hardware/gpio.h"
#include "hardware/irq.h"

#include "bt_emitter.h"
#include "string_helper.h"

BT_Emitter::BT_Emitter(uart_inst *uartInst, uint8_t rx, uint8_t tx, uint32_t baudRate)
    : rx(rx), tx(tx), baudRate(baudRate) {
    inst = uartInst;
}

void BT_Emitter::init() {
    uart_init(inst, baudRate);

    // Initialize UART pins
    gpio_set_function(rx, GPIO_FUNC_UART);
    gpio_set_function(tx, GPIO_FUNC_UART);

    int UART_IRQ = inst == uart0 ? UART0_IRQ : UART1_IRQ;
    irq_set_exclusive_handler(UART_IRQ, uartIRQ);
    irq_set_enabled(UART_IRQ, true);
    uart_set_irq_enables(inst, true, false);
}

void BT_Emitter::sendCommand(std::string& command) {
    uart_puts(inst, command.c_str());
}

bool BT_Emitter::isReady() {
    std::string command = KCX_TEST_COMMAND;
    std::vector<std::string> output = sendIrqCommand(command, 500);

    return output[0].find("OK+") != std::string::npos;
}

std::vector<BT_Device> BT_Emitter::search() {
    std::string command = KCX_SCAN_COMMAND;
    std::vector<std::string> output = sendIrqCommand(command, 5000);

    // Check return "OK+SCAN"
    std::string checkString = output[0];
    if(checkString.find("OK+SCAN") == std::string::npos)
        return {};

    // Read new devices
    std::vector<BT_Device> devices;
    for(uint8_t i = 1; i < output.size(); i++) {
        std::string line = output[i];
        if(line.find("MacAdd") == std::string::npos)
            continue;

        std::vector<std::string> split = splitString(line.c_str(), ',');

        BT_Device device;
        for(std::string& s : split) {
            if(s.find("MacAdd") != std::string::npos) {
                device.macAddress = s.substr(7);
            }

            if(s.find("Name") != std::string::npos) {
                std::string name = s.substr(5);
                device.name = name.erase(name.length() - 1);
            }
        }
        devices.push_back(device);
    }

    return devices;
}


bool BT_Emitter::connect(BT_Device& device) {
    std::string macAddress = device.macAddress;
    if(macAddress.size() != 14)
        macAddress += '0';

    std::string command = KCX_CONNECTION_ADD_COMMAND + macAddress;
    std::vector<std::string> output = sendIrqCommand(command, 4000);

    bool connected = false;
    for(uint8_t i = 1; i < output.size(); i++) {
        std::string line = output[i];

        if(line.find("CONNECTED") != std::string::npos) {
            connected = true;
            break;
        }
    }

    return connected;
}

bool BT_Emitter::disconnect() {
    std::string command = KCX_DISCONNECT_COMMAND;
    std::vector<std::string> output = sendIrqCommand(command, 2000);

    bool disconnected = false;
    for(uint8_t i = 1; i < output.size(); i++) {
        std::string line = output[i];

        if(line.find("DISCONNECT") != std::string::npos) {
            disconnected = true;
            break;
        }
    }

    return disconnected;
}

void BT_Emitter::uartIRQ() {
    bool reading = false;

    std::string line;
    while (uart_is_readable(inst)) {
        reading = true;

        uint8_t ch = uart_getc(inst);
        line += ch;
    }

    if(reading)
        irqLines.push_back(line);
}

std::vector<std::string> BT_Emitter::sendIrqCommand(std::string &command, uint32_t sleepTime) {
    irqLines.clear();

    sendCommand(command);
    sleep_ms(sleepTime);

    std::vector<std::string> temp = irqLines;
    irqLines.clear();

    std::vector<std::string> lines;
    std::string line;
    for(std::string& s : temp) {
        line += s;

        if(s.find('\n') != std::string::npos) {
            lines.push_back(line);
            line.clear();
        }
    }

    if(lines.empty())
        lines.push_back(line);

    return lines;
}

std::vector<std::string> BT_Emitter::irqLines = {};
uart_inst* BT_Emitter::inst = uart0;