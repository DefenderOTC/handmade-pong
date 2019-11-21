#include "utils.c"
#include "math.c"
#include <windows.h>
#include "platform_common.c"


struct {
	int width, height;
	u32* pixels;
	BITMAPINFO bitmapinfo;
} typedef Render_Buffer;

global_variable 	Render_Buffer render_buffer;
global_variable b32	running	= true;

#include "software_rendering.c"
#include "game.c"

internal LRESULT CALLBACK window_callback(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE prev,
	 PSTR cmdline, int cmdshow){
	
	static TCHAR TCgamename [] 	= TEXT("Game_Window_Class");
	HWND	h_window;
	MSG	msg;

	WNDCLASSEX 	window_class;
	window_class.cbSize		= sizeof(window_class);
	window_class.style		= CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc	= window_callback;
	window_class.cbClsExtra	= 0;
	window_class.cbWndExtra	= 0;
	window_class.hInstance	= hinstance;
	window_class.lpszClassName	= TCgamename;
	window_class.lpszMenuName	= NULL;
	window_class.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);
	window_class.hIcon		= LoadIcon(NULL, IDI_APPLICATION);
	window_class.hIconSm	= NULL;
	window_class.hCursor	= LoadCursor(NULL, IDC_CROSS);

	if(!RegisterClassEx(&window_class)){
		MessageBox(NULL,TEXT("Window class failed register!"),
			TCgamename, MB_ICONERROR);
		return(0);
	}

	h_window 	= CreateWindowEx(WS_EX_CLIENTEDGE, TCgamename,TEXT("Game Window"),
			       WS_OVERLAPPEDWINDOW ,
			       CW_USEDEFAULT,CW_USEDEFAULT,1280,720,
			       NULL, NULL, hinstance, NULL);

	ShowWindow(h_window, cmdshow);
	UpdateWindow(h_window);

	HDC	hdc = GetDC(h_window);
	b32	character = false;
	Input 	input = {0};
	
	LARGE_INTEGER last_counter;
	QueryPerformanceCounter(&last_counter);
	LARGE_INTEGER frequency_counter_large;
	QueryPerformanceFrequency(&frequency_counter_large);
	d64 frequency_counter 	= frequency_counter_large.QuadPart;

	d64 last_dt	= 0.16667;

	while(running){
		//input
		for(int i = 0; i < BUTTON_COUNT; i++){
			input.buttons[i].changed	= false;
		}
		while(PeekMessage(&msg,h_window,0,0,PM_REMOVE)){
			switch(msg.message){
				case WM_SYSKEYDOWN:
				case WM_SYSKEYUP:
				case WM_KEYDOWN:
				case WM_KEYUP:{
					u32 vk_code = (u32)msg.wParam;
					b32 was_down= ((msg.lParam & (1 << 30)) != 0);
					b32 is_down = ((msg.lParam & (1 << 31)) == 0);

			
				process_button(VK_LEFT, BUTTON_LEFT)
				process_button(VK_RIGHT, BUTTON_RIGHT)
				process_button(VK_UP, BUTTON_UP)
				process_button(VK_DOWN, BUTTON_DOWN)
				}//KEYUP
				default:{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				}break;
			}
			
			
		}
		//simulation
		simulate_game(&input, last_dt);

		//rendering
		
		StretchDIBits(hdc, 0,0,render_buffer.width,render_buffer.height, 0,0,
			render_buffer.width,render_buffer.height,render_buffer.pixels,
			   &render_buffer.bitmapinfo,DIB_RGB_COLORS,SRCCOPY);
		// get frame time
		LARGE_INTEGER current_counter;
		QueryPerformanceCounter(&current_counter);
		
		last_dt      = (d64)(current_counter.QuadPart - last_counter.QuadPart) / frequency_counter; 

		last_counter = current_counter;

	}//main loop	
	return(0);


}//main 

internal LRESULT CALLBACK window_callback(HWND hwnd, UINT message,
	   		   WPARAM wparam, LPARAM lparam){
	LRESULT lresult = 0;
	RECT rect;

	switch(message){
		case WM_CREATE:{
		GetClientRect(hwnd, &rect);
		}
		break;
		case WM_CLOSE:
		case WM_DESTROY:{
			running	= false;
		}break;
		case WM_SIZE:{
		GetClientRect(hwnd,&rect);
		render_buffer.width	= rect.right  - rect.left;
		render_buffer.height= rect.bottom - rect.top;
		
		if(render_buffer.pixels){
			VirtualFree(render_buffer.pixels, 0, MEM_RELEASE);
		}

		render_buffer.pixels= VirtualAlloc(NULL, sizeof(u32) * render_buffer.width *
					    render_buffer.height, MEM_COMMIT | MEM_RESERVE,
					    PAGE_READWRITE);
		render_buffer.bitmapinfo.bmiHeader.biSize=sizeof(render_buffer.bitmapinfo.bmiHeader);
		render_buffer.bitmapinfo.bmiHeader.biWidth =render_buffer.width;
		render_buffer.bitmapinfo.bmiHeader.biHeight=render_buffer.height;
		render_buffer.bitmapinfo.bmiHeader.biPlanes=1;
		render_buffer.bitmapinfo.bmiHeader.biBitCount	= 32;
		render_buffer.bitmapinfo.bmiHeader.biCompression	= BI_RGB;
		}break;
		default:{
		lresult = DefWindowProc(hwnd, message, wparam, lparam);
		}break;
	}
	return lresult;
}//callback
	        