#include "Resource/Def.hpp"
#include "Resource/Pcx.hpp"

namespace HoMM3
{
    namespace Resource
    {
        /// <summary>Outstream operator overload used for writing</summary>
        /// <param name="os">Output stream to write in by reference</param>
        void Def::Dump_(std::ostream& os) const
        {
            os << "---DEF FILE---" << std::endl;
            os << "def.header_.type=" << this->header_.type << std::endl;
            os << "def.header_.width=" << this->header_.width << std::endl;
            os << "def.header_.height=" << this->header_.height << std::endl;
            os << "def.header_.nb=" << this->header_.nb << std::endl;
            for (uint i = 0; i < Resource::DefHeader::DEFH_PAL_SIZE; ++i)
            { 
                os << "def.header_.palette[" << i << "].r=" << (unsigned) this->header_.palette[i].r << std::endl;
                os << "def.header_.palette[" << i << "].g=" << (unsigned) this->header_.palette[i].g << std::endl;
                os << "def.header_.palette[" << i << "].b=" << (unsigned) this->header_.palette[i].b << std::endl;
            }
            for (uint i = 0, n = this->header_.nb; i < n; ++i)
            {
                os << "def.entries_headers_[" << i << "].type=" << this->entries_headers_[i]->type << std::endl;
                os << "def.entries_headers_[" << i << "].nb=" << this->entries_headers_[i]->nb << std::endl;
                for (uint j = 0, m = this->entries_headers_[i]->nb; j < m; ++j)
                {
                    os << "def.entries_headers_[" << i << "].sequence_header[" << j << "].name=" << this->entries_headers_[i]->seq_frames[j]->name << std::endl;
                    os << "def.entries_headers_[" << i << "].sequence_header[" << j << "].offset=" << this->entries_headers_[i]->seq_frames[j]->offset << std::endl;
                }
            }
            os << "---DEF FILE END---" << std::endl << std::endl;
        }
        
        /// <summary>Method used to load the entries headers of the DEF file</summary>
        void Def::LoadEntriesHeaders_()
        {
            for (uint i = 0, nseq = this->header_.nb; i < nseq; ++i)
            {
                std::unique_ptr<DefSequenceHeader> up_eh(new DefSequenceHeader());
                /// Read only the known fixed size of the header
                this->reader_->Read(reinterpret_cast<char*>(&*up_eh), DefSequenceHeader::DEF_SEQH_FIXED_SIZE);
                /// The header should contain the number of frame in the sequence
                for (uint j = 0, nfrm = up_eh->nb; j < nfrm; ++j)
                {
                    std::unique_ptr<DefSequenceFrame> up_seqf(new DefSequenceFrame());
                    this->ReadNextName_(*up_seqf, j, nfrm);
                    this->ReadNextOffset_(*up_seqf, j, nfrm);
                    up_eh->seq_frames.push_back(std::move(up_seqf));
                }
                this->entries_headers_.push_back(std::move(up_eh));
            }
        }
        
        /// <summary>Reads the next frame name for the sequence</summary>
        /// <param name="sequence_entry_header">Current sequence entry header to get the name for</param>
        /// <param name="current">Current iteration of name extraction</param>
        /// <param name="outof">Total expected iteration count</param>
        void Def::ReadNextName_(DefSequenceFrame& sequence_entry_header, uint current, uint outof)
        {
            if (current != 0)
            {
                /// If an offset was already read, the cursor must be repositionned to the next name to read
                this->reader_->Seek(-sizeof(sequence_entry_header.offset) * current + -sizeof(sequence_entry_header.name) * (outof - current), std::ios::cur);
            }
            /// Read the frame name
            this->reader_->Read(reinterpret_cast<char *>(&sequence_entry_header.name), sizeof(sequence_entry_header.name));
        }
        
        /// <summary>Reads the next frame offset for the sequence</summary>
        /// <param name="sequence_entry_header">Current sequence entry header to get the offset for</param>
        /// <param name="current">Current iteration of offset extraction</param>
        /// <param name="outof">Total expected iteration count</param>
        void Def::ReadNextOffset_(DefSequenceFrame& sequence_entry_header, uint current, uint outof)
        {
            this->reader_->Seek(sizeof(sequence_entry_header.name) * (outof - current - 1) + sizeof(sequence_entry_header.offset) * current, std::ios::cur);
            /// Read the frame offset
            this->reader_->Read(reinterpret_cast<char *>(&sequence_entry_header.offset), sizeof(sequence_entry_header.offset));
        }
        
    	/// <summary>
        /// Constructor of the class HoMM3::Resource::Def. Opens the input file stream.
        /// </summary>
        /// <param name="path">Path of the DEF file to load</param>
        Def::Def(const std::string& path) : AResource(path)
        {
            this->Load();
        }
    	
        /// <summary>
        /// Constructor of the class HoMM3::Resource::Def. Uses the vector as input stream.
        /// </summary>
        /// <param name="bytes">Content of the DEF file to load</param>
        Def::Def(const std::vector<byte>& bytes) : AResource(bytes)
        {
            this->Load();
        }
        
        /// <summary>Destructor if the class HoMM3::Resource::Def</summary>
        Def::~Def()
        {
            for (uint i = 0, n = this->EntriesHeaders().size(); i < n; ++i)
            {
                this->entries_headers_[i]->seq_frames.clear();
            }
        }
        
        /// <summary>Method used to read a sequence frames from the DEF file</summary>
        /// <param name="seqeh">The sequence header structure to read</param>
        /// <returns>The vector containing the list of frame</returns>
        const std::vector<std::vector<byte>> Def::ReadSequence(const DefSequenceHeader& seqh)
        {
            std::vector<std::vector<byte>> frames;

            if (this->loaded_)
            {
                for (uint i = 0, n = seqh.nb, size; i < n; ++i)
                {
                    /// Position the cursor to the offset from the frame header
                    this->reader_->Seek(seqh.seq_frames[i]->offset, std::ios::beg);
                    /// Read the first uint containing the size of the frame
                    this->reader_->Read(reinterpret_cast<char *>(&size), sizeof(size));
                    /// Go back to the offset from the frame header
                    this->reader_->Seek(seqh.seq_frames[i]->offset, std::ios::beg);
                    /// The whole pcx contains the frame + the header
                    std::vector<byte> frame(size + sizeof(PcxHeader));
                    this->reader_->Read(reinterpret_cast<char *>(&frame[0]), frame.size());
                    frames.push_back(frame);
                }
            }
            return (frames);
        }
    }
}
