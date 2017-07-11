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
        }
        
        /// <summary>Method used to load the entries headers of the DEF file</summary>
        void Def::LoadEntriesHeaders_()
        {
            uint n = this->header_.nb;
            for (uint i = 0; i < n; ++i)
            {
                std::cout << "OK 1" << std::endl;
                std::unique_ptr<def_seqh> up_eh(new def_seqh());
                this->ifs_.read(reinterpret_cast<char*>(up_eh.get()), sizeof(*up_eh));
                std::cout << "OK 2" << std::endl;
                for (uint j = 0; j < up_eh.get()->nb; ++j)
                {
                    std::unique_ptr<def_seq> up_seq(new def_seq());
                    std::cout << "OK 3" << std::endl;
                    if (j != 0)
                    {
                        this->ifs_.seekg(-sizeof(up_seq.get()->offset) * j, std::ios::cur);
                        this->ifs_.seekg(-sizeof(up_seq.get()->name) * (up_eh.get()->nb - j), std::ios::cur);
                    }
                    this->ifs_.read(reinterpret_cast<char*>(&up_seq.get()->name), sizeof(up_seq.get()->name));
                    this->ifs_.seekg(sizeof(up_seq.get()->name) * (up_eh.get()->nb - j - 1), std::ios::cur);
                    this->ifs_.seekg(sizeof(up_seq.get()->offset) * j, std::ios::cur);
                    this->ifs_.read(reinterpret_cast<char*>(&up_seq.get()->offset), sizeof(up_seq.get()->offset));
                    std::cout << "OK 4" << std::endl;
                    this->sequences_.push_back(std::move(up_seq));
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
            this->entries_headers_.clear();
            this->ifs_.close();
        }

        const std::vector<std::unique_ptr<def_seq>>& Def::GetSequences() const
        {
            return this->sequences_;
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
