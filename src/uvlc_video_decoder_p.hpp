#ifndef _UVLC_VIDEO_DECODER_P_HPP_
#define _UVLC_VIDEO_DECODER_P_HPP_

#include <cstdlib>
#include <opencv2/core/core.hpp>

#define UVLC_MAX_SIZE (921600)

namespace Private
{
	class UvlcVideoDecoder
	{
	public:
		UvlcVideoDecoder();
		~UvlcVideoDecoder();
		
		bool decode(const unsigned char *const buffer, const size_t length, cv::Mat &image);
	private:
	};
}

#endif
