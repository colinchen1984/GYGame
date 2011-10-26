#coding=utf-8
import time
DataTypeSplitChar = ":"

class PacketDefine():
	def __init__(self):
		self.packetName = ""
		self.packetID = ""
		self.packetComment = ""
		self.dataItems = []
		
	def GetCurrentTime(self):
		TimeTemplate = time.localtime()
		timeTemplate = ("%d/%02d/%02d %02d:%02d") % (TimeTemplate.tm_year, TimeTemplate.tm_mon, TimeTemplate.tm_mday, TimeTemplate.tm_hour, TimeTemplate.tm_min)
		return timeTemplate
		
	def GetPacketName(self):
		return self.packetName
	
	def GetPacketUpperName(self):
		return self.packetName.upper()
	
	def GetPacketID(self):
		return self.packetID
		
	def GetPacketComment(self):
		return self.packetComment	
		
	def GetDeclare(self):
		str = "\t"
		for x in self.dataItems:
			str += x.GetDeclare()
		return str
		
	def GetPreDeclare(self):
		str = ""
		for x in self.dataItems:
			str += x.GetPreDeclare()
		return str
	
	def GetCleanUp(self):
		str = "\t"
		for x in self.dataItems:
			str += x.GetCleanUp()
		return str
		
	def GetConstractorParam(self):
		str = ""
		for x in self.dataItems:
			param = x.GetConstractorParam()
			if("" != param):
				if("" != str):
					param = ", " + param
				str += param
		return str	
		
	def GetConstractor(self):
		str = ""
		for x in self.dataItems:
			param = x.GetConstractor()
			if("" != param):
				if("" != str):
					param = ", " + param
				str += param
		if("" != str):
			str = ":" + str
		return str
		
	def GetIncludeFile(self):
		str = ""
		for x in self.dataItems:
			str += x.GetIncludeFile()
		return str
		
	def GetDataGetSetDeclare(self):
		str = ""
		for x in self.dataItems:
			str += x.GetDataGetSetDeclare()
		return str

	def GetSerializ(self):
		str = ""
		for x in self.dataItems:
			str += x.GetSerializ()
		return str
		
	def GetDataGetSetDefine(self):
		str = ""
		for x in self.dataItems:
			str += x.GetDataGetSetDefine()
		return str
		
class MyException():
	def __init__(self, reason, data):
		self.reason = reason
		self.data = data
		
	def __repr__(self):
		return self.data
		
class DataElement:
	def __init__(self, packet, dataType, dataName, comment):
		self.m_dataName = dataName;
		self.m_comment = "//" + comment
		self.m_dataType = dataType
		self.m_packet = packet
		
	def GetDeclare(self):
		return ""
	
	def GetPreDeclare(self):
		return ""
	
	def GetCleanUp(self):
		return ""
		
	def GetConstractorParam(self):
		return ""
	
	def GetConstractor(self):
		return ""
		
	def GetIncludeFile(self):
		return ""
		
	def GetDataGetSetDeclare(self):
		return ""
		
	def GetSerializ(self):
		return ""
		
	def GetDataGetSetDefine(self):
		return ""
		
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
	
	def __init__(self, packet, dataType, dataName, comment):
		if(True != DataBase.m_baseType.has_key(dataType)):
			raise "DataBase can't handle data type %s" % dataType
		DataElement.__init__(self, packet, dataType, dataName, comment)

	def GetDeclare(self):
		str = DataBase.m_baseType[self.m_dataType][0]
		str += "\t\t\t"
		str += self.m_dataName + ";"
		str += "\t\t\t"
		str += self.m_comment
		str += "\n\t"
		return str
		
	def GetCleanUp(self):
		str = self.m_dataName;
		str += " = ";
		str += DataBase.m_baseType[self.m_dataType][1]
		str += ";"
		str += "\n\t"
		return str
		
	def GetDataGetSetDeclare(self):
		str = "\tGYINLINE\tGYVOID\t\t\tSet%s(%s value){ %s = value; };\n" % (self.m_dataName, DataBase.m_baseType[self.m_dataType][0], self.m_dataName)
		str += "\tGYINLINE\t%s\t\t\tGet%s() const { return %s; };\n" % (DataBase.m_baseType[self.m_dataType][0], self.m_dataName, self.m_dataName)
		return str
			
	def GetSerializ(self):
		return "\tserializer << %s;\n" % self.m_dataName
		
class DataEnum(DataElement):
	m_offerIncludeFile = False
	def __init__(self, packet, dataType, dataName, comment, enumDefine = None):
		DataElement.__init__(self, packet, dataType.upper(), dataName, comment)
		self.m_enumDefine = enumDefine
		
	def GetDeclare(self):
		str = "GYStaticAssert(%s_COUNT <= MAX_CHAR, \"Can't put enum %s in to char\");\n\t" % (self.m_dataType, self.m_dataName)
		str += "GYCHAR"
		str += "\t\t\t"
		str += self.m_dataName + ";"
		str += "\t\t\t"
		str += self.m_comment
		str += "\n\t"
		return str
	
	def GetPreDeclare(self):
		str = ""
		if(self.m_enumDefine):
			str = "enum %s\n" % self.m_dataType
			str += "{\n"
			str += "\t%s_INVALID = INVALID_VALUE,\n" % self.m_dataType
			for x in self.m_enumDefine:
				str += "\t%s_%s,\n" % (self.m_dataType.upper(), x.upper())
			str += "\t%s_COUNT,\n" % self.m_dataType
			str += "};"
		return str
		
	def GetCleanUp(self):
		str = self.m_dataName;
		str += " = ";
		str += self.m_dataType
		str += "_INVALID;"
		str += "\n\t"
		return str
		
	def GetIncludeFile(self):
		str = ""
		if(False == DataEnum.m_offerIncludeFile):
			str = "#include \"static_assert.hpp\"\n"
			DataString.m_offerIncludeFile = True
		return str
				
	def GetDataGetSetDeclare(self):
		str = "\tGYINLINE\tGYVOID\t\t\tSet%s(%s value){ %s = static_cast<GYCHAR>(value);};\n" % (self.m_dataName.upper(), self.m_dataType, self.m_dataName)
		str += "\tGYINLINE\t%s\t\t\tGet%s() const { return static_cast<%s>(%s); };\n" % (self.m_dataType, self.m_dataName.upper(), self.m_dataType, self.m_dataName)
		return str
		
	def GetSerializ(self):
		return "\tserializer << %s;\n" % self.m_dataName
		
class DataString(DataElement):
	m_offerContactorParam = {}
	def __init__(self, packet, dataName, comment):
		DataElement.__init__(self, packet, "GYString", dataName, comment)
		DataString.m_offerContactorParam[packet] = False
		
	def GetDeclare(self):
		str = self.m_dataType
		str += "\t\t\t"
		str += self.m_dataName + ";"
		str += "\t\t\t"
		str += self.m_comment
		str += "\n\t"
		return str
		
	def GetCleanUp(self):
		str = self.m_dataName
		str += ".CleanUp();"
		str += "\n\t"
		return str
	
	def GetConstractorParam(self):
		str = ""
		if(False == DataString.m_offerContactorParam[self.m_packet]):
			str = "GYStringManager& stringManager"
			DataString.m_offerContactorParam[self.m_packet] = True
		return str
		
	def GetConstractor(self):
		str = self.m_dataName + "(stringManager)"
		return str
		
	def GetIncludeFile(self):
		return "#include \"GYString.h\"\n"
		
	def GetDataGetSetDeclare(self):
		str = "\tGYINLINE\tGYVOID\t\t\tSet%s(const GYString& value){ %s = value; };\n" % (self.m_dataName, self.m_dataName)
		str += "\tGYINLINE\tconst GYString&\t\t\tGet%s() const { return %s; };\n" % (self.m_dataName, self.m_dataName)
		return str
		
	def GetSerializ(self):
		return "\tserializer << %s;\n" % self.m_dataName
		
def DataFactory(packet, dataClass, dataType, dataName, comment):
	result = None
	if("Basic" == dataClass):
		result = DataBase(packet, dataType, dataName, comment)
	elif("Enum" == dataClass):
		result = DataEnum(packet, dataType, dataName, comment)
	elif("Struct" == dataClass):
		result = DataStruct(packet, dataType, dataName, comment)
	elif("String" == dataClass):
		result = DataString(packet, dataName, comment)
	return result
	
class DataStruct(DataElement):

	def __init__(self, packet, dataType, dataName, comment):
		DataElement.__init__(self, packet, dataType, dataName, comment)
		self.m_structDefine = ""
		
	def GetDeclare(self):
		str = self.m_dataType
		str += "\t\t\t"
		str += self.m_dataName + ";"
		str += "\t\t\t"
		str += self.m_comment
		str += "\n\t"
		return str
	
	def GetCleanUp(self):
		str = self.m_dataName;
		str += ".CleanUp();"
		str += "\n\t"
		return str
		
	def GetDataGetSetDeclare(self):
		str = "\tGYINLINE\tGYVOID\t\t\tSet%s(const %s& value){ %s = value; };\n" % (self.m_dataName.upper(), self.m_dataType, self.m_dataName)
		str += "\tGYINLINE\tconst %s&\t\t\tGet%s() const { return %s; };\n" % (self.m_dataType, self.m_dataName.upper(), self.m_dataName)
		return str
		
	def GetSerializ(self):
		return "\t%s.Serializer(serializer);\n" % self.m_dataName
		
class DataArray(DataElement):
	def __init__(self, packet, dataClass, dataType, dataName, comment, maxCount):
		self.m_maxCount = maxCount
		DataElement.__init__(self, packet, dataType, dataName, comment)
		self.dataObject = DataFactory(packet, dataClass, dataType, dataName + "[i]", comment)
		self.m_currentCountName = self.m_dataName.upper() + "CURRENTCOUNT"
		self.m_dataType = self.dataObject.m_dataType
		
	def GetDeclare(self):
		str = self.m_dataType
		str += "\t\t\t"
		str += self.m_dataName + "[" + self.m_maxCount + "]" + ";"
		str += "\t\t\t"
		str += self.m_comment
		str += "\n\t"
		str += "GYINT16"
		str += "\t\t\t"
		str += self.m_currentCountName
		str += "\n\t"
		return str
		
	def GetCleanUp(self):
		str = """for(GYINT32 i = 0; i < %s, ++i)
		{
			%s
		}\n\t""" % (self.m_maxCount, self.dataObject.GetCleanUp())
		str += self.m_currentCountName + " = 0;\n\t"
		return str
		
	def GetDataGetSetDeclare(self):
		str = "\tGYBOOL\t\t\tAdd%s(const %s& value);\n\t" % (self.m_dataName, self.m_dataType)
		str += "\tconst %s&\t\t\tGet%s(GYINT32 index) const ;\n\t" % (self.m_dataType, self.m_dataName)
		str += "\tGYINT16\t\t\tGet%sCount() const ;\n\t" % (self.m_dataName)
		return str
		
	def GetSerializ(self):
		count = "\tserializer << %s;\n" % self.m_currentCountName
		data = '''\tfor(GYINT32 i = 0; i < %s; ++i)
	{
		%s
	}
		\n''' % (self.m_currentCountName, self.dataObject.GetSerializ())
		return count + data
		
	def GetDataGetSetDefine(self):
		add = '''GYBOOL %s::Add%s(const %s& value)
{
	GYBOOL result = GYFALSE;
	if(%s < %s)
	{
		%s[%s++] = value;
		result = GYTRUE;
	}
	return result;
}
		\n'''	% (self.m_packet.GetPacketName(), self.m_dataName, self.m_dataType, self.m_currentCountName, self.m_maxCount, self.m_dataName, self.m_currentCountName)
		
		get = '''const %s& %s::Get%s(GYINT32 index) const 
{
	GYAssert(index < %s);
	return %s[index];
}
		\n''' % (self.m_dataType, self.m_packet.GetPacketName(), self.m_dataName, self.m_currentCountName, self.m_dataName)
		
		count = '''GYINT16 %s::Get%sCount() const 
{
	return %s;
}
		\n''' % (self.m_packet.GetPacketName(), self.m_dataName, self.m_currentCountName)
		return add + get + count
		