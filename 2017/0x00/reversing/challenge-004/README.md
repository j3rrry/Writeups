`#anti-debugging`, `#ptrace`, `#obfuscate`, `#got-overwrite`
# challenge-004
50 pt
## Descripton
Platform: Linux 64 bits
Focus: Reversing/ELF

binary: [hello](./hello)

-----------------------

## Writeup

일단 실행시켜보면
```
$ ./hello
Welcome to the Twinlight Zone!!!
Password: testPassword
Keep Trying!
```
`Password` 를 입력 받고  
`"Keep Trying!"` 을 출력한다.  
  
하지만 디버깅에서 보이는 문자열이라고는  
  
![strings](./strings.png)  
`"Hello Word!"` 밖에 안보인다.  
  
그리고 디버깅 모드에서 프로그램을 실행시켜본 결과  
위에 실행시켜본 결과랑 달리  
`"Hello Word!"` 만 뜨고  
입력도 받지 않은 채 종료됬다.  
  
이제 정적 디버깅해보면  
main 함수에 puts 가 있는데
```asm
; int __cdecl main(int, char **, char **)
main proc near
push    rbp
mov     rbp, rsp
mov     edi, offset s   ; "Hello World!\n"
call    puts
mov     eax, 0
pop     rbp
retn
main endp
```
puts의 got인 off_602020 에
```asm
LOAD:0000000000400650 ; int puts(const char *s)
LOAD:0000000000400650 puts            proc near               ; CODE XREF: main+9p
LOAD:0000000000400650                 jmp     cs:off_602020
LOAD:0000000000400656 ; ---------------------------------------------------------------------------
LOAD:0000000000400656                 push    1
LOAD:000000000040065B                 jmp     sub_400630
LOAD:000000000040065B puts            endp
```
XREF 를 보니  
![off_602020의 XREF](./off_602020.png)  
sub_400B49+4 에서 offset 을 가져온 기록이 있었다.
```C
void **sub_400B49()
{
  void **result; // rax@1

  result = &off_602020;
  off_602020 = sub_400AC4;	// got overwrite!!
  return result;
}
```
위와 같이 함수에서 got overwrite 가 일어나고 있었고  
따라서 puts 함수 실행될 때 sub_400AC4 가 대신 실행되는 것이다.  
  
그럼 sub_400AC4 는 무엇인가?
```C
__int64 __fastcall sub_400AC4(const char *a1)
{
  int v1; // eax@1
  size_t v2; // rax@6

  ptrace_400B77();
  dword_6020DC = v1 < 0;
  if ( dword_6020DC )
  {
    v2 = strlen(a1);
    write(1, a1, v2);	// "Hello World!"
  }
  else
  {
    sub_400A03();
  }
  return 0LL;
}
```
sub_400AC4 는 현재 프로세스가 디버깅 모드인지 확인을 한다.  
디버깅 중이면 "Hello World!" 를 출력  
디버깅 중이 아니면 sub_400A03 를 호출한다.  

```C
__int64 sub_400A03()
{
  char s; // [sp+0h] [bp-90h]@1
  __int64 v2; // [sp+88h] [bp-8h]@1

  v2 = v28;
  putchar_4007DD(off_6020A8, dword_6020A0);	// "Welcome to the Twinlight Zone!!!"
  putchar_4007DD(off_6020B8, dword_6020B0);	// "Password: "
  memset(&s, 0, 0x80uLL);
  if ( read(0, &s, 0x80uLL) == 9 )	// read length is 9
    flag_40084E(&s, &s);
  putchar_4007DD(off_6020C8, dword_6020C0);	// "Keep Trying!"
  return 0LL;
}
```
문자열을 암호화 해놓아서  
off_6020A8 와 dword_6020A0 를 짬뽕해야 "Welcome to the Twinlight Zone!!!" 이 나오고  
off_6020B8 와 dword_6020B0 를 짬뽕해야 "Password: " 가 나오게 되어있었다.  
  
"Password" 길이는 9 이어야  
flag_40084E() 함수로 갈 수 있다.  

```C
signed __int64 __fastcall flag_40084E(__int64 a1)
{
  signed __int64 result; // rax@2
  char buf; // [sp+1Bh] [bp-5h]@18
  int i; // [sp+1Ch] [bp-4h]@17

  if ( (off_602088 ^ a1[0]) == '0' )
  {
    if ( (BYTE1((*off_602088)[0]) ^ a1[1]) == 'x' )
    {
      if ( (BYTE2((*off_602088)[0]) ^ a1[2]) == '0' )
      {
        if ( (BYTE3((*off_602088)[0]) ^ a1[3]) == '0' )
        {
          if ( (BYTE4((*off_602088)[0]) ^ a1[4]) == 'C' )
          {
            if ( (BYTE5((*off_602088)[0]) ^ a1[5]) == 'T' )
            {
              if ( (BYTE6((*off_602088)[0]) ^ a1[6]) == 'F' )
              {
                if ( (BYTE7((*off_602088)[0]) ^ a1[7]) == '{' )
                {
                  for ( i = 0; i < dword_602080; ++i )
                  {
                    buf = a1[((((i >> 32) >> 29) + i) & 7) - ((i >> 32) >> 29)] ^ *(off_602088 + i);
                    write(1, &buf, 1uLL);
                  }
                  exit(1);
                }
                ...
```
flag_40084E() 는 입력받은 9자리 문자열 a1 과  
off_602088 를 XOR 연산을 한 결과가  
"0x00CTF{" 가 되면  
변수 buf 에 flag 를 생성 및 저장하고  
write() 를 통해 flag 를 출력해준다.  
  
off_602088 의 값은  
`01 16 79 44 04 64 12 5A  01` 이다.  
  
XOR 연산은  
A ^ B = B ^ A = C 이면 A ^ C = C ^ A = B 가 성립한다.  
  
따라서 password.py 를 만들어서  
off_602088 과 "0x00CTF{" 를 XOR 해서 password 를 알아낸다.  
```python
extrac = '01 16 79 44 04 64 12 5A  01'.split()
extrac = map(lambda x: int(x,16), extrac)
flag = '0x00CTF{'
flag = map(ord, flag)

password = ''
for i, j in zip(extrac, flag):
    password += chr(i ^ j)

print password
```

```
$ python password.py
1nItG0T!
$ ./hello
Welcome to the Twinlight Zone!!!
Password: 1nItG0T!
0x00CTF{0bfU5c473D_PtR4Z3}
```
