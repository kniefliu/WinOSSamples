
#include <Windows.h>

HANDLE stdout_handle;
HANDLE stdin_handle;

CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info;

void NewLine(void);
void ScrollScreenBuffer(HANDLE, INT);

//int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstPre, LPSTR lpCmdLine, int nShowWindow)
int main()
{
	stdin_handle = GetStdHandle(STD_INPUT_HANDLE);
	stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);

	if (stdin_handle == INVALID_HANDLE_VALUE ||
		stdout_handle == INVALID_HANDLE_VALUE) 
	{
		MessageBox(NULL, TEXT("GetStdHandle"), TEXT("ConsoleError"), MB_OK);
		return 1;
	}

	if (!GetConsoleScreenBufferInfo(stdout_handle, &console_screen_buffer_info))
	{
		MessageBox(NULL, TEXT("GetConsoleScreenBufferInfo"), TEXT("Console Error"), MB_OK);
		return 1;
	}

	WORD old_color_attrs = console_screen_buffer_info.wAttributes;

	if (!SetConsoleTextAttribute(stdout_handle, FOREGROUND_RED | FOREGROUND_INTENSITY)) 
	{
		MessageBox(NULL, TEXT("SetConsoleTextAttribute"), TEXT("Console Error"), MB_OK);
		return 1;
	}

	LPSTR prompt_str1 = "Type a line and press Enter, or q to quit: ";
	LPSTR prompt_str2 = "Type any key, or q to quit: ";
	DWORD num_written;
	DWORD num_read;

	CHAR buffer[256];
	while (1) 
	{
		if (!WriteFile(stdout_handle, prompt_str1, lstrlenA(prompt_str1), &num_written, NULL)) 
		{
			MessageBox(NULL, TEXT("WriteFile"), TEXT("Console Error"), MB_OK);
			return 1;
		}
		if (!ReadFile(stdin_handle, buffer, 255, &num_read, NULL))
			break;

		if (buffer[0] == 'q')
			break;
	}
	DWORD mode;
	if (!GetConsoleMode(stdin_handle, &mode)) 
	{
		MessageBox(NULL, TEXT("GetConsoleMode"), TEXT("Console Error"), MB_OK);
		return 1;
	}

	DWORD new_mode = mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
	if (!SetConsoleMode(stdin_handle, new_mode))
	{
		MessageBox(NULL, TEXT("SetConsoleMode"), TEXT("Console Error"), MB_OK);
		return 1;
	}

	NewLine();

	while (1)
	{
		if (!WriteFile(stdout_handle, prompt_str2, lstrlenA(prompt_str2), &num_written, NULL))
		{
			MessageBox(NULL, TEXT("WriteFile"), TEXT("Console Error"), MB_OK);
			return 1;
		}

		if (!ReadFile(stdin_handle, buffer, 1, &num_read, NULL))
			break;
		if (buffer[0] == '\r')
			NewLine();
		else if (!WriteFile(stdout_handle, buffer, num_read, &num_written, NULL))
			break;
		else 
			NewLine();
		if (buffer[0] == 'q')
			break;
	}

	SetConsoleMode(stdin_handle, mode);
	SetConsoleTextAttribute(stdout_handle, old_color_attrs);
	
	return 0;
}

void NewLine(void)
{
	if (!GetConsoleScreenBufferInfo(stdout_handle, &console_screen_buffer_info))
	{
		MessageBox(NULL, TEXT("GetConsoleScreenBufferInfo"), TEXT("Console Error"), MB_OK);
		return;
	}

	console_screen_buffer_info.dwCursorPosition.X = 0;

	if ((console_screen_buffer_info.dwSize.Y - 1) == console_screen_buffer_info.dwCursorPosition.Y)
	{
		ScrollScreenBuffer(stdout_handle, 1);
	}
	else 
	{
		console_screen_buffer_info.dwCursorPosition.Y += 1;
	}

	if (!SetConsoleCursorPosition(stdout_handle, console_screen_buffer_info.dwCursorPosition))
	{
		MessageBox(NULL, TEXT("SetConsoleCursorPosition"), TEXT("Console Error"), MB_OK);
		return;
	}
}

void ScrollScreenBuffer(HANDLE handle, INT x)
{
	SMALL_RECT scroll_rect;
	SMALL_RECT clip_rect;
	CHAR_INFO fill;
	COORD dest;

	scroll_rect.Left = 0;
	scroll_rect.Top = 1;
	scroll_rect.Right = console_screen_buffer_info.dwSize.X - (SHORT)x;
	scroll_rect.Bottom = console_screen_buffer_info.dwSize.Y - (SHORT)x;

	dest.X = 0;
	dest.Y = 0;

	clip_rect = scroll_rect;

	fill.Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
	fill.Char.AsciiChar = (char)' ';

	ScrollConsoleScreenBuffer(handle, &scroll_rect, &clip_rect, dest, &fill);
}
