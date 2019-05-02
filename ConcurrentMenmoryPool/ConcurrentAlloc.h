#pragma once
#include "Common.h"
#include "ThreadCache.h"

void* ConcurrentAlloc(size_t size)
{
	if (size > MAX_BYTES)
	{
		return malloc(size);
	}
	else
	{
		if (tlslist == nullptr)//每个线程第一次来为空
		{
			//cout << std::this_thread::get_id() << endl;//获取一个线程id
			tlslist = new ThreadCache; 
			//cout << tlslist << endl;
		}
		return tlslist->Allocate(size);
	}
}
void ConcurrentFree(void* ptr, size_t size)
{
	if (size > MAX_BYTES)
	{
		free(ptr);
	}
	else
	{
		tlslist->Deallocate(ptr, size);
	}
}