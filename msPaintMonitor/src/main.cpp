#include <Windows.h>
#include <thread>
std::atomic<int> zDelta;
std::atomic<int> mode = 1;
struct myrgb
{
    int r;
    int g;
    int b;
};

static void input(myrgb& c, int& brushWidth);

myrgb c{ 0, 0, 0 };

LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch(msg)
    { 
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
        break;
    
    case WM_MOUSEWHEEL: 
        zDelta = GET_WHEEL_DELTA_WPARAM(wp);
        if (zDelta > 0)
        {
            1 + 1;
        }
        else if (zDelta < 0)
        {
            1 + 1;
        }
        break;
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    myrgb c = { 0, 0, 0 };
    int brushWidth = 25;
    HDC screen = GetDC(NULL);
    HBRUSH redBrush = CreateSolidBrush(RGB(c.r, c.g, c.b));
    HPEN redPen = CreatePen(PS_SOLID, 1, RGB(c.r, c.g, c.b));
    
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    HDC memDC = CreateCompatibleDC(screen);
    HBITMAP memBitmap = CreateCompatibleBitmap(screen, width, height);
    SelectObject(memDC, memBitmap);

    BitBlt(memDC, 0, 0, width, height, screen, 0, 0, SRCCOPY);

    POINT mouse;

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"OverlayClass";
    RegisterClassW(&wc);

    HWND hWnd = CreateWindowExW(
        WS_EX_LAYERED | WS_EX_TOPMOST,
        L"OverlayClass",
        L"Overlay",
        WS_POPUP,
        0,
        0,
        width,
        height,
        NULL,
        NULL,
        hInst,
        NULL
    );

    SetLayeredWindowAttributes(hWnd, 0, 1, LWA_ALPHA);
    ShowWindow(hWnd, SW_SHOW);

    std::thread t1(input, std::ref(c), std::ref(brushWidth));
    t1.detach();

    while (true)
    {
        HBRUSH redBrush = CreateSolidBrush(RGB(c.r, c.g, c.b));
        HPEN redPen = CreatePen(PS_SOLID, 1, RGB(c.r, c.g, c.b));
        SelectObject(memDC, redBrush);
        SelectObject(memDC, redPen);
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
            break;
        if (GetAsyncKeyState('S') & 0x8000)
            mode = 1;

        if (GetAsyncKeyState('R') & 0x8000)
            mode = 2;

        if (GetAsyncKeyState('G') & 0x8000)
            mode = 3;

        if (GetAsyncKeyState('B') & 0x8000)
            mode = 4;

        GetCursorPos(&mouse);
        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) Ellipse(memDC, mouse.x - brushWidth / 2, mouse.y - brushWidth / 2, mouse.x + brushWidth / 2, mouse.y + brushWidth / 2);
        BitBlt(screen, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
    }

    DeleteObject(redBrush);
    DeleteObject(redPen);
    DeleteObject(memBitmap);
    DeleteDC(memDC);
    ReleaseDC(NULL, screen);
    DestroyWindow(hWnd);
    return 0;
}

static void input(myrgb& c, int& brushWidth)
{
    while (true)
    {
        
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
            break;

        if ((GetAsyncKeyState(VK_UP) & 0x8000))
        {
            switch (mode)
            {
            case 1:
                brushWidth += 1;
                break;
            case 2:
                if (c.r < 255) c.r += 1;
                break;
            case 3:
                if (c.g < 255) c.g += 1;
                break;
            case 4:
                 if (c.b < 255) c.b += 1;
                 break;
            default:
                "theres been an error";
            }
        }

        if ((GetAsyncKeyState(VK_DOWN) & 0x8000))
        {
            switch (mode)
            {
            case 1:
                if (!(brushWidth < 1)) brushWidth -= 1;
                break;
            case 2:
                if (c.r > 0) c.r -= 1;
                break;
            case 3:
                if (c.g > 0) c.g -= 1;
                break;
            case 4:
                if (c.b > 0) c.b -= 1;
                break;
            default:
                "theres been an error";
            }

        }
        //Sleep(10);
    }
}