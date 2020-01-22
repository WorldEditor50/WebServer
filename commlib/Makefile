SRC_SUBDIR := $(shell find ./ -type d -not -wholename './')
SRC_FULL_PATH = $(foreach dir,$(SRC_SUBDIR),$(wildcard $(dir)/*.cpp))
INC_FULL_PATH = $(foreach dir,$(SRC_SUBDIR),$(wildcard $(dir)/*.h))
OBJ_FULL_PATH = $(patsubst %.cpp, %.o, $(SRC_FULL_PATH))

TARGET := cwslib.a

$(TARGET):$(OBJ_FULL_PATH) $(INC_FULL_PATH)
        ar crv $(TARGET) $(OBJ_FULL_PATH)

.PHONY:clean
clean:
        rm -f $(TARGET) $(OBJ_FULL_PATH)