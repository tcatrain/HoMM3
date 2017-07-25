#include <sstream>

//#include "Image/Bitmap.hpp"
//#include "Resource/Def.hpp"
//#include "Resource/Pcx.hpp"
#include "gmock/gmock.h"

//void TEST_Bitmap(HoMM3::Resource::Pcx* pcx, const HoMM3::Resource::Def* def)
//{
//    const std::vector<byte> frame_datas = pcx->ReadFrame();
//    const HoMM3::Resource::DefColorIndex* palette = def->Header().palette;
//    HoMM3::Image::BitmapColor bitmap_palette[256];
//
//    HoMM3::Image::Bitmap bitmap;
//    bitmap.Infos().infossize = 40;
//    bitmap.Infos().width = pcx->Header().flwidth;
//    bitmap.Infos().height = pcx->Header().flheight;
//    bitmap.Infos().planes = 1;
//    bitmap.Infos().bpp = 8;
//    bitmap.Infos().rlemode = 0;
//    bitmap.Infos().realsize = 0;
//    bitmap.Infos().xppm = 100;
//    bitmap.Infos().yppm = 100;
//    bitmap.Infos().palettesize = 256;
//    bitmap.Infos().nbimportantcolor = 0;
//
//    for (uint i = 0; i < 256; ++i)
//    {
//        bitmap_palette[i].b = palette[i].b;
//        bitmap_palette[i].g = palette[i].g;
//        bitmap_palette[i].r = palette[i].r;
//        bitmap_palette[i].a = 0;
//    }
//    bitmap.Palette(bitmap_palette);
//    bitmap.Pixels(frame_datas);
//
//    const std::vector<byte> bitmap_bytes = bitmap.Create();
//
//    std::ofstream outfile("test.bmp", std::ios::out | std::ios::binary);
//    outfile.write((const char*)&bitmap_bytes[0], bitmap_bytes.size());
//}

int main(int argc, char **argv)
{
    std::locale::global(std::locale(""));

//    HoMM3::Resource::Def* def = new HoMM3::Resource::Def("sprite.def");
//    def->Load();
//    HoMM3::Resource::Pcx* pcx = new HoMM3::Resource::Pcx("ab01_01.pcx");
//    pcx->Load();
//    TEST_Bitmap(pcx, def);
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
