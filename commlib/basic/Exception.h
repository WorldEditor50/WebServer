

#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <exception>
#include <string>

namespace CWSLib
{
	class Exception : public std::exception
	{
	public:
		Exception(int32_t errorNo, const char* errorMsg);
		~Exception() {}
		const char* what() const noexcept override;
		int32_t error() const;

	private:
		int32_t m_errno;
		std::string m_errMsg;
	};
}

#endif // !__EXCEPTION_H__

