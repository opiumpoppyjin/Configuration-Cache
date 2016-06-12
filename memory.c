#include "common.h"

void memory(char lORs){
	time_mem.accesse++;
	if (lORs=='l')
		time_mem.load++;
	else if (lORs=='s')
		time_mem.store++;
}
