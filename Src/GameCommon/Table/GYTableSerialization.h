/////////////////////////////////////////////
// create time: 2011/9/3 18:39
// author:	colin chen
// file name:	GYTableSerialization
// file type:	h
////////////////////////////////////////////

#ifndef __GYTABLESERIALIZATION_H__
#define __GYTABLESERIALIZATION_H__

#include "GYSerialization.h"
class GYString;
enum EM_TABLE_DATA_TYPE;

class GYTableSerialization : public GYSerializationInteface
{
	GYINT32	m_tableRowCount;
	GYINT32	m_tableColumCount;
	GYINT32 m_fileSize;
	GYINT32 m_serializationDataCount;
	const GYCHAR* m_dataStart;
#ifdef CHECK_DATA_TYPE
	EM_TABLE_DATA_TYPE* m_dataType;
	GYINT32				m_loadColumCount;
#endif

public:
	explicit GYTableSerialization();

	virtual ~GYTableSerialization();
	
	GYINT32 Init(const GYString& fileName);

	GYINLINE GYINT32 GetTableDataRowCount(){return m_tableRowCount - 2;};

	GYINLINE GYINT32 GetTableDataColumCount(){return m_tableColumCount;};

	virtual GYSerializationInteface& operator<<(GYCHAR& value);

	virtual GYSerializationInteface& operator<<(GYUINT8& value);

	virtual GYSerializationInteface& operator<<(GYINT16& value);

	virtual GYSerializationInteface& operator<<(GYUINT16& value);

	virtual GYSerializationInteface& operator<<(GYINT32& value);

	virtual GYSerializationInteface& operator<<(GYUINT32& value);

	virtual GYSerializationInteface& operator<<(GYINT64& value);

	virtual GYSerializationInteface& operator<<(GYUINT64& value);

	virtual GYSerializationInteface& operator<<(GYFLOAT& value);

	virtual GYSerializationInteface& operator<<(GYString& value);

#ifdef CHECK_DATA_TYPE
	GYVOID	BeginLoadRowData(){m_loadColumCount = 0;};
#endif

private:
	const GYCHAR* _GetLine(const GYCHAR* pData, GYBOOL processCommetChar);

	const GYCHAR* _GetDataString(const GYCHAR* pData);

	const GYCHAR* _GetDataString(const GYCHAR* pStart, const GYCHAR* pEnd);

	GYVOID _CleanUp();
};
#endif
