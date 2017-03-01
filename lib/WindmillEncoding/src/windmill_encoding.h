#ifndef WINDMILL_ENCODING_H
#define WINDMILL_ENCODING_H

#include <stdint.h>

struct WindmillValue
{
    int32_t value;
    uint8_t quantity;
};

void encode(WindmillValue value, uint32_t* encodedValue);
void decode(uint32_t encodedValue, WindmillValue* decodedValue);

void encodeToByteArray(WindmillValue value, uint8_t encodedArray[]);
void decodeFromByteArray(uint8_t encodedArray[], WindmillValue decodedValue);

#endif //WINDMILL_ENCODING_H
