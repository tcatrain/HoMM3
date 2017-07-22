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
        std::vector<byte>& Pcx::Mould_(std::vector<byte>& buffer)
        {
            /// Apply the margin bytes
            byte lxpad[this->header_.xmargin];
            byte rxpad[this->header_.flwidth - this->header_.xmargin - this->header_.fmwidth];
            byte typad[this->header_.ymargin * this->header_.flwidth];
            byte bypad[(this->header_.flheight - this->header_.ymargin - this->header_.fmheight) * this->header_.flwidth];
            
            std::fill_n(lxpad, sizeof(lxpad), 0);
            std::fill_n(rxpad, sizeof(rxpad), 0);
            std::fill_n(typad, sizeof(typad), 0);
            std::fill_n(bypad, sizeof(bypad), 0);

            for (uint i = 0; i < this->header_.fmheight; ++i) {
                buffer.insert(buffer.begin() + i * this->header_.flwidth, lxpad, lxpad + sizeof(lxpad));
                buffer.insert(buffer.begin() + (i + 1) * this->header_.flwidth - sizeof(rxpad), rxpad, rxpad + sizeof(rxpad));
            }
            buffer.insert(buffer.begin(), typad, typad + sizeof(typad));
            buffer.insert(buffer.end(), bypad, bypad + sizeof(bypad));
            return buffer;
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
        /// <returns>The byte vector containing the bitmap frame</returns>
        std::vector<byte>& Pcx::ReadFrame()
        {
            byte buf[this->header_.size];

            /// Don't forget to start after the header of the PCX file
            this->ifs_.seekg(sizeof(this->header_), this->ifs_.beg);
            this->ifs_.read(reinterpret_cast<char*>(&buf), sizeof(buf));
            /// Fill in the vector to be processed by the compressor
            std::vector<byte>* buffer = new std::vector<byte>(buf, buf + this->header_.size);
            return this->Mould_(this->rlecompressor_.Deflate(*buffer));
        }
    }
}
