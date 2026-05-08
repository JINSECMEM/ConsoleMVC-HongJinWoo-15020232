#pragma once
#define NOMINMAX        // windows.h의 min/max 매크로가 std::min/max와 충돌하는 것을 방지
#include <string>
#include <vector>
#include <windows.h>

namespace ConsoleHelper {

// 색상 코드 (Windows console)
constexpr WORD COLOR_DEFAULT = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
constexpr WORD COLOR_CYAN    = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
constexpr WORD COLOR_YELLOW  = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
constexpr WORD COLOR_RED     = FOREGROUND_RED | FOREGROUND_INTENSITY;
constexpr WORD COLOR_GREEN   = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
constexpr WORD COLOR_WHITE   = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;

void ClearScreen();
void SetColor(WORD color);
void ResetColor();
void PrintDivider(char ch = '-', int width = 60);
void PrintProgressBar(int pct, int width = 20);
void PrintTable(const std::vector<std::string>& headers,
                const std::vector<std::vector<std::string>>& rows,
                int colWidth = 16);
void Pause();

} // namespace ConsoleHelper
