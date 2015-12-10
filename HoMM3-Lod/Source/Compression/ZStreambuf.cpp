#include "Compression/ZStreambuf.hpp"

namespace HoMM3
{
	namespace Compression
	{

		ZipOStreambuf::ZipOStreambuf(std::streambuf& sbuf, size_t buf_size = ZSTREAMBUF_DEFAULT_BUFSIZE) :
			ostream_(&sbuf), buf_(buf_size)
		{
			this->zstream_.zalloc = static_cast<alloc_func>(NULL);
			this->zstream_.zfree = static_cast<free_func>(NULL);
			this->zstream_.next_in = NULL;
			this->zstream_.next_out = NULL;
			this->zstream_.avail_in = 0;
			this->zstream_.avail_out = 0;

			deflateInit(&this->zstream_, Z_BEST_COMPRESSION);
			setp(reinterpret_cast<char*>(&this->buf_[0]), reinterpret_cast<char*>(&this->buf_[buf_size - 1]));
		}

		ZipOStreambuf::~ZipOStreambuf()
		{
			this->flush();
			this->ostream_.flush();
			deflateEnd(&this->zstream_);
		}

		int ZipOStreambuf::sync()
		{
			if (this->pptr() && this->pptr() > this->pbase()) 
				if (overflow(EOF) == EOF)
					return (ZSTREAMBUF_SYNC_FAILURE);
			return (ZSTREAMBUF_SYNC_SUCCESS);
		}

		int ZipOStreambuf::overflow(int o_char)
		{
			int size = static_cast<int>(pptr() - pbase());
			if (o_char != EOF) {
				 *pptr() = o_char;
				 ++size;
			 }
			 if (zip_to_stream(pbase(), size)) {
				 setp(pbase(), epptr() - 1);
				 return (o_char);
			 } else
				 return (EOF);
		}

		bool ZipOStreambuf::zip_to_stream(char* buf, std::streamsize bufsize)
		{	
			std::streamsize written_byte_size=0, total_written_byte_size = 0;
			size_t remainder=0;
			int def_err;

			this->zstream_.next_in = reinterpret_cast<Bytef*>(buf);
			this->zstream_.avail_in = static_cast<uInt>(bufsize*sizeof(char));
			this->zstream_.avail_out = static_cast<uInt>(this->out_buf_.size());
			this->zstream_.next_out = &(this->out_buf_[0]);
			
			do
			{
				def_err = deflate(&this->zstream_, 0);
	
				if (def_err == Z_OK  || def_err == Z_STREAM_END)
				{
					written_byte_size = static_cast<std::streamsize>(this->out_buf_.size()) - this->zstream_.avail_out;
					total_written_byte_size+=written_byte_size;
					ostream_.write((const char_type*) &(this->out_buf_[0]), static_cast<std::streamsize>(written_byte_size/sizeof(char_type)));
					if ((remainder = written_byte_size % sizeof(char_type)) != 0)
						memcpy(&(this->out_buf_[0]), &(this->out_buf_[written_byte_size-remainder]), remainder);
					this->zstream_.avail_out = static_cast<uInt>(this->out_buf_.size()-remainder);
					this->zstream_.next_out=&this->out_buf_[remainder];
				}
			} while (this->zstream_.avail_in != 0 && def_err == Z_OK);
	
			return (def_err == Z_OK);
		};


	}
}