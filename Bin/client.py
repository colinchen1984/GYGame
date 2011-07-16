import socket
import time
G_ClientCount = 2500
G_ServerAddress = ("127.0.0.1", 5555)
s = []
for x in range(G_ClientCount):
	t = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.append((x, t))

del x
for x in s:
	x[1].connect(G_ServerAddress)
	time.sleep(0.001)
del x
while(1):
	for x in s:
		x[1].send("test")
		x[1].recv(4)
		print x[0], "\n"