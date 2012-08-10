#ifndef _REFLECTION_HPP_
#define _REFLECTION_HPP_
/*
struct ReflectionVariant
{
	enum Type {
		Void,
		SignedChar,
		UnsignedChar,
		SignedShort,
		UnsignedShort,
		SignedInteger,
		UnsignedInteger,
		SignedLong,
		UnsignedLong,
		Double,
		Float,
		Bool,
		SignedCharArray,
		UnsignedCharArray,
		SignedShortArray,
		UnsignedShortArray,
		SignedIntegerArray,
		UnsignedIntegerArray,
		SignedLongArray,
		UnsignedLongArray
	} type;
	unsigned int size;
	
	union {
		signed char signedCharValue;
		unsigned char unsignedCharValue;
		
		signed short signedShortValue;
		unsigned short unsignedShortValue;
		
		signed int signedIntegerValue;
		unsigned int unsignedIntegerValue;
		
		signed long signedLongValue;
		unsigned long unsignedLongValue;
		
		double doubleValue;
		float floatValue;
		
		bool boolValue;
		
		signed char *signedCharArrayValue;
		unsigned char *unsignedCharArrayValue;
		
		signed short *signedShortArrayValue;
		unsigned short *unsignedShortArrayValue;
		
		signed int *signedIntegerArrayValue;
		unsigned int *unsignedIntegerArrayValue;
		
		signed long *signedLongArrayValue;
		unsigned long *unsignedLongArrayValue;
	};
};

typedef std::vector<ReflectionVariant> ReflectionVariantVector;

class ReflectionDelegate
{
public:
	ReflectionVariant call(const std::string& method, const ReflectionVariantVector& parameters);
};

class ReflectionEngine
{
public:
	ReflectionVariant interpret(const std::string& command);
	
	void reset();
	
	static ReflectionEngine *instance();
private:
	ReflectionEngine();
	ReflectionEngine(const ReflectionEngine& rhs);
	ReflectionEngine& operator=(const ReflectionEngine& rhs);
	
	std::map<std::string, ReflectionVariant> m_variables;
};
*/
#endif
