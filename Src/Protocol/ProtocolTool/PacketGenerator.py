#coding=utf-8

DataTypeSplitChar = ":"

class PacketDefine():
	def __init__(self):
		self.packetName = ""
		self.packetID = ""
		self.packetComment = ""
		self.dataItems = []
		
		
class MyException():
	def __init__(self, reason, data):
		self.reason = reason
		self.data = data
		
	def __repr__(self):
		return self.data
		
class DataElement:
	def __init__(self, dataType, dataName, comment):
		self.m_dataName = dataName;
		self.m_comment = "//" + comment
		self.m_dataType = dataType
		
		
def IsBasicType(dataType):
	result = DataBase.m_baseType.has_key(dataType)
	return result
		
class DataBase(DataElement):
	m_baseType = {"int":["GYINT32", "INVALID_VALUE"],
					"float":["GYFLOAT", "INVALID_VALUE"],
					"uint":["GYUINT32", "INVALID_VALUE"],
					"char":["GYCHAR", "INVALID_VALUE"],
					"guid":["GYGUID", "INVALID_VALUE"],
					"int64":["GYINT64", "INVALID_VALUE"],
					"uint64":["GYUINT64", "INVALID_VALUE"],
					"bool":["GYBOOL", "INVALID_VALUE"],
				}
	
	def __init__(self, dataType, dataName, comment):
		if(True != DataBase.m_baseType.has_key(dataType)):
			raise "DataBase can't handle data type %s" % dataType
		DataElement.__init__(self, dataType, dataName, comment)

		
class DataEnum(DataElement):
	
	def __init__(self, dataType, dataName, comment, enumDefine = None):
		DataElement.__init__(self, dataType, dataName, comment)
		self.m_enumDefine = enumDefine
		
class DataString(DataElement):
	m_baseEnum = {}
	def __init__(self, dataName, comment):
		DataElement.__init__(self, "GYString", dataName, comment)
	
def DataFactory(dataClass, dataType, dataName, comment):
	result = None
	if("Basic" == dataClass):
		result = DataBase(dataType, dataName, comment)
	elif("Enum" == dataClass):
		result = DataEnum(dataType, dataName, comment)
	elif("Struct" == dataClass):
		result = DataStruct(dataType, dataName, comment)
	return result
	
# structDefine结构为[a,...]
# a的结构为[数据object的类型, 数据类型名， 数据名，[]]
# 剩下的参数可以用来为枚举的参数,也可以是内嵌的struct的相关参数
class DataStruct(DataElement):

	def __init__(self, dataType, dataName, comment):
		self.m_structDefine = ""
		
class DataArray(DataElement):
	def __init__(self, dataClass, dataType, dataName, comment, maxCount):
		self.m_maxCount = maxCount
		DataElement.__init__(self, dataType, dataName, comment)
		