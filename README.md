# f2c
A program that converts an arbitrary file into a byte array for inclusion in C or C++


## Usage
```
f2c -c [path/to/file] [variable_name] [path/to/out_file]
f2c -cpp [path/to/file] [variable_name] [path/to/out_file] [namespace]
```

## Example
```
f2c -cpp raildex some_file.txt raw output_file.txt raildex
```
will generate the following header file named `output_file.hpp`:
```cpp
#ifndef OUTPUT_FILE_TXT
#define OUTPUT_FILE_TXT

namespace raildex {
	extern const unsigned char raw[4];
	extern const unsigned int raw_size;
}
#endif
```
and a source file named `output_file.cpp` with the following content:
```cpp
#include  "output_file.hpp"

namespace raildex {
	const unsigned char raw[4] = {
	0x41, 0x53, 0x44, 0x46
	};
	const unsigned int raw_size = 4;
}
```