//
// Created by diagon on 28.01.2020.
//

#ifndef SMH_USART_H
#define SMH_USART_H

#include "../lib/Print.h"

class USART : public Print {
public:
    size_t write(uint8_t);
    size_t write(const uint8_t *buffer, size_t size);
    void begin(int BoundRate);
};


#endif //SMH_USART_H
