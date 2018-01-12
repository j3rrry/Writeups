# (challenge-009) OBFSTEANATION
250

## Description
> You need to be really obstinated to solve this one. Grab a cup of tea and do your magic!

```
$ md5sum sc04
ee32fcd7e025f71c772959567b1bdce8  sc04
```

## Write-up
```
$ ./sc04
--------------------------------------------
Guess the password and save the day
=> Password : SittinonTheDockOfTheBayWastingTime
0x00CTF{Th1s_i5_n07_th3_fl4G_Y0uR_l0Ok1n_4}
```
그냥 문제를 풀게 되면 이런식으로 플래그가 나온다.  
이를 인증해보니 역시 아니었다 ㅠㅠ  
  
main 함수부터 분석했는데 문제가 풀리지 않았다...  
그럴 때는 init 함부부분을 분석해봐야한다. main 불러오기 전에 초기화하는 함수이기 때문!  
  
아참!  
현재 상태에서는 gdb, objdump, readelf 등의 툴이 먹히지 않는다.  
헤더 정보가 망가져서 그런건데. 분석해보자.
```
$ readelf -h sc04
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              EXEC (Executable file)
  Machine:                           Advanced Micro Devices X86-64
  Version:                           0x1
  Entry point address:               0x400690
  Start of program headers:          64 (bytes into file)
  Start of section headers:          2105984 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         9
  Size of section headers:           64 (bytes)
  Number of section headers:         25901
  Section header string table index: 28
readelf: Error: Reading 0x194b40 bytes extends past end of file for section headers
```
섹션 헤더 정보가 맞지 않아서 에러가 발생했다.  
`Number of section headers` 가 너무 크다!!  
ELF 파일의 section table 은 파일의 맨 아래에 위치한다.  
다시말해 섹션 헤더가 시작하는 `2105984` 부터 파일의 마지막 끝까지 전부 섹션 헤더의 공간이다.  
그리고 각 섹션 헤더들은 `64` 바이트 크기를 갖는다.
```
$ ls -l sc04
-rwxrwxrwx 1 j3rrry j3rrry 2107840 Dec 13 08:23 sc04
```
파일의 총 크기는 `2107840` 이므로
```
(2107840 - 2105984) / 64 = 29 (0x1d)
```
`Number of section headers` 는 29 가 된다.
```
$ cp sc04 sc04.patch
$ echo -en "\x1d\x00" | dd of=sc04.patch seek=60 count=2 bs=1 conv=notrunc
2+0 records in
2+0 records out
2 bytes copied, 0.000794175 s, 2.5 kB/s
```
이제 readelf 도 잘 되고, gdb 도 잘 된다!!  
  
gdb 로 분석할 수 있게됬으니까 gdb 로 보자.  
entry point 를 보면
```
   0x400690:    xor    ebp,ebp
   0x400692:    mov    r9,rdx
   0x400695:    pop    rsi
   0x400696:    mov    rdx,rsp
   0x400699:    and    rsp,0xfffffffffffffff0
   0x40069d:    push   rax
   0x40069e:    push   rsp
   0x40069f:    mov    r8,0x4011f0
   0x4006a6:    mov    rcx,0x401180		; init
   0x4006ad:    mov    rdi,0x400daa		; main
   0x4006b4:    call   0x400650 <__libc_start_main@plt>
   0x4006b9:    hlt
```
init 은 0x401180
```
   0x401180:    push   r15
   0x401182:    mov    r15d,edi
   0x401185:    push   r14
   0x401187:    mov    r14,rsi
   0x40118a:    push   r13
   0x40118c:    mov    r13,rdx
   0x40118f:    push   r12
   0x401191:    lea    r12,[rip+0x200c70]        # 0x601e08
   0x401198:    push   rbp
   0x401199:    lea    rbp,[rip+0x200c78]        # 0x601e18
   0x4011a0:    push   rbx
   0x4011a1:    sub    rbp,r12
   0x4011a4:    xor    ebx,ebx
   0x4011a6:    sar    rbp,0x3
   0x4011aa:    sub    rsp,0x8
   0x4011ae:    call   0x4005e0
...
```
일반적인 dynamic linked binary 라면 init 은 \_\_libc\_csu\_init 을 호출 할 것이다.  
그리고 .init\_array 라는 특정 영역에 포인터들을 저장하고 있을 것이다.  
  
0x601e08 을 보자.
```
pwndbg> x/4a 0x601e08
0x601e08:       0x400750        0x400a1c
0x601e18:       0x400730        0x0
```
우리가 관심있는 것은 두번째 포인터인 0x400a1c 이다.  
  
Constructor 분석
```
   0x400a1c:    push   rbp
   0x400a1d:    mov    rbp,rsp
   0x400a20:    sub    rsp,0x20
   0x400a24:    mov    DWORD PTR [rbp-0x20],0x0
   0x400a2b:    mov    eax,0xb62
   0x400a30:    mov    DWORD PTR [rbp-0x1c],eax
   0x400a33:    mov    QWORD PTR [rbp-0x18],0x3df512
   0x400a3b:    mov    QWORD PTR [rbp-0x10],0x3e8ce9
   0x400a43:    mov    QWORD PTR [rbp-0x8],0x3ded36
   0x400a4b:    add    QWORD PTR [rbp-0x8],0x212ca
   0x400a53:    mov    DWORD PTR [rbp-0x20],0x0
   0x400a5a:    jmp    0x400aa0
```
함수 프롤로그와 변수 초기화부분이다.
```
[rbp-0x8]    Var1 = 0x3ded36 + 0x212ca = 0x400000
[rbp-0x10]   Var2 = 0x3e8ce9
[rbp-0x18]   Var3 = 0x3df512
[rbp-0x1c]   Var4 = 0xb62
[rbp-0x20]   Var5 = 0
```
다음 루틴을 봅시다.
```
   0x400a5a:    jmp    0x400aa0
   0x400a5c:    mov    eax,DWORD PTR [rbp-0x20]            ; Var5
   0x400a5f:    movsxd rdx,eax
   0x400a62:    mov    rax,QWORD PTR [rbp-0x8]             ; Var1
   0x400a66:    add    rax,rdx
   0x400a69:    movzx  eax,BYTE PTR [rax]
   0x400a6c:    cmp    al,0xc3
   0x400a6e:    jne    0x400a9c
   0x400a70:    mov    eax,DWORD PTR [rip+0x201716]        # 0x60218c
   0x400a76:    mov    edx,DWORD PTR [rbp-0x20]            ; Var5
   0x400a79:    movsxd rcx,edx
   0x400a7c:    mov    rdx,QWORD PTR [rbp-0x8]             ; Var1
   0x400a80:    add    rdx,rcx
   0x400a83:    cdqe
   0x400a85:    mov    QWORD PTR [rax*8+0x6021a0],rdx      ; global g[]
   0x400a8d:    mov    eax,DWORD PTR [rip+0x2016f9]        # 0x60218c
   0x400a93:    add    eax,0x1
   0x400a96:    mov    DWORD PTR [rip+0x2016f0],eax        # 0x60218c
   0x400a9c:    add    DWORD PTR [rbp-0x20],0x1            ; Var5 ++
   0x400aa0:    mov    eax,DWORD PTR [rbp-0x20]            ; Var5
   0x400aa3:    cmp    eax,DWORD PTR [rbp-0x1c]            ; Var4
   0x400aa6:    jge    0x400ab3
   0x400aa8:    mov    eax,DWORD PTR [rip+0x2016de]        # 0x60218c
   0x400aae:    cmp    eax,0x8
   0x400ab1:    jle    0x400a5c
...
```
Var5 는 루프문의 counter 이다. `i` 로 네이밍할 수 있겠다.  
Var1 은 `ptr` 로 네이밍하겠다.  
또 눈여겨 볼만한 것은 전역변수들이다. 0x60218c 는 32 비트 정수형 변수이고 0x6021a0 은 각 인덱스가 QWORD 인 배열변수이다.  
이 둘을 `g_count` 와 `g[]` 로 네이밍하겠다.  
```
Globals
0x60218c    g_count
0x6021a0    qword g[]

Locals
[rbp-0x8]    ptr = 0x3ded36 + 0x212ca = 0x400000
[rbp-0x10]   Var2 = 0x3e8ce9
[rbp-0x18]   Var3 = 0x3df512
[rbp-0x1c]   len = 0xb62
[rbp-0x20]   i = 0
```
잘보면 `0x400000` 부터 `+0xb62` 까지 `0xc3`. 즉, opcode `ret` 를 찾는다.  
다른말로 `ret` 를 포함하는 것. 즉, 함수를 찾고있는 것이다.  
다음 루틴을 보자
```
   0x400ac9:    mov    rax,QWORD PTR [rip+0x2016b0]        # 0x602180 <environ>
   0x400ad0:    mov    edx,DWORD PTR [rbp-0x20]            ; EDX = i
   0x400ad3:    movsxd rdx,edx                             ; RDX = i
   0x400ad6:    shl    rdx,0x3                             ; RDX >>= 3 | RDX = RDX * 8
   0x400ada:    add    rax,rdx                             ; RAX = &environ[i*8]
   0x400add:    mov    rax,QWORD PTR [rax]                 ; RAX = environ[i*8]
   0x400ae0:    test   rax,rax                             ; TEST 0
   0x400ae3:    jne    0x400aee                            ; Jump if not NULL
   0x400ae5:    mov    rax,QWORD PTR [rip+0x2016bc]        # 0x6021a8 -> g[1]
   0x400aec:    jmp    rax                                 ; JMP g[1]
```
gdb 는 이미 0x602180 가 environ 이라고 친절하게 알려주고 있다.  
요기가 이번 문제에서 중요한 부분인데  
특정 환경변수가 있냐 없냐에 따라서 루틴이 달라진다.  
  
```
   0x400b0e:    mov    rax,QWORD PTR [rip+0x20166b]        # 0x602180 <environ>
   0x400b15:    mov    edx,DWORD PTR [rbp-0x20]
   0x400b18:    movsxd rdx,edx
   0x400b1b:    shl    rdx,0x3
   0x400b1f:    add    rax,rdx
   0x400b22:    mov    rax,QWORD PTR [rax]
   0x400b25:    mov    rdi,rax
   0x400b28:    call   0x400630 <strlen@plt>
   0x400b2d:    cmp    rax,0xa
   0x400b31:    jne    0x400ccb
```
요기를 보면 환경변수는 문자열로 저장되어있는데 이 길이를 재서 `0xa (10)` 인지 확인한다.  
만약 길이가 10 이면  
다음 루틴을 보면 비슷한 코드들이 여럿 존재한다.  
그 중 하나를 보자면
```
   0x400b37:    mov    rax,QWORD PTR [rip+0x201642]        # 0x602180 <environ>
   0x400b3e:    mov    edx,DWORD PTR [rbp-0x20]
   0x400b41:    movsxd rdx,edx
   0x400b44:    shl    rdx,0x3
   0x400b48:    add    rax,rdx
   0x400b4b:    mov    rax,QWORD PTR [rax]
   0x400b4e:    movzx  eax,BYTE PTR [rax]                  ; EAX = environ[i*8][0]
   0x400b51:    cmp    al,0x4d                             ; EAX == 0x4d == 'M'
   0x400b53:    jne    0x400ac5
```
환경변수의 첫 문자가 'M' 인지 확인하는 것이다.  
다음 루틴은 두 번째 문자를 확인하고 있다.  
길이 10 짜리 환경변수를 알아보면  
`MAIN=31173` 이다.  
  
```
$ MAIN=31173 ./sc04.patch
Congrats!. You have found the real challenge!!! Let's Play
Password :
```
환경변수를 넣어서 실행시켜보니 확실히 달라졌다.  
real challenge 를 찾았다고 축하한단다 ㅠㅠ  
  
이제 `Password` 의 입력부분부터 분석하면 된다.
```
pwndbg> bt
#0  0x00007ffbbf114750 in __read_nocancel () at ../sysdeps/unix/syscall-template.S:84
#1  0x00007ffbbf0ab230 in _IO_new_file_underflow (fp=0x7ffbbf3d38c0 <_IO_2_1_stdin_>) at fileops.c:600
#2  0x00007ffbbf0ac3a2 in __GI__IO_default_uflow (fp=0x7ffbbf3d38c0 <_IO_2_1_stdin_>) at genops.c:413
#3  0x00007ffbbf09f6ca in __GI__IO_getline_info (fp=fp@entry=0x7ffbbf3d38c0 <_IO_2_1_stdin_>, buf=buf@entry=0x7ffe74b706d0 "", n=127, delim=delim@entry=10, extract_delim=extract_delim@entry=1, eof=eof@entry=0x0) at iogetline.c:60
#4  0x00007ffbbf09f7d8 in __GI__IO_getline (fp=fp@entry=0x7ffbbf3d38c0 <_IO_2_1_stdin_>, buf=buf@entry=0x7ffe74b706d0 "", n=<optimized out>, delim=delim@entry=10, extract_delim=extract_delim@entry=1) at iogetline.c:34
#5  0x00007ffbbf09e56b in _IO_fgets (buf=0x7ffe74b706d0 "", n=<optimized out>, fp=0x7ffbbf3d38c0 <_IO_2_1_stdin_>) at iofgets.c:53
#6  0x0000000000402175 in ?? ()
#7  0x00007ffbbf056561 in __libc_start_main (main=0x402115, argc=1, argv=0x7ffe74b70848, init=<optimized out>, fini=<optimized out>, rtld_fini=<optimized out>, stack_end=0x7ffe74b70838) at ../csu/libc-start.c:297
#8  0x00000000004006b9 in ?? ()
```
bt 해보니 #6 에 .text code 가 눈에 띈다. 심지어 #5 에 fgets 라이브러리 함수가 있어서 신빙성이 높아졌다.  
아마 main 함수 코드일 것이다.  
```
   0x402175:    lea    rax,[rbp-0x90]
   0x40217c:    mov    rdi,rax
   0x40217f:    call   0x402000
   0x402184:    mov    eax,0x0
   0x402189:    mov    rcx,QWORD PTR [rbp-0x8]
   0x40218d:    xor    rcx,QWORD PTR fs:0x28
   0x402196:    je     0x40219d
   0x402198:    call   0x400640 <__stack_chk_fail@plt>
   0x40219d:    leave
   0x40219e:    ret
```
굳이 main 의 프롤로그부터 분석할 필요는 없다. `fgets` 다음 부분부터 보자.  
`[rbp-0x90]` 을 examin 해보면 우리가 `Password` 로 넣은 문자열이 들어가 있고  
해당 main 함수가 종료되기 전까지 `0x402000` 함수밖에 없다.  
따라서 `0x402000` 을 분석하면 끝!  
  
`rdi` 는 fastcall 의 첫번째 매개변수가 저장되는 레지스터이다.  
`0x402000` 은 우리가 입력한 값을 매개변수로 가져가는구나!  
  
그럼 `0x402000` 을 분석해보자.
```
   0x402000:    push   rbp
   0x402001:    mov    rbp,rsp
   0x402004:    sub    rsp,0x20
   0x402008:    mov    QWORD PTR [rbp-0x18],rdi            ; user input
   0x40200c:    mov    DWORD PTR [rbp-0x8],0x0
```
지역 변수에 값을 저장하고
```
   0x402013:    mov    rax,QWORD PTR [rbp-0x18]			   ; s
   0x402017:    mov    rdi,rax
   0x40201a:    call   0x400630 <strlen@plt>			   ; strlen(s)
   0x40201f:    mov    edx,DWORD PTR [rip+0x20007b]        # 0x6020a0 == 0x12
   0x402025:    movsxd rdx,edx
   0x402028:    cmp    rax,rdx
   0x40202b:    ja     0x40203b
   0x40202d:    mov    rax,QWORD PTR [rbp-0x18]
   0x402031:    mov    rdi,rax
   0x402034:    call   0x400630 <strlen@plt>
   0x402039:    jmp    0x402041
   0x40203b:    mov    eax,DWORD PTR [rip+0x20005f]        # 0x6020a0
   0x402041:    mov    DWORD PTR [rbp-0x4],eax			   ; max(strlen(s), 0x12)
```
`s` 의 길이랑 `0x6020a0` 에 저장되있는 `0x12` 중 큰 값이 `[rbp-0x4]` 에 저장된다.  
```
   0x402044:    mov    DWORD PTR [rbp-0xc],0x0			; int i = 0;
   0x40204b:    jmp    0x40207f
   0x40204d:    mov    eax,DWORD PTR [rbp-0xc]
   0x402050:    cdqe
   0x402052:    mov    rax,QWORD PTR [rax*8+0x6020c0]	; (void*) 0x6020c0[i]
   0x40205a:    mov    edx,DWORD PTR [rbp-0xc]
   0x40205d:    movsxd rcx,edx
   0x402060:    mov    rdx,QWORD PTR [rbp-0x18]			; s
   0x402064:    add    rdx,rcx
   0x402067:    movzx  edx,BYTE PTR [rdx]				; s[i]
   0x40206a:    movsx  ecx,dl
   0x40206d:    mov    edx,DWORD PTR [rbp-0xc]			; a3 = i
   0x402070:    mov    rsi,QWORD PTR [rbp-0x18]			; a2 = s
   0x402074:    mov    edi,ecx							; a1 = s[i]
   0x402076:    call   rax								; qword_6020c0(s[i], s, i);
   0x402078:    add    DWORD PTR [rbp-0x8],eax			; var_8 += eax
   0x40207b:    add    DWORD PTR [rbp-0xc],0x1			; i++
   0x40207f:    mov    eax,DWORD PTR [rbp-0xc]
   0x402082:    cmp    eax,DWORD PTR [rbp-0x4]			; if i < var_4: loop
   0x402085:    jl     0x40204d
   0x402087:    cmp    DWORD PTR [rbp-0x8],0x0
   0x40208b:    jne    0x4020f5							; avoid
   0x40208d:    mov    DWORD PTR [rbp-0xc],0x0			; success
   0x402094:    jmp    0x4020e8
```
위 어셈 코드를 C 로 변환하면
```
var_8 = 0;
for( int i=0; i < var_4; i++){
	var_8 += qword_6020c0(s[i], s, i);
}
if(var_8 == 0){
	i = 0;
	goto success;
}
else
	goto avoid;
```
심볼릭 익스큐션을 하면 됩니다만 저는 익숙하지 않아서 제 방식대로 풀겠습니다. ㅠㅠ  
  
`qword_6020c0[i]` 중 하나를 봅시다.
```
0x6020c0:       0x40219f        0x4024d5
0x6020d0:       0x40278f        0x402ac5
0x6020e0:       0x402daf        0x40309c
0x6020f0:       0x40337d        0x403656
0x602100:       0x40390c        0x403c67
0x602110:       0x403f8b        0x40421b
0x602120:       0x4044d7        0x4047cb
0x602130:       0x404ab5        0x404dbe
0x602140:       0x4050ae        0x4053a5
0x602150:       0x0     0x0
```
`qword_6020c0` 를 보면 18 개의 함수들이 모여있는 걸로 보아  
`Password` 는 길이 18 일 것이라고 예측할 수 있겠네요.
```
   0x40219f:    push   rbp
   0x4021a0:    mov    rbp,rsp
   0x4021a3:    push   rbx
   0x4021a4:    sub    rsp,0x58
   0x4021a8:    mov    eax,edi
   0x4021aa:    mov    QWORD PTR [rbp-0x60],rsi	; var_60 = s
   0x4021ae:    mov    DWORD PTR [rbp-0x58],edx ; var_58 = i
   0x4021b1:    mov    BYTE PTR [rbp-0x54],al	; var_54 = s[i]
```
함수의 프롤로그는 이렇고  
에필로그에서 반환하는 값은 0 이어야한다.
```
   0x4024cb:    mov    eax,DWORD PTR [rbp-0x28]    ; return var_28
   0x4024ce:    add    rsp,0x58
   0x4024d2:    pop    rbx
   0x4024d3:    pop    rbp
   0x4024d4:    ret
```
`var_28` 을 반환한다.  
그럼 이제 `var_28` 에 무엇이 저장되는지 확인해야하는데  
함수가 크으으으다...  
`var_28` 관련 명령어들을 모아보면
```
...
   0x4021b4:    mov    DWORD PTR [rbp-0x3c],0x0
   0x4021bb:    mov    eax,DWORD PTR [rbp-0x3c]
   0x4021be:    mov    DWORD PTR [rbp-0x28],eax
...
   0x402307:    movzx  eax,BYTE PTR [rax]
   0x40230a:    movsx  eax,al
   0x40230d:    sub    eax,0x2d
   0x402310:    add    DWORD PTR [rbp-0x28],eax
...
   0x4023c9:    movsx  eax,BYTE PTR [rbp-0x54]
   0x4023cd:    xor    eax,DWORD PTR [rbp-0x58]		; s[i] ^ i
   0x4023d0:    mov    DWORD PTR [rbp-0x28],eax

   0x4023d3:    mov    DWORD PTR [rbp-0x3c],0x56
   0x4023da:    mov    eax,DWORD PTR [rbp-0x3c]
   0x4023dd:    xor    DWORD PTR [rbp-0x28],eax		; (s[i] ^ i) ^ 0x56
...
   0x402476:    xor    DWORD PTR [rbp-0x28],0x26	; ((s[i] ^ i) ^ 0x56) ^ 0x26
...
   0x4024a4:    mov    eax,DWORD PTR [rbp-0x58]
   0x4024a7:    mov    DWORD PTR [rbp-0x20],eax
   0x4024aa:    mov    eax,DWORD PTR [rbp-0x20]
   0x4024ad:    xor    DWORD PTR [rbp-0x28],eax		; (((s[i] ^ i) ^ 0x56) ^ 0x26) ^ i
...
   0x4024cb:    mov    eax,DWORD PTR [rbp-0x28]
...
```
위 instruction 중에서 `0x4023c9` 부터 보면 된다.  
그 위에 것들은 어차피 덮어씌워지기 때문에 영향을 미치지 않기 때문이다.  
  
```
(((s[i] ^ i) ^ 0x56) ^ 0x26) ^ i
= s[i] ^ i ^ 0x56 ^ 0x26 ^ i
= s[i] ^ 0x56 ^ 0x26
= s[i] ^ 0x70
```
분석해보니 XOR 명령만 이루어진다.  
s[i] 를 0 으로 초기화 한다면  
각 함수들의 반환값은 `Password` 가 될 것이다.  
왜냐하면 XOR 의 성질 중에 A ^ 0 = 0 ^ A = A 가 성립하기 때문이다.  
  
1. 환경변수 'MAIN=31137'  
2. s[i] 를 0 으로 초기화하면  
3. 각 함수의 반환값이 `Password`  
  
```
# 0x402028    cmp    rax, rdx   ; RDX = 0x12
passwd = ['a'] * 0x12

# 0x400ae0    test   rax, rax
gdb.Breakpoint('*0x400ae0').silent = True
gdb.execute('r <<< {}'.format(''.join(passwd)))

# MAIN=31173\x00
gdb.execute('p *$rax = 0x4e49414d', to_string=True)
gdb.execute('p *($rax+4) = 0x3131333d', to_string=True)
gdb.execute('p *($rax+8) = 0x003337', to_string=True)

# to not crash
fgets = gdb.Breakpoint('*fgets')
fgets.silent = True
gdb.execute('c')
fgets.delete()

# 0x402076    call   rax
gdb.Breakpoint('*0x402076').silent = True
gdb.Breakpoint('*0x402078').silent = True
for i in range(0x12):
    gdb.execute('c')
    gdb.execute('p $edi = 0', to_string=True)
    gdb.execute('ni')
    eax = int(gdb.parse_and_eval('$eax'))
    passwd[i] = chr(eax)
    print(''.join(passwd))
    gdb.execute('p $eax=0', to_string=True)
gdb.execute('q')
```
```
Breakpoint 1 at 0x400ae0
Breakpoint 2 at 0x7f7c1534d4c0: file iofgets.c, line 37.
Congrats!. You have found the real challenge!!! Let's Play
Breakpoint 3 at 0x402076
Breakpoint 4 at 0x402078
paaaaaaaaaaaaaaaaa
p1aaaaaaaaaaaaaaaa
p1Caaaaaaaaaaaaaaa
p1C0aaaaaaaaaaaaaa
p1C0baaaaaaaaaaaaa
p1C0bfaaaaaaaaaaaa
p1C0bfUaaaaaaaaaaa
p1C0bfU5aaaaaaaaaa
p1C0bfU5Kaaaaaaaaa
p1C0bfU5K4aaaaaaaa
p1C0bfU5K4taaaaaaa
p1C0bfU5K4t0aaaaaa
p1C0bfU5K4t0Raaaaa
p1C0bfU5K4t0R-aaaa
p1C0bfU5K4t0R-2aaa
p1C0bfU5K4t0R-2oaa
p1C0bfU5K4t0R-2o1a
p1C0bfU5K4t0R-2o1T
```
```
$ MAIN=31173 ./sc04.patch
Congrats!. You have found the real challenge!!! Let's Play
Password : p1C0bfU5K4t0R-2o1T
0x00CTF{1tw4SaL0nGtR1p}
```
정말 긴 여정이었다...
