/////////////////////////////////////////////
// create time: 2011/7/4 23:59
// author:	colin chen
// file name:	GYList
// file type:	h
////////////////////////////////////////////

#ifndef __GYLIST_H__
#define __GYLIST_H__
#include "GYCommonDefine.h"


template<typename T>
class GYList
{
	T*				m_list;
	T*				m_tail;
	GYINT32			m_itemCount;
public:
	GYList()
	{
		CleanUp();
	}

	~GYList()
	{
	}
	
	GYVOID CleanUp()
	{
		m_list = GYNULL;
		m_tail = GYNULL;
		m_itemCount = 0;
	}
	GYINLINE GYINT32 GetItemCount()
	{
		return m_itemCount;
	}

	GYINLINE T* PickUpFirstItem()
	{
		if (GYNULL == m_list || m_itemCount <= 0)
		{
			return GYNULL;
		}
		
		T* t = m_list;
		m_list = static_cast<T*>(t->m_next);
		if (GYNULL == m_list)
		{
			m_tail = GYNULL;
		}
		
		t->m_prev = GYNULL;
		t->m_next = GYNULL;
		--m_itemCount;
		return t;
	}

	GYBOOL Add(T& data)
	{
		data.m_prev = GYNULL;
		data.m_next = GYNULL;
		if (GYNULL != m_list)
		{
			m_list->m_prev = &data;
			data.m_next = m_list;
		}
		else
		{
			m_tail = &data;
		}
		m_list = &data;
		++m_itemCount;
		return GYTRUE;
	}
	GYBOOL Add(GYList<T>& data)
	{
		if (0 == data.m_itemCount)
		{
			return GYTRUE;
		}
		
		if (GYNULL != m_tail)
		{
			m_tail->m_next = data.m_list;
		}

		if (GYNULL != data.m_list)
		{
			data.m_list->m_prev = m_tail;
		}

		m_tail = data.m_tail;
		m_itemCount += data.m_itemCount;
		data.CleanUp();
		return GYTRUE;
	}

	GYBOOL Delete(T& data)
	{
		if (GYNULL == m_list)
		{
			return GYFALSE;
		}
		if (GYNULL != data.m_next)
		{
			data.m_next->m_prev = data.m_prev;
		}
		if (GYNULL != data.m_prev)
		{
			data.m_prev->m_next = data.m_next;
		}
		if (&data == m_list)
		{
			m_list = static_cast<T*>(data.m_next);
		}
		data.m_prev = GYNULL;
		data.m_next = GYNULL;
		--m_itemCount;		
		return GYTRUE;
	}
	
	typedef GYVOID (*ForEach)(T& element, GYVOID* param);

	GYVOID ForEachElement(ForEach function, GYVOID* param)
	{
		T* element = m_list;
		while (GYNULL != element)
		{
			function(*element, param);
		}
	}

};
#endif
