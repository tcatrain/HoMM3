#include "Resource/Lod.hpp"
#include "Compression/ZHelper.hpp"
#include <iterator>

namespace HoMM3
{
	namespace Resource
	{
	#pragma region private:
		/// <summary>Method used to load the header of the LOD file</summary>
		void Lod::LoadHeader()
		{
			this->ifs_.read(reinterpret_cast<char*>(&this->header_), sizeof(this->header_));
		}

		/// <summary>Method used to load the entries headers of the LOD file</summary>
		void Lod::LoadEntriesHeaders()
		{
			int n = this->header_.nb;

			for (int i = 0; i < n; ++i)
			{
				std::unique_ptr<Lod::lod_eh> up_eh(new Lod::lod_eh());

				this->ifs_.read(reinterpret_cast<char*>(up_eh.get()), sizeof(*up_eh));
				this->entries_headers_.push_back(std::move(up_eh));
			}
		}
	#pragma endregion


	#pragma region public:
		/// <summary>
		/// Constructor of the class Lod::File. Opens the input file stream
		/// and parses the file to locate content.
		/// </summary>
		/// <param name="path">Path of the LOD file to load</param>
		Lod::Lod(std::string const& path) : ifs_(path, std::ios::binary)
		{
			this->LoadHeader();
			this->LoadEntriesHeaders();
		}

		/// <summary>
		/// Destructor if the class Lod::File.
		/// </summary>
		Lod::~Lod()
		{
			this->entries_headers_.clear();
			this->ifs_.close();
		}

		/// <summary>Getter for LOD file header</summary>
		/// <returns>The LOD file header</returns>
		Lod::lod_h const& Lod::GetHeader() const
		{
			return (this->header_);
		}

		/// <summary>Getter for the LOD file entries headers</summary>
		/// <returns>The LOD file entries headers</returns>
		std::vector<std::unique_ptr<Lod::lod_eh>> const& Lod::GetEntriesHeaders() const
		{
			return (this->entries_headers_);
		}

		/// <summary>Method used to read an entry into the load file</summary>
		/// <param name="eh">The entry header structure to read</param>
		/// <returns>The byte vector containing the entry</returns>
		std::vector<byte> const Lod::ReadEntry(Lod::lod_eh const& eh)
		{
			bool is_compressed(eh.zsize != 0);
			std::vector<byte> entry(is_compressed ? eh.zsize : eh.size);
			
			this->ifs_.seekg(eh.offset, this->ifs_.beg);
			this->ifs_.read(reinterpret_cast<char*>(&entry[0]), entry.size());
			return (is_compressed ? HoMM3::Compression::ZHelper::Inflate(entry) : entry);
		}

	#pragma endregion


	#pragma region friend:
		/// <summary>Outstream operator overload used for writing</summary>
		/// <param name="os">Output stream to write in by reference</param>
		/// <param name="file">LOD file object by reference</param>
		/// <returns>The outstream given in input</returns>
		std::ostream& operator <<(std::ostream& os, Lod const& file)
		{
			int n = file.header_.nb;

			os << "file.header_.key=" << file.header_.key << std::endl;
			os << "file.header_.type=" << file.header_.type << std::endl;
			os << "file.header_.nb=" << file.header_.nb << std::endl;
			for (int i = 0; i < n; ++i)
			{
				os << "file.entries_headers_.[" << i << "].name=" << file.entries_headers_[i]->name << std::endl;
				os << "file.entries_headers_.[" << i << "].offset=" << file.entries_headers_[i]->offset << std::endl;
				os << "file.entries_headers_.[" << i << "].size=" << file.entries_headers_[i]->size << std::endl;
				os << "file.entries_headers_.[" << i << "].type=" << file.entries_headers_[i]->type << std::endl;
				os << "file.entries_headers_.[" << i << "].zsize=" << file.entries_headers_[i]->zsize << std::endl;
			}
			return (os);
		}
	#pragma endregion
	}
}