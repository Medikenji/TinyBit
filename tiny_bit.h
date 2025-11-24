#pragma once


namespace tiny_bit {

    void _encode(int &_data, int _offsetInBits, int _sizeInBits, int _value ) {
        int mask = 0;
        for (int i = 0; i < _sizeInBits; i++) {
            mask |= (1 << i);
        }
        mask = mask << _offsetInBits;
        _value = _value << _offsetInBits;
        _data &= (~mask);
        _data |= _value;
    }

    static int _decode(int _data, int _offsetInBits, int _sizeInBits) {
        int mask = 0;
        for (int i = 0; i < _sizeInBits; i++) {
            mask |= (1 << i);
        }
        mask = mask << _offsetInBits;
        _data &= mask;
        return _data >> _offsetInBits;
    }


    class Property {
    public:

    };

    template<typename T, typename M>
    class PropertyImpl {
    public:

    };

    class RegisteredObject {
    public:
        template<typename T, typename M>
        RegisteredObject& property(M T::* member, size_t size_in_bits) {

            return *this;
        }


    private:


    };


}
