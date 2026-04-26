#include <Windows.h>

int main()
{
	HDC screen = GetDC(NULL);
	POINT mouse;
	while (true)
	{
		GetCursorPos(&mouse);
		Ellipse(screen, mouse.x, mouse.y, mouse.x + 50, mouse.y + 50);
	}
	
	return 0;
}