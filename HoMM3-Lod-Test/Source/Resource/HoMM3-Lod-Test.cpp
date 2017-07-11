// HoMM3-Lod-Test.cpp : Defines the entry point for the console application.
//

#include "Resource/Lod.hpp"
#include "Compression/ZHelper.hpp"
#include <string>
#include <sstream>

void TEST_Lod(std::string const& file, bool woupt)
{
	HoMM3::Resource::Lod txt_content(file);
	std::vector<byte> entry = txt_content.ReadEntry(*txt_content.GetEntriesHeaders()[36].get());
		
	std::cout << "---LOD FILE---" << std::endl << txt_content << std::endl;
	std::cout << "---ENTRY---" << std::endl;
    if (woupt)
        for (std::vector<byte>::const_iterator i = entry.begin(); i != entry.end(); ++i)
            std::cout << *i;
    else
        std::cout << "Skipped" << std::endl;
	std::cout << std::endl;
}

void TEST_Def()
{
    
}

void TEST_Compression(std::string const& str)
{
	std::cout << "---TO DEFLATE---" << std::endl << str << std::endl;
	std::vector<byte> to_deflate(str.begin(), str.end());
	std::vector<byte> deflated = HoMM3::Compression::ZHelper::Deflate(to_deflate);
	std::cout << "---DEFLATED---" << std::endl << deflated.data() << std::endl;
	std::vector<byte> inflated = HoMM3::Compression::ZHelper::Inflate(deflated);
	std::cout << "---INFLATED---"  << std::endl;
	for (std::vector<byte>::const_iterator i = inflated.begin(); i != inflated.end(); ++i)
		std::cout << *i;
	std::cout << std::endl;
}

int main(int argc, char **argv)
{
	std::locale::global(std::locale(""));

	if (argc != 3)
		return (EXIT_FAILURE);
    
    TEST_Lod(argv[1], std::string("true").compare(argv[2]) == 0);
	TEST_Compression("This is a deflated string");
	return (EXIT_SUCCESS);
}

