server:
	g++ client.cpp src/MyEpoll.cpp  src/MySocket.cpp src/MyChannel.cpp src/MyEventLoop.cpp src/MyServer.cpp src/MyAcceptor.cpp src/MyConnection.cpp -o client && \
	g++  server.cpp src/MyEpoll.cpp  src/MySocket.cpp src/MyChannel.cpp src/MyEventLoop.cpp src/MyServer.cpp src/MyAcceptor.cpp src/MyConnection.cpp -o server
clean:
	rm server && rm client