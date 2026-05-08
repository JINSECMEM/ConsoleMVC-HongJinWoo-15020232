#include "ConsoleHelper.h"
#include <iostream>
#include <iomanip>

namespace ConsoleHelper {

void ClearScreen() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(h, &csbi);
    DWORD size = csbi.dwSize.X * csbi.dwSize.Y;
    DWORD written;
    COORD origin{ 0, 0 };
    FillConsoleOutputCharacterA(h, ' ', size, origin, &written);
    FillConsoleOutputAttribute(h, csbi.wAttributes, size, origin, &written);
    SetConsoleCursorPosition(h, origin);
}

void SetColor(WORD color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void ResetColor() { SetColor(COLOR_DEFAULT); }

void PrintDivider(char ch, int width) {
    std::cout << std::string(width, ch) << '\n';
}

void PrintProgressBar(int pct, int width) {
    int filled = pct * width / 100;
    std::cout << '[';
    for (int i = 0; i < width; ++i)
        std::cout << (i < filled ? '#' : '.');
    std::cout << "] " << std::setw(3) << pct << '%';
}

void PrintTable(const std::vector<std::string>& headers,
                const std::vector<std::vector<std::string>>& rows,
                int colWidth) {
    SetColor(COLOR_CYAN);
    for (auto& h : headers)
        std::cout << std::left << std::setw(colWidth) << h;
    std::cout << '\n';
    ResetColor();
    PrintDivider('-', static_cast<int>(headers.size()) * colWidth);
    for (auto& row : rows) {
        for (auto& cell : row)
            std::cout << std::left << std::setw(colWidth) << cell;
        std::cout << '\n';
    }
}

void Pause() {
    std::cout << "\n[Enter] 계속...";
    std::cin.ignore(1000, '\n');
    std::cin.get();
}

} // namespace ConsoleHelper
