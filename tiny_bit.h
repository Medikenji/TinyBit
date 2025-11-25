#ifndef TINY_BIT_IMPLEMENTATION
#define TINY_BIT_IMPLEMENTATION


#include <unordered_map>

/*
 * #define DEBUG_OPTIONS
 * Adds the print_tree function that prints out all of the stored data,
 */


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



    struct Property {
        int size : 8;
        int offset : 8;
        size_t parent_hash : 8;
    };



    class RegisteredObject {
    public:

        template<typename T, typename M>
        RegisteredObject& property(M T::* member, int size_in_bits) {
            _properties.push_back(Property{
                size_in_bits,
                get_offset(),
                typeid(T).hash_code()
            });
            return *this;
        }

        const std::vector<Property>& properties() const {
            return _properties;
        }

    private:


        int get_offset() const {
            int offset = 0;

            for (int i=0; i<_properties.size(); i++) {
                offset += _properties[i].size;
            }

            return offset;
        }

        int get_size() const {
            int size = 0;

            for (int i=0; i<_properties.size(); i++) {
                size += _properties[i].size;
            }

            return size;
        }

        std::vector<Property> _properties;
    };


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