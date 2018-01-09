# challenge-007 (HexaPass(TM))
50 pt

## Description
> The new and revolutionary HexaPass protection system is unbeatable... Can you break it?

## Writeup
```
     1	__int64 __fastcall main(__int64 a1, char **a2, char **a3)
...
    12	  fgets(s, 1024, stdin);
    13	  s[(signed int)(strlen(s) - 1)] = 0;
    14	  if ( sub_4006FD(s) )
    15	    puts("Wrong Key!\n");
...
```
우리가 봐야할 함수는 `sub_4006FD()` 이며 반환값이 0 (false) 여야한다.  
  
`sub_4006FD()` 는 입력된 값이 hexadecimal 인지 확인하여 `byte_601080[:4]` 와 비교하는 함수이다.  
일단 반환값이 0 이어야 하므로 프로그램의 흐름이 어떻게 흘러가야하는지 확인한다.  
```
     1	signed __int64 __fastcall sub_4006FD(_BYTE *a1)
     2	{
...
    10	  while ( *v4 )
    11	  {
...
    23	    if ( !*v5 )
    24	      return 0xFFFFFFFFLL;
...
    34	    if ( byte_601080[(unsigned __int64)(v3 & 3)] != v2 )
    35	      return 1LL;
...
    38	  }
...
    42	  return 0LL;
    43	}
```
결국 42 번째 줄의 return 을 만나야 한다는 것을 알 수가 있다.  
  
좀 더 분석해보면
```
     1	signed __int64 __fastcall sub_4006FD(_BYTE *a1)
     2	{
...
     8	  v4 = a1;
...
    10	  while ( *v4 )
    11	  {
    12	    v2 = 0;
    13	    if ( *v4 <= 0x2F || *v4 > 0x39 )
    14	    {
    15	      if ( *v4 > 0x60 && *v4 <= 0x66 )          // if [a-f]
    16	        v2 = 16 * (*v4 - 0x57);                 // [a-f] -> 0xa0~0xf0
    17	    }
    18	    else
    19	    {                                           // if [0-9]
    20	      v2 = 16 * (*v4 - 0x30);                   // [0-9] -> 0x00~0x90
    21	    }
    22	    v5 = v4 + 1;
...
```
13-21 의 분기문은  
입력값이 hexadecimal 의 'a'~'f' 인지  
아니면 hexadecimal 의 '0'~'9' 인지 체크하고  
4 bit shift 연산해서  
`v2` 에 저장한다.  
  
만약 `*v4` 가 'b' 이면 `v2` 는 0xb0 이 저장된다.  

```
     1	signed __int64 __fastcall sub_4006FD(_BYTE *a1)
     2	{
...
    10	  while ( *v4 )
    11	  {
...
    22	    v5 = v4 + 1;
    23	    if ( !*v5 )
    24	      return 0xFFFFFFFFLL;
    25	    if ( *v5 <= 0x2F || *v5 > 0x39 )
    26	    {
    27	      if ( *v5 > 0x60 && *v5 <= 0x66 )          // if [a-f]
    28	        v2 |= *v5 - 0x57;                       // [a-f] -> 0x0a~0x0f
    29	    }
    30	    else
    31	    {                                           // if [0-9]
    32	      v2 |= *v5 - 0x30;                         // [0-9] -> 0x00~0x09
    33	    }
    34	    if ( byte_601080[(unsigned __int64)(v3 & 3)] != v2 )
    35	      return 1LL;
...
    37	    v4 = v5 + 1;
    38	  }
...
```
25-33 번째 줄을 보면  
13-21 번째 줄과 비슷한데 `v2` 에 OR 연산을 하고있다는게 차이점이다.  
  
만약 `*v4` 가 'a' 이면 `v2` 는 0xba 가 된다. (`v2 = 0xb0 | 0x0a`)  
  
  
그렇게 만들어진 `v2` 를 `byte_601080` 와 같은지 한 바이트씩 비교한다 (line 34)
  
```
byte_601080 = BA BE CA B1
```
`babecab1` 을 입력하면 `v2` 에 차례로 `0xba`, `0xbe`, `0xca`, `0xb1` 이 들어게 되는 것이다.
```
$ ./c4
Program protected by HexaPass(TM)

Enter key: babecab1
**********************
**  0x00CTF{H3x4p4sS_SucK5!-babecab1-R0cK5!}
**********************
Greetings to:

  .:xKOo'              ,dKKd;.   'd0Kx:.   :ddddddddd: 'dddddddd:   :dddddddd;
 kMMMMMMMN.          'NMMMMMMWo.NMMMMMMWd  OMMMMMMMMMO lMMMMMMMM0 :NMMMMMMMMMk
 0MW00MMMM'          ,MMXkXMMMk.MMNkOMMMk  OMMMx''KMMO lMMMK''''. OMMMx''xMMMk
 d:. lMMMW...      . 'x'  OMMMx.O'  lMMMk  OMMMK:..''. lMMM0....  OMMMl  :XXXd
 ONO:dMMMM'kWOc.'lOW',WOl'OMMMk'W0o,dMMMk  .dKMMMWO;   lMMMMMMMMl OMMMl
 0MMMo'lON'kMMMMMMX: ,MMMM;:xNk'MMMMd;dXk     'dNMMMNc lMMMNdddd' OMMMl
 0MMMl .:O..lNMMMMMW.,MMMM' ,xd.WMMMl 'dd  ,:::. dMMMO lMMMO      OMMMl  .:::'
 0MMMKKMMM':KW0dxKMM',MMMM0NMMk'MMMMXNMMk  OMMMO:xMMMO lMMMX::::: OMMMO::OMMMk
 dWMMMMMM0.;:.    .c..XMMMMMMX:.0MMMMMMNo  OMMMMMMMMMO lMMMMMMMMM.OMMMMMMMMWx.
   'dOx:.              'lkOo'    .ckOd'    ,:::::::::, .::::::::: ,::::::::.

--
Crackme by pico
  Greetings to the 0x00sec.org Community
```
