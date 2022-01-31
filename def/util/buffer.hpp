#pragma once

#include <stdint.h>

class Buffer {
public:
    char *data;
public:
    explicit Buffer(char *_data) : data(_data) {}
    ~Buffer() = default;
public: // READ
    uint8_t ubyte() {
        return *data++;
    }
    int8_t ibyte() {
        return *data++;
    }

    uint16_t ushort() {
        uint16_t i = 0;
        i += ((uint16_t) (*data++ & 255) << 8);
        i += ((uint16_t) (*data++ & 255));
        return i;
    }
    int16_t ishort() {
        int16_t i = 0;
        i += ((int16_t) (*data++ & 255) << 8);
        i += ((int16_t) (*data++ & 255));
        return i;
    }

    uint32_t uint() {
        uint32_t i = 0;
        i += ((uint32_t) (*data++ & 255) << 24);
        i += ((uint32_t) (*data++ & 255) << 16);
        i += ((uint32_t) (*data++ & 255) << 8);
        i += ((uint32_t) (*data++ & 255));
        return i;
    }
    int32_t iint() {
        int32_t i = 0;
        i += ((int32_t) (*data++ & 255) << 24);
        i += ((int32_t) (*data++ & 255) << 16);
        i += ((int32_t) (*data++ & 255) << 8);
        i += ((int32_t) (*data++ & 255));

        return i;
    }

    uint64_t ulong() {
        uint64_t i = 0;
        i += ((uint64_t) (*data++ & 255) << 56);
        i += ((uint64_t) (*data++ & 255) << 48);
        i += ((uint64_t) (*data++ & 255) << 40);
        i += ((uint64_t) (*data++ & 255) << 32);
        i += ((uint64_t) (*data++ & 255) << 24);
        i += ((uint64_t) (*data++ & 255) << 16);
        i += ((uint64_t) (*data++ & 255) << 8);
        i += ((uint64_t) (*data++ & 255));
        return i;
    }
    int64_t ilong() {
        int64_t i = 0;
        i += ((int64_t) (*data++ & 255) << 56);
        i += ((int64_t) (*data++ & 255) << 48);
        i += ((int64_t) (*data++ & 255) << 40);
        i += ((int64_t) (*data++ & 255) << 32);
        i += ((int64_t) (*data++ & 255) << 24);
        i += ((int64_t) (*data++ & 255) << 16);
        i += ((int64_t) (*data++ & 255) << 8);
        i += ((int64_t) (*data++ & 255));
        return i;
    }

    double rdouble() {
        uint64_t i = ilong();
        return * (double*) (&i);
    }
    float rfloat() {
        uint32_t i = iint();
        return * (float*) (&i);
    }

    void ubytes(uint32_t len, uint8_t *out) {
        for (uint32_t i = 0; i < len; i++) out[i] = ubyte();
    }
    void ibytes(uint32_t len, int8_t *out) {
        for (uint32_t i = 0; i < len; i++) out[i] = ibyte();
    }

    void string(uint32_t len, char *out) {
        for (uint32_t i = 0; i < len; i++) out[i] = ibyte();
    }
public: // WRITE
    void ubyte(const uint8_t i) {
        *(data++) = i;
    }
    void ibyte(const int8_t i) {
        *(data++) = i;
    }

    void ushort(const uint16_t i) {
        *(data++) = i >> 8;
        *(data++) = i;
    }
    void ishort(const int16_t i) {
        *(data++) = i >> 8;
        *(data++) = i;
    }

    void uint(const uint32_t i) {
        *(data++) = i >> 24;
        *(data++) = i >> 16;
        *(data++) = i >> 8;
        *(data++) = i;
    }
    void iint(const int32_t i) {
        *(data++) = i >> 24;
        *(data++) = i >> 16;
        *(data++) = i >> 8;
        *(data++) = i;
    }

    void ulong(const uint64_t i) {
        *(data++) = i >> 56;
        *(data++) = i >> 48;
        *(data++) = i >> 40;
        *(data++) = i >> 32;
        *(data++) = i >> 24;
        *(data++) = i >> 16;
        *(data++) = i >> 8;
        *(data++) = i;
    }
    void ilong(const int64_t i) {
        *(data++) = i >> 56;
        *(data++) = i >> 48;
        *(data++) = i >> 40;
        *(data++) = i >> 32;
        *(data++) = i >> 24;
        *(data++) = i >> 16;
        *(data++) = i >> 8;
        *(data++) = i;
    }

    void rdouble(const double i) {
        uint64_t d = * (uint64_t*) (&i);
        ulong(d);
    }
    void rfloat(const float i) {
        uint32_t d = * (uint32_t*) (&i);
        uint(d);
    }

    void ubytes(const uint8_t *bytes, uint32_t len) {
        for (uint32_t i = 0; i < len; i++) ubyte(bytes[i]);
    }
    void ibytes(const int8_t *bytes, uint32_t len) {
        for (uint32_t i = 0; i < len; i++) ibyte(bytes[i]);
    }

    void string(const char *str, uint32_t len) {
        for (uint32_t i = 0; i < len; i++) ibyte(str[i]);
    }

    void utfstring(const char *str, uint32_t len) {
        ushort(len);
        for (uint32_t i = 0; i < len; i++) ibyte(str[i]);
    }
};
