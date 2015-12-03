#include "Compression/ZHelper.hpp"
#include "zlib.h"

namespace HoMM3
{
	namespace Compression
	{
		/// <summary>Method used to inflate a deflated byte vector</summary>
		/// <param name="in_bytes">The input deflated byte vector</param>
		/// <param name="out_size">The size of the output byte vector</param>
		/// <returns>The output inflated byte vector</returns>
		std::vector<byte> const ZHelper::Inflate(std::vector<byte> const& in_bytes, uint out_size)
		{
			std::vector<byte> out_bytes(out_size);
			z_stream defstream;
		
			defstream.zalloc = Z_NULL;
			defstream.zfree = Z_NULL;
			defstream.opaque = Z_NULL;
			defstream.avail_in = static_cast<uInt>(in_bytes.size());
			defstream.next_in = const_cast<Bytef*>(in_bytes.data());
			defstream.avail_out = static_cast<uInt>(out_size);
			defstream.next_out = const_cast<Bytef*>(out_bytes.data());
	    
			inflateInit(&defstream);
			inflate(&defstream, Z_NO_FLUSH);
			inflateEnd(&defstream);

			return (out_bytes);
		}

		/// <summary>Method used to deflate an inflated byte vector</summary>
		/// <param name="in_bytes">The input inflated byte vector</param>
		/// <param name="out_size">The size of the output byte vector</param>
		/// <returns>The output deflated byte vector</returns>
		std::vector<byte> const ZHelper::Deflate(std::vector<byte> const& in_bytes, uint out_size)
		{
			std::vector<byte> out_bytes(out_size);
			z_stream defstream;

			defstream.zalloc = Z_NULL;
			defstream.zfree = Z_NULL;
			defstream.opaque = Z_NULL;
			defstream.avail_in = static_cast<uInt>(in_bytes.size());
			defstream.next_in = const_cast<Bytef*>(in_bytes.data());
			defstream.avail_out = static_cast<uInt>(out_size);
			defstream.next_out = const_cast<Bytef*>(out_bytes.data());

			deflateInit(&defstream, Z_BEST_COMPRESSION);
			deflate(&defstream, Z_FINISH);
			deflateEnd(&defstream);

			return (out_bytes);
		}
	}
}