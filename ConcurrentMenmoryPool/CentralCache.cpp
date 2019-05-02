#include "CentralCache.h"

//�����Ļ����ȡһ��������obj��threadcache
size_t CentralCache:: FetchRangeObj(void*& start, void*& end, size_t n, size_t byte_size)
{
	//����spanlist�Ĳ���
	size_t index = SizeClass::Index(byte_size);////ͨ�������С����ö���������������ĸ�λ��
	SpanList& spanlist = _spanlist[index];//��Ӧ��spanlist��indexλ��
	Span* span = spanlist.Begin();//������ʼ
	while (span != spanlist.End())
	{
		if (span->_list != nullptr)
			break;//�ҵ���
		else
			span = span->_next;//��spanΪ�գ�����һ��span
	}

}