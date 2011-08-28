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
	GYCHAR*				m_stringBuffer;
	GYINT32				m_stringLength;
	GYStringManager&	m_strManager;

public:
	explicit GYString(GYStringManager& stringManager);
	explicit GYString(const GYString& str);
	explicit GYString(const GYCHAR* pString, GYINT32 stringLength, GYStringManager& stringManger);
	~GYString();

	GYINLINE const GYCHAR* c_str()const{return m_stringBuffer;}
	GYINLINE GYINT32 length(){return m_stringLength;}
	GYINLINE GYBOOL empty(){return m_stringLength > 0 ? GYFALSE : GYTRUE;}

	GYBOOL	operator==(const GYString& str)const;
	GYBOOL	operator==(const GYCHAR* str)const;
	GYVOID	operator=(const GYString& str);
	GYVOID	operator=(const GYCHAR* str);
	//GYVOID	operator=(const GYWCHAR* str);
};
#endif

