all: anon_voting

anon_voting: anon_voting.c
	gcc -o anon_voting anon_voting.c -lm -g