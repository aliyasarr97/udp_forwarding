all : 
	gcc -pthread udp_forward.c -o udp_forward
remove :
	rm udp_forward
