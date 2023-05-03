#include "base64.h"

#undef max

static const char b64_table[ 65 ] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
std::string base64::encode( std::string bin ) {
    using ::std::string;
    using ::std::numeric_limits;

    if ( bin.size( ) > ( numeric_limits<string::size_type>::max( ) / 4u ) * 3u ) {

    }

    const ::std::size_t binlen = bin.size( );
    // Use = signs so the end is properly padded.
    string retval( ( ( ( binlen + 2 ) / 3 ) * 4 ), '=' );
    ::std::size_t outpos = 0;
    int bits_collected = 0;
    unsigned int accumulator = 0;
    const string::const_iterator binend = bin.end( );

    for ( string::const_iterator i = bin.begin( ); i != binend; ++i ) {
        accumulator = ( accumulator << 8 ) | ( *i & 0xffu );
        bits_collected += 8;
        while ( bits_collected >= 6 ) {
            bits_collected -= 6;
            retval[ outpos++ ] = b64_table[ ( accumulator >> bits_collected ) & 0x3fu ];
        }
    }
    if ( bits_collected > 0 ) { // Any trailing bits that are missing.
        accumulator <<= 6 - bits_collected;
        retval[ outpos++ ] = b64_table[ accumulator & 0x3fu ];
    }
    return retval;
}

std::string base64::decode(std::string const& str)
{
	const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	int in_len = str.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

	while (in_len-- && (str[in_] != '=') && is_base64(str[in_]))
	{
		char_array_4[i++] = str[in_]; in_++;
		if (i == 4) {
			for (i = 0; i < 4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}

	if (i) {
		for (j = i; j < 4; j++)
			char_array_4[j] = 0;

		for (j = 0; j < 4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}

	return ret;
}