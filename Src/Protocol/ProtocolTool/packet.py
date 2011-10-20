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
	
def CreateData(valueConfig):
	# print valueConfig
	dataItem = None
	print valueConfig
	if("Enum" == valueConfig["DataType"]):
		# 枚举类型的数组暂时不提供数组的支持
		# 处理已经定义过的枚举类型
		data = valueConfig["TypeName"].split(DataTypeSplitChar)
		dataCount = len(data)
		# print data
		if(2 == dataCount):
			dataItem = DataEnum(data[1], valueConfig["ValueName"], valueConfig["ExtraParam"])
			
		# 尚未定义的枚举类型
		elif(3 == dataCount):
			dataTypeName = data[1]
			strings = data[2]
			if(("{" not in strings) or ("}" not in strings)):
				raise MyException("", ("Error Enmu type %s" % valueConfig["TypeName"]))
			strings = strings.replace(" ", "").replace("{", "").replace("}", "").replace("\t", "").split(",")
			if(0 == len(strings)):
				raise MyException("", ("Error Enmu type %s" % valueConfig["TypeName"]))
			dataItem =  DataEnum(dataTypeName, valueConfig["ValueName"], valueConfig["ExtraParam"], strings)	
	elif("String" == valueConfig["DataType"]):
		dataItem = DataString(valueConfig["ValueName"], valueConfig["ExtraParam"])
	elif("Struct" == valueConfig["DataType"]):
		strings = valueConfig["TypeName"]
		strings = strings.replace(" ", "")
		strings = strings.split(DataTypeSplitChar)
		typeName = strings[0]
		strings = strings[1:]
		print strings
		maxCount = strings[-1]
		param = None
		if(len(strings) > 1):
			strings = strings[:-1]
			count = len(strings)
			# print typeName, count % 2
			if(0 == count % 2):
				param = []
				for x in range(count / 2):
					a = strings[2 * x]
					b = strings[2 * x + 1]
					str = ""
					if(IsBasicType(a)):
						str = "Basic"
					else:
						str = "Enum"
					param.append([str, a, b])
		# print valueConfig
		dataItem = DataArray("Struct", typeName, valueConfig["ValueName"], valueConfig["ExtraParam"], isLast, maxCount, valueConfig["NoCheck"], param)
		
	elif(True == IsBasicType(valueConfig["DataType"])):
		if(True == valueConfig["Array"]):
			data = valueConfig["TypeName"].replace("\t", "")
			data = data.replace(" ", "")
			data = data.split(DataTypeSplitChar)
			dataItem = DataArray("Basic", data[0], valueConfig["ValueName"], valueConfig["ExtraParam"], isLast, data[1], valueConfig["NoCheck"], None)
		elif(False == valueConfig["Array"]):
			dataItem = DataBase(valueConfig["TypeName"], valueConfig["ValueName"], valueConfig["ExtraParam"])
		else:
			raise MyException("", ("Error Data Element %s, Array packet is wrong" % valueConfig["ValueName"]))
	
	if(None == dataItem):
		raise MyException("", ("Error Data Element %s" % valueConfig["ValueName"]))
	
	return dataItem
	
def CreateDataElement(packet, valueConfigs):
	
	for x in valueConfigs:
		try:
			packet.dataItems.append(CreateData(x))
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
			packet.packetComment = comment
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

def OutPut(logConfig):
	head = HeadFileTemplate.replace("$struct$", logConfig.structName)
	head = head.replace("$comment$", logConfig.LogEnumComment)
	head = head.replace("$data$", logConfig.MakeStrcutDateDefine())
	head = head.replace("$clean$", logConfig.MakeStructDataCleanUp())
	head = head.replace("$prevdefine$", logConfig.MakePrevDefine())
	
	f = open("./head.h", "a+")
	fileConten = f.readlines()
	contentLen = len(fileConten)
	if(0 == contentLen):
		fileConten.append("[enum]\n")
		fileConten.append("[struct]\n")
		fileConten.append("[function]\n")
	contentLen = len(fileConten)
	for x in range(contentLen):
		if("[struct]" in fileConten[x]):
			# 插入枚举声明
			fileConten[x-1] += "\n" + logConfig.logEnumName + "\t\t\t\t\t = " + logConfig.LogEnumNumber + ",\t\t\t\t\t//" + logConfig.LogEnumComment + "\n"
		elif("[function]" in fileConten[x])	:
			# 插入数据结构定义
			fileConten[x-1] += "\n" + head + "\n"
	f.truncate(0)
	f.writelines(fileConten)
	f.close()
	f = open("./cpp.cpp", "a+")
	cpp = CppFileTemplate.replace("$struct$", logConfig.structName)
	cpp = cpp.replace("$function$", logConfig.logFunctionName)
	cpp = cpp.replace("$logem$", logConfig.logEnumName)
	cpp = cpp.replace("$logcheck$", logConfig.MakeCheck())
	cpp = cpp.replace("$format$", logConfig.MakeFormat())
	f.write(cpp)
	f.close()
	
	
HeadFileTemplate = None
CppFileTemplate = None

if __name__ == "__main__":
	logDefine = open("11", "r")
	lexer = lex.lex(module = fileAnalysis)
	lexer.input(logDefine.read())
	# HeadFileTemplate = LoadTempFile("./head")
	# CppFileTemplate = LoadTempFile("./cpp")
	while(True):
		try:
			logConfig = ProcessConfig(lexer)
			if(None != logConfig):
				# print logConfig
				# OutPut(logConfig)
				pass
			else:
				break
		except MyException, e:
			# print "error"
			print e