#pragma once
#include <string>


class base64 {
public:
	std::string encode( std::string data );
    std::string decode(std::string const& str);

	bool is_base64(unsigned char c)
	{
		return (isalnum(c) || (c == '+') || (c == '/'));
	}
};