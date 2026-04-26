#include <Windows.h>

int main()
{
    HDC screen = GetDC(NULL);

    // Create a persistent memory DC the size of the screen
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    HDC memDC = CreateCompatibleDC(screen);
    HBITMAP memBitmap = CreateCompatibleBitmap(screen, width, height);
    SelectObject(memDC, memBitmap);

    // Copy current screen contents into memory DC
    BitBlt(memDC, 0, 0, width, height, screen, 0, 0, SRCCOPY);

    POINT mouse;
    while (true)
    {
        GetCursorPos(&mouse);

        // Draw circle onto the memory DC (persists between frames)
        Ellipse(memDC, mouse.x, mouse.y, mouse.x + 50, mouse.y + 50);

        // Blit memory DC to screen each frame
        BitBlt(screen, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
    }

    // Cleanup (unreachable here, but good practice)
    DeleteObject(memBitmap);
    DeleteDC(memDC);
    ReleaseDC(NULL, screen);

    return 0;
}
