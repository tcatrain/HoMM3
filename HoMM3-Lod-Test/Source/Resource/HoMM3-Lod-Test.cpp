#include <string>
#include <sstream>
#include "Resource/Lod.hpp"
#include "Resource/Def.hpp"
#include "Compression/ZHelper.hpp"

void TEST_Lod(std::string const& file, bool woupt)
{
    HoMM3::Resource::Lod txt_content(file);
    txt_content.Load();
    
    std::vector<byte> entry = txt_content.ReadEntry(*txt_content.GetEntriesHeaders()[36]);
        
    std::cout << "---LOD FILE---" << std::endl << txt_content << std::endl;
    std::cout << "---ENTRY---" << std::endl;
    if (woupt)
    {
        for (std::vector<byte>::const_iterator i = entry.begin(); i != entry.end(); ++i)
            std::cout << *i;
    }
    else
        std::cout << "Skipped" << std::endl;
    std::cout << std::endl;
}

void TEST_Def(std::string const& file, bool woupt)
{
    HoMM3::Resource::Def sprite_content(file);
    sprite_content.Load();
    
    std::cout << "---DEF FILE---" << std::endl << sprite_content << std::endl;
    std::cout << "---SEQUENCE---" << std::endl;
    if (woupt)
    {
        
    }
    else
        std::cout << "Skipped" << std::endl;
    std::cout << std::endl;
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

    if (argc != 4)
        return (EXIT_FAILURE);
    
    TEST_Lod(argv[1], std::string("true").compare(argv[3]) == 0);
    TEST_Def(argv[2], std::string("true").compare(argv[3]) == 0);
    TEST_Compression("This is a deflated string");
    return (EXIT_SUCCESS);
}
