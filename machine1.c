#include "common.h"

/*This fiel for L1 cache + Memory*/


/* Cache size: 64KB, Cacheline size: 8 Byte, Direct-mapped*/
void M1(FILE *fr, FILE *fw){
	/*---analysis address---*/
	struct{
		unsigned tag,lineNO,addrInBlock;
		int nr_t,nr_l,nr_a;
	}addr;
	memset(&addr,0,sizeof(addr));
	int i;
	for(i=MEMSIZE*1024/64;i!=1;i=i/2)
		addr.nr_t++;
	for (i=64*1024/8;i!=1;i=i/2)
		addr.nr_l++;
	for (i=8;i!=1;i=i/2)
		addr.nr_a++;
	addr.nr_a+=3;

	/*---create L1cache---*/
	struct {
		char v; //value
		int tag;
	}cache[64*1024/8];

	/*---exe test files---*/
	char instr[16];
	while(fgets(instr,17,fr)){
		/*analysis instr*/
		unsigned tempaddr;
		sscanf(&instr[2],"%x",&tempaddr);
		addr.tag=addr.lineNO=addr.addrInBlock=tempaddr;
		addr.tag=addr.tag>>(addr.nr_l+addr.nr_a);
		addr.lineNO=addr.lineNO<<addr.nr_t>>(addr.nr_l+addr.nr_a);
		addr.addrInBlock=addr.addrInBlock<<(addr.nr_t+addr.nr_l)>>(addr.nr_t+addr.nr_l);

		time_L1.accesse++;
		if (instr[0]=='l') time_L1.load++;
		else if (instr[0]=='s') time_L1.store++;
		if (cache[addr.lineNO].v==0){
			memory(instr[0]);
			cache[addr.lineNO].v=1;
			cache[addr.lineNO].tag=addr.tag;
			/*copy data info*/
		}
		else{
			if (cache[addr.lineNO].tag==addr.tag){
				time_L1.hit++;
				if (instr[0]=='l') time_L1.load_hit++;
				else if (instr[0]=='s') time_L1.store_hit++;
			}
			else{
				memory(instr[0]);
				cache[addr.lineNO].tag=addr.tag;
			}
		}
		total_exe_cycle+=atoi(&instr[13]);		
	}
}

/*Cache size:32KB, Cacheline size:32 Byte, 4-way set-associative, LRU*/
void M2(FILE *fr,FILE *fw){
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
				memory(instr[0]);
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
			memory(instr[0]);
			q->next->next->next=NULL;
			p2->tag=addr.tag;
			p2->next=q;
			cache[addr.groupNO].head=p2;
		}
		total_exe_cycle+=atoi(&instr[13]);
	}
}

/*Cache size: 8KB, Cacheline size: 64 Byte, Fully-associative, Random*/
void M3(FILE* fr,FILE *fw){
	/*---analysis address---*/	
	struct{
		unsigned tag,addrInBlock;
		int nr_t,nr_a;
	}addr;
	memset(&addr,0,sizeof(addr));
	int i;
	for (i=MEMSIZE*1024*1024/64;i!=1;i=i/2)
		addr.nr_t++;
	for (i=64;i!=1;i=i/2)
		addr.nr_a++;
	addr.nr_a+=3;

	/*---create L1cache---*/
	int cachesize=8*1024/64;
	struct {
		char v; //value
		int tag;
	}cache[cachesize];

	/*---exe test files---*/
	char instr[16];
	while(fgets(instr,17,fr)){
		/*analysis instr*/
		unsigned tempaddr;
		sscanf(&instr[2],"%x",&tempaddr);
		addr.tag=addr.addrInBlock=tempaddr;
		addr.tag=addr.tag>>addr.nr_a;
		addr.addrInBlock=addr.addrInBlock<<addr.nr_t>>addr.nr_t;

		time_L1.accesse++;
		if (instr[0]=='l') time_L1.load++;
		else if (instr[0]=='s') time_L1.store++;
		char replace=1;
		for (i=0;i<cachesize;i++){
			if (cache[i].v==0){
				memory(instr[0]);
				cache[i].v=1;
				cache[i].tag=addr.tag;
				replace=0;
				break;
			}
			else{
				if (cache[i].tag==addr.tag){
					time_L1.hit++;
					if (instr[0]=='l') time_L1.load_hit++;
					else if (instr[0]=='s') time_L1.store_hit++;
					replace=0;
					break;
				}
			}
		}
		if (replace==1){
			memory(instr[0]);
			int n=(int)rand()%(cachesize+1);
			cache[n].tag=addr.tag;
		}
		total_exe_cycle+=atoi(&instr[13]);
	}
}

