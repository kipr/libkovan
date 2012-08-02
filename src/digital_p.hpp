#ifndef _DIGITAL_P_HPP_
#define _DIGITAL_P_HPP_

namespace Private
{
	class Digital
	{
	public:
		enum Direction {
			In,
			Out
		};
		
		bool value(const unsigned char& port) const;
		bool setValue(const unsigned char& port, const bool& value);
		
		const Digital::Direction direction(const unsigned char& port) const;
		bool setDirection(const unsigned char& port, const Digital::Direction& direction);
		
		bool pullup(const unsigned char& port) const;
		bool setPullup(const unsigned char& port, const bool& pullup);
		
	private:
		Digital();
		Digital(const Digital& rhs);
		Digital& operator=(const Digital& rhs);
	};
}

#endif
