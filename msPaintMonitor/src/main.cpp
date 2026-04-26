#include <Windows.h>

int main()
{
    HDC screen = GetDC(NULL);

    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    HDC memDC = CreateCompatibleDC(screen);
    HBITMAP memBitmap = CreateCompatibleBitmap(screen, width, height);
    SelectObject(memDC, memBitmap);

    BitBlt(memDC, 0, 0, width, height, screen, 0, 0, SRCCOPY);

    POINT mouse;
    while (true)
    {
        // Press Escape to exit
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
            break;

        GetCursorPos(&mouse);
        Ellipse(memDC, mouse.x, mouse.y, mouse.x + 50, mouse.y + 50);
        BitBlt(screen, 0, 0, width, height, memDC, 0, 0, SRCCOPY);

        
    }

    DeleteObject(memBitmap);
    DeleteDC(memDC);
    ReleaseDC(NULL, screen);

    return 0;
}