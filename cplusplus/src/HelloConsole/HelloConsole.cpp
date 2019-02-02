#include <Windows.h>
#include <stdio.h>

int main()
{
	HANDLE stdout_handle;
	HANDLE new_screen_buffer_handle;
	SMALL_RECT read_rect;
	SMALL_RECT write_rect;
	CHAR_INFO char_info_buffer[160];
	COORD buffer_size;
	COORD buffer_coord;
	BOOL success;

	printf("Reading and Writing Blocks of Characters and Attributes\n");

	stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	new_screen_buffer_handle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	if (stdout_handle == INVALID_HANDLE_VALUE || 
		new_screen_buffer_handle == INVALID_HANDLE_VALUE)
	{
		printf("CreateConsoleScreenBuffer failed - (%d)\n", GetLastError());
		return 1;
	}

	if (!SetConsoleActiveScreenBuffer(new_screen_buffer_handle))
	{
		printf("SetConsoleActiveScreenBuffer failed - (%d)\n", GetLastError());
		return 1;
	}

	read_rect.Top = 0;
	read_rect.Left = 0;
	read_rect.Bottom = 1;
	read_rect.Right = 79;

	buffer_size.Y = 2;
	buffer_size.X = 80;

	buffer_coord.X = 0;
	buffer_coord.Y = 0;

	success = ReadConsoleOutput(
		stdout_handle,		// screen buffer to read from
		char_info_buffer,	// buffer to copy into
		buffer_size,		// size of char_info_buffer
		buffer_coord,		// top left dest cell in char_info_buffer
		&read_rect);		// screen buffer source rectangle
	if (!success)
	{
		printf("ReadConsoleOutput failed - (%d)\n", GetLastError());
		return 1;
	}

	write_rect.Top = 10;
	write_rect.Left = 0;
	write_rect.Bottom = 11;
	write_rect.Right = 79;

	success = WriteConsoleOutput(
		new_screen_buffer_handle,	// screen buffer to write to 
		char_info_buffer, 			// buffer to copy from
		buffer_size, 				// size of char_info_buffer
		buffer_coord, 				// top left src cell in char_info_buffer
		&write_rect);				// screen buffer dest rectangle

	if (!success)
	{
		printf("WriteConsoleOutput failed - (%d)\n", GetLastError());
		return 1;
	}

	Sleep(5000);

	if (!SetConsoleActiveScreenBuffer(stdout_handle))
	{
		printf("SetConsoleActiveScreenBuffer failed - (%d)\n", GetLastError());
		return 1;
	}
	
	return 0;
}
