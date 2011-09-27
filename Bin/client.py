import socket
import time
import struct
import random
G_LoopCount = 1
G_ClientCount = 44
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
			testPacket += struct.pack("h", len(userName))
			testPacket += struct.pack("h", len(userName))
			testPacket += struct.pack("h", len(userName))
			testPacket += struct.pack("h", len(userName))

			packetID = 0
			packetHead = struct.pack("hhc", 0, len(testPacket), '\0')

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
		print "Recving Packet"
		recvPacket = t[1][0].recv(len(t[0]) + 16)
		back.append(t)
		end = time.time() - begin
		timeData = recvPacket[len(t[0]) - 8:]
		
		gatewayReceive, logicReceive, gatewayReceiveLogic = struct.unpack("QQQ", timeData)
		# print "recv packet len %d, send packet len %d, count is %d spend %f" % (len(recvPacket), (len(t[0])) , count, end)
		print "%u\t%d\t%d\t%d\n" % (gGUID[t[1][1]], logicReceive-gatewayReceive, gatewayReceiveLogic-logicReceive, gatewayReceiveLogic-gatewayReceive)
		# except:
			# print t[0]
	s = back
	time.sleep(0.1)
	count += 1
