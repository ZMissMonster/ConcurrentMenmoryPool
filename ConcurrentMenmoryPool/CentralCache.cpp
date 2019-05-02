#include "CentralCache.h"

//从中心缓存获取一定数量的obj给threadcache
size_t CentralCache:: FetchRangeObj(void*& start, void*& end, size_t n, size_t byte_size)
{
	//遍历spanlist的步骤
	size_t index = SizeClass::Index(byte_size);////通过对象大小计算该对象在自由链表的哪个位置
	SpanList& spanlist = _spanlist[index];//对应到spanlist的index位置
	Span* span = spanlist.Begin();//遍历开始
	while (span != spanlist.End())
	{
		if (span->_list != nullptr)
			break;//找到了
		else
			span = span->_next;//该span为空，找下一个span
	}

}