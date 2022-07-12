#include "convert.hpp"
#include <iostream>
#include <fstream>
#include <vector>
namespace rdx::f2c {
}	// namespace rdx::f2c

int main(int argc, char** argv) {
	if(argc == 1) {
		std::cout << "Usage :\n";
		std::cout << "f2c -c [path/to/file] [variable_name] "
					 "[path/to/out_file] -- converts a file to a C header";
		std::cout << "f2c -cpp [path/to/file] [variable_name] "
					 "[path/to/out_file] [namespace] -- converts a file to a C++ header with a "
					 "namespace\n";
		std::cout << std::flush;
		return 0;
	}
	if(argc == 5) {
		if(!strcmp(argv[1], "-c")) {
			std::filesystem::path out_file = argv[4];
			auto bytes = rdx::f2c::read_file_to_byte_array(argv[2]) ;
			std::cout << "Byte Count: " << bytes.size();
			auto converter = rdx::f2c::c_converter{.variable_name = argv[3], .bytes = bytes, .out_file = out_file};
			auto out_src_stream = std::ofstream(out_file.replace_extension("c"));
			auto out_header_stream = std::ofstream(out_file.replace_extension("h"));
			out_src_stream << converter.get_source_content();
			out_header_stream << converter.get_header_content();

		}
	} else if(argc == 6) {
		if(!strcmp(argv[1], "-cpp")) {
			std::filesystem::path out_file = argv[4];
			auto bytes = rdx::f2c::read_file_to_byte_array(argv[2]);
			std::cout << "Byte Count: " << bytes.size();
			auto converter = rdx::f2c::cpp_converter{.variable_name = argv[3], .bytes = bytes, .out_file = out_file, .name_space = argv[5]};
			auto out_src_stream = std::ofstream(out_file.replace_extension("cpp"));
			auto out_header_stream = std::ofstream(out_file.replace_extension("hpp"));
			out_src_stream << converter.get_source_content();
			out_header_stream << converter.get_header_content();
		}
	}
}