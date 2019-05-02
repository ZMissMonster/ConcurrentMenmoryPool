#pragma once
#include "Common.h"

//设计成单例模式
class CentralCache
{
public:
	static CentralCache* GetInstance()
	{
		return &_inst;//获取单例对象
	}
	//从中心缓存获取一定数量的obj给threadcache
	size_t FetchRangeObj(void*& start, void*& end, size_t n, size_t byte_size);
private:
	SpanList _spanlist[NLISTS];
private:
	CentralCache()
	{}
	//防拷贝
	CentralCache(const CentralCache&) = delete;
	//声明该单例对象
	static CentralCache _inst;//设计为单例模式---饿汉模式，一上来就创建，在main函数之前就创建了。
};
//定义该单例对象
CentralCache CentralCache::_inst;//调用默认构造函数，所以不传参数