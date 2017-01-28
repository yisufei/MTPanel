

all: MTPanel

MTPanel:
	@g++ -std=c++0x -I/usr/local/include \
	ConfigWithLua.cpp main.cpp ConfigBusiness.cpp -llua \
	MessageBase.cpp LoadLua.cpp \
	-luv -L/usr/local/lib/ -llua -ldl -lm -llog4cpp \
	-o MTPanel || exit -1
	@echo "compile succ"

clean:
	@rm -rf MTPanel
