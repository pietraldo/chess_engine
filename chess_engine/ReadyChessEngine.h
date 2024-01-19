#pragma once
#undef N
#include <windows.h>
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <locale>
#include <codecvt>

using namespace std;


/*
This class is for comunicating with ready chess engine in exe file
*/
class ReadyChessEngine
{
public:
	static wstring engine_path_exe;
	static string GiveAnswer(const char* command);
	
};

