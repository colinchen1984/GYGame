import socket
import time
G_ClientCount = 24
G_ServerAddress = ("127.0.0.1", 5555)
s = []
for x in range(G_ClientCount):
	t = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	try:
		t.connect(G_ServerAddress)	
		time.sleep(0.0001)
		s.append((x, t))
	except :
		print x


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