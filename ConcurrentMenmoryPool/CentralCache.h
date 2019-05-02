#pragma once
#include "Common.h"

//��Ƴɵ���ģʽ
class CentralCache
{
public:
	static CentralCache* GetInstance()
	{
		return &_inst;//��ȡ��������
	}
	//�����Ļ����ȡһ��������obj��threadcache
	size_t FetchRangeObj(void*& start, void*& end, size_t n, size_t byte_size);
private:
	SpanList _spanlist[NLISTS];
private:
	CentralCache()
	{}
	//������
	CentralCache(const CentralCache&) = delete;
	//�����õ�������
	static CentralCache _inst;//���Ϊ����ģʽ---����ģʽ��һ�����ʹ�������main����֮ǰ�ʹ����ˡ�
};
//����õ�������
CentralCache CentralCache::_inst;//����Ĭ�Ϲ��캯�������Բ�������