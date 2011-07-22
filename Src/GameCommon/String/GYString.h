/////////////////////////////////////////////
// create time: 2011/7/22 15:55
// author:	colin chen
// file name:	GYString
// file type:	h
////////////////////////////////////////////

#ifndef __GYSTRING_H__
#define __GYSTRING_H__

#include "GYCommonDefine.h"
class GYStringManager;

class GYString
{
	GYWCHAR*			m_stringBuffer;
	GYINT32				m_stringLength;
	GYStringManager&	m_strManager;

public:
	GYString(GYStringManager& stringManager);
	GYString(const GYString& str);
	GYString(const GYWCHAR* pString, GYINT32 stringLength, GYStringManager& stringManger);
	~GYString();

	GYINLINE const GYWCHAR* c_str(){return m_stringBuffer;}
	GYINLINE GYINT32 length(){return m_stringLength;}
	GYINLINE GYBOOL empty(){return m_stringLength > 0 ? GYFALSE : GYTRUE;}

	GYBOOL	operator==(const GYString& str)const;
	GYBOOL	operator==(const GYWCHAR* str)const;
	GYVOID	operator=(const GYString& str);
	//GYVOID	operator=(const GYWCHAR* str);
};
#endif

