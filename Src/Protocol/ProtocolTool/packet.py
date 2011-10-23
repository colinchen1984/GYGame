#coding=utf-8
from PacketGenerator import *
import ply.lex as lex
import fileAnalysis

def LoadTempFile(fileName):
	f = open(fileName, "r")
	template = ""
	for x in f.readlines():
		template += x
	return template
	
def CreateData(packet, valueConfig):
	# print valueConfig
	dataItem = None
	# print valueConfig
	if("Enum" == valueConfig["DataType"]):
		# 枚举类型的数组暂时不提供数组的支持
		# 处理已经定义过的枚举类型
		data = valueConfig["TypeName"].split(DataTypeSplitChar)
		dataCount = len(data)
		# print data
		if(2 == dataCount):
			dataItem = DataEnum(packet, data[1], valueConfig["ValueName"], valueConfig["ExtraParam"])
			
		# 尚未定义的枚举类型
		elif(3 == dataCount):
			dataTypeName = data[1]
			strings = data[2]
			if(("{" not in strings) or ("}" not in strings)):
				raise MyException("", ("Error Enmu type %s\t%s" % (valueConfig["TypeName"], valueConfig["ValueName"])))
			strings = strings.replace(" ", "").replace("{", "").replace("}", "").replace("\t", "").split(",")
			if(0 == len(strings)):
				raise MyException("", ("Error Enmu type %s" % valueConfig["TypeName"]))
			dataItem =  DataEnum(packet, dataTypeName, valueConfig["ValueName"], valueConfig["ExtraParam"], strings)	
	elif("String" == valueConfig["DataType"]):
		
		if(True == valueConfig["Array"]):
			data = valueConfig["TypeName"].replace("\t", "")
			data = data.replace(" ", "")
			data = data.split(DataTypeSplitChar)
			dataItem = DataArray(packet, "String", data[0], valueConfig["ValueName"], valueConfig["ExtraParam"], data[1])
		elif(False == valueConfig["Array"]):
			dataItem = DataString(packet, valueConfig["ValueName"], valueConfig["ExtraParam"])
		else:
			raise MyException("", ("Error Data Element %s, Array packet is wrong" % valueConfig["ValueName"]))
			
	elif("Struct" == valueConfig["DataType"]):
		if(True == valueConfig["Array"]):
			data = valueConfig["TypeName"].replace("\t", "")
			data = data.replace(" ", "")
			data = data.split(DataTypeSplitChar)
			dataItem = DataArray(packet, "Struct", data[0], valueConfig["ValueName"], valueConfig["ExtraParam"], data[1])
		elif(False == valueConfig["Array"]):
			dataItem = DataStruct(packet, valueConfig["TypeName"], valueConfig["ValueName"], valueConfig["ExtraParam"])
		else:
			raise MyException("", ("Error Data Element %s, Array packet is wrong" % valueConfig["ValueName"]))
		# print valueConfig
		
	elif(True == IsBasicType(valueConfig["DataType"])):
		if(True == valueConfig["Array"]):
			data = valueConfig["TypeName"].replace("\t", "")
			data = data.replace(" ", "")
			data = data.split(DataTypeSplitChar)
			dataItem = DataArray(packet, "Basic", data[0], valueConfig["ValueName"], valueConfig["ExtraParam"], data[1])
		elif(False == valueConfig["Array"]):
			dataItem = DataBase(packet, valueConfig["TypeName"], valueConfig["ValueName"], valueConfig["ExtraParam"])
		else:
			raise MyException("", ("Error Data Element %s, Array packet is wrong" % valueConfig["ValueName"]))
	
	if(None == dataItem):
		raise MyException("", ("Error Data Element %s" % valueConfig["ValueName"]))
	
	return dataItem
	
def CreateDataElement(packet, valueConfigs):
	
	for x in valueConfigs:
		try:
			packet.dataItems.append(CreateData(packet, x))
		except MyException, e:
			raise e
	return packet
	
def ProcessConfig(lexer):
	packet = None
	comment = ""
	valueConfigs = []
	while(True):
		result = None
		try:
			t = lexer.token()
			if(None != t):
				result = t.value
				# print result
		except Exception, e:
			raise e
			
		if(None == result):
			break;
		elif("Comment" == result["Type"]):
			comment = result["Value"]
		elif("End" == result["Type"]):
			break;
		elif("Begin" == result["Type"]):
			str = result["Value"]
			packet = PacketDefine()
			head = str.replace("[", "").replace("]", "").split(DataTypeSplitChar)
			packet.packetName = head[0]
			packet.packetID = head[1]
			packet.packetComment = comment.replace("#", "//")
		elif("Type" == result["Type"]):
			valueConfig = result
			valueConfig.pop("Type")
			valueConfigs.append(valueConfig)
		elif("ValueName" == result["Type"]):
			valueConfig = valueConfigs.pop()
			valueConfig["ValueName"] = result["ValueName"]
			valueConfig["NoCheck"] = True;			
			valueConfigs.append(valueConfig)
		elif("NoCheck" == result["Type"]):
			valueConfig = valueConfigs.pop()
			valueConfig["NoCheck"] = False;			
			valueConfigs.append(valueConfig)
		elif("ExtraParam" == result["Type"]):
			valueConfig = valueConfigs.pop()
			valueConfig["ExtraParam"] = result["Value"]
			valueConfig["ExtraParam"] = valueConfig["ExtraParam"].replace("]", "").replace("\n", "");		
			valueConfigs.append(valueConfig)
		else:
			raise MyException("", ("Error token type %s" % ["Type"]))
	if(None == packet):
		return packet
	if(None != packet and 0 >= len(valueConfigs)):
		raise MyException("", ("Error struct define"))
	else:
		# print config, valueConfigs
		packet = CreateDataElement(packet, valueConfigs)
	return packet

HeadFileTemplate = None
CppFileTemplate = None

HeadFileCharacter = {"$TIME$":PacketDefine.GetCurrentTime, "$FILENAME$":PacketDefine.GetPacketName, "$FILEUPPERNAME$":PacketDefine.GetPacketUpperName, "$INCLUDEFILE$":PacketDefine.GetIncludeFile, "$PREDECLARE$":PacketDefine.GetPreDeclare, "$PACKETCOMMENT$":PacketDefine.GetPacketComment, "$DATADECLARE$":PacketDefine.GetDeclare, "$CONSTRACTORPARAM$":PacketDefine.GetConstractorParam, "$CONSTRACTOR$":PacketDefine.GetConstractor, "$PACKETID$":PacketDefine.GetPacketID, "$DATASETGETDECLARE$":PacketDefine.GetDataGetSetDeclare, "$DATASETGETDECLARE$":PacketDefine.GetDataGetSetDeclare}

def OutPutHeadFile(packetConfig):
	packetName = packetConfig.GetPacketName()
	str = HeadFileTemplate
	for x in HeadFileCharacter.keys():
		str = str.replace(x, HeadFileCharacter[x](packetConfig))
	
	f = open(("../%s.h" % packetName), "wb")
	f.write(str)
	f.close()
	
CppFileCharacter = {"$TIME$":PacketDefine.GetCurrentTime, "$FILENAME$":PacketDefine.GetPacketName, "$SERIALIZ$":PacketDefine.GetSerializ, "$CLEANUP$":PacketDefine.GetCleanUp, "$DATASETGETDEFINE$":PacketDefine.GetDataGetSetDefine}

def OutPutCppFile(packetConfig):
	packetName = packetConfig.GetPacketName()
	str = CppFileTemplate
	for x in CppFileCharacter.keys():
		str = str.replace(x, CppFileCharacter[x](packetConfig))
	
	f = open(("../%s.cpp" % packetName), "wb")
	f.write(str)
	f.close()
	
def AddToVcproj(packetConfig):
	headFile = "\t\t<File\n\t\t\tRelativePath=\"..\..\Src\Protocol\%s.h\"\n\t\t\t>\n\t\t</File>" % packetConfig.GetPacketName()
	cppFile = "\t\t<File\n\t\t\tRelativePath=\"..\..\Src\Protocol\%s.cpp\"\n\t\t\t>\n\t\t</File>" % packetConfig.GetPacketName()
	filePath = "../../../Protocol/Protocol/Protocol.vcproj"
	tag = "\t</Files>"
	str = headFile + cppFile
	f = open(filePath, "r")
	data = f.read()
	f.close()
	if(-1 != data.find(packetConfig.GetPacketName())):
		print packetConfig.GetPacketName()
		return
		
	data = data.replace(tag, str)
	f = open(filePath, "w")
	f.write(data)
	f.close()
	
def OutPut(packetConfig):
	OutPutHeadFile(packetConfig)
	OutPutCppFile(packetConfig)
	AddToVcproj(packetConfig)

if __name__ == "__main__":
	logDefine = open("11", "r")
	lexer = lex.lex(module = fileAnalysis)
	lexer.input(logDefine.read())
	HeadFileTemplate = LoadTempFile("./PacketHeadTemplate")
	CppFileTemplate = LoadTempFile("./PacketCppTemplate")
	while(True):
		try:
			packetConfig = ProcessConfig(lexer)
			if(None != packetConfig):
				# print packetConfig
				OutPut(packetConfig)
				pass
			else:
				break
		except MyException, e:
			# print "error"
			print e