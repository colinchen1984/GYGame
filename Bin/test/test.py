import random
rangeBeing = 1
rangeEnd = 100000000000L

numCountForEachRow = 4
rowCount = 1024 * 8
numCount = numCountForEachRow * rowCount
numMap = {}
while(len(numMap) < numCount):
	num = random.randrange(rangeBeing, rangeEnd)
	if(False == numMap.has_key(num)):
		numMap[num] = 0


file = open("test.tab", "wb")
rowCount = 0
columCount = 0
str = ""
for x in numMap.iterkeys():
	if(0 == columCount):
		str ="%d\t%u\t" % (rowCount, x) 
		columCount = columCount + 1
	elif(numCountForEachRow - 1 > columCount % numCountForEachRow):
		str ="%u\t" % (x) 
		columCount = columCount +1
	else:
		str ="%u\n" % (x)
		columCount = 0
		rowCount = rowCount + 1
	file.write(str)
file.close()