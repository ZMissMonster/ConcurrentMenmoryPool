#include "ThreadCache.h"
#include "CentralCache.h"


//�����Ļ����ȡ��������
void* ThreadCache::FetchFromCentralCache(size_t index, size_t size)
{
	//������ȡ�ڴ���󣬴�С�����㷨���㲻�Ƕ����ģ���Ϊÿ�λ�ȡ�Ĵ�С����	
	size_t numtomove= 20;//numtomove���Ϊ���ڴ�����Ļ����ƶ�������
	void* start = nullptr;
	void* end = nullptr;
	//size�ǵ�������Ĵ�С,numtomove����Ҫ���ٸ�����batchsize��ʵ�ʸ��˶��ٸ��������Լ���Ҫ��ʱҲҪ����Ϊ���������룬����ֻ��Ҫһ��
	size_t batchsize =CentralCache::GetInstance()->FetchRangeObj(start, end, numtomove, size);//����ģʽ��centralcache���ã���֤ÿ��threadcache������ͬһ��centralcache
	
	if (batchsize > 1)
	{
		FreeList* freelist = &_freelist[index];
		freelist->PushRange(NEXT_OBJ(start), end, batchsize - 1);//��Ϊ�ù����ĵ�һ����ʹ���ˣ����԰�ʣ�µ�push��ȥ��
	}
	return start;
}
//�����ڴ����
void* ThreadCache::Allocate(size_t size)
{
	size_t index = SizeClass::Index(size);
	FreeList* freelist = &_freelist[index];
	if (!freelist->Empty())//��Ϊ�գ����ڴ����
	{
		return freelist->Pop();
	}
	else//û����CentralCache��
	{
		return FetchFromCentralCache(index, SizeClass::Roundup(size));//���뵽8�ֽ�
	}
}
//�ͷ��ڴ����
void ThreadCache::Deallocate(void* ptr, size_t size)
{
	size_t index = SizeClass::Index(size);
	FreeList* freelist = &_freelist[index];
	freelist->Push(ptr);
	//�����������ͷŻ����Ļ���

}

// �ͷŶ���ʱ���������ʱ�������ڴ�ص����Ķ�
void ListTooLong(FreeList* list, size_t size)
{

}