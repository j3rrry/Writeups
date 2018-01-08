#include <stdio.h>
#include <string.h>

void handler(){
	puts("Bye!");
	exit(0);
}

void __init(){
	signal(14, handler);    // SIGALRM
	alarm(0x40);
	setvbuf(stdout, 0, 2, 0);
	setvbuf(stdin, 0, 2, 0);
	setvbuf(stderr, 0, 2, 0);
}

int menu(){
	char buf[4] = {0,};

	puts("\n--==[[ Spiritual Memo ]]==--\n");
	puts("[1] Create a memo");
	puts("[2] Show memo");
	puts("[3] Delete memo");
	puts("[4] Tap out");
	printf("> ");
	if ( read(0, buf, 4) <= 0)
	{
		puts("Read error!");
		exit(-1);
	}
	return atol(buf);
}

struct __attribute__ ((__packed__)) pwning {
	char buf[24];
	char* ptr;
};

void alloc(struct pwning* pwn){
	char s1[8] = {0,};

	fprintf(stderr, "Data: ");
	if( read(0, pwn->buf, 0x30) <= 0)
	{
		puts("Read error!");
		exit(1);
	}
	pwn->ptr = strdup(pwn->buf);
	if( !pwn->ptr )
	{
		puts("Malloc error!");
		exit(1);
	}
	printf("Are you done? [yes/no] ");
	if( read(0, s1, 6) <= 0)
	{
		printf("Read error!");
		exit(1);
	}
	if( strcmp(s1, "yes") ){
		if( strcmp(s1, "no") )
		{
			fprintf(stderr, "Which part of [yes/no] did you not understand?");
			exit(0);
		}
		printf("Data: ");
		if( read(0, pwn->ptr, 0x30) <= 0){
			puts("Read error!");
			exit(1);
		}
	}
	puts("Done!");
}

void dump(char* ptr){
	printf("Data: %s\n", ptr);
}

void del(char* ptr){
	free(ptr);
	puts("Done!");
}

void main(){
	struct pwning pwn;

	__init();
	while(1){
		switch(menu()){
		case 1:
			alloc(&pwn);
			break;
		case 2:
			dump(pwn.ptr);
			break;
		case 3:
			del(pwn.ptr);
			break;
		case 4:
			fprintf(stderr, "Giving up already? [yes/no] ");
			if( read(0, pwn.buf, 0x30) <= 0 )
			{
				puts("Read error!");
				exit(1);
			}
			if( !strcmp(pwn.buf, "yes") ){
				puts("Quitter!");
				return;
			}
			if( strcmp(pwn.buf, "no") )
			{
				fprintf(stderr, "Which part of [yes/no] did you not understand?");
				exit(0);
			}
			break;
		default:
			fprintf(stderr, "Can't you read mate?");
			exit(1);
		}
	}
}
