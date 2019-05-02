#include "Common.h"

class ThreadCache
{
public:
	//������ͷ��ڴ����
	void* Allocate(size_t size);//����size��С���ڴ����
	void Deallocate(void* ptr, size_t size);//��һ��ָ�룬�ͷ�size��С���ڴ����

	//�����Ļ����ȡ����
	void* FetchFromCentralCache(size_t index, size_t size);//���ĸ�λ��ȡ�����ڴ����
	// �ͷŶ����������ʱ�������ڴ�ص����Ķ�
	void ListTooLong(FreeList* list, size_t size);

private:
	FreeList _freelist[NLISTS];//128������������װ�ɶ���
};

//TLS
_declspec (thread) static ThreadCache* tlslist = nullptr;//TLS��֤ÿ���̶߳���һ��threadcache��ָ�룬static��֤ȫ�ֿɼ����൱��һ���߳���ȫ�ֵģ�