#include "common.h"

/* This fiel for L2 cache */

/* L2 cache size: 2MB, Cacheline size: 128 Byte, 8-way set-associative, LRU */

/*---analysis address---*/
static struct{
	unsigned tag,groupNO,addrInBlock;
	int nr_t,nr_g,nr_a;
}addr;
#define temp 2048//{2*1024*1024/128/8)

/*---create L2cache---*/
typedef struct Group_ group;
struct Group_{
	char v;
	int tag;
	group *next;
};
static struct{
	group *head;
}cache[temp+1];

void initL2(){
	/*---analysis address---*/
	memset(&addr,0,sizeof(addr));
	int i;
	for (i=MEMSIZE*1024*1024/128/temp;i!=1;i=i/2)
		addr.nr_t++;
	for (i=temp;i!=1;i=i/2)
		addr.nr_g++;
	for (i=128;i!=1;i=i/2)
		addr.nr_a++;
	addr.nr_a+=3;

	/*---create L2cache---*/
	for (i=0;i<=temp;i++){
		cache[i].head=(group*)malloc(sizeof(group));
		group *p=cache[i].head;
		p->v=0;
		int j;
		for (j=0;j<8;j++){
			p->next=(group*)malloc(sizeof(group));
			p=p->next;
			p->v=0;
		}
		p->next=NULL;
	}
}

void L2(char *instr){
	/*analysis instr*/
	unsigned tempaddr;
	sscanf(&instr[2],"%x",&tempaddr);
	addr.tag=addr.groupNO=addr.addrInBlock=tempaddr;
	addr.tag=addr.tag>>(addr.nr_g+addr.nr_a);
	addr.groupNO=addr.groupNO<<addr.nr_t>>(addr.nr_t+addr.nr_a);
	addr.addrInBlock=addr.addrInBlock<<(addr.nr_t+addr.nr_g)>>(addr.nr_t+addr.nr_g);

	time_L2.accesse++;
	if (instr[0]=='l') time_L2.load++;
	else if (instr[0]=='s') time_L2.store++;

	group *p1=cache[addr.groupNO].head;
	group *p2=p1,*q=p1;
	char replace=1;
	int i;
	for(i=0;i<8;i++){
		if (p1->v==0){
			memory(instr[0]);
			p1->v=1;
			p1->tag=addr.tag;		
			replace=0;
			break;
		}
		else{
			if (p1->tag==addr.tag){
				time_L2.hit++;
				if (instr[0]=='l') time_L2.load_hit++;
				else if (instr[0]=='s') time_L2.store_hit++;
				replace=0;
				if (i==0)
					break;

				p2->next=p1->next;
				p1->next=q;
				cache[addr.groupNO].head=p1;
				break;
			}	
			else{
				p2=p1;
				p1=p1->next;
			}
		}
	}
	if (replace==1){
		memory(instr[0]);
		p2->tag=addr.tag;
		p2->next=q;
		cache[addr.groupNO].head=p2;
		for (i=1;i<8;i++){
			p2=p2->next;
		}
		p2->next=NULL;
	}
}

