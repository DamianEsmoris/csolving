COMMON_PATH=../../common/
CFLAGS=-I$(COMMON_PATH) -lm
BUILD_PATH=./builds/
COMMON_OBJS=$(COMMON_PATH)/linklst.o $(COMMON_PATH)/str.o $(COMMON_PATH)/hashmap.o

SRC_FOLDERS := $(shell ls -d ./[0-9][0-9])

all: $(SRC_FOLDERS)

$(SRC_FOLDERS): $(COMMON_OBJS)
	mkdir -pv $(BUILD_PATH) 
	gcc $@/*.c  -o $(BUILD_PATH)/$@-`ls $@/*.c | sed  -e 's:[0-9]\{2\}\/::' -e 's:.c$$::'` $(CFLAGS) $(COMMON_OBJS)
clean:
	rm -rf $(BUILD_PATH)*
