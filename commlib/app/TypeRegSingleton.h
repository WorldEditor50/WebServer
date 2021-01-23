

#ifndef __TYPE_REG_SINGLETON_H__
#define __TYPE_REG_SINGLETON_H__

#include <string>
#include <functional>
#include <map>
#include "../basic/CommonSingleton.h"

template<class BaseClass>
class TypeRegContainer
{
	using CbFunc = std::function<BaseClass* ()>;
	friend CWSLib::CommSingleton<TypeRegContainer>;
public:
	CbFunc create(const std::string& className)
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

	void join(const std::string& className, CbFunc __cb)
	{
		cbFuncMap.insert(std::make_pair(className, __cb));
	}

private:
	TypeRegContainer() {}
	TypeRegContainer(TypeRegContainer& alFac) {}

	std::map<std::string, CbFunc> cbFuncMap;
};

template<class BaseClass>
class TypeFactory
{
	using CbFunc = std::function<BaseClass*()>;
	using CTypeRegContainer = CWSLib::CommSingleton<TypeRegContainer<BaseClass>>;
	friend CWSLib::CommSingleton<TypeFactory>;
public:
	BaseClass* create(const std::string& className)
	{
		CbFunc funcPtr = CTypeRegContainer::instance().create(className);
		if (funcPtr == nullptr)
		{
			return nullptr;
		}
		return funcPtr();
	}

	BaseClass* create()
	{
		return create(m_defaultTypeName);
	}

	void join(const std::string& className, CbFunc __cb)
	{
		CTypeRegContainer::instance().join(className, __cb);
	}

	void join(const std::string& className, CbFunc __cb, bool setDefault)
	{
		if (setDefault)
		{
			m_defaultTypeName = className;
		}
		this->join(className, __cb);
	}

private:
	TypeFactory() {}
	TypeFactory(TypeFactory& alFac) {}

private:
	std::string m_defaultTypeName;
};

template<class BaseClass, class ImplClass>
class TypeRegCaller
{
public:
	TypeRegCaller(const std::string& className)
	{
		CWSLib::CommSingleton<TypeFactory<BaseClass>>::instance().join(className, getInstance);
	}

	TypeRegCaller(const std::string& className, bool)
	{
		CWSLib::CommSingleton<TypeFactory<BaseClass>>::instance().join(className, getInstance, true);
	}

	static BaseClass* getInstance()
	{
		return new ImplClass;
	}
};

#define REG_TYPE(BaseType, TypeName, __cb_func) static TypeRegCaller<BaseType, TypeName> caller_##__cb_func(#__cb_func)
#define REG_TYPE_DEFAULT(BaseType, TypeName, __cb_func) static TypeRegCaller<BaseType, TypeName> caller_##__cb_func(#__cb_func, true)

#endif // !__TYPE_REG_SINGLETON_H__

