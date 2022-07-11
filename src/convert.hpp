#ifndef CONVERT_HPP
#define CONVERT_HPP
#include <regex>
#include <sstream>
#include <string>
#include <string_view>
#include <filesystem>
#include <vector>
namespace rdx::f2c {
	std::vector<unsigned char> read_file_to_byte_array(std::filesystem::path file) ;
	std::string generate_include_guard(const std::string& in_file);

	template<typename Converter>
	struct converter {
		std::string get_header_content() {
			return static_cast<Converter*>(this)->get_header_content();
		}
		std::string get_source_content() {
			return static_cast<Converter*>(this)->get_source_content();
		}
	};

	struct c_converter: public converter<c_converter> {
		std::string variable_name;
		std::vector<unsigned char> bytes;
		std::filesystem::path out_file;

		std::string get_header_content() {
			std::stringstream header_stream;
			std::string include_guard = generate_include_guard(out_file.filename().generic_string());
			header_stream << "#ifndef " << include_guard << "\n";
			header_stream << "#define " << include_guard << "\n\n";
			header_stream << "extern const unsigned char " << variable_name << "[" << bytes.size() << "];\n";
			header_stream << "extern const unsigned int " << variable_name << "_size; \n";
			header_stream << "#endif";
			return header_stream.str();
		}

		std::string get_source_content() {
			std::filesystem::path header_file_name = out_file;
			header_file_name.replace_extension(".h");
			std::stringstream source_stream;
			source_stream << "#include " << header_file_name.filename() << "\n\n";
			source_stream << "const unsigned char " << variable_name << "[" << bytes.size() << "] = { \n";
			source_stream << std::hex << std::showbase;
			int columns = 0;
			for(auto it = bytes.begin(); it != bytes.end(); it++) {
				source_stream << (int)*it;
				if(it != bytes.end() - 1) {
					source_stream << ", ";
				}
				columns++;
				if(columns >= 8) {
					source_stream << "\n";
					columns = 0;
				}
			}
			source_stream << "\n};\n\n";
			source_stream << "const unsigned int " << variable_name << "_size = " << std::dec << bytes.size() << ";\n";
			return source_stream.str();
		}
	};

	struct cpp_converter: public converter<cpp_converter> {
		std::string variable_name;
		std::vector<unsigned char> bytes;
		std::filesystem::path out_file;
		std::string name_space;

		std::string get_header_content() {
			std::stringstream header_stream;
			std::string include_guard = generate_include_guard(out_file.filename().generic_string());
			header_stream << "#ifndef " << include_guard << "\n";
			header_stream << "#define " << include_guard << "\n\n";
			header_stream << "namespace " << name_space << " { \n";
			header_stream << "\textern const unsigned char " << variable_name << "[" << bytes.size() << "];\n";
			header_stream << "\textern const unsigned int " << variable_name << "_size; \n";
			header_stream << "}\n";
			header_stream << "#endif";
			return header_stream.str();
		}

		std::string get_source_content() {
			std::filesystem::path header_file_name = out_file;
			header_file_name.replace_extension(".hpp");
			std::stringstream source_stream;
			source_stream << "#include " << header_file_name.filename() << "\n\n";
			source_stream << "namespace " << name_space << "{\n";
			source_stream << "\tconst unsigned char " << variable_name << "[" << bytes.size() << "] = { \n\t";
			source_stream << std::hex << std::showbase;
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
			source_stream << "\n\t};\n\n";
			source_stream << "\tconst unsigned int " << variable_name << "_size = " << std::dec << bytes.size() << ";\n";
			source_stream << "}";
			return source_stream.str();
		}
	};
}	// namespace rdx::f2c
#endif