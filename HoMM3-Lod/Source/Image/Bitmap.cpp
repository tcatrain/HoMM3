#include "Image/Bitmap.hpp"

namespace HoMM3
{
    namespace Image
    {
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

        const std::vector<byte> Bitmap::Create() const
        {
            std::vector<byte> bitmap_content;
            return bitmap_content;
        }
    }
}
