#include "Image/Bitmap.hpp"
#include <iostream>

namespace HoMM3
{
    namespace Image
    {
        void Bitmap::PrepareHeader_()
        {
            /// The type of bitmap (should be 0x424D)
            this->header_.type = 0x4D42;
            this->header_.size = sizeof(this->header_) + sizeof(this->infos_) + sizeof(BitmapColor) * this->infos_.palettesize + this->pixels_.size();
            this->header_.offset = sizeof(this->header_) + sizeof(this->infos_) + sizeof(BitmapColor) * this->infos_.palettesize;
        }
        
        Bitmap::Bitmap()
        {

        }

        BitmapInfos& Bitmap::GetInfos()
        {
            return this->infos_;
        }

        void Bitmap::SetPalette(BitmapColor* palette)
        {
            this->palette_ = palette;
        }

        void Bitmap::SetPixels(const std::vector<byte>& pixels)
        {
            this->pixels_ = pixels;
        }

        const std::vector<byte> Bitmap::Create()
        {
            this->PrepareHeader_();
            std::vector<byte> bitmap_content;
            
            byte* header_bytes = reinterpret_cast<byte*>(&this->header_);
            byte* infos_bytes = reinterpret_cast<byte*>(&this->infos_);
            
            bitmap_content.insert(bitmap_content.end(), header_bytes, header_bytes + sizeof(this->header_));
            bitmap_content.insert(bitmap_content.end(), infos_bytes, infos_bytes + sizeof(this->infos_));
            for (uint i = 0; i < this->infos_.palettesize; ++i)
            {
                bitmap_content.push_back(this->palette_[i].b);
                bitmap_content.push_back(this->palette_[i].g);
                bitmap_content.push_back(this->palette_[i].r);
                bitmap_content.push_back(this->palette_[i].a);
            }
            for (uint i = 1; i <= this->infos_.height; ++i)
            {
                for (uint j = 0; j < this->infos_.width; ++j)
                {
                    bitmap_content.push_back(this->pixels_[(this->infos_.height - i) * this->infos_.width + j]);
                }
            }
            return bitmap_content;
        }
    }
}