// HoMM3-Lod-Test.cpp : Defines the entry point for the console application.
//

#include "Resource/Lod.hpp"

int main(int argc, char **argv)
{
	std::locale::global(std::locale(""));

	if (argc > 1)
	{
		HoMM3::Resource::Lod txt_content(const_cast<char*>(argv[1]));
		std::vector<byte> entry = txt_content.ReadEntry(*txt_content.GetEntriesHeaders()[36].get());
		
		//std::cout << txt_content;
		//std::cout << entry.size() << std::endl;
		for (std::vector<byte>::const_iterator i = entry.begin(); i != entry.end(); ++i)
		    std::cout << *i;
	}
	
	return 0;
}

