

#ifndef __COMMON_SINGLETON_H__
#define __COMMON_SINGLETON_H__

namespace CWSLib
{
	template<typename T>
	class CommSingleton
	{
	public:
		static T& instance()
		{
			static T inst;
			return inst;
		}

	private:
		CommSingleton() {}
		CommSingleton(CommSingleton& comm) {}
	};
}

#endif // !__COMMON_SINGLETON_H__

