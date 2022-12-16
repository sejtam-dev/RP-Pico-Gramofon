#ifndef GRAMOFON_MEMORY_HPP
#define GRAMOFON_MEMORY_HPP

#include "pico.h"
#include "pico/types.h"

#include "hardware/gpio.h"
#include "hardware/flash.h"
#include "hardware/sync.h"

#include <cmath>

#define FLASH_MEMORY_POSITION (1024 * 1536)

namespace Memory
{
    template <typename T>
    T* read(uint32_t offset)
    {
        return reinterpret_cast<T*>((XIP_BASE + FLASH_MEMORY_POSITION + offset));
    }

    template <typename T>
    void write(uint32_t offset, T &object, size_t size)
    {
        if (size == 0)
            size = sizeof(T);

        const size_t pageSize = (floor(size / FLASH_PAGE_SIZE) + 1) * FLASH_PAGE_SIZE;
        const size_t sectorSize = (floor(size / FLASH_SECTOR_SIZE) + 1) * FLASH_SECTOR_SIZE;

        const auto dataPointer = reinterpret_cast<uint8_t*>(&object);

        uint8_t dataArray[pageSize];
        for (int i = 0; i < pageSize; ++i)
        {
            if (size >= (i * sizeof(uint8_t)))
                dataArray[i] = dataPointer[i];
            else
                dataArray[i] = 0;
        }

        const uint32_t interrupts = save_and_disable_interrupts();

        flash_range_erase(FLASH_MEMORY_POSITION, sectorSize);
        flash_range_program(FLASH_MEMORY_POSITION, dataArray, pageSize);

        restore_interrupts(interrupts);
    }

}

#endif //GRAMOFON_MEMORY_HPP
