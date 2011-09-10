#-*- coding: utf-8 -*-
import dircache
import os
import string

def GetDir(path, *filter):
	folderList = []
	list = dircache.listdir(path)
	for x in list:
		if(x not in filter):
			folderList.append(x)
	list = []
	for x in folderList:
		if(True == os.path.isdir(x)):
			list.append(x)
	return list
	
def GetFileList(pathNameList, *filter):
	fileList = []
	filterLen = len(filter)
	if(pathNameList is None):
		pathNameList = [] 
		pathNameList.append(os.getcwd())
	for pathName in pathNameList:
		print pathName
		for root, dirs, files in os.walk(pathName, True):
			for fileName in files:
				if(0 == filterLen):
					fileList.append(os.path.join(root, fileName))
				else:
					fileExt = os.path.splitext(fileName)
					fileExt = fileExt[1]
					if(fileExt in filter):
						fileList.append(os.path.join(root, fileName))
	return fileList

if "__main__" == __name__:
	cwd = os.getcwd()
	dirList = GetDir(cwd, ".svn")
	fileList = GetFileList([cwd], ".cpp")
	finalMap = {}
	for d in dirList:
		finalMap[d] = []
	DEFAULT = "SOURCE_DEFULT"
	finalMap[DEFAULT] = []
	for f in fileList:
		fileStr = f.replace(cwd, "")
		relatedPath = fileStr.replace("\\", "/")
		fileStr = "." + relatedPath
		for d in dirList:
			pathList = relatedPath.split("/")
			if(d in pathList):
				finalMap[d].append(fileStr)
				break
		else:
			finalMap[DEFAULT].append(fileStr)
	
	template = "SET($HEAD$ $FILES$)"
	fileList = ""
	headList = ""
	for f in finalMap:
		str = string.join(finalMap[f], " ")
		str = template.replace("$FILES$", str)
		str = str.replace("$HEAD$", f.upper())
		fileList += str + "\n"
		headList += "${" + f.upper() + "} "
	file = open("CMakeListsTemplate", "rb")
	data = file.read()
	file.close
	data = data.replace("$FILELIST$", fileList)
	data = data.replace("$HEADLIST$", headList)
	file = open("CMakeLists.txt", "wb")
	file.write(data)
	file.close()