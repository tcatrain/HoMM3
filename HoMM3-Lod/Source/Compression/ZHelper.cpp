#include "Compression/ZHelper.hpp"
#include <iostream>

namespace HoMM3
{
	namespace Compression
	{
	#pragma region private:
		/// Size increase ratio of the buffer to inflate in
		const float ZHelper::INCREASE_RATIO = 1.1f;
	#pragma endregion

	#pragma region public:
		/// <summary>Method used to inflate a deflated byte vector</summary>
		/// <param name="in_bytes">The input deflated byte vector</param>
		/// <returns>The output inflated byte vector</returns>
		std::vector<byte> const ZHelper::Inflate(std::vector<byte> const& in_bytes)
		{
			std::vector<byte> out_bytes;
			uLongf out_size = static_cast<uLongf>(in_bytes.size());
			int status = Z_BUF_ERROR;
			
			do
			{
				out_size = static_cast<uLongf>(out_size * INCREASE_RATIO);
				out_bytes.resize(out_size);
				status = uncompress(out_bytes.data(), &out_size, in_bytes.data(), static_cast<uLong>(in_bytes.size()));
			}
			while(status == Z_BUF_ERROR);
			
			out_bytes.resize(out_size);
			out_bytes.shrink_to_fit();
			return (out_bytes);
		}

		/// <summary>Method used to deflate an inflated byte vector</summary>
		/// <param name="in_bytes">The input inflated byte vector</param>
		/// <returns>The output deflated byte vector</returns>
		std::vector<byte> const ZHelper::Deflate(std::vector<byte> const& in_bytes)
		{
			uLongf out_size = static_cast<uLongf>(compressBound(static_cast<uLong>(in_bytes.size())));
			std::vector<byte> out_bytes(out_size);
			compress(out_bytes.data(), &out_size, in_bytes.data(), static_cast<uLong>(in_bytes.size()));
			out_bytes.resize(out_size);
			out_bytes.shrink_to_fit();
			return (out_bytes);
		}
	#pragma endregion
	}
}