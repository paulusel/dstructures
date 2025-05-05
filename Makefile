config:
	cmake -S . -B ./build -G Ninja -DBUILD_TEST=ON
compile:
	cmake --build ./build
clear:
	rm -rf build
all:
	$(MAKE) clear
	$(MAKE) config
	$(MAKE) compile
