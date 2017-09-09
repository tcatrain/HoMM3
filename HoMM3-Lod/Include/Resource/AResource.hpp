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
        private:
            /// <summary>Abstract Class AInternalReader</summary>
            class AInternalReader
            {
            private:
                /// The current position in the reader
                std::streamoff offset_;
                /// The way to seek when repositionning the current position
                std::ios_base::seekdir way_;
                
            protected:
                /// <summary>
                /// Constructor of the class HoMM3::Resource::AResource::AInternalReader.
                /// </summary>
                AInternalReader()
                {
                    this->offset_ = 0;
                    this->way_ = std::ios::beg;
                }
                
            public:
                /// <summary>Destructor of the class HoMM3::Resource::AResource::AInternalReader</summary>
                virtual ~AInternalReader()
                {
                }
                
                virtual bool IsReadable() = 0;
                
                /// <summary>Method used to read within the resource content</summary>
                /// <param name="buffer">The buffer where to store read content</param>
                /// <param name="length">The length of the data section to read</param>
                virtual void Read(char* buffer, size_t length) = 0;
                
                /// <summary>Method used to reposition the cursor within the content of the resource</summary>
                /// <param name="offset">The position where to relocate the cursor</param>
                /// <param name="way">The direction where to count the offset from</param>
                virtual void Seek(std::streamoff offset, std::ios_base::seekdir way)
                {
                    this->offset_ = offset;
                    this->way_ = way;
                }
                
                /// <summary>Method used to retrieve the current position within the resoure content</summary>
                /// <returns>The current position within the file from the begining</returns>
                std::streamoff Tell()
                {
                    return this->offset_;
                }
            };
            
            /// <summary>Class FileReader</summary>
            class FileReader : public AInternalReader
            {
            private:
                /// The input file stream of the resource file
                std::ifstream ifs_;
            public:
                /// <summary>Constructor of the class HoMM3::Resource::AResource::FileReader</summary>
                /// <param name="path">Path of the file to read from</param>
                FileReader(const std::string& path) : AInternalReader(), ifs_(path, std::ios::binary)
                {
                }
                
                /// <summary>Destructor of the class HoMM3::Resource::AResource::FileReader</summary>
                ~FileReader()
                {
                    this->ifs_.close();
                }
                
                virtual bool IsReadable() override
                {
                    return this->ifs_.is_open();
                }
                
                /// <summary>Method used to reposition the cursor within the content of the resource</summary>
                /// <param name="offset">The position where to relocate the cursor</param>
                /// <param name="way">The direction where to count the offset from</param>
                virtual void Seek(std::streamoff offset, std::ios_base::seekdir way) override
                {
                    AInternalReader::Seek(offset, way);
                    this->ifs_.seekg(offset, way);
                }
                
                /// <summary>Method used to read within the resource content</summary>
                /// <param name="buffer">The buffer where to store read content</param>
                /// <param name="length">The length of the data section to read</param>
                virtual void Read(char* buffer, size_t length) override
                {
                    this->ifs_.read(buffer, length);
                }
            };
            
            /// <summary>Class VectorReader</summary>
            class VectorReader : public AInternalReader
            {
            private:
                /// The content of the resource file
                std::vector<byte> bytes_;
            public:
                /// <summary>Constructor of the class HoMM3::Resource::AResource::VectorReader</summary>
                /// <param name="bytes">Content of the file to read from</param>
                VectorReader(const std::vector<byte>& bytes) : AInternalReader()
                {
                    this->bytes_ = bytes;
                }
                
                virtual bool IsReadable() override
                {
                    return (this->bytes_.size() != 0);
                }
                
                /// <summary>Method used to reposition the cursor within the content of the resource</summary>
                /// <param name="offset">The position where to relocate the cursor</param>
                /// <param name="way">The direction where to count the offset from</param>
                virtual void Seek(std::streamoff offset, std::ios_base::seekdir way) override
                {
                    switch (way)
                    {
                        case std::ios::cur:
                            offset += this->Tell();
                            break;
                        case std::ios::end:
                            offset = this->bytes_.size() - offset;
                            break;
                        default:
                            break;
                    }
                    AInternalReader::Seek(offset, std::ios::beg);
                }
                
                /// <summary>Method used to read within the resource content</summary>
                /// <param name="buffer">The buffer where to store read content</param>
                /// <param name="length">The length of the data section to read</param>
                virtual void Read(char* buffer, size_t length) override
                {
                    memcpy(buffer, this->bytes_.data() + this->Tell(), length);
                    this->Seek(this->Tell() + length, std::ios::beg);
                }
            };
            
        protected:
            /// The header of the Resource file
            THeader header_;
            /// The entries headers of the Resource file
            std::vector<std::unique_ptr<TEntryHeader>> entries_headers_;
            /// The state of the resource, loaded or not
            bool loaded_;
            /// The reader used to get Resource file segment
            AInternalReader* reader_;
            
            /// <summary>
            /// Constructor of the class HoMM3::Resource::AResource. Opens the input file stream.
            /// </summary>
            /// <param name="path">Path of the Resource file to load</param>
            AResource(const std::string& path)
            {
                this->loaded_ = false;
                this->reader_ = new FileReader(path);
            }
            
            /// <summary>
            /// Constructor of the class HoMM3::Resource::AResource. Uses the vector as input stream.
            /// </summary>
            /// <param name="bytes"The vector containing the bytes of the resource file</param>
            AResource(const std::vector<byte>& bytes)
            {
                this->loaded_ = false;
                this->reader_ = new VectorReader(bytes);
            }
            
            /// <summary>Destructor of the class HoMM3::Resource::AResource</summary>
            virtual ~AResource()
            {
                this->entries_headers_.clear();
                delete this->reader_;
            }
        
            /// <summary>Method used to load the header of the Resource file</summary>
            virtual void LoadHeader_()
            {
                this->reader_->Read(reinterpret_cast<char *>(&this->header_), sizeof(this->header_));
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
                if (this->reader_->IsReadable())
                {
                    this->LoadHeader_();
                    this->LoadEntriesHeaders_();
                    this->loaded_ = true;
                }
            }
            
            /// <summary>Method used to know if the resource has been already loaded or not</summary>
            /// <returns>The state of the resource, loaded or not</returns>
            bool IsLoaded()
            {
                return this->loaded_;
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
