#include "Resource/Pcx.hpp"

namespace HoMM3
{
    namespace Resource
    {
        /// <summary>Outstream operator overload used for writing</summary>
        /// <param name="os">Output stream to write in by reference</param>
        /// <param name="resource">PCX resource object by reference</param>
        /// <returns>The outstream given in input</returns>
        void Pcx::Dump_(std::ostream& os) const
        {
            os << "pcx.header_.size=" << this->header_.size << std::endl;
            os << "pcx.header_.type=" << this->header_.type << std::endl;
            os << "pcx.header_.flwidth=" << this->header_.flwidth << std::endl;
            os << "pcx.header_.flheight=" << this->header_.flheight << std::endl;
            os << "pcx.header_.fmwidth=" << this->header_.fmwidth << std::endl;
            os << "pcx.header_.fmheight=" << this->header_.fmheight << std::endl;
            os << "pcx.header_.xmargin=" << this->header_.xmargin << std::endl;
            os << "pcx.header_.ymargin=" << this->header_.ymargin << std::endl;
        }
        
        /// <summary>Method used to load the header of the PCX file and set the compressor context</summary>
        void Pcx::LoadHeader_()
        {
            this->AResource::LoadHeader_();
            this->rlecompressor_.SetChunkSize(32);
            this->rlecompressor_.SetUnpackedSize(this->header_.fmheight * this->header_.fmwidth);
            this->rlecompressor_.SetUnpackedChunkSize(32);
        }
        
        /// <summary>Method used to load the entries headers of the LOD file</summary>
        void Pcx::LoadEntriesHeaders_()
        {
            /// Nothing to do here
        }
        
        Image::Bitmap& Pcx::PrepareBitmap_(const PcxHeader& ph, Image::Bitmap& bitmap) const
        {
            bitmap.GetInfos().width = ph.fmwidth;
            bitmap.GetInfos().height = ph.fmheight;
            //bitmap.GetInfos().palettesize
            return bitmap;
        }
        
        /// <summary>
        /// Constructor of the class HoMM3::Resource::Pcx. Opens the input file stream
        /// and parses the file to locate content.
        /// </summary>
        /// <param name="path">Path of the PCX file to load</param>
        Pcx::Pcx(const std::string& path) : AResource(path)
        {
            
        }
        
        /// <summary>Method used to read an entry from the PCX file</summary>
        /// <param name="eh">The frame header structure to read</param>
        /// <returns>The byte vector containing the bitmap frame</returns>
        const Image::Bitmap Pcx::ReadFrame(const PcxHeader& ph)
        {
            byte buf[ph.size];
            std::vector<byte> frame_content;
            Image::Bitmap frame_bitmap;
            
            this->PrepareBitmap_(ph, frame_bitmap);
            
            /// Don't forget to start after the header of the PCX file
            this->ifs_.seekg(sizeof(ph), this->ifs_.beg);
            this->ifs_.read(reinterpret_cast<char*>(&buf), sizeof(buf));
            
            /// Fill in the vector to be processed by the compressor
            frame_content = this->rlecompressor_.Deflate(std::vector<byte>(buf, buf + ph.size));
            return frame_bitmap;
        }
    }
}
