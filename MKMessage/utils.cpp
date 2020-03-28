#include <utils.h>


int log2(int a) {
    int count = 0;
    while(a > 1) {
        a /= 2;
        count++;
    }
    return count;
}

bool checkBit(uint16_t a, uint16_t bitIndex) {
    a >> bitIndex;
    if (a & 1 == 0) {
        return 0;
    }
    return 1;
}
