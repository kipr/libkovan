#ifndef _UVLC_VIDEO_DECODER_P_HPP_
#define _UVLC_VIDEO_DECODER_P_HPP_

#include <cstdlib>

#define UVLC_MAX_SIZE (921600)

namespace Private
{
	struct Image
	{
		unsigned char data[UVLC_MAX_SIZE]; 
		int width;
		int height;
	};
	
	class UvlcVideoDecoder
	{
	public:
		UvlcVideoDecoder();
		~UvlcVideoDecoder();
		
		void decode(const unsigned char *const buffer, const size_t length, Image &image);
	private:
	};
}

#endif
