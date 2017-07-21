#include <string>
#include <sstream>

#include "gmock/gmock.h"
#include "Image/Bitmap.hpp"
#include "Resource/Lod.hpp"
#include "Resource/Def.hpp"
#include "Resource/Pcx.hpp"


//void TEST_ZCompression(std::string const& str)
//{
//    HoMM3::Compression::ZHelper zcompressor;
//
//    std::cout << "---TO DEFLATE---" << std::endl << str << std::endl;
//    std::vector<byte> to_deflate(str.begin(), str.end());
//    std::vector<byte> deflated = zcompressor.Deflate(to_deflate);
//    std::cout << "---DEFLATED---" << std::endl << deflated.data() << std::endl;
//    std::vector<byte> inflated = zcompressor.Inflate(deflated);
//    std::cout << "---INFLATED---"  << std::endl;
//    for (std::vector<byte>::const_iterator i = inflated.begin(); i != inflated.end(); ++i)
//    {
//        std::cout << *i;
//    }
//    std::cout << std::endl;
//}
//
//void TEST_Bitmap(HoMM3::Resource::Pcx* pcx, const HoMM3::Resource::Def* def)
//{
//    const std::vector<byte> frame_datas = pcx->ReadFrame();
//    const HoMM3::Resource::DefColorIndex* palette = def->GetHeader().palette;
//    HoMM3::Image::BitmapColor bitmap_palette[256];
//
//    HoMM3::Image::Bitmap bitmap;
//    bitmap.GetInfos().infossize = 40;
//    bitmap.GetInfos().width = pcx->GetHeader().flwidth;
//    bitmap.GetInfos().height = pcx->GetHeader().flheight;
//    bitmap.GetInfos().planes = 1;
//    bitmap.GetInfos().bpp = 8;
//    bitmap.GetInfos().rlemode = 0;
//    bitmap.GetInfos().realsize = 0;
//    bitmap.GetInfos().xppm = 100;
//    bitmap.GetInfos().yppm = 100;
//    bitmap.GetInfos().palettesize = 256;
//    bitmap.GetInfos().nbimportantcolor = 0;
//
//    for (uint i = 0; i < 256; ++i)
//    {
//        bitmap_palette[i].b = palette[i].b;
//        bitmap_palette[i].g = palette[i].g;
//        bitmap_palette[i].r = palette[i].r;
//        bitmap_palette[i].a = 0;
//    }
//    bitmap.SetPalette(bitmap_palette);
//    bitmap.SetPixels(frame_datas);
//
//    const std::vector<byte> bitmap_bytes = bitmap.Create();
//
//    std::ofstream outfile("test.bmp", std::ios::out | std::ios::binary);
//    outfile.write((const char*)&bitmap_bytes[0], bitmap_bytes.size());
//}

int main(int argc, char **argv)
{
    std::locale::global(std::locale(""));

    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();

//    if (argc != 2)
//    {
//        std::cerr << "Missing the dump parameter" << std::endl;
//        return (EXIT_FAILURE);
//    }
//
//    HoMM3::Resource::Lod* lod = TEST_Lod("txt_content.lod");
//    HoMM3::Resource::Def* def = TEST_Def("sprite.def");
//    HoMM3::Resource::Pcx* pcx = TEST_Pcx("ab01_01.pcx");
//    if (std::string("true").compare(argv[1]) == 0)
//    {
//        std::cout << *lod << *def << *pcx << std::endl;
//    }
//    TEST_Bitmap(pcx, def);
//    TEST_ZCompression("This is a deflated string");
//    return (EXIT_SUCCESS);

}
