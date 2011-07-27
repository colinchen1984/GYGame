import socket
import time
G_LoopCount = 29
G_ClientCount = 32
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
	print i
	time.sleep(2)
time.sleep(10000)



while(1):
	back = []
	for t in s:
		try:
			print "%d OK" % t[0]
			t[1].send("test")
			t[1].recv(4)
			back.append(t)
		except:
			print t[0]
	s = back
