#include "pch.h"
#include "LodFile.h"
#include "zlib.h"
#include <fstream>

/// <summary>
/// Constructor of the class Lod::File. Opens the input file stream
/// and parses the file to locate content.
/// </summary>
/// <param name="path">Path of the LOD file to load</param>
Lod::File::File(const char *path) : ifs_(path, std::ios::binary)
{
	this->ReadHeader();
	this->ReadEntriesHeaders();
}

/// <summary>
/// Destructor if the class Lod::File.
/// </summary>
Lod::File::~File()
{
	this->entries_headers_.clear();
	this->ifs_.close();
}

/// <summary>Method used to read the header of the LOD file</summary>
/// <param name="ifs">The input stream object by reference</param>
void Lod::File::ReadHeader()
{
	this->ifs_.read((char*)&this->header_, sizeof(this->header_));
}

/// <summary>Method used to read the entry headers of the LOD file</summary>
/// <param name="ifs">The input stream object by reference</param>
void Lod::File::ReadEntriesHeaders()
{
	int n = this->header_.nb;
	
	for (int i = 0; i < n; ++i)
	{
		std::unique_ptr<Lod::lod_eh> up_eh(new Lod::lod_eh());

		this->ifs_.read((char*)&*up_eh, sizeof(*up_eh));
		this->entries_headers_.push_back(std::move(up_eh));
	}
}

/// 
/// 
/// 
byte* Lod::File::GetEntry(const std::unique_ptr<Lod::lod_eh>&& eh)
{
	this->ifs_.seekg(eh->offset, this->ifs_.beg);
	byte* entry = new byte[eh->size + 1];
	
	if (eh->zsize != 0) {
		byte* zentry = new byte[eh->zsize + 1];
		this->ifs_.read((char*)zentry, eh->zsize);
		zentry[eh->zsize] = '\0';

		z_stream defstream;
		
	    defstream.zalloc = Z_NULL;
	    defstream.zfree = Z_NULL;
	    defstream.opaque = Z_NULL;
	    defstream.avail_in = eh->zsize;
	    defstream.next_in = (Bytef*)zentry;
	    defstream.avail_out = eh->size;
	    defstream.next_out = (Bytef*)entry;
	    
	    inflateInit(&defstream);
	    inflate(&defstream, Z_NO_FLUSH);
	    inflateEnd(&defstream);

		delete[] zentry;
	} else {
		this->ifs_.read((char*)entry, eh->size);
	}
	entry[eh->size] = '\0';
	return (entry);
}

/// <summary>Getter for LOD file header</summary>
/// <returns>The LOD file header</returns>
Lod::lod_h const& Lod::File::GetHeader() const
{
	return (this->header_);
}

/// <summary>Getter for the LOD file entries headers</summary>
/// <returns>The LOD file entries headers</returns>
std::vector<std::unique_ptr<Lod::lod_eh>> const& Lod::File::GetEntriesHeaders() const
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
		os << "file.entries_headers_.[" << i << "].name=" << file.entries_headers_.at(i)->name << std::endl;
		os << "file.entries_headers_.[" << i << "].offset=" << file.entries_headers_.at(i)->offset << std::endl;
		os << "file.entries_headers_.[" << i << "].size=" << file.entries_headers_.at(i)->size << std::endl;
		os << "file.entries_headers_.[" << i << "].type=" << file.entries_headers_.at(i)->type << std::endl;
		os << "file.entries_headers_.[" << i << "].zsize=" << file.entries_headers_.at(i)->zsize << std::endl;
	}
	return (os);
}