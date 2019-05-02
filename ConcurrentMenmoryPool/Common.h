#pragma once
#include <iostream>
#include <thread>
#include <vector>
using std::cout;
using std::endl;

//const size_t NLISTS = 128;//�ݶ���128�������������
const size_t MAX_BYTES = 64 * 1024;//threadcacheһ�����������ô���ֽ�
const size_t NLISTS = MAX_BYTES / 8; //NLISTS�������������

inline static void*& NEXT_OBJ(void* obj)
{
	return *((void**)obj);
}
class FreeList//��װ�������������
{
public:
	void Push(void* obj)//����һ������
	{
		NEXT_OBJ(obj)= _list;
		_list = obj;
		++_size;
	}
	void PushRange(void* start, void*end, size_t n)
	{
		//ͷ����һ����������
		NEXT_OBJ(end) = _list;
		_list = start;
		_size += n;
	}
	void* Pop()//����һ������
	{
		void* obj = _list;
		_list = NEXT_OBJ(obj);
		--_size;
		return obj;
	}
	bool Empty()//�ж��Ƿ�Ϊ��
	{
		return _list==nullptr;
	}  
private:
	void* _list=nullptr;//ָ��ָ���ڴ��
	size_t _size=0;//_size��¼�ж��ٸ�����
};

class SizeClass//û�г�Ա���������Զ����stctic����Ϊ���ʹ�ã����Զ����inline
{
public:

	inline static size_t _Index(size_t size,size_t align)//ͨ�������С����ö���������������ĸ�λ��
	{
		//alignΪ��������align=3��alignnum=8��2^3��
		size_t alignnum = 1 << align;
		return ((size + alignnum-1) >> align) - 1;
		//return ((size + 7) >> 3) - 1;
	}
	inline static size_t Index(size_t size)//�����ṩһ�������ĺ��������ڵ����Ӻ���
	{
		return _Index(size, 3);
	}

	//������ԽС���ڴ���Ƭ����Խ�ᣬ�˷��ڴ�Խ�٣�����������
	inline static size_t _Roundup(size_t size,size_t align)//���뵽8�ֽ�
	{
		size_t alignnum = 1 << align;
		return (size + alignnum - 1)&~(alignnum - 1);
		//return (size + 7)&~7;//��Ϊ16ȡ������ (size + 15)&~15
	}
	inline static size_t Roundup(size_t size)//���뵽8�ֽ�
	{
		return _Roundup(size, 3);
	}
};

struct Span//���и����˷���
{
	Span* _prev=nullptr;//ǰ��ָ��
	Span* _next=nullptr;
	
	void* _list=nullptr;//���Ӷ������������
	size_t _objsize=0;//����Ĵ�С

	size_t _usecount=0;//�����ʹ�ü�������Ϊ0 ��ʾ����������
};

//˫���ͷѭ����span����
class SpanList
{
public:
	SpanList()
	{
		_head = new Span;
		_head->_next = _head;
		_head->_prev = _head;
	}
	~SpanList()
	{
		Span* cur = _head->_next;
		while (cur != _head)//֤��������Ϊ�գ���߻���span
		{
			Span* next = cur->_next;
			delete cur;
			cur = next;
		}
		delete _head;
		_head = nullptr;
	}
//��Ϊ����Ҫ�������죬д�ɷ�����
	SpanList& operator=(const SpanList&) = delete;
	SpanList(const SpanList&) = delete;

	Span* Begin()
	{
		return _head->_next;
	}
	Span* End()
	{
		return _head;
	}

	void Insert(Span* pos,Span* newspan)//��pos��ǰ�����newspan
	{
		//prev newspan pos
		Span* prev = pos->_prev;
		prev->_next = newspan;
		newspan->_next = pos;
		newspan->_prev = prev;
		pos->_prev = newspan;
	}
	void Erase(Span* pos)//�Ƴ�pos����span������delete���������Ƴ���
	{
		Span* prev = pos->_prev;
		Span* next = pos->_next;
		prev->_next = next;
		next->_prev = prev;
	}

private:
	Span* _head;//ͷָ��
};