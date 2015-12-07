#pragma once
#include <vector>
#include <streambuf>
#include "zlib.h"
#include "Types.hpp"

namespace HoMM3
{
	namespace Compression
	{
		/// <summary>Class ZipStreambuf</summary>
		class ZipStreambuf : public std::basic_streambuf
		{
		private:
			
			
		public:
			ZipStreambuf(std::basic_ostream&);
			~ZipStreambuf();
		};
		
		/// <summary>Class UnzipStreambuf</summary>
		class UnzipStreambuf : public std::basic_streambuf
		{
		private:
			
			
		public:
			UnzipStreambuf();
			~UnzipStreambuf();
		};
	}
}