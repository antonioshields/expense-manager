#include "data_mgr.h"

#include "../libraries/json/json.h"

using json = nlohmann::json;
void data_mgr::write_data(const std::string name, const std::string price, const std::string date) {
    base64 base64;
    //std::cout << std::format("(+) writing [{}, {}, {}]\n", name, price, date);
    json j;
    j["expense_name"] = name;
    j["expense_price"] = price;
    j["expense_date"] = date;

    // create file
    try {
        std::ofstream o(std::format("data/{}.zgrid", name));
        o << base64.encode(j.dump()) << std::endl;
        o.close();
    }
    catch (const std::exception& e) {
        std::cerr << "(-) Failed writing data: " << e.what() << "\n";
    }
    return;
}

void data_mgr::read_data(const std::string& file_name) {
    base64 base64;

    //std::cout << std::format("(+) reading [{}]\n", file_name);
    std::ifstream i(file_name);
    std::string encoded_data((std::istreambuf_iterator<char>(i)), std::istreambuf_iterator<char>());
    i.close();

    std::string decoded_data = base64.decode(encoded_data);

    try {
        json j = json::parse(decoded_data);

        expense_name = j["expense_name"];
        expense_price = j["expense_price"];
        expense_date = j["expense_date"];
    }
    catch (const std::exception& e) {
        std::cerr << "(-) Failed reading data: " << e.what() << "\n";
    }

    return;
}