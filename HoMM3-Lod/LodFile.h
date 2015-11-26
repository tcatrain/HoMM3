﻿#pragma once
#include <iostream>
#include <fstream>

namespace Lod {

	/// Size of an unknown portion of the LOD header
	static const int LODH_UKWN_SIZE = 80;
	/// Size of an entry name in a LOD file
	static const int LODEH_NAME_SIZE = 16;
	
	typedef unsigned char byte;

	/// <summary>
	/// Structure s_lodh (lod header)
	/// 0x00				key of the file
	/// 0x04				type of lod file
	/// 0x08				number of file entries
	/// 0x0C				unknown byte array
	/// 0x5C				offset of file entries
	/// </summary>
	typedef struct		s_lod_h
	{
		/// Key of the LOD file
		unsigned int	key;
		/// Type of the LOD file
		unsigned int	type;
		/// Number of included file
		unsigned int	nb;
		/// Unknown portion of the LOD file (useless ?)
		unsigned char	ukwn[Lod::LODH_UKWN_SIZE];
	}					lod_h;
	
	/// <summary>
	/// Structure s_lodeh (lod entry header)
	/// 0x00				name of the entry
	/// 0x10				offset of the entry into the lod file
	/// 0x14				size of the decompressed file
	/// 0x18				type of the file
	/// 0x1C				size of the compressed file
	/// </summary>
	typedef struct		s_lod_eh
	{
		/// Name if the entry
		unsigned char	name[Lod::LODEH_NAME_SIZE];
		/// Offset of the entry into the LOD file
		unsigned int	offset;
		/// Size of the entry when compressed
		unsigned int	size;
		/// Type of entry
		unsigned int	type;
		/// Size of then entry when uncompressed
		unsigned int	zsize;
	}					lod_eh;

	//Class File
	class File
	{
	private:
		/// The header of the LOD file
		lod_h			header_;
		/// The entries headers of the LOD file
		lod_eh*			entries_headers_;
		/// The input file stream to the LOD file
		std::ifstream	ifs_;
		
		/// <summary>Method used to read the header of the LOD file</summary>
		void ReadHeader();
		/// <summary>Method used to read the entries headers of the LOD file</summary>
		void ReadEntriesHeaders();
		
	public:
		/// <summary>
		/// Constructor of the class Lod::File. Opens the input file stream
		/// and parses the file to locate content.
		/// </summary>
		/// <param name="path">Path of the LOD file to load</param>
		File(char*);

		/// <summary>
		/// Destructor if the class Lod::File.
		/// </summary>
		~File();

		/// <summary>Getter for LOD file header</summary>
		/// <returns>The LOD file header</returns>
		lod_h GetHeader();

		/// <summary>Getter for the LOD file entries headers</summary>
		/// <returns>The LOD file entries headers</returns>
		lod_eh* GetEntriesHeaders();

		///
		///
		byte* GetEntry(lod_eh&);

		/// Declaring friend operator <<
		friend std::ostream& operator <<(std::ostream&, const Lod::File&);
	};

	/// <summary>Outstream operator overload used for writing</summary>
	/// <param name="os">Output stream to write in by reference</param>
	/// <param name="file">LOD file object by reference</param>
	/// <returns>The outstream given in input</returns>
	std::ostream& operator <<(std::ostream&, const Lod::File&);
}