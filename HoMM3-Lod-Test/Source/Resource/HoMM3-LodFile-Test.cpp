// HoMM3-Lod-Test.cpp : Defines the entry point for the console application.
//

#include "Resource/Lod.h"
#include <windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
	std::locale::global(std::locale(""));

	Lod::File file("Resources\\h3abp_bm.lod");
	//Lod::File file("Resources\\H3sprite.lod");
	std::cout << file.GetEntry(std::move(file.GetEntriesHeaders()[1]));
	
	system("PAUSE");
	return 0;
}

