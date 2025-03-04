default_target:
	cmake -S . -B build && cd build && make

test:
	cd build && ./test

clean:
	rm -rf build

.PHONY: default_target test clean