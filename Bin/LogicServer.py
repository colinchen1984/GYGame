import socket
import time
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(("127.0.0.1", 5556))
s.listen(5)
c = []
while(1):
	c.append(s.accept())
print c
c = c[0]
c.setblocking(False)
while True:
	try:
		content = c.recv(1024)
		print content
		c.send(content)
	except:
		time.sleep(0.1)
	