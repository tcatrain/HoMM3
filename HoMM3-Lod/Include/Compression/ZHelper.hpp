#pragma once
#include <vector>
#include <streambuf>
#include "zlib.h"
#include "Types.hpp"

namespace HoMM3
{
	namespace Compression
	{
		/// <summary>Class ZHelper</summary>
		class ZHelper
		{
		private:
			/*std::streambuf osbuf_;
			z_stream zs_;
			
			void InitZStream();*/
		public:
			static const float INCREASE_RATIO;
			/*ZHelper(std::streambuf const&);
			~ZHelper();*/
			
			/// <summary>Method used to inflate a deflated byte vector</summary>
			/// <param name="in_bytes">The input deflated byte vector</param>
			/// <param name="out_size">The size of the output byte vector</param>
			/// <returns>The output inflated byte vector</returns>
			static std::vector<byte> const Inflate(std::vector<byte> const&);

			/// <summary>Method used to deflate an inflated byte vector</summary>
			/// <param name="in_bytes">The input inflated byte vector</param>
			/// <param name="out_size">The size of the output byte vector</param>
			/// <returns>The output deflated byte vector</returns>
			static std::vector<byte> const Deflate(std::vector<byte> const&);
		};
	}
}