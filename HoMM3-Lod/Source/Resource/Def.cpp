#include "Resource/Def.hpp"
#include "Compression/ZHelper.hpp"

namespace HoMM3
{
    namespace Resource
    {
        /// <summary>Outstream operator overload used for writing</summary>
        /// <param name="os">Output stream to write in by reference</param>
        /// <param name="resource">DEF resource object by reference</param>
        /// <returns>The outstream given in input</returns>
        void Def::Dump_(std::ostream& os) const
        {
            uint n = this->header_.nb;
            
            os << "resource.header_.type=" << this->header_.type << std::endl;
            os << "resource.header_.width=" << this->header_.width << std::endl;
            os << "resource.header_.height=" << this->header_.height << std::endl;
            os << "resource.header_.nb=" << this->header_.nb << std::endl;
            for (uint i = 0; i < Resource::def_h::DEFH_PAL_SIZE; ++i)
            { 
                os << "resource.header_.palette[" << i << "].r=" << (unsigned) this->header_.palette[i].r << std::endl;
                os << "resource.header_.palette[" << i << "].g=" << (unsigned) this->header_.palette[i].g << std::endl;
                os << "resource.header_.palette[" << i << "].b=" << (unsigned) this->header_.palette[i].b << std::endl;
            }
            for (uint i = 0; i < n; ++i)
            {
                os << "resource.entries_headers_[" << i << "].type=" << this->entries_headers_[i]->type << std::endl;
                os << "resource.entries_headers_[" << i << "].nb=" << this->entries_headers_[i]->nb << std::endl;
                for (uint j = 0; j < this->entries_headers_[i]->nb; ++j)
                {
                    os << "resource.entries_headers_[" << i << "].sequence_header[" << j << "].name=" << this->entries_headers_[i]->seq_entries_headers[j]->name << std::endl;
                    os << "resource.entries_headers_[" << i << "].sequence_header[" << j << "].offset=" << this->entries_headers_[i]->seq_entries_headers[j]->offset << std::endl;
                }
            }
        }
        
        /// <summary>Method used to load the entries headers of the DEF file</summary>
        void Def::LoadEntriesHeaders_()
        {
            uint nseq = this->header_.nb;
            
            for (uint i = 0; i < nseq; ++i)
            {
                std::unique_ptr<def_seqh> up_eh(new def_seqh());
                /// Read only the known fixed size of the header
                this->ifs_.read(reinterpret_cast<char*>(up_eh.get()), def_seqh::DEF_SEQH_FIXED_SIZE);
                /// The header should contain the number of frame in the sequence
                for (uint j = 0, nfrm = up_eh->nb; j < nfrm; ++j)
                {
                    std::unique_ptr<def_seqh::def_seqeh> up_seqeh(new def_seqh::def_seqeh());
                    if (j != 0)
                    {
                        /// If an offset was already read, the cursor must be repositionned to the next name to read
                        this->ifs_.seekg(
                            -sizeof(up_seqeh->offset) * j +
                            -sizeof(up_seqeh->name) * (nfrm - j), std::ios::cur);
                    }
                    /// Read the frame name
                    this->ifs_.read(reinterpret_cast<char*>(&up_seqeh->name), sizeof(up_seqeh->name));
                    /// Go forward to the next offset to read
                    this->ifs_.seekg(
                        sizeof(up_seqeh->name) * (nfrm - j - 1) +
                        sizeof(up_seqeh->offset) * j, std::ios::cur);
                    /// Read the frame offset
                    this->ifs_.read(reinterpret_cast<char*>(&up_seqeh->offset), sizeof(up_seqeh->offset));
                    up_eh->seq_entries_headers.push_back(std::move(up_seqeh));
                }
                this->entries_headers_.push_back(std::move(up_eh));
            }
        }
        
    	/// <summary>
        /// Constructor of the class HoMM3::Resource::Def. Opens the input file stream
        /// and parses the file to locate content.
        /// </summary>
        /// <param name="path">Path of the DEF file to load</param>
        Def::Def(const std::string& path) : AResource(path)
        {
        }
    	
        /// <summary>Destructor if the class HoMM3::Resource::Def</summary>
        Def::~Def()
        {
            uint n = this->header_.nb;
            
            for (uint i = 0; i < n; ++i)
                this->entries_headers_[i]->seq_entries_headers.clear();
            this->entries_headers_.clear();
            this->ifs_.close();
        }
        
        /// <summary>Method used to read an entry into the DEF file</summary>
        /// <param name="seqh">The sequence entry header structure to read</param>
        /// <returns>The byte vector containing the sequence</returns>
        const std::vector<byte> Def::ReadEntry(const def_seqh& seqh)
        {   
            std::vector<byte> entry;
            return entry;
        }
    }
}
