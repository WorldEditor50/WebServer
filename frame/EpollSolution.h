

#ifndef __EPOLL_SOLUTION_H__
#define __EPOLL_SOLUTION_H__

#include <string>

template<typename SolutionType, typename ParamType>
void SolutionFunc(ParamType& content)
{
	SolutionType object;
	return object.process(content);
}

#endif // !__EPOLL_SOLUTION_H__

