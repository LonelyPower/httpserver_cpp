CXX = g++
CXXFLAGS = -pthread
SRC = src/MyEpoll.cpp src/MySocket.cpp src/MyChannel.cpp src/MyEventLoop.cpp \
      src/MyServer.cpp src/MyAcceptor.cpp src/MyConnection.cpp src/MyThreadPool.cpp
OBJ = $(SRC:.cpp=.o)

all: client server test

client: client.cpp $(OBJ)
	$(CXX) $^ $(CXXFLAGS) -o $@

server: server.cpp $(OBJ)
	$(CXX) $^ $(CXXFLAGS) -o $@

test: test.cpp $(OBJ)
	$(CXX) $^ $(CXXFLAGS) -o $@

clean:
	rm -f client server test $(OBJ)
