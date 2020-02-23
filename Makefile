INC_PATH = ./
SRC_FILE = main.cpp
COM_FLAG = -std=c++11
#COM_FLAG+=__VERSION_FOR_RELEASE__
COMM_LIB = commlib/cwslib.a
OUT_FILE = cws_server

COM_OBJ_FILE = 

TEST_PATH = ./test/
TEST_SRC_FILE = $(wildcard $(TEST_PATH)*.cpp)
TEST_HED_FILE = $(wildcard $(TEST_PATH)*.h)
TEST_OBJ_FILE = $(patsubst %.cpp, %.o, $(TEST_SRC_FILE))
COM_OBJ_FILE += $(TEST_OBJ_FILE)

$(OUT_FILE) : $(COMM_LIB) $(SRC_FILE) $(COM_OBJ_FILE)
	g++ -o $(OUT_FILE) $(SRC_FILE) $(COM_OBJ_FILE) $(COMM_LIB) $(COM_FLAG)

%.o:%.cpp
	g++ -o $@ -c -I$(INC_PATH) $< $(COM_FLAG)

.PHONY:clean
clean:
	rm -f $(OUT_FILE) $(COMM_LIB) $(COM_OBJ_FILE)
