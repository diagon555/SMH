//
// Created by diagon on 28.01.2020.
//

#include "USART.h"

size_t USART::write(uint8_t val) {
    fwrite(&val, 1, 1, stdout);
    return 1;
}

size_t USART::write(const uint8_t *buffer, size_t size) {
    fwrite(buffer, size, 1, stdout);
    return size;
}
