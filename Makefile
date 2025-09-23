CXX = g++
CXXFLAGS = -pthread -O2 -DNDEBUG

SRC = src/MyEpoll.cpp src/MySocket.cpp src/MyChannel.cpp \
      src/MyEventLoop.cpp src/MyServer.cpp src/MyAcceptor.cpp \
      src/MyConnection.cpp src/MyThreadPool.cpp

OBJ = $(SRC:.cpp=.o)

all: client server test clean_obj

client: client.cpp $(OBJ)
	$(CXX) $^ $(CXXFLAGS) -o $@

server: server.cpp $(OBJ)
	$(CXX) $^ $(CXXFLAGS) -o $@

test: test.cpp $(OBJ)
	$(CXX) $^ $(CXXFLAGS) -o $@

clean:
	rm -f client server test $(OBJ)

clean_obj:
	rm -f $(OBJ)
