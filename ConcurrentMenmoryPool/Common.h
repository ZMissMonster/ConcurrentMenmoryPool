#pragma once
#include <iostream>
#include <thread>
#include <vector>
using std::cout;
using std::endl;

//const size_t NLISTS = 128;//暂定给128个自由链表对象
const size_t MAX_BYTES = 64 * 1024;//threadcache一次最多申请这么多字节
const size_t NLISTS = MAX_BYTES / 8; //NLISTS个自由链表对象

inline static void*& NEXT_OBJ(void* obj)
{
	return *((void**)obj);
}
class FreeList//封装成自由链表对象
{
public:
	void Push(void* obj)//插入一个对象
	{
		NEXT_OBJ(obj)= _list;
		_list = obj;
		++_size;
	}
	void PushRange(void* start, void*end, size_t n)
	{
		//头插这一段批量对象
		NEXT_OBJ(end) = _list;
		_list = start;
		_size += n;
	}
	void* Pop()//返回一个对象
	{
		void* obj = _list;
		_list = NEXT_OBJ(obj);
		--_size;
		return obj;
	}
	bool Empty()//判断是否为空
	{
		return _list==nullptr;
	}  
private:
	void* _list=nullptr;//指针指向内存块
	size_t _size=0;//_size记录有多少个对象
};

class SizeClass//没有成员函数，所以定义成stctic，因为多次使用，所以定义成inline
{
public:

	inline static size_t _Index(size_t size,size_t align)//通过对象大小计算该对象在自由链表的哪个位置
	{
		//align为对齐数，align=3，alignnum=8（2^3）
		size_t alignnum = 1 << align;
		return ((size + alignnum-1) >> align) - 1;
		//return ((size + 7) >> 3) - 1;
	}
	inline static size_t Index(size_t size)//对外提供一个参数的函数，对内调用子函数
	{
		return _Index(size, 3);
	}

	//对齐数越小，内存碎片问题越轻，浪费内存越少，但链表数多
	inline static size_t _Roundup(size_t size,size_t align)//对齐到8字节
	{
		size_t alignnum = 1 << align;
		return (size + alignnum - 1)&~(alignnum - 1);
		//return (size + 7)&~7;//若为16取整数则 (size + 15)&~15
	}
	inline static size_t Roundup(size_t size)//对齐到8字节
	{
		return _Roundup(size, 3);
	}
};

struct Span//公有给别人访问
{
	Span* _prev=nullptr;//前后指针
	Span* _next=nullptr;
	
	void* _list=nullptr;//链接对象的自由链表
	size_t _objsize=0;//对象的大小

	size_t _usecount=0;//对象的使用计数，若为0 表示都还回来了
};

//双向带头循环的span链表
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
		while (cur != _head)//证明链表还不为空，后边还有span
		{
			Span* next = cur->_next;
			delete cur;
			cur = next;
		}
		delete _head;
		_head = nullptr;
	}
//因为不需要拷贝构造，写成防拷贝
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

	void Insert(Span* pos,Span* newspan)//在pos的前面插入newspan
	{
		//prev newspan pos
		Span* prev = pos->_prev;
		prev->_next = newspan;
		newspan->_next = pos;
		newspan->_prev = prev;
		pos->_prev = newspan;
	}
	void Erase(Span* pos)//移除pos处的span。不能delete掉，仅仅移除。
	{
		Span* prev = pos->_prev;
		Span* next = pos->_next;
		prev->_next = next;
		next->_prev = prev;
	}

private:
	Span* _head;//头指针
};