#pragma once
#include <windows.h>
#include <fstream>
#include <iostream>
#include <regex>

#include "../libraries/base64/base64.h"


class data_mgr
{
private:
    std::string add_ordinal_indicator(std::string input) {
        std::regex numberRegex("\\d+");
        std::smatch match;
        if (std::regex_search(input, match, numberRegex)) {
            std::string number = match.str();
            int num = std::stoi(number);
            std::string ordinalIndicator;
            switch (num % 100) {
            case 11:
            case 12:
            case 13:
                ordinalIndicator = "th";
                break;
            default:
                switch (num % 10) {
                case 1:
                    ordinalIndicator = "st";
                    break;
                case 2:
                    ordinalIndicator = "nd";
                    break;
                case 3:
                    ordinalIndicator = "rd";
                    break;
                default:
                    ordinalIndicator = "th";
                    break;
                }
                break;
            }
            input.replace(match.position(), number.length(), number + ordinalIndicator);
        }
        return input;
    }

	std::string expense_name{};
	std::string expense_price{};
	std::string expense_date{};

public:
	void write_data(const std::string name, const std::string price, const std::string date);
	void read_data(const std::string& file_name);

	std::string get_expense_name() {
		return expense_name;
	}

	std::string get_expense_price() {
		return expense_price;
	}

	std::string get_expense_data() {
		return add_ordinal_indicator(expense_date);
	}
};

