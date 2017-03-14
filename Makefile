CC = gcc

.PHONY:	main.luarepo

main:	main.c
	gcc -o $@ main.c -llua
main.luarepo:	main.c
	git submodule update -i
	make CC=${CC} -C lua a
	gcc -o main main.c lua/liblua.a -ldl -lreadline -lm -DUSE_LUA_REPO
clean:	
	rm -rf main
	make -C lua clean
