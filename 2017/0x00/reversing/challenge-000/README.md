`#C++`
# challenge-000 (guessme)
50 pt
## Description
```
Hi there. Can you find the right key that unlocks the flag?

Platform: 64 bit Linux (developed on Ubuntu)
```

------------------------
다음은 main 함수입니다.
```C++
__int64 __fastcall main(signed int a1, char **a2, char **a3)
{
  _DWORD *v3; // rax@1
  _DWORD *v4; // rax@1
  signed int *v5; // rax@3
  __int64 v6; // rax@7
  signed int v7; // ebx@8
  unsigned __int64 v8; // rax@10
  _BYTE *v9; // rax@11
  int v10; // ebx@11
  _DWORD *v11; // rax@11
  __int64 v12; // rax@14
  __int64 v13; // rax@14
  __int64 result; // rax@15
  __int64 v15; // rcx@15
  int i; // [sp+1Ch] [bp-84h]@9
  char v17; // [sp+20h] [bp-80h]@1
  int v18; // [sp+30h] [bp-70h]@1
  char v19; // [sp+40h] [bp-60h]@1
  char v20; // [sp+60h] [bp-40h]@4
  __int64 v21; // [sp+88h] [bp-18h]@1

  v21 = *MK_FP(__FS__, 40LL);
  sub_4011FA(&v18, a2, a3);
  sub_401232(&v19, 14LL, &v18);
  sub_401216(&v18);
  LODWORD(v3) = algorism_4012D8(&v19, 13LL);
  *v3 = 0xE9;
  LODWORD(v4) = algorism_4012D8(&v19, 12LL);
  *v4 = 0x90;
  v18 = 0xB;
  sub_4010DF(&v19, &v18);
  sub_4012F8(&v17, &v19);
  while ( 1 )
  {
    sub_401338(&v18, &v19);
    if ( !(unsigned __int8)sub_40136C(&v17, &v18) )
      break;
    LODWORD(v5) = sub_4013B2(&v17);
    *v5 %= 26;
    sub_401394(&v17);
  }
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::basic_string(&v20);
  if ( a1 > 1 )
  {
    std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::operator=(&v20, a2[1]);
  }
  else
  {
    std::operator<<<std::char_traits<char>>(&std::cout, "Enter a key: ");
    std::operator>><char,std::char_traits<char>,std::allocator<char>>(&std::cin, &v20);
  }
  LODWORD(v6) = std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::length(&v20);	// length check
  if ( v6 != sub_401402((__int64)&v19) )
  {
    std::operator<<<std::char_traits<char>>(&std::cout, "FAIL\n");
    v7 = 1;
  }
  else
  {
    for ( i = 0; ; ++i )
    {
      LODWORD(v8) = std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::length(&v20);
      if ( i >= v8 )
        break;
      LODWORD(v9) = std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::operator[](&v20, i);
      v10 = *v9 - 'a';                          // ascii_lowercase
      LODWORD(v11) = algorism_4012D8(&v19, i);
      if ( v10 != *v11 )
      {
        std::operator<<<std::char_traits<char>>(&std::cout, "FAIL\n");
        v7 = 2;
        goto LABEL_15;
      }
    }
    std::operator<<<std::char_traits<char>>(&std::cout, "Good key!\n");
    LODWORD(v12) = std::operator<<<std::char_traits<char>>(&std::cout, "The flag is: 0x00CTF{");
    LODWORD(v13) = std::operator<<<char,std::char_traits<char>,std::allocator<char>>(v12, &v20);
    std::operator<<<std::char_traits<char>>(v13, "}\n");
    v7 = 0;
  }
LABEL_15:
  std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::~basic_string(&v20);
  sub_401294(&v19);
  result = (unsigned int)v7;
  v15 = *MK_FP(__FS__, 40LL) ^ v21;
  return result;
}
```

위 소스를 잘라서  
첫번째 분기점 부분을 봅니다.  
```C++
  LODWORD(v6) = std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::length(&v20); // length check
  if ( v6 != sub_401402((__int64)&v19) )
  {
    std::operator<<<std::char_traits<char>>(&std::cout, "FAIL\n");
    v7 = 1;
  }
```
길이 확인을 하는 부분입니다.  
어셈으로는 아래와 같으며 
```asm
.text:0000000000400F67                 call    sub_401402
.text:0000000000400F6C                 cmp     rbx, rax		; rax == 0xE
.text:0000000000400F6F                 setnz   al
.text:0000000000400F72                 test    al, al
.text:0000000000400F74                 jz      short loc_400F8F
.text:0000000000400F76                 mov     esi, offset aFail ; "FAIL\n"
.text:0000000000400F7B                 mov     edi, offset _ZSt4cout ; std::cout
.text:0000000000400F80                 call    __ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc ; std::operator<<<std::char_traits<char>>(std::basic_ostream<char,std::char_traits<char>> &,char const*)
```
`0000000000400F6C` 에 `rax` 값이 0xE(14) 였으며  
따라서 요구되는 `key` 값의 길이는 14이다.  
  
그리고 해당 키값이 맞는지 한 문자씩 검증합니다.
```C++
    for ( i = 0; ; ++i )
    {
      LODWORD(v8) = std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::length(&v20);
      if ( i >= v8 )
        break;
      LODWORD(v9) = std::__cxx11::basic_string<char,std::char_traits<char>,std::allocator<char>>::operator[](&v20, i);
      v10 = *v9 - 'a';                          // ascii_lowercase
      LODWORD(v11) = algorism_4012D8(&v19, i);
      if ( v10 != *v11 )
      {
        std::operator<<<std::char_traits<char>>(&std::cout, "FAIL\n");
        v7 = 2;
        goto LABEL_15;
      }
    }
    std::operator<<<std::char_traits<char>>(&std::cout, "Good key!\n");
    LODWORD(v12) = std::operator<<<std::char_traits<char>>(&std::cout, "The flag is: 0x00CTF{");
    LODWORD(v13) = std::operator<<<char,std::char_traits<char>,std::allocator<char>>(v12, &v20);
    std::operator<<<std::char_traits<char>>(v13, "}\n");
    v7 = 0;
  }
```
`v10 = *v9 - 'a'` 를 보아 소문자인 것으로 어림짐작했었고  
14자리의 키값의 검증이 모두 끝나면 `break` 를 통해 for 문을 나오게되고  
그렇게되면 `"Good key!"` 가 출력됩니다.  
  
1. 각 문자는 소문자 a-z. 26자로 이루어져 있고
2. 문자 하나씩 검증을 하기 때문에
3. 브루트 포스로 최대 14\*26 번만 하면 키값을 알아낼 수 있음

따라서 스크립트를 짰습니다.
```python
import gdb
import string

charset = string.ascii_lowercase
key = ['a'] * 0xE

def exp_write():
    f = open('./exp', 'w')
    f.write(''.join(key))
    f.close()

bp = gdb.Breakpoint('*0x0000000000400FF5')
bp.silent = True

for i in range(len(key)):
    for c in charset:
        key[i] = c
        exp_write()
        bp.ignore_count = i
        gdb.execute('r < ./exp')
        eflags = gdb.execute('p $eflags', to_string=True)
        if 'ZF' in eflags:
            break
gdb.execute('c')
gdb.execute('q')
# 0x00CTF{abbcdfinvidloz}
```
스크립트는 약 53초 걸렸습니다.
