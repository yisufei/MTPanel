

all: MTPanel

MTPanel:
	@g++ -I/usr/local/include \
	ConfigWithLua.cpp main.cpp ConfigBusiness.cpp -llua \
	MessageBase.cpp LoadLua.cpp \
	-luv -L/usr/local/lib/ -llua -ldl -lm \
	-o MTPanel || exit -1
	@echo "compile succ"

clean:
	@rm -rf MTPanel
