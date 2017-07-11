#pragma once
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "Types.hpp"

namespace HoMM3
{
    namespace Resource
    {
        template <typename Header, typename EntryHeader>
        class AResource
        {
        protected:
            /// The header of the Resource file
            Header header_;
            /// The entries headers of the Resource file
            std::vector<std::unique_ptr<EntryHeader>> entries_headers_;
            /// The input file stream to the Resource file
            std::ifstream ifs_;
        
            /// <summary>
            /// Constructor of the class HoMM3::Resource::AResource. Opens the input file stream
            /// and parses the file to locate content.
            /// </summary>
            /// <param name="path">Path of the Resource file to load</param>
            AResource(const std::string& path) : ifs_(path, std::ios::binary)
            {
                this->LoadHeader_();
                this->LoadEntriesHeaders_();
            }
            
            /// <summary>Destructor if the class HoMM3::Resource::AResource</summary>
            ~AResource()
            {
                this->entries_headers_.clear();
                this->ifs_.close();
            }
        
            /// <summary>Method used to load the header of the Resource file</summary>
            void LoadHeader_()
            {
                this->ifs_.read(reinterpret_cast<char*>(&this->header_), sizeof(this->header_));
            }
            
            /// <summary>Method used to load the entries headers of the Resource file</summary>
            void LoadEntriesHeaders_()
            {
                int n = this->header_.nb;
    
                for (int i = 0; i < n; ++i)
                {
                    std::unique_ptr<EntryHeader> up_eh(new EntryHeader());
    
                    this->ifs_.read(reinterpret_cast<char*>(up_eh.get()), sizeof(*up_eh));
                    this->entries_headers_.push_back(std::move(up_eh));
                }
            }
            
            /// <summary>
            /// Virtual method used to dump the content of the AResource object.
            /// must be overriden in every derivated classes.
            /// </summary>
    		/// <param name="os">The output stream where to write the dump</param>
            virtual void Dump_(std::ostream&) const = 0;
            
        public:
            /// <summary>Getter for Resource file header</summary>
            /// <returns>The Resource file header</returns>
            const Header& GetHeader() const
            {
                return (this->header_);
            }
            
            /// <summary>Getter for the Resource file entries headers</summary>
            /// <returns>The Resource file entries headers</returns>
            const std::vector<std::unique_ptr<EntryHeader>>& GetEntriesHeaders() const
            {
                return (this->entries_headers_);
            }
            
            /// Declaring friend operator <<
            /// <summary>Outstream operator overload used for writing</summary>
            /// <param name="os">Output stream to write in by reference</param>
            /// <param name="resource">Resource object by reference</param>
            /// <returns>The outstream given in input</returns>
            friend std::ostream& operator <<(std::ostream& os, const AResource<Header, EntryHeader>& resource)
            {
                resource.Dump_(os);
                return (os);
            }
            
            /// <summary>Method used to read an entry from the Resource file</summary>
            /// <param name="eh">The entry header structure to read</param>
            /// <returns>The byte vector containing the entry</returns>
            virtual const std::vector<byte> ReadEntry(EntryHeader const&) = 0;
        };
    }
}