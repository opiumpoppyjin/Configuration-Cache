#include "common.h"

/*This fiel for L1 cache + L2 cache + Memory*/

/* L1 cache size: 32KB, Cacheline size: 32 Byte, 4-way set-associative, LRU
 * L2 cache size: 2MB, Cacheline size: 128 Byte, 8-way set-associative, LRU */
void M4(FILE *fr,FILE *fw){
	/*---analysis address---*/
	struct{
		unsigned tag,groupNO,addrInBlock;
		int nr_t,nr_g,nr_a;
	}addr;
	memset(&addr,0,sizeof(addr));
	int i;
	const int temp=32*1024/32/4;
	for (i=MEMSIZE*1024*1024/32/temp;i!=1;i=i/2)
		addr.nr_t++;
	for (i=temp;i!=1;i=i/2)
		addr.nr_g++;
	for (i=32;i!=1;i=i/2)
		addr.nr_a++;
	addr.nr_a+=3;

	/*---create L1cache---*/
	typedef struct Group_ group;
	struct Group_{
		char v;
		int tag;
		group *next;
	};
	struct{
		group *head;
	}cache[temp+1];
	for (i=0;i<=temp;i++){
		cache[i].head=(group*)malloc(sizeof(group));
		group *p=cache[i].head;
		p->v=0;
		int j;
		for (j=0;j<4;j++){
			p->next=(group*)malloc(sizeof(group));
			p=p->next;
			p->v=0;
		}
		p->next=NULL;
	}

	/*---init L2 cache---*/
	initL2();
	/*---exe test files---*/
	char instr[16];
	while(fgets(instr,17,fr)){
		/*analysis instr*/
		unsigned tempaddr;
		sscanf(&instr[2],"%x",&tempaddr);
		addr.tag=addr.groupNO=addr.addrInBlock=tempaddr;
		addr.tag=addr.tag>>(addr.nr_g+addr.nr_a);
		addr.groupNO=addr.groupNO<<addr.nr_t>>(addr.nr_t+addr.nr_a);
		addr.addrInBlock=addr.addrInBlock<<(addr.nr_t+addr.nr_g)>>(addr.nr_t+addr.nr_g);

		time_L1.accesse++;
		if (instr[0]=='l') time_L1.load++;
		else if (instr[0]=='s') time_L1.store++;

		group *p1=cache[addr.groupNO].head;
		group *p2=p1,*q=p1;
		char replace=1;
		for(i=0;i<4;i++){
			if (p1->v==0){
				L2(instr);
				p1->v=1;
				p1->tag=addr.tag;		
				replace=0;
				break;
			}
			else{
				if (p1->tag==addr.tag){
					time_L1.hit++;
					if (instr[0]=='l') time_L1.load_hit++;
					else if (instr[0]=='s') time_L1.store_hit++;
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
			L2(instr);
			q->next->next->next=NULL;
			p2->tag=addr.tag;
			p2->next=q;
			cache[addr.groupNO].head=p2;
		}
		total_exe_cycle+=atoi(&instr[13]);
	}
}

