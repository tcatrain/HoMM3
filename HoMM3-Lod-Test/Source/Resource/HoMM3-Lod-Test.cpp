#include <string>
#include <sstream>
#include "Resource/Lod.hpp"
#include "Resource/Def.hpp"
#include "Resource/Pcx.hpp"
#include "Compression/ZHelper.hpp"

void TEST_Lod(std::string const& file, bool woupt)
{
    HoMM3::Resource::Lod txt_content(file);
    txt_content.Load();

    std::cout << "---LOD FILE---" << std::endl << txt_content << std::endl;
    
    std::vector<byte> entry;
    for (uint i = 0, n = txt_content.GetHeader().nb; i < n; ++i)
    {
        std::cout << "---ENTRY " << txt_content.GetEntriesHeaders()[i]->name << "---" << std::endl;
        std::string name((const char*) txt_content.GetEntriesHeaders()[i]->name), needle(".txt");
        if (name.rfind(needle) == name.size() - needle.size())
        {
            entry = txt_content.ReadEntry(*txt_content.GetEntriesHeaders()[i]);
            if (woupt)
            {
                for (std::vector<byte>::const_iterator k = entry.begin(); k != entry.end(); ++k)
                {
                    std::cout << *k;
                }
            }
            else
            {
                std::cout << "Skipped because byte dump is not required" << std::endl;
            }
        }
        else
        {
            std::cout << "Skipped because entry is not txt" << std::endl;
        }
    }
    
    std::cout << std::endl;
}

void TEST_Def(std::string const& file, bool woupt)
{
    HoMM3::Resource::Def sprite_content(file);
    sprite_content.Load();
    
    std::cout << "---DEF FILE---" << std::endl << sprite_content << std::endl;
    
    std::vector<std::vector<byte>> entry;
    for (uint i = 0, n = sprite_content.GetHeader().nb; i < n; ++i)
    {
        std::cout << "---SEQUENCE " << sprite_content.GetEntriesHeaders()[i]->type << "---" << std::endl;
        entry = sprite_content.ReadSequence(*sprite_content.GetEntriesHeaders()[i]);
        for (uint j = 0; j < entry.size(); ++j) {
            uint size = *reinterpret_cast<uint *>(&entry[j][0]);
            std::cout << sprite_content.GetEntriesHeaders()[i]->seq_frames[j]->name << " has size " << size << std::endl;
            if (woupt)
            {
            }
            else
            {
                std::cout << "\tSkipped because byte dump is not required" << std::endl;
            }
        }
    }
    
    std::cout << std::endl;
}

void TEST_Pcx(std::string const& file, bool woupt)
{
    HoMM3::Resource::Pcx frame_content(file);
    frame_content.Load();
    
    std::cout << "---PCX FILE---" << std::endl << frame_content << std::endl;
    
    std::vector<byte> entry;
    entry = frame_content.ReadFrame(frame_content.GetHeader());
    if (woupt)
    {
        for (std::vector<byte>::const_iterator i = entry.begin(); i != entry.end(); ++i)
        {
            std::cout << (int) *i << "-";
        }
    }
    else
    {
        std::cout << "\tSkipped because byte dump is not required" << std::endl;
    }
}

void TEST_ZCompression(std::string const& str)
{
    HoMM3::Compression::ZHelper zcompressor;
    
    std::cout << "---TO DEFLATE---" << std::endl << str << std::endl;
    std::vector<byte> to_deflate(str.begin(), str.end());
    std::vector<byte> deflated = zcompressor.Deflate(to_deflate);
    std::cout << "---DEFLATED---" << std::endl << deflated.data() << std::endl;
    std::vector<byte> inflated = zcompressor.Inflate(deflated);
    std::cout << "---INFLATED---"  << std::endl;
    for (std::vector<byte>::const_iterator i = inflated.begin(); i != inflated.end(); ++i)
    {
        std::cout << *i;
    }
    std::cout << std::endl;
}

int main(int argc, char **argv)
{
    std::locale::global(std::locale(""));

    if (argc != 2)
    {
        return (EXIT_FAILURE);
    }
    
    TEST_Lod("txt_content.lod", std::string("true").compare(argv[1]) == 0);
    TEST_Def("sprite.def", std::string("true").compare(argv[1]) == 0);
    TEST_Pcx("ab01_01.pcx", std::string("true").compare(argv[1]) == 0);
    TEST_ZCompression("This is a deflated string");
    return (EXIT_SUCCESS);
}
