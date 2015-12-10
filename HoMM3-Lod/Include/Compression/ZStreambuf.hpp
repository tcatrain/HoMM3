#pragma once
#include <istream>
#include <ostream>
#include <streambuf>
#include <vector>
#include <zlib.h>
#include "Types.hpp"

namespace HoMM3
{
	namespace Compression
	{

		static size_t const ZSTREAMBUF_DEFAULT_BUFSIZE = 512;
		static int const ZSTREAMBUF_SYNC_FAILURE = -1;
		static int const ZSTREAMBUF_SYNC_SUCCESS = 0;
		
		/// <summary>Class ZipOStreambuf</summary>
		class ZipOStreambuf : public std::streambuf
		{
		private:
			z_stream zstream_;
			std::vector<byte> buf_;
			std::vector<byte> out_buf_;
			std::ostream ostream_;
			
		public:
			ZipOStreambuf(std::streambuf&, size_t);
			~ZipOStreambuf();

			int sync();
			int overflow(int);
			std::streamsize flush();

			bool zip_to_stream( char_type*, std::streamsize);
			size_t fill_input_buffer();
		};

		/// <summary>Class ZipIStreambuf</summary>
		class ZipIStreambuf : public std::streambuf
		{
		private:
			z_stream zstream_;
			std::vector<byte> buf_;
			std::istream istream_;
			
		public:
			ZipIStreambuf(std::streambuf&);
			~ZipIStreambuf();

			int_type underflow();
		};
	}
}