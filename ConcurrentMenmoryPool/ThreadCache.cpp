#include "ThreadCache.h"
#include "CentralCache.h"


//从中心缓存获取批量对象
void* ThreadCache::FetchFromCentralCache(size_t index, size_t size)
{
	//批量获取内存对象，大小经过算法计算不是定死的，因为每次获取的大小不定	
	size_t numtomove= 20;//numtomove理解为把内存从中心缓存移动到这里
	void* start = nullptr;
	void* end = nullptr;
	//size是单个对象的大小,numtomove是想要多少个对象，batchsize是实际给了多少个，不足自己需要的时也要，因为是批量申请，本质只需要一个
	size_t batchsize =CentralCache::GetInstance()->FetchRangeObj(start, end, numtomove, size);//单例模式的centralcache调用，保证每个threadcache看到的同一个centralcache
	
	if (batchsize > 1)
	{
		FreeList* freelist = &_freelist[index];
		freelist->PushRange(NEXT_OBJ(start), end, batchsize - 1);//因为拿过来的第一个被使用了，所以把剩下的push进去。
	}
	return start;
}
//申请内存对象
void* ThreadCache::Allocate(size_t size)
{
	size_t index = SizeClass::Index(size);
	FreeList* freelist = &_freelist[index];
	if (!freelist->Empty())//不为空，有内存对象
	{
		return freelist->Pop();
	}
	else//没有找CentralCache拿
	{
		return FetchFromCentralCache(index, SizeClass::Roundup(size));//对齐到8字节
	}
}
//释放内存对象
void ThreadCache::Deallocate(void* ptr, size_t size)
{
	size_t index = SizeClass::Index(size);
	FreeList* freelist = &_freelist[index];
	freelist->Push(ptr);
	//满足条件，释放回中心缓存

}

// 释放对象时，链表过长时，回收内存回到中心堆
void ListTooLong(FreeList* list, size_t size)
{

}