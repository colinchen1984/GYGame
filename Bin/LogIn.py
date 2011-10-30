import socket
import time
import struct
import random
G_LoopCount = 1
G_ClientCount = 1
G_ServerAddress = ("127.0.0.1", 5555)
s = []
gGUID = []
for i in range(G_LoopCount):
	for x in range(G_ClientCount):
		t = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		try:
			t.connect(G_ServerAddress)	
			time.sleep(0.0001)
			userID = random.randrange(100, 1222554585500)
			gGUID.append(userID)
			userName = "TestName"
			testPacket = struct.pack("Q", userID)
			testPacket += struct.pack("h", len(userName))
			testPacket += struct.pack(("%ds" % len(userName)), userName)
			testPacket += struct.pack("hh", 1, 0)

			packetID = 0
			packetHead = struct.pack("hhc", 2, len(testPacket), '\0')

			testPacket = packetHead + testPacket
			s.append((testPacket, (t, i)))
		except :
			print x
	time.sleep(0.1)


count = 1
while(1):
	back = []
	for t in s:
		begin = time.time()
		print "Sending Packet"
		t[1][0].send(t[0])
		t[1][0].recv(11111)
	s = back
	time.sleep(0.1)
	count += 1
