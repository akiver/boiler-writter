#ifndef BoilerException_h__
#define BoilerException_h__
#include <exception>
#include <string>

class BoilerException : public std::exception
{
public:
	BoilerException(const std::string& what);
	virtual const char* what() const throw();

private:
	std::string m_what;
};

#endif // BoilerException_h__
