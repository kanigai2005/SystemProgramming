#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#define MAX_KEY 32
#define MAX_VAL 128

void set(const char *key , const char *val);

typedef struct{
	char key[MAX_KEY];
	char val[MAX_VAL];
}entry;

void set(const char *key , const char *val){
	FILE *fp = fopen("data.db","ab");
	entry e;
	memset(&e,0,sizeof(e));
	snprintf(e.key,MAX_KEY,"%s",key);
	snprintf(e.val,MAX_VAL,"%s",val);
	fwrite(&e,sizeof(e),1,fp);
	fclose(fp);
}

char* get(const char *key){
	FILE *fp=fopen("data.db","rb");
	entry e;
	char *result=NULL;
	char *c=NULL;
	while(fread(&e,sizeof(e),1,fp)!=0){
		if(strcmp(e.key,key)==0){
			//strncpy(result,e.val,MAX_VAL);
			result = strdup(e.val);
		}
	}
	fclose(fp);
	return  result;
}

int main(int argc , char* argv[]){
	if(strcmp(argv[1],"set")==0 && argc == 4){
		set(argv[2],argv[3]);
	}
	else if(strcmp(argv[1],"get")==0){
		char *val=get(argv[2]);
		if(val){
			printf("found:%s",val);
			free(val);
		}
		else{
			printf("Not found");
		}
	}
	return 0;
}
