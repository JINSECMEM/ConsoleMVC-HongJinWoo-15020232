#include "Controllers/AppController.h"
#include <windows.h>

int main() {
    // 콘솔 UTF-8 출력 설정
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    AppController app;
    app.Run();
    return 0;
}
