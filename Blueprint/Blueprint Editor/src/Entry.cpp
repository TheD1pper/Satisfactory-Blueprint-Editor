#include <Windows.h>
#include <stdio.h>

import std;

import Editor.Application;

int main() 
{
	Editor::Application SBE("Satisfactory Blueprint Editor", 1920, 1080);
	SBE.Run();
}