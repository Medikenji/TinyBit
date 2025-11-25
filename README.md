# TinyBit

[![Mit](https://img.shields.io/badge/License-MIT-red.svg)](https://opensource.org/licenses/MIT)
[![Language](https://img.shields.io/badge/language-C++-blue.svg)](https://isocpp.org/)
[![C++11](https://img.shields.io/badge/C%2B%2B-11-yellow.svg)](https://en.cppreference.com/w/cpp/11)
[![Status](https://img.shields.io/badge/status-early--development-lightgray.svg)]()

 A lightweight, single header compression library for C++ 11 and above

---
## Requirements
 - C++
 - the library is only depend on `std::unordered_map` and `std::vector`
---
## Features
 - [x] Automatic compression powered via the reflection system
## Future features
- [ ] Automatic serialization
- [ ] removing the use of `std::vector` with a custom implementation.
- [ ] removing the use of `std::unordered_map` with a custom implementation.
---

## How to use:
###### 🛑 IMPORTANT: always use ``tiny_bit::clean_up`` to avoid tiny_bit leaking memory
### Registering objects to tiny_bit
```c++
struct Foo {
    float f;
    double d;
};


tiny_bit::register_object<Foo>()
    .property(&Foo::f, 4) // 4 is the size in bits
    .property(&Foo::d, 2);

//Important! otherwise tiny_bit will leak memory.
tiny_bit::clean_up();
```

### Debug options

```c++
#define DEBUG_OPTIONS
#include "tiny_bit.h"

tiny_bit::print_tree();

//Output:
//registered type: 1159731404123986513
// - registered property:
// -- property.size: 4
// -- property.offset: 0
// - registered property:
// -- property.size: 2
// -- property.offset: 4


```