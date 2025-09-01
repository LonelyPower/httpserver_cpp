server:
	g++ client.cpp src/MyEpoll.cpp  src/MySocket.cpp src/MyChannel.cpp src/MyEventLoop.cpp src/MyServer.cpp  -o client && \
	g++  server.cpp src/MyEpoll.cpp  src/MySocket.cpp src/MyChannel.cpp src/MyEventLoop.cpp src/MyServer.cpp  -o server
clean:
	rm server && rm client