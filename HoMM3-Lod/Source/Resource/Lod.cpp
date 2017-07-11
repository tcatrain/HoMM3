#include "Resource/Lod.hpp"
#include "Compression/ZHelper.hpp"

namespace HoMM3
{
    namespace Resource
    {
        /// <summary>Outstream operator overload used for writing</summary>
        /// <param name="os">Output stream to write in by reference</param>
        /// <param name="resource">LOD resource object by reference</param>
        /// <returns>The outstream given in input</returns>
        void Lod::Dump_(std::ostream& os) const
        {
            int n = this->header_.nb;

            os << "resource.header_.key=" << this->header_.key << std::endl;
            os << "resource.header_.type=" << this->header_.type << std::endl;
            os << "resource.header_.nb=" << this->header_.nb << std::endl;
            for (int i = 0; i < n; ++i)
            {
                os << "resource.entries_headers_.[" << i << "].name=" << this->entries_headers_[i]->name << std::endl;
                os << "resource.entries_headers_.[" << i << "].offset=" << this->entries_headers_[i]->offset << std::endl;
                os << "resource.entries_headers_.[" << i << "].size=" << this->entries_headers_[i]->size << std::endl;
                os << "resource.entries_headers_.[" << i << "].type=" << this->entries_headers_[i]->type << std::endl;
                os << "resource.entries_headers_.[" << i << "].zsize=" << this->entries_headers_[i]->zsize << std::endl;
            }
        }
        
    	/// <summary>
        /// Constructor of the class HoMM3::Resource::Lod. Opens the input file stream
        /// and parses the file to locate content.
        /// </summary>
        /// <param name="path">Path of the LOD file to load</param>
        Lod::Lod(const std::string& path) : AResource(path)
        {
        }
    	
        /// <summary>Destructor if the class HoMM3::Resource::Lod</summary>
        Lod::~Lod()
        {
            this->entries_headers_.clear();
            this->ifs_.close();
        }

        /// <summary>Method used to read an entry into the LOD file</summary>
        /// <param name="eh">The entry header structure to read</param>
        /// <returns>The byte vector containing the entry</returns>
        const std::vector<byte> Lod::ReadEntry(const lod_eh& eh)
        {
            bool is_compressed(eh.zsize != 0);
            std::vector<byte> entry(is_compressed ? eh.zsize : eh.size);
            
            this->ifs_.seekg(eh.offset, this->ifs_.beg);
            this->ifs_.read(reinterpret_cast<char*>(&entry[0]), entry.size());
            return (is_compressed ? HoMM3::Compression::ZHelper::Inflate(entry) : entry);
        }
    }
}
