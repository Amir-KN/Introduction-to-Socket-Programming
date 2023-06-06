CC = g++
BUILD_DIR = build
CPP_DIR = src
HPP_DIR = headers
STD = -std=c++11

SERVER_EXE_FILE = server.out
CLIENT_EXE_FILE = client.out

all: $(BUILD_DIR) $(SERVER_EXE_FILE) \
	$(BUILD_DIR) $(CLIENT_EXE_FILE)


SERVER_OBJECTS = \
	$(BUILD_DIR)/server.o 

CLIENT_OBJECTS = \
	$(BUILD_DIR)/client.o 


ServerSensitivityList = \
	$(CPP_DIR)/server.cpp \
	$(HPP_DIR)/server.hpp

ClientSensitivityList = \
	$(CPP_DIR)/client.cpp \
	$(HPP_DIR)/client.hpp


$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)


$(BUILD_DIR)/server.o: $(ServerSensitivityList)
	$(CC) $(STD) -c $(CPP_DIR)/server.cpp -o $(BUILD_DIR)/server.o

$(BUILD_DIR)/client.o: $(ClientSensitivityList)
	$(CC) $(STD) -c $(CPP_DIR)/client.cpp -o $(BUILD_DIR)/client.o


$(SERVER_EXE_FILE): $(SERVER_OBJECTS)
	$(CC) $(STD) $(SERVER_OBJECTS) -o $(SERVER_EXE_FILE)

$(CLIENT_EXE_FILE): $(CLIENT_OBJECTS)
	$(CC) $(STD) $(CLIENT_OBJECTS) -o $(CLIENT_EXE_FILE)



.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) *.o *.out