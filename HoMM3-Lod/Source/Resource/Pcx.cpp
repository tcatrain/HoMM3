#include "Resource/Pcx.hpp"

namespace HoMM3
{
    namespace Resource
    {
        /// <summary>Outstream operator overload used for writing</summary>
        /// <param name="os">Output stream to write in by reference</param>
        void Pcx::Dump_(std::ostream& os) const
        {
            os << "---PCX FILE---" << std::endl;
            os << "pcx.header_.size=" << this->header_.size << std::endl;
            os << "pcx.header_.type=" << this->header_.type << std::endl;
            os << "pcx.header_.flwidth=" << this->header_.flwidth << std::endl;
            os << "pcx.header_.flheight=" << this->header_.flheight << std::endl;
            os << "pcx.header_.fmwidth=" << this->header_.fmwidth << std::endl;
            os << "pcx.header_.fmheight=" << this->header_.fmheight << std::endl;
            os << "pcx.header_.xmargin=" << this->header_.xmargin << std::endl;
            os << "pcx.header_.ymargin=" << this->header_.ymargin << std::endl;
            os << "---PCX FILE END---" << std::endl << std::endl;
        }
        
        /// <summary>Method used to load the header of the PCX file and set the compressor context</summary>
        void Pcx::LoadHeader_()
        {
            AResource::LoadHeader_();
            this->rlecompressor_.ChunkSize(32);
            this->rlecompressor_.UnpackedSize(this->header_.fmheight * this->header_.fmwidth);
            this->rlecompressor_.UnpackedChunkSize(32);
        }
        
        /// <summary>Method used to load the entries headers of the LOD file</summary>
        void Pcx::LoadEntriesHeaders_()
        {
            /// Nothing to do here
        }
        
        /// <summary>Method used to mould the PCX entry file to full dimensions</summary>
        /// <param name="buffer">The buffer to work on containing the extracted PCX entry file</param>
        /// <returns>The provided vector updated</returns>
        std::vector<byte> Pcx::Mould_(std::vector<byte>& buffer)
        {
            /// Apply the margin bytes
            std::vector<byte> lxpad(this->header_.xmargin, 0);
            std::vector<byte> rxpad(this->header_.flwidth - this->header_.xmargin - this->header_.fmwidth, 0);
            std::vector<byte> typad(this->header_.ymargin * this->header_.flwidth, 0);
            std::vector<byte> bypad((this->header_.flheight - this->header_.ymargin - this->header_.fmheight) * this->header_.flwidth, 0);

            for (uint i = 0; i < this->header_.fmheight; ++i)
            {
                buffer.insert(buffer.begin() + i * this->header_.flwidth, lxpad.begin(), lxpad.end());
                buffer.insert(buffer.begin() + (i + 1) * this->header_.flwidth - rxpad.size(), rxpad.begin(), rxpad.end());

            }
            buffer.insert(buffer.begin(), typad.begin(), typad.end());
            buffer.insert(buffer.end(), bypad.begin(), bypad.end());
            return (buffer);
        }
        
        /// <summary>
        /// Constructor of the class HoMM3::Resource::Pcx. Opens the input file stream.
        /// </summary>
        /// <param name="path">Path of the PCX file to load</param>
        Pcx::Pcx(const std::string& path) : AResource(path)
        {
            this->Load();
        }
        
        /// <summary>
        /// Constructor of the class HoMM3::Resource::Pcx. Opens the input file stream.
        /// </summary>
        /// <param name="bytes">Content of the PCX file to load</param>
        Pcx::Pcx(const std::vector<byte>& bytes) : AResource(bytes)
        {
            this->Load();
        }
        
        /// <summary>Method used to read an entry from the PCX file</summary>
        /// <returns>The byte vector containing the bitmap frame</returns>
        std::vector<byte> Pcx::ReadFrame()
        {
            std::vector<byte> buffer(this->header_.size);
            if (this->loaded_)
            {
                /// Don't forget to start after the header of the PCX file
                this->reader_->Seek(sizeof(this->header_), std::ios::beg);
                this->reader_->Read(reinterpret_cast<char *>(&buffer), sizeof(buffer));
                buffer = this->rlecompressor_.Inflate(buffer);
                buffer = this->Mould_(buffer);
            }
            return (buffer);
        }
    }
}
