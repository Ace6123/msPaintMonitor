#include "app.h"

myrgb c{ 0, 0, 0 };
std::atomic<int> zDelta;
std::atomic<int> mode = 1;
int brushWidth = 25;

void updateMouseProps() {
    if ((GetAsyncKeyState(VK_UP) & 0x8000)) {
        switch (mode) {
        case 1: {
            brushWidth += 1;
            break;
        }
        case 2: {
            if (c.r < 255) c.r += 1;
            break;
        }
        case 3: {
            if (c.g < 255) c.g += 1;
            break;
        }
        case 4: {
            if (c.b < 255) c.b += 1;
            break;
        }
        default:
            "theres been an error";
        }
    }

    if ((GetAsyncKeyState(VK_DOWN) & 0x8000)) {
        switch (mode)
        {
        case 1:
            if (brushWidth > 1) brushWidth -= 1;
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
}

void input(myrgb& c, int& brushWidth) {
    while (true) {

        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
            break;
        updateMouseProps();

        Sleep(10);
    }
}