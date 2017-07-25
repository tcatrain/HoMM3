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
        /// <summary>Abstract Template Class AResource<Header, EntryHeader></summary>
        template <typename THeader, typename TEntryHeader>
        class AResource
        {
        protected:
            /// The header of the Resource file
            THeader header_;
            /// The entries headers of the Resource file
            std::vector<std::unique_ptr<TEntryHeader>> entries_headers_;
            /// The input file stream to the Resource file
            std::ifstream ifs_;
            /// The state of the resource, loaded or not
            bool loaded_;
        
            /// <summary>
            /// Constructor of the class HoMM3::Resource::AResource. Opens the input file stream.
            /// </summary>
            /// <param name="path">Path of the Resource file to load</param>
            AResource(const std::string& path) : ifs_(path, std::ios::binary)
            {
                this->loaded_ = false;
            }
            
            /// <summary>Destructor if the class HoMM3::Resource::AResource</summary>
            virtual ~AResource()
            {
                this->entries_headers_.clear();
                this->ifs_.close();
            }
        
            /// <summary>Method used to load the header of the Resource file</summary>
            virtual void LoadHeader_()
            {
                this->ifs_.read(reinterpret_cast<char *>(&this->header_), sizeof(this->header_));
            }
            
            /// <summary>Method used to load the entries headers of the Resource file</summary>
            virtual void LoadEntriesHeaders_() = 0;
            
            /// <summary>
            /// Virtual method used to dump the content of the AResource object.
            /// must be overriden in every derivated classes.
            /// </summary>
    		/// <param name="os">The output stream where to write the dump</param>
            virtual void Dump_(std::ostream&) const = 0;
            
        public:
            /// <summary>Method used to load the header and entries headers from the provided resource</summary>
            void Load()
            {
                this->LoadHeader_();
                this->LoadEntriesHeaders_();
                this->loaded_ = true;
            }
            
            /// <summary>Getter for Resource file header</summary>
            /// <returns>The Resource file header</returns>
            const THeader& Header() const
            {
                return (this->header_);
            }
            
            /// <summary>Getter for the Resource file entries headers</summary>
            /// <returns>The Resource file entries headers</returns>
            const std::vector<std::unique_ptr<TEntryHeader>>& EntriesHeaders() const
            {
                return (this->entries_headers_);
            }
            
            /// Declaring friend operator <<
            /// <summary>Outstream operator overload used for writing</summary>
            /// <param name="os">Output stream to write in by reference</param>
            /// <param name="resource">Resource object by reference</param>
            /// <returns>The outstream given in input</returns>
            friend std::ostream& operator <<(std::ostream& os, const AResource<THeader, TEntryHeader>& resource)
            {
                resource.Dump_(os);
                return (os);
            }
        };
    }
}
