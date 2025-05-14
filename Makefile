CC=gcc
CFLAGS=-I.

client: client.c requests.c helpers.c buffer.c parson.c
	$(CC) -o client client.c requests.c helpers.c buffer.c parson.c -Wall

run: client
	./client

clean:
	rm -f *.o client





# CC = gcc
# CFLAGS = -O0 -g -Wall -Wextra -std=c89 -pedantic-errors -DTESTS_MAIN

# CPPC = g++
# CPPFLAGS = -O0 -g -Wall -Wextra -DTESTS_MAIN 

# all: test testcpp test_hash_collisions

# .PHONY: test testcpp test_hash_collisions
# test: tests.c parson.c
# 	$(CC) $(CFLAGS) -o $@ tests.c parson.c
# 	./$@

# testcpp: tests.c parson.c
# 	$(CPPC) $(CPPFLAGS) -o $@ tests.c parson.c
# 	./$@

# test_hash_collisions: tests.c parson.c
# 	$(CC) $(CFLAGS) -DPARSON_FORCE_HASH_COLLISIONS -o $@ tests.c parson.c
# 	./$@

# clean:
# 	rm -f test *.o

