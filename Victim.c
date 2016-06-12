#include "common.h"
/* Victim cache size: 1KB (32 entries), Cacheline size: 32 Byte, Fullyassociative, LRU*/

/*---analysis address---*/	
static struct{
	unsigned tag,addrInBlock;
	int nr_t,nr_a;
}addr;

/*---create Victim cache---*/
#define cacheSize 32//1024/32
typedef struct _Cache Cache;
struct _Cache{
	char v; //value
	int tag;
	Cache *prior,*next;
};

static Cache *head,*tail;

void initVictim(){
	memset(&addr,0,sizeof(addr));
	int i;
	for (i=MEMSIZE*1024*1024/32;i!=1;i=i/2)
		addr.nr_t++;
	for (i=32;i!=1;i=i/2)
		addr.nr_a++;
	addr.nr_a+=3;

	head=(Cache*)malloc(sizeof(Cache));
	Cache *p=head,*q=head;
	for (i=1;i<cacheSize;i++){
		p->next=(Cache *)malloc(sizeof(Cache));
		p=p->next;
		p->prior=q;
		q=p;
	}
	p->next=NULL;
	tail=p;
}

int Victim(char *instr,int addTag){
	/*---exe test files---*/
	/*analysis instr*/
	unsigned tempaddr;
	sscanf(&instr[2],"%x",&tempaddr);
	addr.tag=addr.addrInBlock=tempaddr;
	addr.tag=addr.tag>>addr.nr_a;
	addr.addrInBlock=addr.addrInBlock<<addr.nr_t>>addr.nr_t;

	time_V.accesse++;
	if (instr[0]=='l') time_V.load++;
	else if (instr[0]=='s') time_V.store++;

	Cache *p=head;
	for (;p!=NULL;p=p->next){
		if (p->v==0){
			p->v=1;
			p->tag=addTag;
			return 0;
		}
		else{
			if (p->tag==addr.tag){
				time_V.hit++;
				if (instr[0]=='l') time_V.load_hit++;
				else if (instr[0]=='s') time_V.store_hit++;

				if (p==tail)
					tail=p->prior;
				p->prior->next=p->next;
				p->next->prior=p->prior;
				p->next=head;
				p->prior=NULL;
				head->prior=p;
				head=p;
				return 1;
			}
		}
	}
	p=tail;
	tail=p->prior;
	tail->next=NULL;
	p->tag=addTag;
	p->prior=NULL;
	p->next=head;
	head->prior=p;
	head=p;
	return 0;
}
