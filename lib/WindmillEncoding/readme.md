# Windmill encoding
The values of the Windmill sensors are encoded in a 24 bit format which is then piggybacked
on the RfxMeter protocol before it is sent through the 433 mhz transmitter.
In this way the signal can be understood and decoded by the RfxCom transceiver device.

## Overview of the format
|23|22|21|20|19|18|17|16|
|:------:|:--:|:--------:|
|qty|qty|qty|qty|signbit|value|value|value

|15|14|13|12|11|10|9|8|
|:-|:-|:-|:-|:-|:-|:|::|
|value|value|value|value|value|value|value|value|

|7|6|5|4|3|2|1|0|
|:-|:-|:-|:-|:-|:-|:|::|
|value|value|value|value|value|value|value|value|

### Quantity
The quantity is specified in bits 20-23, according to the following table: (the leftmost bit in the 'bin' column goes in bit 23 etc.)

|bin|hex|dec|Description|
|---|---|---|-----------|
|0000|0x00|0|temperature (C)|
|0001|0x01|1|humidity (%)|
|0010|0x02|2|(air) pressure (hPa)|
|0011|0x03|3|luminance (lux)|
|0100|0x04|4|loudness (dB)|
|0101|0x05|5|current (mA)|
|0110|0x06|6|voltage (V)|
|0111-1111|0x07-0x0F|7-15|reserved for future use|

### Value range
Values between **-524287** and **+524287** can be represented in this encoding, since there are 18 bits available for the value (0-18) and an additional sign bit (19).


## Encode/decode

### Encode and pack a value into a single 32 bits integer
- Given `value` containing an 32 bits signed integer with the value to be encoded
- `encoded_value = abs(value)` (N.B. `encoded_value` should be an **unsigned** integer)
- Set the sign bit by `encoded_value |= 0x80000` depending on the sign of `value`
- Set the quantity type by `encoded_value |= quantity_type << 20`

### Encode to a byte array
- Given `value` containing an 32 bits signed integer with the value to be encoded
- `encoded_value = abs(value)` (N.B. `encoded_value` should be an **unsigned** integer)
- Set the sign bit by `encoded_value |= 0x80000` depending on the sign of `value`
- Set the quantity type by `encoded_value |= quantity_type << 20`
- `byte[0] = (value >> 16) & 0xff`
- `byte[1] = (value >> 8) & 0xff`
- `byte[2] = value & 0xff`

Resulting array:

| 0        | 1  |     2    |
|:--------:|:--:|:--------:|
|B2 (MSB)  | B1 | B0 (LSB) |


### Decode and unpack from a single 32 bits integer
- Given `encoded_value` as the 32 bits integer containing a valid Windmill encoded value
- `signbit = (encoded_value & 0x80000 != 0)`
- `quantity_type = encoded_value >> 20`
- `abs_value = encoded_value & 0x7FFFF`
- `value = +/-abs_value` (depends on the value of `signbit`)

### Decode from byte array
- Given the following byte array `input` (all the bytes are **unsigned**):

| 0        | 1  |     2    |
|:--------:|:--:|:--------:|
|B2 (MSB)  | B1 | B0 (LSB) |

- `received_value = (input[0] << 16) + (uint)(input[1] << 8) + (uint)input[2]`
- `signbit = value & 0x80000 != 0`
- `quantity_type = received_value >> 20`
- `abs_value = (received_value & 0x7FFFF)`
- `value = +/-abs_value` (depends on the value of `signbit`)
