#include "Common.h"

class ThreadCache
{
public:
	//申请和释放内存对象
	void* Allocate(size_t size);//申请size大小的内存对象
	void Deallocate(void* ptr, size_t size);//给一个指针，释放size大小的内存对象

	//从中心缓存获取对象
	void* FetchFromCentralCache(size_t index, size_t size);//从哪个位置取多大的内存对象
	// 释放对象，链表过长时，回收内存回到中心堆
	void ListTooLong(FreeList* list, size_t size);

private:
	FreeList _freelist[NLISTS];//128个自由链表，封装成对象
};

//TLS
_declspec (thread) static ThreadCache* tlslist = nullptr;//TLS保证每个线程独有一个threadcache的指针，static保证全局可见（相当于一个线程是全局的）