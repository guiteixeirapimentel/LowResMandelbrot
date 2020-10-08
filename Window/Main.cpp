#include <Windows.h>
#include <cmath>
#include <ctime>

#include <string>

#include "Timer.h"

int cores[255];
double factor = 1.0;
double dX = 0.0;
double dY = 0.0;

void CriarCores()
{
	srand(time(NULL));
	for (int i = 0; i < 255; i++)
	{
		double perc = sin((double(i) / 255.0) * 3.14);// double(rand() % 256) / 255;//;
		unsigned char r = static_cast<unsigned char>(perc * 255);
		unsigned char g = static_cast<unsigned char>(perc * 255);
		unsigned char b = static_cast<unsigned char>(perc * 255);

		int cor = 0 | (r << 16) | (g << 8) | b;

		cores[i] = cor;
	}
}
void DrawMandelBrot(char* buffer, int width, int height, double factor, double dX, double dY)
{
	double xmin = -2.1 * factor;
	double ymin = -1.3 * factor;
	double xmax = 1.0 * factor;
	double ymax = 1.3 * factor;

	double initigralX = (xmax - xmin) / double(width);
	double initigralY = (ymax - ymin) / double(height);

	double x = (-2.07 + dX) * factor;// -2.07;

	double xx = 0.0;

	for (int s = 0; s < width; s++)
	{
		double y = (ymin + dY) * factor;
		for (int z = 0; z < height; z++)
		{
			double x1 = 0.0;
			double y1 = 0.0;

			int looper = 0;

			while (looper < 100 && (x1 * x1) + (y1 * y1) < 4)
			{
				looper++;
				xx = (x1 * x1) - (y1 * y1) + x;
				y1 = (2 * x1 * y1) + y;
				x1 = xx;
			}

			double perc = double(looper) / 100.0;

			int iCor = int(perc * 255.0);

			reinterpret_cast<unsigned int*>(buffer)[s + (z * width)] = cores[iCor];

			y += initigralY;
		}
		x += initigralX;
	}
}

LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
	case WM_QUIT:
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_KEYUP:
		if (wparam == VK_ESCAPE)
		{
			PostQuitMessage(0);
		}
		break;
	case WM_KEYDOWN:
		if(wparam == VK_DOWN)
		{
			factor *= 0.97;
		}
		else if(wparam == VK_UP)
		{
			factor *= 1.03;
		}
		else if (wparam == 'C')
		{
			CriarCores();
		}
		else if (wparam == 'W')
		{
			dY += 0.1;
		}
		else if (wparam == 'S')
		{
			dY -= 0.1;
		}
		else if (wparam == 'A')
		{
			dX += 0.1;
		}
		else if (wparam == 'D')
		{
			dX -= 0.1;
		}
		break;
	}
	return DefWindowProc(hWnd, msg, wparam, lparam);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
	CriarCores();
	const wchar_t* nomeClasse = L"classeWind";
	const wchar_t* nomeJanela = L"Janela";
	WNDCLASS wc = {};
	wc.hInstance = hInst;
	wc.lpszClassName = nomeClasse;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = MsgProc;

	RegisterClass(&wc);

	HWND hWnd = CreateWindow(nomeClasse, nomeJanela, WS_OVERLAPPEDWINDOW, 0, 0, 1366, 768, NULL, NULL, hInst, NULL);

	UpdateWindow(hWnd);
	ShowWindow(hWnd, SW_SHOW);
	
	Timer* timer = new Timer();

	RECT r = {};

	GetWindowRect(hWnd, &r);

	int width = r.right - r.left;
	int height = r.bottom - r.top;

	char* buffer = new char[width * height * 4];

	timer->Start();
		
	memset(buffer, 0xFFFFFF, width * height * 4);
	DrawMandelBrot(buffer, width, height, factor, dX, dY);

	timer->Tick();

	float tempoDraw = timer->DeltaTime();

	BITMAPINFO bi = {};
	bi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bi.bmiHeader.biWidth = width;
	bi.bmiHeader.biHeight = -height;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biCompression = BI_RGB;

	HDC hdc = GetDC(hWnd);
	StretchDIBits(hdc, 0, 0, width, height, 0, 0, width, height, buffer, &bi, DIB_RGB_COLORS, SRCCOPY);
	ReleaseDC(hWnd, hdc);

	timer->Tick();
	float tempoStrech = timer->DeltaTime();
	std::wstring buff;

	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			buff = L"Draw: ";
			buff += std::to_wstring(tempoDraw);
			buff += L" Strech: ";
			buff += std::to_wstring(tempoStrech);
			SetWindowText(hWnd, buff.c_str());
			
			timer->Tick();
			DrawMandelBrot(buffer, width, height, factor, dX, dY);
			timer->Tick();
			tempoDraw = timer->DeltaTime();

			HDC hdc = GetDC(hWnd);
			StretchDIBits(hdc, 0, 0, width, height, 0, 0, width, height, buffer, &bi, DIB_RGB_COLORS, SRCCOPY);
			ReleaseDC(hWnd, hdc);

			timer->Tick();
			tempoStrech = timer->DeltaTime();

		}
	}
	
	DestroyWindow(hWnd);
	UnregisterClass(nomeClasse, hInst);
	return 0;
}