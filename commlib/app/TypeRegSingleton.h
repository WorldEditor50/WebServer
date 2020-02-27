

#ifndef __TYPE_REG_SINGLETON_H__
#define __TYPE_REG_SINGLETON_H__

#include <string>
#include <map>
#include "../basic/CommonSingleton.h"

class TypeRegContainer
{
	friend CWSLib::CommSingleton<TypeRegContainer>;
public:
	void* create(const std::string& className)
	{
		auto iter = cbFuncMap.find(className);
		if (iter != cbFuncMap.end())
		{
			return iter->second;
		}
		else
		{
			return nullptr;
		}
	}

	void join(const std::string& className, void* __cb)
	{
		cbFuncMap.insert(std::make_pair(className, __cb));
	}

private:
	TypeRegContainer() {}
	TypeRegContainer(TypeRegContainer& alFac) {}

	std::map<std::string, void*> cbFuncMap;
};
typedef CWSLib::CommSingleton<TypeRegContainer> CTypeRegContainer;

template<class BaseClass>
class TypeFactory
{
	typedef BaseClass* (*CbFunc)();
	friend CWSLib::CommSingleton<TypeFactory>;
public:
	BaseClass* create(const std::string& className)
	{
		CbFunc funcPtr = (CbFunc)CTypeRegContainer::instance().create(className);
		if (funcPtr == nullptr)
		{
			return nullptr;
		}
		return funcPtr();
	}

	void join(const std::string& className, CbFunc __cb)
	{
		CTypeRegContainer::instance().join(className, (void*)__cb);
	}

private:
	TypeFactory() {}
	TypeFactory(TypeFactory& alFac) {}
};

template <typename T>
T* SafeNew()
{
	T* ptr = nullptr;
	try
	{
		ptr = new T;
	}
	catch (...)
	{
		///Do nothing for now ...
	}
	return ptr;
}

template<class BaseClass>
class TypeRegCaller
{
public:
	TypeRegCaller(const std::string& className)
	{
		CWSLib::CommSingleton<TypeFactory<BaseClass>>::instance().join(className, getInstance);
	}

	static BaseClass* getInstance()
	{
		return SafeNew<BaseClass>();
	}
};

#define REG_TYPE(TypeName, __cb_func) TypeRegCaller<TypeName> caller_##__cb_func(#__cb_func)

#endif // !__TYPE_REG_SINGLETON_H__

