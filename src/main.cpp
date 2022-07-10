#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

namespace rdx::f2c {

	std::vector<unsigned char> read_file_to_byte_array(std::filesystem::path file) {
		auto byte_count = std::filesystem::file_size(file);
		std::vector<unsigned char> bytes(byte_count);
		auto ifs = std::ifstream(file);
		ifs.read((char*)bytes.data(), byte_count);
		return bytes;
	}

	std::string generate_include_guard(const char* in_file) {
		std::string include_guard = in_file;
		std::regex e("[^a-zA-Z1-9]+");
		include_guard = std::regex_replace(include_guard, e, "_");
		std::transform(include_guard.begin(), include_guard.end(), include_guard.begin(), [](unsigned char c) {
			return std::toupper(c);
		});
		return include_guard;
	}

	void convert_to_header(const char* in_file, const char* var_name, const char* out_file, const char* name_space, const char* source_ext, const char* header_ext) {
		auto bytes = read_file_to_byte_array(in_file);
		auto include_guard = generate_include_guard(in_file);
		std::stringstream header_stream;
		std::filesystem::path header_file_name = out_file;
		header_file_name.replace_extension(header_ext);
		header_stream << "#ifndef " << include_guard << "\n";
		header_stream << "#define " << include_guard << "\n\n";
		if(name_space) {
			header_stream << "namespace " << name_space << " {\n";
		}

		header_stream << "\textern const unsigned char " << var_name << "[" << bytes.size() << "];\n";
		header_stream << "\textern const unsigned int " << var_name << "_size; \n";
		if(name_space) {
			header_stream << "}\n";
		}

		header_stream << "#endif";
		header_stream.flush();

		std::filesystem::path source_file_name = out_file;
		source_file_name.replace_extension(source_ext);
		std::stringstream source_stream;
		source_stream << "#include " << header_file_name << "\n\n";
		if(name_space) {
			source_stream << "namespace " << name_space << " { \n";
		}
		source_stream << "\tconst unsigned char " << var_name << "[" << bytes.size() << "] = { \n";
		source_stream << std::hex << std::showbase << "\t";
		int columns = 0;
		for(auto it = bytes.begin(); it != bytes.end(); it++) {
			source_stream << (int)*it;
			if(it != bytes.end() - 1) {
				source_stream << ", ";
			}
			columns++;
			if(columns >= 8) {
				source_stream << "\n\t";
				columns = 0;
			}
		}
		source_stream << "\n\t};\n";
		source_stream << "\tconst unsigned int " << var_name << "_size = " << std::dec << bytes.size() << ";\n";
		if(name_space) {
			source_stream << "}";
		}
		source_stream.flush();
		{
			auto header_ofs = std::ofstream(header_file_name);
			header_ofs << header_stream.rdbuf();
		}
		{
			auto source_ofs = std::ofstream(source_file_name);
			source_ofs << source_stream.rdbuf();
		}
	}
}	// namespace rdx::f2c

int main(int argc, char** argv) {
	if(argc == 1) {
		std::cout << "Usage :\n";
		std::cout << "f2c -c [path/to/file] [variable_name] "
					 "[path/to/out_file] -- converts a file to a C++ header with a "
					 "namespace\n";
		std::cout << "f2c -cpp [namespace] [path/to/file] [variable_name] "
					 "[path/to/out_file] -- converts a file to a C++ header with a "
					 "namespace\n";
		std::cout << std::flush;
	}
	if(argc == 5) {
		if(!strcmp(argv[1], "-c")) {
			rdx::f2c::convert_to_header(argv[2], argv[3], argv[4], nullptr, "c", "h");
		}
	} else if(argc == 6) {
		if(!strcmp(argv[1], "-cpp")) {
			rdx::f2c::convert_to_header(argv[3], argv[4], argv[5], argv[2], "cpp", "hpp");
		}
	}
}