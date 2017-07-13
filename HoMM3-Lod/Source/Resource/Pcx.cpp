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
            os << "pcx.header_.width=" << this->header_.width << std::endl;
            os << "pcx.header_.height=" << this->header_.height << std::endl;
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
        
        /// <summary>Method used to read a frame into the PCX file</summary>
        /// <param name="ph">The pcx header structure to read</param>
        /// <returns>The byte vector containing the frame</returns>
        const std::vector<byte> Pcx::ReadFrame(const PcxHeader& ph)
        {
            std::cout << ph.fmheight << std::endl;
            return std::vector<byte>();
        }
    }
}
