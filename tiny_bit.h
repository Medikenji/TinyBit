/*
 * tiny_bit.h is a small, lightweight, single-header compression library.
 * Its purpose is to help storing smaller bits into one big chunk of bits
 */
#ifndef TINY_BIT_IMPLEMENTATION
#define TINY_BIT_IMPLEMENTATION
#include <unordered_map>

/*
 * #define DEBUG_OPTIONS
 * Adds the print_tree function that prints out all of the stored data,
 */


namespace tiny_bit {

    ///Encodes the bits of _value into _data.
    /// This means that _data should always be bigger than _value
    /// offset: should be how many bits to the RIGHT the value should be placed.
    /// sizeInBits: should be the size in bits.
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

    /// Decodes/Extracts a value out of the data.
    /// offset: should be how many bits to the RIGHT the value is placed.
    /// sizeInBits: should be the size in bits.
    static int _decode(int _data, int _offsetInBits, int _sizeInBits) {
        int mask = 0;
        for (int i = 0; i < _sizeInBits; i++) {
            mask |= (1 << i);
        }
        mask = mask << _offsetInBits;
        _data &= mask;
        return _data >> _offsetInBits;
    }



    /// Property is a small struct that holds all of the need information about a property of a registered object.
    struct Property {
        int size : 8;
        int offset : 8;
        size_t parent_hash : 8;
    };


    /// RegisteredObject is a small class that holds all of the need information about a registered object.
    class RegisteredObject {
    public:

        ///Registers and processes all of the needed information about a field/property of a object.
        template<typename T, typename M>
        RegisteredObject& property(M T::* member, int size_in_bits) {
            _properties.push_back(Property{
                size_in_bits,
                get_offset(),
                typeid(T).hash_code()
            });
            return *this;
        }

        /// Returns a non-changeable vector to iterate through.
        const std::vector<Property>& properties() const {
            return _properties;
        }

    private:
        std::vector<Property> _properties;

        /// Returns the offset/position for the next property.
        int get_offset() const {
            int offset = 0;

            for (int i=0; i<_properties.size(); i++) {
                offset += _properties[i].size;
            }

            return offset;
        }

        ///  Returns the total size of this object.
        int get_size() const {
            int size = 0;

            for (int i=0; i<_properties.size(); i++) {
                size += _properties[i].size;
            }

            return size;
        }

    };


    /// ONLY TINY_BIT SHOULD ACCESS THIS! DO NOT TOUCH!
    static std::unordered_map<size_t, RegisteredObject*> __registry;

    typedef std::unordered_map<size_t, RegisteredObject*>::iterator __iterator;

    template<typename T>
    static RegisteredObject& register_object() {
        size_t hash = typeid(T).hash_code();

        __iterator it = __registry.find(hash);
        if (it != __registry.end()) {
            return *it->second;
        }

        RegisteredObject* registered_object = new RegisteredObject{};

        __registry.insert({hash, registered_object});

        return *registered_object;
    }

    static void clean_up() {
        for (std::pair<const size_t, RegisteredObject*>& pair : __registry) {
            delete pair.second;
        }
    }

#ifdef DEBUG_OPTIONS

    static void print_tree() {
        for (std::pair<const size_t, RegisteredObject*>& pair : __registry) {

            std::cout << "registered type: " << pair.first << "\n";

            for (const Property& property : pair.second->properties()) {
                std::cout << " - registered property:\n";
                std::cout << " -- property.size: " << property.size << "\n";
                std::cout << " -- property.offset: " << property.offset << "\n";
            }
        }
    }
#endif



}

#endif