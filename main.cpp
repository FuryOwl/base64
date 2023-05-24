#include <iostream>
#include <string>
#include <bitset>
#include <algorithm>
#include <locale>

#ifdef _WIN32 // Compilation for Windows
#include "getopt_windows.h" // Alternative implementation getopt() for Windows
#else // Compilation for UNIX
#include <unistd.h>
#include <getopt.h>
#endif

static const std::string base64_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string StringToBase64(const std::string& source_string) {
	// Converting a source string to a bit sequence
	std::string bit_string;
	std::bitset<8> binary_char;
	for (const char& c : source_string) {
		binary_char = std::bitset<8>(c);
		bit_string += binary_char.to_string();
	}
	// Finding the remainder of the division, then adding zero bits for alignment
	size_t remainder = bit_string.size() % 24;
	if (remainder == 8) {
		bit_string.append(4, '0');
	}
	else if (remainder == 16) {
		bit_string.append(2, '0');
	}
	// Converting a bit sequence to base64
	std::string base64_string;
	std::bitset<6> binary_number;
	for (size_t i = 0; i < bit_string.size(); i += 6) {
		binary_number = std::bitset<6>(bit_string.substr(i, 6));
		base64_string += base64_alphabet[binary_number.to_ulong()];
	}
	// Filling with "=" characters if the source data is not completely divided into 3 bytes
	if (remainder == 8) {
		base64_string.append(2, '=');
	}
	else if (remainder == 16) {
		base64_string.append(1, '=');
	}

	return base64_string;
}

std::string Base64ToString(const std::string& base64_string) {
	// Finding the number of occurrences of the '=' character to skip it
	int counter = std::count(base64_string.begin(), base64_string.end(), '=');
	// Converting a base64 string to a bit sequence
	std::string bit_string;
	std::bitset<6> binary_number;
	for (size_t i = 0; i < base64_string.size() - counter; i++) {
		binary_number = std::bitset<6>(base64_alphabet.find(base64_string[i]));
		bit_string += binary_number.to_string();
	}
	// Removing extra zero bits
	if (counter == 2) {
		bit_string.resize(bit_string.size() - 4);
	}
	else if (counter == 1) {
		bit_string.resize(bit_string.size() - 2);
	}
	// Converting a bit sequence to a regular string
	std::string decoded_string;
	std::bitset<8> binary_char;
	for (size_t i = 0; i < bit_string.size(); i += 8) {
		binary_char = std::bitset<8>(bit_string.substr(i, 8));
		decoded_string += static_cast<char>(binary_char.to_ulong());
	}

	return decoded_string;
}

void PrintHelp() {
	std::cout << "Usage: program_name [OPTIONS] [TEXT]" << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << "  -e, --encode    Encode the following text to base64 and print the result." << std::endl;
	std::cout << "  -d, --decode    Decode the following base64 text to text and print the result." << std::endl;
	std::cout << "  -?, -h, --help  Print this help message." << std::endl;
}

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "");

	std::string input_text;
	bool encode_mode = false;
	bool decode_mode = false;

	const option long_options[] = {
		{"encode", no_argument, nullptr, 'e'},
		{"decode", no_argument, nullptr, 'd'},
		{"help", no_argument, nullptr, 'h'},
		{nullptr, 0, nullptr, 0}
	};
	// Parsing arguments using 'getopt' function
	int opt;
	while ((opt = getopt_long(argc, argv, "edh?", long_options, nullptr)) != -1) {
		switch (opt) {
		case 'e':
			encode_mode = true;
			break;
		case 'd':
			decode_mode = true;
			break;
		case 'h':
		case '?':
			PrintHelp();
			return 0;
		default:
			std::cerr << "Unknown option: " << opt << std::endl;
			PrintHelp();
			return 1;
		}
	}
	// Text processing
	if (optind < argc) {
		input_text = argv[optind];
	}
	// Encoding or decoding, depending on the selected mode
	if (encode_mode && decode_mode) {
		std::cerr << "Error: Cannot use both encode and decode options." << std::endl;
		return 1;
	}
	else if (encode_mode) {
		std::string encoded_text = StringToBase64(input_text);
		std::cout << "Encoded string in base64: " << encoded_text << std::endl;
	}
	else if (decode_mode) {
		std::string decoded_text = Base64ToString(input_text);
		std::cout << "Decoded string: " << decoded_text << std::endl;
	}
	else {
		std::cerr << "No mode selected. Use -e for encoding or -d for decoding." << std::endl;
		return 1;
	}

	return 0;
}