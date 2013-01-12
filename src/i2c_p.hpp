#ifndef _I2C_P_HPP_
#define _I2C_P_HPP_

namespace Private
{
	class I2C
	{
	public:
		bool pickSlave(const char *slave);
		bool write(const unsigned char &addr, const unsigned char &val, const bool &readback);
		unsigned char read(const unsigned char &addr);
		
		static I2C *instance();
	private:
		I2C();
		~I2C();
		
		int m_fd;
	};
}

#endif
