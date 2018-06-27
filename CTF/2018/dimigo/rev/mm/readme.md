`#GDB`, `#script`, `#brute`, `#force`, `#PIE`
# mm
1. [Description](#description)
2. [Writeup](#writeup)
3. [시나리오](#시나리오)
4. [GDB script](#gdb-script)
5. [FLAG](#flag)

## Description
> mm  
> [mm](https://drive.google.com/uc?authuser=0&id=1vJt1gDFTh_ryt8YrVEy8n-tnJX7zttp3&export=download)

## Writeup
```
__int64 __fastcall main(__int64 a1, char **a2, char **a3)
{
  __int64 result; // rax@4
  __int64 v4; // rcx@4
  char s[264]; // [sp+0h] [bp-110h]@1
  __int64 v6; // [sp+108h] [bp-8h]@1

  v6 = *MK_FP(__FS__, 40LL);
  srand(0x17A3u);
  memset(s, 0, 0x100uLL);
  write(1, "Input: ", 7uLL);
  s[read(0, s, 0x100uLL) - 1] = 0;
  if ( (unsigned int)check(s) )
    write(1, "Correct\n", 8uLL);
  else
    write(1, "Wrong\n", 6uLL);
  result = 0LL;
  v4 = *MK_FP(__FS__, 40LL) ^ v6;
  return result;
}
```
Input 입력 받고  
check(s) 한 후  
Correct 또는 Wrong 으로 결과를 알려준다.  
  
check(s) 는 뭐하는 함수일까?  
```
__int64 __fastcall check(const char *s)
{
  int v1; // ST1C_4@2
  __int64 result; // rax@4
  __int64 v3; // rcx@4
  int i; // [sp+14h] [bp-21Ch]@1
  int len; // [sp+18h] [bp-218h]@1
  __int16 s1[260]; // [sp+20h] [bp-210h]@1
  __int64 v7; // [sp+228h] [bp-8h]@1

  v7 = *MK_FP(__FS__, 40LL);
  len = strlen(s);
  memset(s1, 0, 0x200uLL);
  for ( i = 0; i < len; ++i )
  {
    v1 = (unsigned __int16)rand();
    s1[i] = v1 * s[i] % (v1 + 1);
  }
  result = memcmp(s1, enc_key, 0x74uLL) == 0;
  v3 = *MK_FP(__FS__, 40LL) ^ v7;
  return result;
}
```
rand() 함수를 이용해서  
s(입력)를 암호화하고  
enc_key 와 비교한다.  
  
srand() 는 main 함수에 있었다.
```
     9    srand(0x17A3u);
```
이 문제는 역연산으로도 풀 수 있는데  
이번엔 GDB script 를 만들어 brute force 로 풀어볼꺼다.

## 시나리오
방법은 간단하다  
  
check() 에서 키값이 만들어지는 부분이다.
```
    17      s1[i] = v1 * s[i] % (v1 + 1);
```
s1 에 값이 생성될 때마다  
enc_key 와 같은지 비교할 것이다.  
해당 오프셋은
```
.text:0000000000000907                 mov     [rbp+rax*2+s1], dx
```
PIE 라서 rand symbol 를 이용해서 breakpoint 를 건다.  

1. 입력 문자열을 브루트포싱한다. (0123456789abcde...)
2. s1 에 값이 들어가기 직전에 $dx 와 enc_key 와 비교한다.



## GDB script
```
import re
import string

chset = string.ascii_lowercase+string.digits+'{}?_;)'+string.ascii_uppercase
flag = ''#'dimi{ca1cul4t3d_inv3rs3?_0r_us3d_z3?_0h_y0u_ar3_4_F0Ol_;)}'
length = len(flag)

bp1 = gdb.Breakpoint('*rand')
bp1.silent = True
for i in range(0x74//2-len(flag)):
    flag_ = re.sub('[;)]', lambda m:{';':r'\;', ')':r'\)'}[m.group()], flag)
    for c in chset:
        bp1.ignore_count = length
        gdb.execute('r <<< {}'.format(flag_+'\\'+c))
        gdb.execute('ni {}'.format(21))
        key = gdb.execute('x/hd $rip+0x200719+{}'.format(length * 2), to_string=True).split()[-1]
        key = int(key)
        dx = gdb.parse_and_eval('$dx')
        dx = int(dx)
        if key == dx:
            flag += c
            length += 1
            break
    sys.stderr.write(flag+'\n')
gdb.execute('q')
```
## FLAG
```
d
di
dim
dimi
dimi{
dimi{c
dimi{ca
dimi{ca1
dimi{ca1c
dimi{ca1cu
dimi{ca1cul
dimi{ca1cul4
dimi{ca1cul4t
dimi{ca1cul4t3
dimi{ca1cul4t3d
dimi{ca1cul4t3d_
dimi{ca1cul4t3d_i
dimi{ca1cul4t3d_in
dimi{ca1cul4t3d_inv
dimi{ca1cul4t3d_inv3
dimi{ca1cul4t3d_inv3r
dimi{ca1cul4t3d_inv3rs
dimi{ca1cul4t3d_inv3rs3
dimi{ca1cul4t3d_inv3rs3?
dimi{ca1cul4t3d_inv3rs3?_
dimi{ca1cul4t3d_inv3rs3?_0
dimi{ca1cul4t3d_inv3rs3?_0r
dimi{ca1cul4t3d_inv3rs3?_0r_
dimi{ca1cul4t3d_inv3rs3?_0r_u
dimi{ca1cul4t3d_inv3rs3?_0r_us
dimi{ca1cul4t3d_inv3rs3?_0r_us3
dimi{ca1cul4t3d_inv3rs3?_0r_us3d
dimi{ca1cul4t3d_inv3rs3?_0r_us3d_
dimi{ca1cul4t3d_inv3rs3?_0r_us3d_z
dimi{ca1cul4t3d_inv3rs3?_0r_us3d_z3
dimi{ca1cul4t3d_inv3rs3?_0r_us3d_z3?
dimi{ca1cul4t3d_inv3rs3?_0r_us3d_z3?_
dimi{ca1cul4t3d_inv3rs3?_0r_us3d_z3?_0
dimi{ca1cul4t3d_inv3rs3?_0r_us3d_z3?_0h
dimi{ca1cul4t3d_inv3rs3?_0r_us3d_z3?_0h_
dimi{ca1cul4t3d_inv3rs3?_0r_us3d_z3?_0h_y
dimi{ca1cul4t3d_inv3rs3?_0r_us3d_z3?_0h_y0
dimi{ca1cul4t3d_inv3rs3?_0r_us3d_z3?_0h_y0u
dimi{ca1cul4t3d_inv3rs3?_0r_us3d_z3?_0h_y0u_
dimi{ca1cul4t3d_inv3rs3?_0r_us3d_z3?_0h_y0u_a
dimi{ca1cul4t3d_inv3rs3?_0r_us3d_z3?_0h_y0u_ar
dimi{ca1cul4t3d_inv3rs3?_0r_us3d_z3?_0h_y0u_ar3
dimi{ca1cul4t3d_inv3rs3?_0r_us3d_z3?_0h_y0u_ar3_
dimi{ca1cul4t3d_inv3rs3?_0r_us3d_z3?_0h_y0u_ar3_4
dimi{ca1cul4t3d_inv3rs3?_0r_us3d_z3?_0h_y0u_ar3_4_
dimi{ca1cul4t3d_inv3rs3?_0r_us3d_z3?_0h_y0u_ar3_4_F
dimi{ca1cul4t3d_inv3rs3?_0r_us3d_z3?_0h_y0u_ar3_4_F0
dimi{ca1cul4t3d_inv3rs3?_0r_us3d_z3?_0h_y0u_ar3_4_F0O
dimi{ca1cul4t3d_inv3rs3?_0r_us3d_z3?_0h_y0u_ar3_4_F0Ol
dimi{ca1cul4t3d_inv3rs3?_0r_us3d_z3?_0h_y0u_ar3_4_F0Ol_
dimi{ca1cul4t3d_inv3rs3?_0r_us3d_z3?_0h_y0u_ar3_4_F0Ol_;
dimi{ca1cul4t3d_inv3rs3?_0r_us3d_z3?_0h_y0u_ar3_4_F0Ol_;)
dimi{ca1cul4t3d_inv3rs3?_0r_us3d_z3?_0h_y0u_ar3_4_F0Ol_;)}

real    7m54.800s
user    5m45.268s
sys     1m53.087s
```
시간이 좀 많이 걸렸다.
