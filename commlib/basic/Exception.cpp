

#include "Exception.h"

namespace CWSLib
{
	Exception::Exception(int32_t errorNo, const char* errorMsg) :
		m_errno(errorNo), m_errMsg(errorMsg)
	{
	}

	const char* Exception::what() const noexcept
	{
		return m_errMsg.c_str();
	}

	int32_t Exception::error() const
	{
		return m_errno;
	}

}