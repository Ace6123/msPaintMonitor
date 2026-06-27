#pragma once

#include <Windows.h>
#include <thread>

struct myrgb {
    int r;
    int g;
    int b;
};

void input(myrgb& c, int& brushWidth);

void updateMouseProps();

LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

extern myrgb c;
extern std::atomic<int> zDelta;
extern std::atomic<int> mode;
extern int brushWidth;
