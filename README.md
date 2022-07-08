# f2c
A program that converts an arbitrary file into a byte array for inclusion in C or C++


## Usage
```
f2c -c [input-file] [variable-name] [output-file]
f2c -cpp [namespace-name] [input-file] [variable-name] [output-file]
```

## Example
```
f2c -cpp raildex some_file.txt raw output_file.txt
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