server:main.o worker.o tcpInit.o epoll.o sendfd.o transfile.o
	gcc main.o worker.o tcpInit.o epoll.o  transfile.o sendfd.o -o server -lpthread
main.o:main.c
	gcc -c main.c -o main.o -g -Wall
worker.o:worker.c
	gcc -c worker.c -o worker.o -g -Wall 
tcpInit.o:tcpInit.c
	gcc -c tcpInit.c -o tcpInit.o -g -Wall
epoll.o:epoll.c
	gcc -c epoll.c -o epoll.o -g -Wall
sendfd.o:sendfd.c
	gcc -c sendfd.c -o sendfd.o -g -Wall 
transfile.o:transfile.c
	gcc -c transfile.c -o transfile.o -g -Wall
