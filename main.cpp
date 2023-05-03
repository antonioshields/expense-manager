#include <chrono>
#include <filesystem>
#include <string>
#include <cstring>

#include "data_manager/data_mgr.h"
int main();

void clear() {
    COORD topLeft = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    FillConsoleOutputAttribute(
        console,
        FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    SetConsoleCursorPosition(console, topLeft);
}

void check() {
    clear();
    std::cout << "(!) ZGrid Expenses\n\n";

    data_mgr data;
    std::vector<std::filesystem::path> files;
    bool found_files{ false };
    std::string input;

    for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path() / "data")) {
        if (entry.is_regular_file() && entry.path().extension() == ".zgrid") {
            found_files = true;
            try {
                files.push_back(entry.path());
            }
            catch (const std::filesystem::filesystem_error& e) {
                std::cerr << "(-) Error fetching data: " << e.what() << "\n";
                Sleep(2500);
            }
        }
    }

    // no expense files exists
    if (!found_files) {
        std::cout << "(-) No expenses found!\n";
        Sleep(2500);

        // reset
        main();
        return;
    }

    for (const auto& file : files) {
        data.read_data(std::format("data/{}", file.filename().string()));
        std::cout << std::format("(!) {} | Amount Due: {} | Date Due: {}\n", data.get_expense_name(), data.get_expense_price(), data.get_expense_data());
    }

    std::cout << "(!) Type home to go back.\n";
    std::cin >> input;

    // do it this way to ignore case
    if (_stricmp(input.c_str(), "home") == 0)
        main();
}

void update() {
reset_update:
    clear();
    std::cout << "(!) ZGrid Expenses\n\n";

    data_mgr data;
    std::string name, price, date, input;

    std::cout << "Enter expense name: ";
    std::cin >> name;

    std::cout << "Enter expense price: ";
    std::cin >> price;
    price = "$" + price;

    std::cout << "Enter expense due date (number-only): ";
    std::cin >> date;

    data.write_data(name, price, date);
    clear();

    std::cout << "(+) Saved expense! Would you like to add another expense?\n";
    std::cin >> input;

    // do it this way to ignore case
    if (_stricmp(input.c_str(), "yes") == 0)
        goto reset_update;

    // reset
    main();
}

void destroy() {
    clear();    
    std::cout << "(!) ZGrid Expenses\n\n";

    std::string input;

    std::cout << "(?) Are you sure you want to destroy all data?\n";
    std::cin >> input;

    if (_stricmp(input.c_str(), "yes") == 0) {
        // keep it clean
        clear();
        std::filesystem::path path = std::filesystem::current_path() / "data";
        std::string extension = ".zgrid";

        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            if (entry.is_regular_file() && entry.path().extension() == extension) {
                std::cout << "(+) Deleting: " << entry.path().stem().string() << " data!\n";
                try {
                    std::filesystem::remove(entry.path());
                }
                catch (const std::filesystem::filesystem_error& e) {
                    std::cerr << "(-) Error deleting data: " << e.what() << "\n";
                }
            }
        }
        // pause long enough to read whats being deleted
        Sleep(2500);
    }

    // reset
    main();
}

int main() {
    SetConsoleTitle(L"ZGrid Expense Tracker");

    if (!std::filesystem::exists(std::filesystem::current_path() / "data"))
        std::filesystem::create_directory(std::filesystem::current_path() / "data");

reset_main:
    clear();
    std::cout << "(!) Welcome to ZGrid Expense Tracker! Please select an option.\n\n";

    int option{};
    std::cout << "(?) [1] - Check your expenses\n";
    std::cout << "(?) [2] - Update your expenses\n";
    std::cout << "(?) [3] - Destroy all expense data\n";

    std::cout << "(!) Please enter a number correlating to your choice.\n\n";

    std::cin >> option;
    switch (option)
    {
    case 1: // check
        check();
        break;
    case 2: // update
        update();
        break;
    case 3: // delete
        destroy();
        break;
    default:
        std::cout << "(-) Please type a valid number.";
        Sleep(1000);

        goto reset_main;
        break;
    }

    return 0;
}