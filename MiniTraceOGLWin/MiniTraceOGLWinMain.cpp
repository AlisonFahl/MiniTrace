/*---------------------------------------------------------------------
*
* Copyright © 2015  Minsi Chen
* E-mail: m.chen@derby.ac.uk
*
* The source is written for the Graphics I and II modules. You are free
* to use and extend the functionality. The code provided here is functional
* however the author does not guarantee its performance.
---------------------------------------------------------------------*/
// OGLWin32.cpp : Defines the entry point for the application.
//

#include "MiniTraceOGLWinMain.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "OGLApplication.h"

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")

void PrintUsage()
{
	printf("Use F1 - F6 keys to switch ray trace complexity\n");
	printf("F1: Ambient only\n");
	printf("F2: Full lighting no shadow, reflection and transmission\n");
	printf("F3: Full lighting with shadow\n");
	printf("F4: Full lighting  reflection\n");
	printf("F5: Full lighting  refraction\n");
	printf("F6: Ray trace everything (default)\n");
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR    lpCmdLine,
                     int       nCmdShow)
{
	int exitcode = 0;
	//Allocate a console window
	//so that messages can be redirected to stdout
	AllocConsole();

	HANDLE handle_out = GetStdHandle( STD_OUTPUT_HANDLE );
	int hCrt = _open_osfhandle( (long) handle_out, _O_TEXT );
	FILE* hf_out = _fdopen(hCrt, "w");
	
	setvbuf(hf_out, NULL, _IONBF, 1);
	*stdout = *hf_out;
	PrintUsage();
	//Create the application instance
	OGLApplication* myapp = OGLApplication::CreateApplication(hInstance);
	
	exitcode = myapp->Run();
		
	myapp->DestroyApplication();
	
	return exitcode;
}
