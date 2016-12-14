all: shellcoder testShellcode
CFLAGS=-Wno-int-to-pointer-cast -Wno-format
clean:
		rm -f shellcoder testShellcode
