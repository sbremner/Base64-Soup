#define _CRT_SECURE_NO_WARNINGS

#include "includes.h"
#include "argopts.h"
#include "banners.h"

#include <io.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <string>

std::vector<char> alphabet = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', '+', '/'
};

// Global options
bool encode = true; // default (decode is when encode = false)
bool hidebanner = false;
bool randomize_alphabet = false;

std::string to_decode;
std::string to_encode;

// Base64 tables
static char * encoding_table = NULL;
static char *decoding_table = NULL;
static int mod_table[] = { 0, 2, 1 };

// Function prototypes
void build_encoding_table( void );
void build_decoding_table( void );

char *base64_encode(const char *data,
	size_t input_length,
	size_t *output_length) {

	if (encoding_table == NULL) build_encoding_table();

	*output_length = 4 * ((input_length + 2) / 3);

	char *encoded_data = (char *)malloc(*output_length);
	if (encoded_data == NULL) return NULL;

	for (int i = 0, j = 0; i < input_length;) {

		uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
		uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
		uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;

		uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

		encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
		encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
		encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
		encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
	}

for (int i = 0; i < mod_table[input_length % 3]; i++)
	encoded_data[*output_length - 1 - i] = '=';

return encoded_data;
}


unsigned char *base64_decode(const char *data,
	size_t input_length,
	size_t *output_length) {

	if (decoding_table == NULL) build_decoding_table();

	if (input_length % 4 != 0) return NULL;

	*output_length = input_length / 4 * 3;
	if (data[input_length - 1] == '=') (*output_length)--;
	if (data[input_length - 2] == '=') (*output_length)--;

	unsigned char *decoded_data = (unsigned char *)malloc(*output_length);
	if (decoded_data == NULL) return NULL;

	for (int i = 0, j = 0; i < input_length;) {

		uint32_t sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
		uint32_t sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
		uint32_t sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
		uint32_t sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];

		uint32_t triple = (sextet_a << 3 * 6)
			+ (sextet_b << 2 * 6)
			+ (sextet_c << 1 * 6)
			+ (sextet_d << 0 * 6);

		if (j < *output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
		if (j < *output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
		if (j < *output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
	}

	return decoded_data;
}

void build_encoding_table() {

	encoding_table = (char *)malloc(alphabet.size() * sizeof(char));

	int element = 0;

	// If we are randomizing, we build the table slightly differently
	if (randomize_alphabet){
		srand(time(0));

		std::vector<char> alpha_copy(alphabet.begin(), alphabet.end());

		for (int i = 0; alpha_copy.size() > 0; i++){
			element = rand() % alpha_copy.size();
			encoding_table[i] = alpha_copy[element];
			alpha_copy.erase(alpha_copy.begin() + element);
		}
	}
	else{
		for (int i = 0; i < alphabet.size(); i++){
			encoding_table[i] = alphabet[i];
		}
	}

	// encoding table should never be null here
	assert(encoding_table != NULL);
}

void build_decoding_table() {

	decoding_table = (char *)malloc(256);

	for (int i = 0; i < 64; i++)
		decoding_table[(char)encoding_table[i]] = i;

	// decoding table should never be null here
	assert(decoding_table != NULL);
}

void print_encoding_table() {
	for (int i = 0; i < 64; i++){
		printf("%c", encoding_table[i]);
	}
}

void base64_cleanup() {
	if (decoding_table != NULL){
		free(decoding_table);
	}

	if (encoding_table != NULL){
		free(encoding_table);
	}
}

void cleanup(){
	base64_cleanup();
}

bool is_valid_b64_alphabet(std::string str)
{
	char ary[256] = { false };

	if (str.length() != 64){
		return false; // must be 64 character length string
	}

	for (std::string::iterator it = str.begin(); it != str.end(); it++)
	{
		// If it is set to true, we must have already encountered this character
		if (((int)(*it) < 256 && (int)(*it) >= 0) && ary[(int)(*it)] == true){
			return false;
		}

		ary[(int)(*it)] = true;
	}

	return true; // none of our tests failed (must be valid!)
}

void opthandler(OPTIONS opts)
{
	//Used for looping through the params below
	PARAM p;

	//Loop 
	while ((p = opts.nextarg()) != INVALID_PARAM)
	{
		std::string flag = p.getflag();
		if (flag == "-e" || flag == "-E" || flag == "--encode")
		{
			to_encode = p.getdata();
		}
		else if (flag == "-d" || flag == "-D" || flag == "--decode")
		{
			to_decode = p.getdata();
		}
		else if (flag == "--alphabet")
		{
			std::string data = p.getdata();

			if (!is_valid_b64_alphabet(data)){
				throw OptionError("ERROR :: Input B64 alphabet is not a valid alphabet (check unique characters and length)");
			}

			alphabet.clear();
			alphabet = std::vector<char>(data.begin(), data.end());
		}
		else if (flag == "--random")
		{
			randomize_alphabet = true;
		}
		else if (flag == "--hide-banner")
		{
			hidebanner = true;
		}
	}
}

void print_string(char * str, int length)
{
	for (int i = 0; i < length; i++)
	{
		printf("%c", str[i]);
	}
}

void optexec()
{
	build_encoding_table();

	putc('\n', stdout);
	printf("Alphabet:\n ");
	print_encoding_table();
	printf("\n\n");

	size_t output_length;
	char * output_string;

	// We are encoding
	if (!to_encode.empty()){
		output_string = (char *)base64_encode(to_encode.c_str(), to_encode.length(), &output_length);

		printf("Encoder:\n");
		printf(" Input Data:\t%s\n", to_encode.c_str());
		printf(" Output Data:\t");
		print_string(output_string, output_length);
		printf("\n\n");
	}
	
	//We are decoding
	if(!to_decode.empty()){
		output_string = (char *)base64_decode(to_decode.c_str(), to_decode.length(), &output_length);

		printf("Decoder:\n");
		printf(" Input Data:\t%s\n", to_decode.c_str());
		printf(" Output Data:\t");
		print_string(output_string, output_length);
		printf("\n\n");
	}
}

int main(int argc, char * argv[]){
	OPTIONS opts;

	atexit(cleanup);

	if (argc <= 1)
	{
		//no arguments provided... print usage
		printusage(std::string(argv[0]));
		return 0;
	}

	getargs(argc, argv, &opts);

	//printopts(opts);
	try{
		opthandler(opts);

		//this should execute the command (args should be populated in the opthandler function)
		optexec();

		if (!hidebanner){
			printbanner();
		}
	}
	catch (OptionError ex){
		// If we made it here, we had an issue in opthandler (didn't pass a test)
		printf("%s\n", ex.what());
		printusage(opts.execPath);
	}

	return 0;

}