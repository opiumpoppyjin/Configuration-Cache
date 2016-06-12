#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>

#define MEMSIZE 512//MB

#define WAY4SET 2
#define WAY8SET 3

struct{
	int accesse,load,store;
}time_mem;

struct{
	int accesse,load,store;
	int hit,load_hit,store_hit;
}time_L1;

struct{
	int accesse,load,store;
	int hit,load_hit,store_hit;
}time_V;

struct{
	int accesse,load,store;
	int hit,load_hit,store_hit;
}time_L2;


int total_exe_cycle;
int latency;
int cputime;

