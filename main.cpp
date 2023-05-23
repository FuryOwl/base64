#include <iostream>
#include <string>
#include <bitset>
#include <algorithm>

const std::string base64_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string StringToBase64(const std::string& source_string) {
	//ѕреобразование исходной строки в битовую последовательность
	std::string bit_string;
	std::bitset<8> binary_char;
	for (const char& c : source_string) {
		binary_char = std::bitset<8>(c);
		bit_string += binary_char.to_string();
	}
	//Ќахождение остатка от делени€, последующее добавление нулевых бит дл€ выравнивани€
	size_t remainder = bit_string.size() % 24;
	if (remainder == 8) {
		bit_string.append(4, '0');
	}
	else if (remainder == 16) {
		bit_string.append(2, '0');
	}
	//ѕреобразование битовой последовательности в base64
	std::string base64_string;
	std::bitset<6> binary_number;
	for (size_t i = 0; i < bit_string.size(); i += 6) {
		binary_number = std::bitset<6>(bit_string.substr(i, 6));
		base64_string += base64_alphabet[binary_number.to_ulong()];
	}
	//«аполнение символами "=", если исходные данные не дел€тс€ нацело на 3 байта
	if (remainder == 8) {
		base64_string.append(2, '=');
	}
	else if (remainder == 16) {
		base64_string.append(1, '=');
	}

	return base64_string;
}

std::string Base64ToString(const std::string& base64_string) {
	//Ќахождение количества вхождений символа '=', дл€ его пропуска
	int counter = std::count(base64_string.begin(), base64_string.end(), '=');
	//ѕреобразование строки base64 в битовую последовательность
	std::string bit_string;
	std::bitset<6> binary_number;
	for (size_t i = 0; i < base64_string.size() - counter; i++) {
		binary_number = std::bitset<6>(base64_alphabet.find(base64_string[i]));
		bit_string += binary_number.to_string();
	}
	//”даление лишних нулевых бит
	if (counter == 2) {
		bit_string.resize(bit_string.size() - 4);
	}
	else if (counter == 1) {
		bit_string.resize(bit_string.size() - 2);
	}
	//ѕреобразование битовой последовательности в обычную строку
	std::string decoded_string;
	std::bitset<8> binary_char;
	for (size_t i = 0; i < bit_string.size(); i += 8) {
		binary_char = std::bitset<8>(bit_string.substr(i, 8));
		decoded_string += static_cast<char>(binary_char.to_ulong());
	}

	return decoded_string;
}


int main() {
	std::string test_string = "This is a test string for encoding and decoding.";
	std::cout << "Encoded string in base64: " << StringToBase64(test_string) << std::endl;
	std::cout << "Reverse: " << Base64ToString(StringToBase64(test_string)) << std::endl;

	return 0;
}