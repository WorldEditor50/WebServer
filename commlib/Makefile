SRC_SUBDIR := $(shell find ./ -type d -not -wholename './')
SRC_FULL_PATH = $(foreach dir,$(SRC_SUBDIR),$(wildcard $(dir)/*.cpp))
INC_FULL_PATH = $(foreach dir,$(SRC_SUBDIR),$(wildcard $(dir)/*.h))
OBJ_FULL_PATH = $(patsubst %.cpp, %.o, $(SRC_FULL_PATH))
COMPILE_INC = -I ../
OPTIONS = -std=c++11
OPTIONS += $(COMPILE_INC)

TARGET := cwslib.a

$(TARGET):$(OBJ_FULL_PATH) $(INC_FULL_PATH)
	ar crv $(TARGET) $(OBJ_FULL_PATH) $(OPTIONS)

%.o:%.cpp
	g++ -o $@ -c $< $(OPTIONS)

.PHONY:clean
clean:
	rm -f $(TARGET) $(OBJ_FULL_PATH)