#include "Image/Bitmap.hpp"
#include <iostream>

namespace HoMM3
{
    namespace Image
    {
        /// <summary>Prepare the bitmap header before creating the bitmap byte vector</summary>
        void Bitmap::PrepareHeader_()
        {
            /// The type of bitmap (should be 0x424D)
            this->header_.type = 0x4D42;
            /// The size of the all file is the sum of the header, infos, palette en bitmap actual content
            this->header_.size = sizeof(this->header_) + sizeof(this->infos_) + sizeof(BitmapColor) * this->infos_.palettesize + this->pixels_.size();
            /// The offset of the bitmap content itself should be just after the header, infos and the palette
            this->header_.offset = sizeof(this->header_) + sizeof(this->infos_) + sizeof(BitmapColor) * this->infos_.palettesize;
        }
        
        /// <summary>Constructor of the class HoMM3::Image::Bitmap</summary>
        Bitmap::Bitmap()
        {
            /// Nothing to do here
        }

        /// <summary>Getter for the bitmap infos</summary>
        /// <returns>The bitmap infos</returns>
        BitmapInfos& Bitmap::Infos()
        {
            return this->infos_;
        }

        /// <summary>Setter for the bitmap palette</summary>
        /// <param name="palette">The bitmap palette</returns>
        void Bitmap::Palette(BitmapColor *palette)
        {
            this->palette_ = palette;
        }

        /// <summary>Setter for the bitmap pixels</summary>
        /// <param name="pixels">The bitmap pixels</returns>
        void Bitmap::Pixels(const std::vector<byte>& pixels)
        {
            this->pixels_ = pixels;
        }

        /// <summary>Method used to generate the actual bitmap byte vector</summary>
        /// <returns>The bitmap byte vector</returns>
        const std::vector<byte> Bitmap::Create()
        {
            /// First of all prepare the bitmap header
            this->PrepareHeader_();
            
            byte *header_bytes = reinterpret_cast<byte *>(&this->header_);
            byte *infos_bytes = reinterpret_cast<byte *>(&this->infos_);
            
            std::vector<byte> bitmap_content;
            /// Push the bitmap header first into the vector
            bitmap_content.insert(bitmap_content.end(), header_bytes, header_bytes + sizeof(this->header_));
            /// Push the bitmap infos first into the vector
            bitmap_content.insert(bitmap_content.end(), infos_bytes, infos_bytes + sizeof(this->infos_));
            /// Push one by one each entry in the palette in reverse rgb
            for (uint i = 0; i < this->infos_.palettesize; ++i)
            {
                bitmap_content.push_back(this->palette_[i].b);
                bitmap_content.push_back(this->palette_[i].g);
                bitmap_content.push_back(this->palette_[i].r);
                bitmap_content.push_back(this->palette_[i].a);
            }
            /// Pixels are added from bottom line up to the first
            for (uint i = 1; i <= this->infos_.height; ++i)
            {
                /// Pixels within a line are added from left to right
                for (uint j = 0; j < this->infos_.width; ++j)
                {
                    bitmap_content.push_back(this->pixels_[(this->infos_.height - i) * this->infos_.width + j]);
                }
            }
            return (bitmap_content);
        }
    }
}
