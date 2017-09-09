#include "Resource/Lod.hpp"

namespace HoMM3
{
    namespace Resource
    {
        /// <summary>Outstream operator overload used for writing</summary>
        /// <param name="os">Output stream to write in by reference</param>
        void Lod::Dump_(std::ostream& os) const
        {
            uint n = this->header_.nb;

            os << "---LOD FILE---" << std::endl;
            os << "lod.header_.key=" << this->header_.key << std::endl;
            os << "lod.header_.type=" << this->header_.type << std::endl;
            os << "lod.header_.nb=" << this->header_.nb << std::endl;
            for (uint i = 0; i < n; ++i)
            {
                os << "lod.entries_headers_.[" << i << "].name=" << this->entries_headers_[i]->name << std::endl;
                os << "lod.entries_headers_.[" << i << "].offset=" << this->entries_headers_[i]->offset << std::endl;
                os << "lod.entries_headers_.[" << i << "].size=" << this->entries_headers_[i]->size << std::endl;
                os << "lod.entries_headers_.[" << i << "].type=" << this->entries_headers_[i]->type << std::endl;
                os << "lod.entries_headers_.[" << i << "].zsize=" << this->entries_headers_[i]->zsize << std::endl;
            }
            os << "---LOD FILE END---" << std::endl << std::endl;
        }
        
        /// <summary>Method used to load the entries headers of the LOD file</summary>
        void Lod::LoadEntriesHeaders_()
        {
            for (int i = 0, n = this->header_.nb; i < n; ++i)
            {
                std::unique_ptr<LodEntryHeader> up_eh(new LodEntryHeader());
                this->reader_->Read(reinterpret_cast<char *>(&*up_eh), sizeof(*up_eh));
                this->entries_headers_.push_back(std::move(up_eh));
            }
        }
        
    	/// <summary>
        /// Constructor of the class HoMM3::Resource::Lod. Opens the input file stream.
        /// </summary>
        /// <param name="path">Path of the LOD file to load</param>
        Lod::Lod(const std::string& path) : AResource(path)
        {
            this->Load();
        }

        /// <summary>
        /// Constructor of the class HoMM3::Resource::Lod. Opens the input file stream.
        /// </summary>
        /// <param name="bytes">Content of the LOD file to load</param>
        Lod::Lod(const std::vector<byte>& bytes) : AResource(bytes)
        {
            this->Load();
        }
        
        /// <summary>Method used to read an entry into the LOD file</summary>
        /// <param name="eh">The entry header structure to read</param>
        /// <returns>The byte vector containing the entry</returns>
        const std::vector<byte> Lod::ReadEntry(const LodEntryHeader& eh)
        {
            bool is_compressed(eh.zsize != 0);
            std::vector<byte> buffer(is_compressed ? eh.zsize : eh.size), entry;

            if (this->loaded_)
            {
                this->reader_->Seek(eh.offset, std::ios::beg);
                this->reader_->Read(reinterpret_cast<char *>(entry.data()), entry.size());
                entry = is_compressed ? this->zcompressor_.Inflate(buffer) : buffer;
            }
            return (entry);
        }
    }
}
