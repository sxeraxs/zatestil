MAKEFLAGS=--no-print-directory

debug: header
	mkdir -p cmake-build-debug
	cd cmake-build-debug && cmake -DCMAKE_BUILD_TYPE=Debug -Wno-dev -G "Unix Makefiles" .. && cmake --build . --target all -- -j 10

release: header
	mkdir -p cmake-build-release
	cd cmake-build-release && cmake -DCMAKE_BUILD_TYPE=Release -Wno-dev -G "Unix Makefiles" .. && cmake --build . --target all -- -j 10

clear:
	rm -rf cmake-build-*

header:
	@if [ -d .git ]; then \
		echo "================================================================================"; \
		echo "Building at branch [`git rev-parse --abbrev-ref HEAD`]"; \
		echo "================================================================================"; \
		echo ""; \
	fi
