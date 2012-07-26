#ifndef _CREATE_HPP_
#define _CREATE_HPP_

class Create
{
public:
	
	static Create *instance();
private:
	Create();
	Create(const Create& rhs);
	Create& operator=(const Create& rhs);
};

#endif
