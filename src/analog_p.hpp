#ifndef _ANALOG_P_HPP_
#define _ANALOG_P_HPP_

namespace Private
{
	class Analog
	{
	public:
		void setPullup(const unsigned char& port, const bool& pullup);
		bool pullup(const unsigned char& port) const;
		unsigned short value(const unsigned char& port) const;
	
		static Analog *instance();
	private:
		Analog();
		Analog(const Analog& rhs);
		Analog& operator=(const Analog& rhs);
	};
}

#endif
