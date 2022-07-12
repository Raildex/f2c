#include "convert.hpp"
#include <fstream>
#include <regex>
#include <algorithm>
namespace rdx::f2c {
	std::vector<unsigned char> read_file_to_byte_array(std::filesystem::path file) {
		auto byte_count = std::filesystem::file_size(file);
		std::vector<unsigned char> bytes(byte_count);
		auto ifs = std::ifstream(file,std::ios_base::in | std::ios_base::binary);
		ifs.read((char*)bytes.data(), byte_count);
		return bytes;
	}

	std::string generate_include_guard(const std::string& in_file) {
		std::string include_guard = "F2C_" + in_file + "_EMBEDDED";
		std::regex e("[^a-zA-Z1-9]+");
		include_guard = std::regex_replace(include_guard, e, "_");
		std::transform(include_guard.begin(), include_guard.end(), include_guard.begin(), [](unsigned char c) {
			return std::toupper(c);
		});
		return include_guard;
	}
}
