#pragma once

#include <vector>

#include "Types.hpp"

namespace HoMM3
{
    namespace Image
    {
        #pragma pack(push, 1)
        /// <summary>
        /// Structure BitmapHeader
        /// 0x00 type of bitmap
        /// 0x02 size of the bitmap file
        /// 0x06 reserved section
        /// 0x0A offset of the pixel array
        /// </summary>
        struct HOMM3_LOD_LIB BitmapHeader
        {
            /// The type of bitmap (should be 0x424D)
            usint type;
            /// The size of the whole bitmap file
            uint size;
            /// The reserved section in the header
            uint reserved;
            /// The offset of the pixel array itself within the file
            uint offset;
        };
        #pragma pack(pop)

        #pragma pack(push, 1)
        /// <summary>
        /// Structure BitmapInfos
        /// 0x00 size of the pixel array
        /// 0x04 width of the bitmap
        /// 0x08 height of the bitmap
        /// 0x0A number of planes (1)
        /// 0x0C bits per pixel
        ///     1 is monochrome
        ///     4 is 16 color bmp with 16 color palette
        ///     8 is 256 color bmp with 256 color palette
        ///     18 is 16 777 216 color bmp without palette
        /// 0x0E rle compression
        ///     0 is not compressed
        ///     1 is RLE 8 bits per pixel
        ///     2 is RLE 4 bits per pixel
        /// 0x12 uncompressed size (0 if not compressed)
        /// 0x16 horizontal resolution (pixel per meter)
        /// 0x1A vertical resolution (pixel per meter)
        /// 0x1E number of colors in the palette
        /// 0x22 number of important colors in the palette (0 if all)
        /// </summary>
        struct HOMM3_LOD_LIB BitmapInfos
        {
            /// The size of the pixel array
            uint infossize;
            /// The width of the bitmap
            uint width;
            /// The height of the bitmap
            uint height;
            /// The number of planes (should be 1)
            usint planes;
            /// The number of bits per pixel
            usint bpp;
            /// The RLE compression to use
            uint rlemode;
            /// The real size of the bitmap once uncompressed
            uint realsize;
            /// The horizontal resolution in pixels per meter
            uint xppm;
            /// The vertical resolution in pixels per meter
            uint yppm;
            /// The number of colors in the palette
            uint palettesize;
            /// The number of important color in the palette
            uint nbimportantcolor;
        };
        #pragma pack(pop)

        #pragma pack(push, 1)
        /// <summary>
        /// Structure BitmapColor
        /// 0x00 blue value
        /// 0x01 green value
        /// 0x02 red value
        /// 0x03 unused alpha value
        /// </summary>
        struct HOMM3_LOD_LIB BitmapColor
        {
            /// The blue value of the color
            byte b;
            /// The green value of the color
            byte g;
            /// The red value of the color
            byte r;
            /// The unused alpha value of the color
            byte a;
        };
        #pragma pack(pop)

        class HOMM3_LOD_LIB Bitmap
        {
        private:
            /// The header of the bitmap
            BitmapHeader header_;
            /// The infos of the bitmap
            BitmapInfos infos_;
            /// The color palette of the bitmap
            BitmapColor* palette_;
            /// Pixels of the bitmap
            std::vector<byte> pixels_;
            
            /// <summary>Prepare the bitmap header before creating the bitmap byte vector</summary>
            void PrepareHeader_();
            
        public:
            /// <summary>Constructor of the class HoMM3::Image::Bitmap</summary>
            Bitmap();
            
            /// <summary>Getter for the bitmap infos</summary>
            /// <returns>The bitmap infos</returns>
            BitmapInfos& Infos();
            
            /// <summary>Setter for the bitmap palette</summary>
            /// <param name="palette">The bitmap palette</param>
            void Palette(BitmapColor *);
            
            /// <summary>Setter for the bitmap pixels</summary>
            /// <param name="pixels">The bitmap pixels</param>
            void Pixels(const std::vector<byte>&);
            
            /// <summary>Method used to generate the actual bitmap byte vector</summary>
            /// <returns>The bitmap byte vector</returns>
            const std::vector<byte>& Create();
        };
    }
}
