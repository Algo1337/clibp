DEBUG_FLAGS= -g -g3 -ggdb -ggdb3
.PHONY:

all: install

install:
	git clone https://github.com/algo1337/clibp.git
	cd clibp


compile_socket:
	gcc test/net/socket/socket.c \
	src/stdlib/*.c \
	src/net/*.c \
	${DEBUG_FLAGS}

compile_lib:
	gcc -c src/*.c \
	src/stdlib/*.c \
	src/net/*.c \
	-Wl,-z,noexecstack \
	${DEBUG_FLAGS}
	sudo ar rcs libclibp.a *.o; rm *.o; sudo mv libclibp.a /usr/local/lib/libclibp.a

move_headers:
	sudo cp -r headers/* /usr/local/include