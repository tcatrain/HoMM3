#include "pch.h"
#include "LodFile.h"
#include <fstream>
#include "zlib.h"

/// <summary>
/// Constructor of the class Lod::File. Opens the input file stream
/// and parses the file to locate content.
/// </summary>
/// <param name="path">Path of the LOD file to load</param>
Lod::File::File(char *path) : ifs_(path, std::ios::binary)
{
	ReadHeader();
	ReadEntriesHeaders();
}

/// <summary>
/// Destructor if the class Lod::File.
/// </summary>
Lod::File::~File()
{
	delete[] this->entries_headers_;
	this->ifs_.close();
}

/// <summary>Method used to read the header of the LOD file</summary>
/// <param name="ifs">The input stream object by reference</param>
void Lod::File::ReadHeader()
{
	this->ifs_.read((char*)&this->header_.key, sizeof(unsigned int));
	this->ifs_.read((char*)&this->header_.type, sizeof(unsigned int));
	this->ifs_.read((char*)&this->header_.nb, sizeof(unsigned int));
	this->ifs_.read((char*)&this->header_.ukwn, sizeof(byte) * Lod::LODH_UKWN_SIZE);
}

/// <summary>Method used to read the entry headers of the LOD file</summary>
/// <param name="ifs">The input stream object by reference</param>
void Lod::File::ReadEntriesHeaders()
{
	int n = this->header_.nb;
	
	this->entries_headers_ = new Lod::lod_eh[n];
	for (int i = 0; i < n; ++i)
	{
		this->ifs_.read((char*)&this->entries_headers_[i].name, sizeof(byte) * Lod::LODEH_NAME_SIZE);
		this->ifs_.read((char*)&this->entries_headers_[i].offset, sizeof(unsigned int));
		this->ifs_.read((char*)&this->entries_headers_[i].size, sizeof(unsigned int));
		this->ifs_.read((char*)&this->entries_headers_[i].type, sizeof(unsigned int));
		this->ifs_.read((char*)&this->entries_headers_[i].zsize, sizeof(unsigned int));
	}
}

Lod::byte* Lod::File::GetEntry(Lod::lod_eh& eh)
{
	this->ifs_.seekg(eh.offset, this->ifs_.beg);
	byte* entry = new byte[eh.size + 1];
	
	if (eh.zsize != 0) {
		byte* zentry = new byte[eh.zsize + 1];
		this->ifs_.read((char*)zentry, eh.zsize);
		zentry[eh.zsize] = '\0';

		z_stream defstream;
		
	    defstream.zalloc = Z_NULL;
	    defstream.zfree = Z_NULL;
	    defstream.opaque = Z_NULL;
	    defstream.avail_in = eh.zsize;
	    defstream.next_in = (Bytef*)zentry;
	    defstream.avail_out = eh.size;
	    defstream.next_out = (Bytef*)entry;
	    
	    inflateInit(&defstream);
	    inflate(&defstream, Z_NO_FLUSH);
	    inflateEnd(&defstream);

		delete[] zentry;
	} else {
		this->ifs_.read((char*)entry, eh.size);
	}
	entry[eh.size] = '\0';
	return (entry);
}

/// <summary>Getter for LOD file header</summary>
/// <returns>The LOD file header</returns>
Lod::lod_h Lod::File::GetHeader()
{
	return (this->header_);
}

/// <summary>Getter for the LOD file entries headers</summary>
		/// <returns>The LOD file entries headers</returns>
Lod::lod_eh* Lod::File::GetEntriesHeaders()
{
	return (this->entries_headers_);
}

/// <summary>Outstream operator overload used for writing</summary>
/// <param name="os">Output stream to write in by reference</param>
/// <param name="file">LOD file object by reference</param>
/// <returns>The outstream given in input</returns>
std::ostream& Lod::operator <<(std::ostream& os, const Lod::File& file)
{
	int n = file.header_.nb;
	
	os << "file.header_.key=" << file.header_.key << std::endl;
	os << "file.header_.type=" << file.header_.type << std::endl;
	os << "file.header_.nb=" << file.header_.nb << std::endl;
	for (int i = 0; i < n; ++i)
	{
		os << "file.entries_headers_.[" << i << "].name=" << file.entries_headers_[i].name << std::endl;
		os << "file.entries_headers_.[" << i << "].offset=" << file.entries_headers_[i].offset << std::endl;
		os << "file.entries_headers_.[" << i << "].size=" << file.entries_headers_[i].size << std::endl;
		os << "file.entries_headers_.[" << i << "].type=" << file.entries_headers_[i].type << std::endl;
		os << "file.entries_headers_.[" << i << "].zsize=" << file.entries_headers_[i].zsize << std::endl;
	}
	return (os);
}