import socket
import time
import struct
G_LoopCount = 1
G_ClientCount = 1
G_ServerAddress = ("127.0.0.1", 5555)
s = []
for i in range(G_LoopCount):
	for x in range(G_ClientCount):
		t = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		try:
			t.connect(G_ServerAddress)	
			time.sleep(0.0001)
			s.append((x, t))
		except :
			print x
	time.sleep(2)



userID = 1001
userName = "TestName"
testPacket = struct.pack("i", userID)
testPacket += struct.pack("h", len(userName))
testPacket += struct.pack(("%ds" % len(userName)), userName)

packetID = 0
packetHead = struct.pack("hhc", 0, len(testPacket), '\0')

testPacket = packetHead + testPacket

print "%s\t%d" % (testPacket, len(testPacket))
count = 1
while(1):
	back = []
	for t in s:
		# try:
		t[1].send(testPacket)
		recvPacket = t[1].recv(len(testPacket))
		back.append(t)
		print "recv packet len %d, send packet len %d, count is %d" % (len(recvPacket), len(testPacket) , count)
		# except:
			# print t[0]
	s = back
	time.sleep(1)
	count += 1
