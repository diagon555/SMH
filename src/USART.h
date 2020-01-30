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
    int available(){return false;};
    int read(){return 0;};
    int peek(){return 0;};
    void flush(){};
};


#endif //SMH_USART_H
