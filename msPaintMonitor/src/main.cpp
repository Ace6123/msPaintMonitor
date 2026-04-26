#include <Windows.h>

int main()
{
	HDC screen = GetDC(NULL);
	while (true)
	{
		Rectangle(screen, 0, 10, 10, 10);
		Sleep(20);
	}
	return 0;
}