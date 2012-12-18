#include "channel_p.hpp"
#include "warn.hpp"
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <zbar.h>

using namespace Private::Camera;

HsvChannelImpl::HsvChannelImpl()
{
	cv::namedWindow("Hsv Channel");
	cv::namedWindow("Sub");
}

void HsvChannelImpl::update(const cv::Mat *image)
{
	cv::cvtColor(*image, m_image, CV_BGR2HSV);
	cv::imshow("Hsv Channel", m_image);
}

Camera::ObjectVector HsvChannelImpl::objects(const Config &config)
{
	// TODO: This lookup is really slow compared to the rest of
	// the algorithm.
	cv::Vec3b top(config.intValue("th"),
		config.intValue("ts"), config.intValue("tv"));
	cv::Vec3b bottom(config.intValue("bh"),
		config.intValue("bs"), config.intValue("bv"));
	
	cv::Mat fixed;
	if(bottom[0] > top[0]) {
		cv::Vec3b adj(180 - bottom[0], 0, 0);
		// Offset by bottom overflow
		cv::add(adj, m_image, fixed);
		
		// Modulo 180
		// TODO: Optimize for ARM?
		for(int i = 0; i < fixed.rows; ++i) {
			uchar *row = fixed.ptr<uchar>(i);
			for(int j = 0; j < fixed.cols; ++j) {
				row[j * fixed.elemSize()] %= 180;
			}
		}
		
		bottom = cv::Vec3b(0, bottom[1], bottom[2]);
		cv::add(adj, top, top);
	} else fixed = m_image;
	
	cv::Mat only;
	cv::inRange(fixed, bottom, top, only);
	
	cv::imshow("Sub", only);
	
	std::vector<std::vector<cv::Point> > c;
	cv::findContours(only, c, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_L1);
	
	std::vector<cv::Moments> m(c.size());
	for(std::vector<cv::Moments>::size_type i = 0; i < c.size(); ++i) {
		m[i] = moments(c[i], false);
	}
	
	::Camera::ObjectVector ret;
	for(::Camera::ObjectVector::size_type i = 0; i < c.size(); ++i) {
		const cv::Rect rect = cv::boundingRect(c[i]);
		if(rect.width < 3 && rect.height < 3) continue;
		
		ret.push_back(::Camera::Object(Point2<unsigned>(m[i].m10 / m[i].m00, m[i].m01 / m[i].m00),
			Rectangle<unsigned>(rect.x, rect.y, rect.width, rect.height), 1.0));
	}
	
	return ret;
}

BarcodeChannelImpl::BarcodeChannelImpl()
{
	m_image.set_format("Y800");
	
	m_scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 0);
	m_scanner.set_config(zbar::ZBAR_QRCODE, zbar::ZBAR_CFG_ENABLE, 1);
	cv::namedWindow("Gray");
}

void BarcodeChannelImpl::update(const cv::Mat *image)
{
	cv::cvtColor(*image, m_gray, CV_BGR2GRAY);
	cv::imshow("Gray", m_gray);
	m_image.set_data(m_gray.data, m_gray.cols * m_gray.rows);
	m_image.set_size(m_gray.cols, m_gray.rows);
}

::Camera::ObjectVector BarcodeChannelImpl::objects(const Config &config)
{
	double coherence = 15.0;
	if(config.containsKey("coherence")) coherence = config.doubleValue("coherence");
	std::cout << "ret = " << m_scanner.scan(m_image) << std::endl;
	
	zbar::SymbolSet symbols = m_scanner.get_results();
	std::cout << "Number of symbols: " << symbols.get_size() << std::endl;
	::Camera::ObjectVector ret;
	zbar::SymbolIterator it = symbols.symbol_begin();
	for(; it != symbols.symbol_end(); ++it) {
		zbar::Symbol symbol = *it;
		
		// Determine bounding box and centroid
		int left = m_image.get_width();
		int right = 0;
		int bottom = m_image.get_height();
		int top = 0;
		
		zbar::Symbol::Symbol::PointIterator pit = symbol.point_begin();
		for(int i = 0; i < symbol.get_location_size(); ++i) {
			const int &x = symbol.get_location_x(i);
			if(x > right) right = x;
			if(x < left) left = x;
			
			const int &y = symbol.get_location_y(i);
			std::cout << y << std::endl;
			if(y > top) top = y;
			if(y < bottom) bottom = y;
		}
		
		// Confidence = 1.0 after a number of coherence frames
		double confidence = symbol.get_count() / coherence;
		if(confidence > 1.0) confidence = 1.0;
		
		ret.push_back(::Camera::Object(Point2<unsigned>((left + right) / 2, (top + bottom) / 2),
			Rectangle<unsigned>(left, bottom, right - left, top - bottom),
			confidence, zbar_symbol_get_data(symbol),
			zbar_symbol_get_data_length(symbol)));
	}
	
	return ret;
}