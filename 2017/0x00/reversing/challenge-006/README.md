# challenge-006 (Suffled Love)
50 pt

## Description
> Find out if you and your computer are twin souls

## Writeup
`main()`:
```
...
    13	  __isoc99_scanf("%d", &v4);
    14	  v6 = sub_40069D(v5, v4);
    15	  if ( v6 )
    16	    puts("Oh. You are not the one :(.");
    17	  else
    18	    printf("You read my mind!!!. We are twin souls\n--> 0x00CTF{Y0uR_th3_0n3_%x} <--\n\n", v4);
...
```
`main()` 에는 `sub_40069D()` 의 값에 따라 플래그가 나타난다.  
  
`sub_40069D()`:
```
     1  __int64 __fastcall sub_40069D(signed int a1, int a2)
     2  {
...
    10	  unsigned int v9; // [sp+1Ch] [bp-34h]@1
...
    33    result = a2 ^ v9;
    34    v3 = *MK_FP(__FS__, 40LL) ^ v12;
    35    return result;
    36  }
```
`sub_40069D()` 는 0을 반환해야 한다.  
따라서 33번째 줄에 BP 걸고 v9 를 확인한다.
```
.text:00000000004007C5 mov     eax, [rbp+var_48]
.text:00000000004007C8 xor     [rbp+var_34], eax               ; BP and look [rbp+var_34]
.text:00000000004007CB mov     eax, [rbp+var_34]
```
```
pwndbg> x/wd $rbp-0x34
0x7fffffffdcac: 314066
```
```
$ ./c1
Shuffled Love
by p1c0

My PIN is 371173
Your PIN:314066

You read my mind!!!. We are twin souls
--> 0x00CTF{Y0uR\_th3\_0n3\_4cad2} <--
```
