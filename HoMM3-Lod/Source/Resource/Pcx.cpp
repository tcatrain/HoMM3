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
        
        /// <summary>Method used to load the entries headers of the LOD file</summary>
        void Pcx::LoadEntriesHeaders_()
        {
            /// Nothing to do here
        }
        
        Pcx::Pcx(const std::string& path) : AResource(path)
        {
            
        }
        
        /// <summary>Method used to read an entry from the PCX file</summary>
        /// <param name="eh">The frame header structure to read</param>
        /// <returns>The byte vector containing the bitmap frame</returns>
        const std::vector<byte> Pcx::ReadFrame(const PcxHeader& ph)
        {
            std::vector<byte> pixels;
            byte nextbyte;
            uint length;
            
            for (uint i = 0; i < ph.fmheight; ++i)
            {
                usint lnoffsets[ph.fmwidth / 32];
                this->ifs_.read(reinterpret_cast<char*>(&lnoffsets), sizeof(lnoffsets));
                for (uint j = 0, rowlength = 32; j < ph.fmwidth / 32; ++j)
                {
                    this->ifs_.seekg(lnoffsets[j] + sizeof(ph), this->ifs_.beg);
                    do
                    {
                        this->ifs_.read(reinterpret_cast<char*>(&nextbyte), sizeof(nextbyte));
                        length = (nextbyte & 0x1F) + 1;
                        byte buf[length];
                        
                        if ((nextbyte & 0xE0) == 0xE0)
                        {
                            this->ifs_.read(reinterpret_cast<char*>(&buf), sizeof(buf));
                        }
                        else
                        {
                            for (uint k = 0; k < length; ++k)
                            {
                                buf[k] = nextbyte & 0xE0;
                            }
                        }
                        pixels.insert(pixels.cend(), buf, buf + length);
                        rowlength += length;
                    } while(rowlength < 32);
                    rowlength = 0;
                }
                this->ifs_.seekg(sizeof(ph) + (i + 1) * sizeof(lnoffsets), this->ifs_.beg);
            }
            return pixels;
        }
    }
}
