#include "common.h"

void init(){
	memset(&time_L1,0,sizeof(time_L1));
	memset(&time_V,0,sizeof(time_V));
	memset(&time_L2,0,sizeof(time_L2));
	memset(&time_mem,0,sizeof(time_mem));
	total_exe_cycle=0;
	latency=0;
}

int config(){
	printf("This is a configurable cache simulator.\n");
	printf("Please choose one of the configurations:\n");
	printf("• L1 cache + Memory\n");
	printf(" 1. Cache size: 64KB, Cacheline size: 8 Byte, Direct-mapped\n");
	printf(" 2. Cache size: 32KB, Cacheline size: 32 Byte, 4-way set-associative, LRU\n");
	printf(" 3. Cache size: 8KB, Cacheline size: 64 Byte, Fully-associative, Random\n");
	printf("• L1 cache + L2 cache + Memory\n");
	printf(" 4. L1 cache size: 32KB, Cacheline size: 32 Byte, 4-way set-associative, LRU\n");
	printf("    L2 cache size: 2MB, Cacheline size: 128 Byte, 8-way set-associative, LRU\n");
	printf("• L1 cache + Victim cache + L2 cache + Memory\n");
	printf(" 5. L1 cache size: 32KB, Cacheline size: 32 Byte, 4-way set-associative, LRU\n");
	printf("    Victim cache size: 1KB (32 entries), Cacheline size: 32 Byte, Fullyassociative, LRU\n");
	printf("    L2 cache size: 2MB, Cacheline size: 128 Byte, 8-way set-associative, LRU\n");
	int op;
	scanf("%d",&op);
	return op;
}

void resultInfo(int op,char *fname,FILE *fw ){
	switch (op){
		case 1:case 2:case 3:
			fprintf(fw,"Config:L1 cache + Memory M%d\n",op);
			fprintf(fw,"The result of %s:\n",fname);
			fprintf(fw,"Num of memory: accesses:%d, loads:%d, stores:%d\n",time_mem.accesse,time_mem.load,time_mem.store);
			fprintf(fw,"Num of cache: accesses:%d, loads:%d, stores:%d\n",time_L1.accesse,time_L1.load,time_L1.store);
			fprintf(fw,"Cache hit rate: %2.2f%%\n",100*(float)(time_L1.hit)/time_L1.accesse);
			fprintf(fw,"Cache hit rate for load:%2.2f%%\n",100*(float)time_L1.load_hit/time_L1.load);
			fprintf(fw,"Cache hit rate for store: %2.2f%%\n",100*(float)time_L1.store_hit/time_L1.store);
			latency=time_mem.accesse*100+time_L1.accesse;
			cputime=total_exe_cycle+latency;
			fprintf(fw,"CPU time: %d cycles\n",cputime);
			fprintf(fw,"CPI: %2.2f\n",(float)cputime/time_L1.accesse);
			fprintf(fw,"=========================================================\n");
			break;
		case 4:
			fprintf(fw,"Config:L1 cache + L2 cache + Memory M%d\n",op);
			fprintf(fw,"The result of %s:\n",fname);
			fprintf(fw,"Num of memory: accesses:%d, loads:%d, stores:%d\n",time_mem.accesse,time_mem.load,time_mem.store);
			fprintf(fw,"Num of L1 Cache: accesses:%d, loads:%d, stores:%d\n",time_L1.accesse,time_L1.load,time_L1.store);
			fprintf(fw,"L1 Cache hit rate: %2.2f%%\n",100*(float)(time_L1.hit)/time_L1.accesse);
			fprintf(fw,"L1 Cache hit rate for load:%2.2f%%\n",100*(float)time_L1.load_hit/time_L1.load);
			fprintf(fw,"L1 Cache hit rate for store: %2.2f%%\n",100*(float)time_L1.store_hit/time_L1.store);

			fprintf(fw,"Num of L2 cache: accesses:%d, loads:%d, stores:%d\n",time_L2.accesse,time_L2.load,time_L2.store);
			fprintf(fw,"L2 Cache hit rate: %2.2f%%\n",100*(float)(time_L2.hit)/time_L2.accesse);
			fprintf(fw,"L2 Cache hit rate for load:%2.2f%%\n",100*(float)time_L2.load_hit/time_L2.load);
			fprintf(fw,"L2 Cache hit rate for store: %2.2f%%\n",100*(float)time_L2.store_hit/time_L2.store);

			latency=time_mem.accesse*100+time_L1.accesse+time_L2.accesse*10;
			cputime=total_exe_cycle+latency;
			fprintf(fw,"CPU time: %d cycles\n",cputime);
			fprintf(fw,"CPI: %2.2f\n",(float)cputime/time_L1.accesse);
			fprintf(fw,"=========================================================\n");
			break;
		case 5:
			fprintf(fw,"Config:L1 cache  + Victim cache + L2 cache + Memory M%d\n",op);
			fprintf(fw,"The result of %s:\n",fname);
			fprintf(fw,"Num of memory: accesses:%d, loads:%d, stores:%d\n",time_mem.accesse,time_mem.load,time_mem.store);
			fprintf(fw,"Num of L1 Cache: accesses:%d, loads:%d, stores:%d\n",time_L1.accesse,time_L1.load,time_L1.store);
			fprintf(fw,"L1 Cache hit rate: %2.2f%%\n",100*(float)(time_L1.hit)/time_L1.accesse);
			fprintf(fw,"L1 Cache hit rate for load:%2.2f%%\n",100*(float)time_L1.load_hit/time_L1.load);
			fprintf(fw,"L1 Cache hit rate for store: %2.2f%%\n",100*(float)time_L1.store_hit/time_L1.store);

			fprintf(fw,"Num of Victim cache: accesses:%d, loads:%d, stores:%d\n",time_V.accesse,time_V.load,time_V.store);
			fprintf(fw,"Victim Cache hit rate: %2.2f%%\n",100*(float)(time_V.hit)/time_V.accesse);
			fprintf(fw,"Victim Cache hit rate for load:%2.2f%%\n",100*(float)time_V.load_hit/time_V.load);
			fprintf(fw,"Victim Cache hit rate for store: %2.2f%%\n",100*(float)time_V.store_hit/time_V.store);

			fprintf(fw,"Num of L2 cache: accesses:%d, loads:%d, stores:%d\n",time_L2.accesse,time_L2.load,time_L2.store);
			fprintf(fw,"L2 Cache hit rate: %2.2f%%\n",100*(float)(time_L2.hit)/time_L2.accesse);
			fprintf(fw,"L2 Cache hit rate for load:%2.2f%%\n",100*(float)time_L2.load_hit/time_L2.load);
			fprintf(fw,"L2 Cache hit rate for store: %2.2f%%\n",100*(float)time_L2.store_hit/time_L2.store);

			latency=time_mem.accesse*100+time_L1.accesse+time_V.accesse+time_L2.accesse*10;
			cputime=total_exe_cycle+latency;
			fprintf(fw,"CPU time: %d cycles\n",cputime);
			fprintf(fw,"CPI: %2.2f\n",(float)cputime/time_L1.accesse);
			fprintf(fw,"=========================================================\n");
			break;
	}
	/*	fprintf(fw,"%d \n",time_mem.accesse);
		fprintf(fw,"%d \n",time_mem.load);
		fprintf(fw,"%d \n",time_mem.store);
		fprintf(fw,"%d \n",time_L1.accesse);
		fprintf(fw,"%2.2f%% \n",100*(float)(time_L1.hit)/time_L1.accesse);
		fprintf(fw,"%d \n",time_L1.load);
		fprintf(fw,"%2.2f%% \n",100*(float)(time_L1.load_hit)/time_L1.load);
		fprintf(fw,"%d \n",time_L1.store);
		fprintf(fw,"%2.2f%% \n",100*(float)(time_L1.store_hit)/time_L1.store);

		fprintf(fw,"%d \n",time_V.accesse);
		fprintf(fw,"%2.2f%% \n",100*(float)(time_V.hit)/time_V.accesse);
		fprintf(fw,"%d \n",time_V.load);
		fprintf(fw,"%2.2f%% \n",100*(float)(time_V.load_hit)/time_V.load);
		fprintf(fw,"%d \n",time_V.store);
		fprintf(fw,"%2.2f%% \n",100*(float)(time_V.store_hit)/time_V.store);
		fprintf(fw,"%d \n",time_L2.accesse);
		fprintf(fw,"%2.2f%% \n",100*(float)(time_L2.hit)/time_L2.accesse);
		fprintf(fw,"%d \n",time_L2.load);
		fprintf(fw,"%2.2f%% \n",100*(float)(time_L2.load_hit)/time_L2.load);
		fprintf(fw,"%d \n",time_L2.store);
		fprintf(fw,"%2.2f%% \n",100*(float)(time_L2.store_hit)/time_L2.store);

		latency=time_mem.accesse*100+time_L1.accesse+time_V.accesse+time_L2.accesse*10;
		cputime=total_exe_cycle+latency;
		fprintf(fw,"%d \n",cputime);
		fprintf(fw,"%2.2f \n",(float)cputime/time_L1.accesse);
		fprintf(fw,"=========================================================\n");
		*/
}

int main(int args, char *argv[]){
	int op=config();	
	FILE *fr,*fw;
	fw=fopen("ret.txt","w+");
	int i;
	for (i=1;i<args;i++){
		init();
		fr=fopen(argv[i],"r");
		if (fr==NULL)
			perror("openfile");
		switch (op){
			case 1: M1(fr,fw); break;
			case 2: M2(fr,fw); break;
			case 3: M3(fr,fw); break;
			case 4:	M4(fr,fw); break;
			case 5: M5(fr,fw); break;
			default:printf("========= Wrong input! ========\n"); return;
		}
		resultInfo(op,argv[i],fw);
	}
}
