/////////////////////////////////////////////
// create time: 2011/7/9 10:18
// author:	colin chen
// file name:	GYObject
// file type:	h
////////////////////////////////////////////

#ifndef __GYOBJECT_H__
#define __GYOBJECT_H__
//所有的需要使用基础数据结构容器的数据类型都需要集成这个类
//基础数据结构容器包括链表，双向链表，
struct GYObject
{
	GYObject*	m_next;
	GYObject*	m_prev;
	GYINT32		m_index;
};
#endif

