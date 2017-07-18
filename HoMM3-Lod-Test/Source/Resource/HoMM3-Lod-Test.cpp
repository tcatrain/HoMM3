#include <string>
#include <sstream>
#include <ostream>
#include "Image/Bitmap.hpp"
#include "Resource/Lod.hpp"
#include "Resource/Def.hpp"
#include "Resource/Pcx.hpp"
#include "Compression/ZHelper.hpp"

HoMM3::Resource::Lod* TEST_Lod(std::string const& file, bool woupt)
{
    HoMM3::Resource::Lod* txt_content = new HoMM3::Resource::Lod(file);
    txt_content->Load();

    std::cout << "---LOD FILE---" << std::endl << *txt_content << std::endl;
    
    std::vector<byte> entry;
    for (uint i = 0, n = txt_content->GetHeader().nb; i < n; ++i)
    {
        std::cout << "---ENTRY " << txt_content->GetEntriesHeaders()[i]->name << "---" << std::endl;
        std::string name((const char*) txt_content->GetEntriesHeaders()[i]->name), needle(".txt");
        if (name.rfind(needle) == name.size() - needle.size())
        {
            entry = txt_content->ReadEntry(*txt_content->GetEntriesHeaders()[i]);
            if (woupt)
            {
                for (std::vector<byte>::const_iterator k = entry.begin(); k != entry.end(); ++k)
                {
                    //std::cout << *k;
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
    return txt_content;
}

HoMM3::Resource::Def* TEST_Def(std::string const& file, bool woupt)
{
    HoMM3::Resource::Def* sprite_content = new HoMM3::Resource::Def(file);
    sprite_content->Load();
    
    std::cout << "---DEF FILE---" << std::endl << *sprite_content << std::endl;
    
    std::vector<std::vector<byte>> entry;
    for (uint i = 0, n = sprite_content->GetHeader().nb; i < n; ++i)
    {
        std::cout << "---SEQUENCE " << sprite_content->GetEntriesHeaders()[i]->type << "---" << std::endl;
        entry = sprite_content->ReadSequence(*sprite_content->GetEntriesHeaders()[i]);
        for (uint j = 0; j < entry.size(); ++j) {
            uint size = *reinterpret_cast<uint *>(&entry[j][0]);
            std::cout << sprite_content->GetEntriesHeaders()[i]->seq_frames[j]->name << " has size " << size << std::endl;
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
    return sprite_content;
}

HoMM3::Resource::Pcx* TEST_Pcx(std::string const& file, bool woupt)
{
    HoMM3::Resource::Pcx* frame_content = new HoMM3::Resource::Pcx(file);
    frame_content->Load();
    
    std::cout << "---PCX FILE---" << std::endl << *frame_content << std::endl;
    
    std::vector<byte> entry;
    entry = frame_content->ReadFrame(frame_content->GetHeader());
    if (woupt)
    {
        for (std::vector<byte>::const_iterator i = entry.begin(); i != entry.end(); ++i)
        {
            std::cout << (int) *i << "-";
        }
        std::cout << std::endl;
    }
    else
    {
        std::cout << "\tSkipped because byte dump is not required" << std::endl;
    }
    return frame_content;
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

void TEST_Bitmap(HoMM3::Resource::Pcx* pcx, const HoMM3::Resource::DefColorIndex* palette)
{
    const std::vector<byte> frame_datas = pcx->ReadFrame(pcx->GetHeader());
    HoMM3::Image::BitmapColor bitmap_palette[256];
    
    HoMM3::Image::Bitmap bitmap;
    bitmap.GetInfos().infossize = 40;
    bitmap.GetInfos().width = pcx->GetHeader().fmwidth;
    bitmap.GetInfos().height = pcx->GetHeader().fmheight;
    bitmap.GetInfos().planes = 1;
    bitmap.GetInfos().bpp = 8;
    bitmap.GetInfos().rlemode = 0;
    bitmap.GetInfos().realsize = 0;
    bitmap.GetInfos().xppm = 100;
    bitmap.GetInfos().yppm = 100;
    bitmap.GetInfos().palettesize = 256;
    bitmap.GetInfos().nbimportantcolor = 0;
    
    for (uint i = 0; i < 256; ++i)
    {
        bitmap_palette[i].b = palette[i].b;
        bitmap_palette[i].g = palette[i].g;
        bitmap_palette[i].r = palette[i].r;
        bitmap_palette[i].a = 0;
    }
    bitmap.SetPalette(bitmap_palette);
    bitmap.SetPixels(frame_datas);
    
    const std::vector<byte> bitmap_bytes = bitmap.Create();
    
    std::ofstream outfile("test.bmp", std::ios::out | std::ios::binary); 
    outfile.write((const char*)&bitmap_bytes[0], bitmap_bytes.size());
}

int main(int argc, char **argv)
{
    std::locale::global(std::locale(""));

    if (argc != 2)
    {
        return (EXIT_FAILURE);
    }
    
    HoMM3::Resource::Lod* lod = TEST_Lod("txt_content.lod", std::string("true").compare(argv[1]) == 0);
    HoMM3::Resource::Def* def = TEST_Def("sprite.def", std::string("true").compare(argv[1]) == 0);
    HoMM3::Resource::Pcx* pcx = TEST_Pcx("ab01_01.pcx", std::string("true").compare(argv[1]) == 0);
    TEST_Bitmap(pcx, def->GetHeader().palette);
    TEST_ZCompression("This is a deflated string");
    return (EXIT_SUCCESS);
}
