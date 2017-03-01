#include "windmill_encoding.h"
#include "Arduino.h"

void encode(WindmillValue value, uint32_t* encodedValue) {

    *encodedValue = abs(value.value);

    if (value.value < 0) {
      //Add sign bit
      *encodedValue |= (uint32_t)(0x80000);
    }

    *encodedValue |= (uint32_t)(value.quantity) << 20;
}
void decode(uint32_t encodedValue, WindmillValue* decodedValue){
    uint32_t isSignBitSet = encodedValue & 0x80000;
    uint32_t type = encodedValue >> 20;
    uint32_t value = encodedValue & 0x7FFFF;

    int32_t endvalue = isSignBitSet != 0 ? -value : value;

    decodedValue->quantity = type;
    decodedValue->value = endvalue;
}

void encodeToByteArray(WindmillValue value, uint8_t encodedArray[]) {
  //TODO:

}
void decodeFromByteArray(uint8_t encodedArray[], WindmillValue decodedValue) {
  //TODO:
}
