#pragma once
#include <vector>
#include <streambuf>
#include "zlib.h"
#include "Types.hpp"

namespace HoMM3
{
	namespace Compression
	{
		/// <summary>Class ZBuffer</summary>
		class ZBuffer : public std::streambuf
		{
		/*private:
			std::vector<byte> buf_;
			z_stream zs_;
				
		public:
			/// Inflate mode used for decompression within instance scope
			static int const ZFLATOR_INFLATE_MODE = 0;
			/// Deflate mode used for compression within instance scope
			static int const ZFLATOR_DEFLATE_MODE = 1;

			ZBuffer(int const);
			~ZBuffer();

			void Append(std::vector<byte> const&);
			std::vector<byte> Process();*/

		};
	}
}