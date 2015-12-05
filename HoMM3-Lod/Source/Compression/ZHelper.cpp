#include "Compression/ZHelper.hpp"
#include <iostream>

namespace HoMM3
{
	namespace Compression
	{
		
		const float ZHelper::INCREASE_RATIO = 1.1f;
		
		/// <summary>Method used to inflate a deflated byte vector</summary>
		/// <param name="in_bytes">The input deflated byte vector</param>
		/// <returns>The output inflated byte vector</returns>
		std::vector<byte> const ZHelper::Inflate(std::vector<byte> const& in_bytes)
		{
			std::vector<byte> out_bytes;
			uint out_size = in_bytes.size();
			uLongf uncompressed_size;
			int status;
			
			do
			{
				out_size *= INCREASE_RATIO;
				out_bytes.resize(out_size, 'a');
				out_bytes.empty();
				
				uncompressed_size = out_size;
				status = uncompress(out_bytes.data(), &uncompressed_size, in_bytes.data(), in_bytes.size());
			} while(status == Z_BUF_ERROR);
			out_bytes.resize(uncompressed_size);
			out_bytes.shrink_to_fit();
			return (out_bytes);
		}

		/// <summary>Method used to deflate an inflated byte vector</summary>
		/// <param name="in_bytes">The input inflated byte vector</param>
		/// <param name="out_size">The size of the output byte vector</param>
		/// <returns>The output deflated byte vector</returns>
		std::vector<byte> const ZHelper::Deflate(std::vector<byte> const& in_bytes)
		{
			std::vector<byte> out_bytes;
			out_bytes.resize(in_bytes.size());
			/*this->zs_.avail_in = static_cast<uInt>(in_bytes.size());
			this->zs_.next_in = const_cast<Bytef*>(in_bytes.data());
			this->zs_.avail_out = static_cast<uInt>(out_size);
			this->zs_.next_out = const_cast<Bytef*>(out_bytes.data());

			deflateInit(&this->zs_, Z_BEST_COMPRESSION);
			deflate(&this->zs_, Z_FINISH);
			deflateEnd(&this->zs_);*/

			return (out_bytes);
		}
	}
}