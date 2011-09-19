import socket
import time
import struct
import random
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
			userID = random.randrange(12225545855, 1222554585500)
			userName = "TestName"
			testPacket = struct.pack("Q", userID)
			testPacket += struct.pack("h", len(userName))
			testPacket += struct.pack(("%ds" % len(userName)), userName)

			packetID = 0
			packetHead = struct.pack("hhc", 0, len(testPacket), '\0')

			testPacket = packetHead + testPacket
			s.append((testPacket, t))
		except :
			print x
	time.sleep(0.1)


count = 1
while(1):
	back = []
	for t in s:
		begin = time.time()
		t[1].send(t[0])
		recvPacket = t[1].recv(len(t[0]))
		back.append(t)
		end = time.time() - begin
		print "recv packet len %d, send packet len %d, count is %d spend %f" % (len(recvPacket), (len(t[0])) , count, end)
		# except:
			# print t[0]
	s = back
	time.sleep(0.1)
	count += 1
