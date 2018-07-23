`#gdb`, `#command`, `#script`
# dingJMax (106pts) 94 solversEasyreversing
> I prepared the Rhythm game "dingJMax" for you.  
> This is really hard... Can you get prefect score for flag?  
>   
> [dingJMax](https://s3.ap-northeast-2.amazonaws.com/sctf2018-qual-binaries/dingJMax_e524bb0f4410c9be57cf1c387ab79872dcbbf592)  
```
md5sum
0a55a302e2da26f2e4cd327f056f8219  dingJMax
```

## 목차
1. [실행 화면](#실행-화면)
2. [분석](#분석)
3. [gdb\_command](#gdb\_command)

## 실행 화면

```
           _ _                  _ __  __
          | (_)                | |  \/  |
        __| |_ _ __   __ _     | | \  / | __ ___  __
       / _` | | '_ \ / _` |_   | | |\/| |/ _` \ \/ /
      | (_| | | | | | (_| | |__| | |  | | (_| |>  <
       \__,_|_|_| |_|\__, |\____/|_|  |_|\__,_/_/\_\
                      __/ |
                     |___/





Press any key to conitnue...
```

```


      1. Press D, F, J, K Button when 'o' is closed to baseline(thick one)
      2. You can get score depending on your correctness
      3. If you got MAX SCORE(1000000), you would see correct flag on flag dialog
      4. This is a really hard game, so you need some practice :)
      5. Bad luck :P






Press any key to conitnue...
```

```
┌─ dingJMax ────────────────┐      ┌─ SCORE (max 1000000) ─┐
│ ┌─────┬─────┬─────┬─────┐ │      │                       │
│ │     │     │     │     │ │      │                       │
│ │     │     │     │     │ │      │                       │
│ │     │     │     │     │ │      └───────────────────────┘
│ │     │     │     │     │ │
│ │     │     │     │     │ │
│ │     │     │     │     │ │      ┌─ here is your FLAG (Did you get perfect score?) ─────────┐
│ │     │     │     │     │ │      │                                                          │
│ │     │     │     │     │ │      │                                                          │
│ │     │ ┌──────────────────────────────────────┐                                            │
│ │     │ │                                      │────────────────────────────────────────────┘
│ │     │ │             stage start              │
│ │     │ │                                      │
│ │     │ └──────────────────────────────────────┘
│ │     │     │     │     │ │      ┌─ Log ────────────┐
│ │     │     │     │     │ │      │                  │
│ │     │     │     │     │ │      │                  │
│ │     │     │     │     │ │      │                  │
│ │     │     │     │     │ │      └──────────────────┘
│ │     │     │     │     │ │
│ │     │     │     │     │ │
│ ┝━━━━━┿━━━━━┿━━━━━┿━━━━━┥ │
│ │     │     │     │     │ │
│ │  d  │  f  │  j  │  k  │ │
│ │     │     │     │     │ │
│ └─────┴─────┴─────┴─────┘ │
│                           │
│                           │
└───────────────────────────┘
```

```
┌─ dingJMax ────────────────┐      ┌─ SCORE (max 1000000) ─┐
│ ┌─────┬─────┬─────┬─────┐ │      │                       │
│ │     │     │     │     │ │      │ 0                     │
│ │     │     │     │     │ │      │                       │
│ │     │     │     │     │ │      └───────────────────────┘
│ │     │     │     │     │ │
│ │     │     │     │     │ │
│ │  o  │     │     │     │ │      ┌─ Here is your FLAG (Did you get perfect score?) ─────────┐
│ │     │     │     │     │ │      │                                                          │
│ │     │     │     │     │ │      │    SCTF{qN7BuRx4rElDv84dgNaaNBanZf0HSHFjqOvbkFfgTRg3r}   │
│ │     │     │     │     │ │      │                                                          │
│ │     │     │     │     │ │      └──────────────────────────────────────────────────────────┘
│ │     │     │     │     │ │
│ │     │     │     │     │ │
│ │     │     │     │     │ │
│ │     │  o  │     │     │ │      ┌─ Log ────────────┐
│ │     │     │     │     │ │      │                  │
│ │     │     │     │     │ │      │                  │
│ │     │     │     │     │ │      │                  │
│ │     │     │     │     │ │      └──────────────────┘
│ │     │     │     │     │ │
│ │     │     │     │     │ │
│ ┝━━━━━┿━━━━━┿━━━━━┿━━━━━┥ │
│ │     │     │     │     │ │
│ │  d  │  f  │  j  │  k  │ │
│ │     │     │     │     │ │
│ └─────┴─────┴─────┴─────┘ │
│                           │
│                           │
└───────────────────────────┘
                                 PERFECT    : 0
                                 GREAT      : 0
                                 MISS       : 0
                                 TOTAL NOTES: 299
```

```
┌─ dingJMax ────────────────┐      ┌─ SCORE (max 1000000) ─┐
│ ┌─────┬─────┬─────┬─────┐ │      │                       │
│ │     │     │     │     │ │      │ 66889                 │
│ │     │     │     │     │ │      │                       │
│ │     │     │     │     │ │      └───────────────────────┘
│ │     │     │     │     │ │
│ │     │     │     │     │ │
│ │     │     │     │     │ │      ┌─ Here is your FLAG (Did you get perfect score?) ─────────┐
│ │     │     │     │     │ │      │                                                          │
│ │     │     │     │     │ │      │    SCTF{jTaYtTKJ0CqqCZEkh6O8xQHtkSkaV4aH810JGAe7lHUh9}   │
│ │     │     │     │     │ │      │                                                          │
│ │     │     │     │     │ │      └──────────────────────────────────────────────────────────┘
│ │     │     │     │     │ │
│ │     │     │     │     │ │
│ │     │     │     │     │ │
│ │     │     │     │     │ │      ┌─ Log ────────────┐
│ │     │     │     │     │ │      │                  │
│ │     │     │     │     │ │      │      MISS!       │
│ │     │     │     │     │ │      │                  │
│ │     │     │     │     │ │      └──────────────────┘
│ │     │     │     │     │ │
│ │     │     │     │     │ │
│ ┝━━━━━┿━━━━━┿━━━━━┿━━━━━┥ │
│ │     │     │     │     │ │
│ │  d  │  f  │  j  │  k  │ │
│ │     │     │     │     │ │
│ └─────┴─────┴─────┴─────┘ │
│                           │
│                           │
└───────────────────────────┘
                                 PERFECT    : 1
                                 GREAT      : 38
                                 MISS       : 124
                                 TOTAL NOTES: 299
```
  
  

## 분석
직접 플레이 해보면 키를 누를 때마다 FLAG가 바뀐다는 것을 알 수가 있는데  
코드를 확인해보면  
```
    76      key = wgetch(stdscr);
    77      if ( key == 'f' )
    78      {
              ...
    83      }
    84      if ( key > 'f' )
    85      {
    86        if ( key == 'j' )
    87        {
                ...
    92        }
    93        if ( key == 'k' )
    94        {
                ...
    99        }
   100      }
   101      else if ( key == 'd' )
   102      {
              ...
   107      }
```
`d`, `f`, `j`, `k` 키를 누를 때마다 FLAG 에 영향을 주고  
  
대표로 `f` 눌렀을 때를 보면
```
    77      if ( key == 'f' )
    78      {
    79        key_f = 1;
    80        sub_401C9A('f' * time_clock);             // 
    81        sub_400C5E(dest);
    82        goto LABEL_20;
    83      }
```
`f` 와 `time_clock` 를 이용해서 FLAG 를 바꾼다는 것을 예측할 수 있습니다.  
  
`PERFECT!` 일 조건을 알아보자면
```
   135      if ( key_d )
   136      {
   137        if ( note_d == 'o' )
   138        {
   139          if ( time_clock == 20 * ((unsigned __int64)(0x0CCCCCCCCCCCCCCCDLL * (unsigned __int128)time_clock >> 64) >> 4) )
   140          {
   141            show_log(window_log_obj, (__int64)"PERFECT!", 200);
   142            ++perfect_count;
   143          }
   144          else
   145          {
   146            show_log(window_log_obj, (__int64)"GREAT!", 200);
   147            ++great_count;
   148          }
   149        }
   150        else
   151        {
   152          show_log(window_log_obj, (__int64)"MISS!", 200);
   153          ++miss_count;
   154        }
   155      }
```
조건은 137과 139번째의 줄에 있습니다.  
`note(o)` 가 전역변수 `note_d`에 있으면서 `time_clock` 역시 20의 배수일 때에 `PERFECT!` 입니다.  
  
이번엔 `note(o)` 테이블 부분입니다.
```
   122      if ( time_clock == 20 * ((unsigned __int64)(0x0CCCCCCCCCCCCCCCDLL * (unsigned __int128)time_clock >> 64) >> 4) )
   123      {
   124        for ( k = 19; k > 0; --k )                // note 가 1칸 내려옴
   125          dword_607600[k] = dword_607600[k - 1];
   126        dword_607600[0] = *(_DWORD *)off_603280[(unsigned __int64)(0x0CCCCCCCCCCCCCCCDLL * (unsigned __int128)time_clock >> 64) >> 4];
   127      }
```
딱 `PERFECT!` 시간에 한칸씩 업데이트됩니다.  
  
앞에서 본 내용을 정리하면  
1. `dword_607600` 은 note 정보가 들어있는 테이블이고  
2. 테이블이 업데이트되는 `time_clock`은 20의 배수이며 `PERFECT!` 시간이다.  
  
그래서 저는 122 번째 줄의 if 문 안으로 들어올 때(`time_clock`이 `PERFECT!` 일 때)  
key 를 자동으로 누르도록 gdb 명령을 짜봤습니다.  

## gdb\_command
```
b *0x0000000000401623
command 1
if *(int *)(0x607600+0x48) != 0x20202020
    if *(char *)(0x607600+0x48) == 0x6f
        p/c $eax='d'
        end
    if *(char *)(0x607600+0x48+1) == 0x6f
        p/c $eax='f'
        end
    if *(char *)(0x607600+0x48+2) == 0x6f
        p/c $eax='j'
        end
    if *(char *)(0x607600+0x48+3) == 0x6f
        p/c $eax='k'
        end
    p $rip=0x000000000040141A
    ignore 1 1
    end
c
end
```

```
┌─ dingJMax ────────────────┐      ┌─ SCORE (max 1000000) ─┐
│ ┌─────┬─────┬─────┬─────┐ │      │                       │
│ │     │  o  │     │     │ │      │ 244147                │
│ │     │     │     │     │ │      │                       │
│ │  o  │     │     │     │ │      └───────────────────────┘
│ │     │     │     │     │ │
│ │  o  │     │     │     │ │
│ │     │     │     │     │ │      ┌─ Here is your FLAG (Did you get perfect score?) ─────────┐
│ │  o  │     │     │     │ │      │                                                          │
│ │     │     │     │     │ │      │    SCTF{v_Zt_JfOk__U7PJGfmxCPQLoWUobxR55M963VNEzbNN_c}   │
│ │     │  o  │     │     │ │      │                                                          │
│ │     │     │     │     │ │      └──────────────────────────────────────────────────────────┘
│ │  o  │     │     │     │ │
│ │     │     │     │     │ │
│ │     │     │  o  │     │ │
│ │     │     │     │     │ │      ┌─ Log ────────────┐
│ │  o  │     │     │     │ │      │                  │
│ │     │     │     │     │ │      │     PERFECT!     │
│ │     │     │     │  o  │ │      │                  │
│ │     │     │     │     │ │      └──────────────────┘
│ │  o  │     │     │     │ │
│ │     │     │     │     │ │
│ ┝━━━━━┿━━━━━┿━━━━━┿━━━━━┥ │
│ │     │     │     │     │ │
│ │  d  │  f  │  j  │  k  │ │
│ │     │     │     │     │ │
│ └─────┴─────┴─────┴─────┘ │
│                           │
│                           │
└───────────────────────────┘
                                 PERFECT    : 73
                                 GREAT      : 0
                                 MISS       : 0
                                 TOTAL NOTES: 299
```

```
┌─ dingJMax ────────────────┐      ┌─ SCORE (max 1000000) ─┐
│ ┌─────┬─────┬─────┬─────┐ │      │                       │
│ │     │     │     │     │ │      │ 782608                │
│ │     │     │  o  │     │ │      │                       │
│ │     │     │     │     │ │      └───────────────────────┘
│ │     │     │     │     │ │
│ │  o  │     │     │     │ │
│ │     │     │  o  │     │ │      ┌─ Here is your FLAG (Did you get perfect score?) ─────────┐
│ │     │     │     │     │ │      │                                                          │
│ │     │     │     │     │ │      │    SCTF{zGqvftvPhQwk8PvhVhqE026AlJbjgJVi4B8YNkTicMJ_b}   │
│ │     │     │     │     │ │      │                                                          │
│ │  o  │     │     │     │ │      └──────────────────────────────────────────────────────────┘
│ │     │     │     │     │ │
│ │     │     │     │     │ │
│ │  o  │     │     │     │ │
│ │     │     │     │     │ │      ┌─ Log ────────────┐
│ │     │     │     │     │ │      │                  │
│ │     │     │     │     │ │      │     PERFECT!     │
│ │     │     │     │     │ │      │                  │
│ │     │     │     │     │ │      └──────────────────┘
│ │     │     │     │     │ │
│ │     │     │     │     │ │
│ ┝━━━━━┿━━━━━┿━━━━━┿━━━━━┥ │
│ │     │     │     │     │ │
│ │  d  │  f  │  j  │  k  │ │
│ │     │     │     │     │ │
│ └─────┴─────┴─────┴─────┘ │
│                           │
│                           │
└───────────────────────────┘
                                 PERFECT    : 234
                                 GREAT      : 0
                                 MISS       : 0
                                 TOTAL NOTES: 299
```

```
┌─ dingJMax ────────────────┐      ┌─ SCORE (max 1000000) ─┐
│ ┌─────┬─────┬─────┬─────┐ │      │                       │
│ │     │     │     │     │ │      │ 1000000               │
│ │     │     │     │     │ │      │                       │
│ │     │     │     │     │ │      └───────────────────────┘
│ │     │     │     │     │ │
│ │     │     │     │     │ │
│ │     │     │     │     │ │      ┌─ Here is your FLAG (Did you get perfect score?) ─────────┐
│ │     │     │     │     │ │      │                                                          │
│ │     │     │     │     │ │      │    SCTF{I_w0u1d_l1k3_70_d3v3l0p_GUI_v3rs10n_n3x7_t1m3}   │
│ │     │     │     │     │ │      │                                                          │
│ │     │     │     │     │ │      └──────────────────────────────────────────────────────────┘
│ │     │     │     │     │ │
│ │     │     │     │     │ │
│ │     │     │     │     │ │
│ │     │     │     │     │ │      ┌─ Log ────────────┐
│ │     │     │     │     │ │      │                  │
│ │     │     │     │     │ │      │     PERFECT!     │
│ │     │     │     │     │ │      │                  │
│ │     │     │     │     │ │      └──────────────────┘
│ │     │     │     │     │ │
│ │     │     │     │     │ │
│ ┝━━━━━┿━━━━━┿━━━━━┿━━━━━┥ │
│ │     │     │     │     │ │
│ │  d  │  f  │  j  │  k  │ │
│ │     │     │     │     │ │
│ └─────┴─────┴─────┴─────┘ │
│                           │
│                           │
└───────────────────────────┘
                                 PERFECT    : 299
                                 GREAT      : 0
                                 MISS       : 0
                                 TOTAL NOTES: 299
```

`SCTF{I_w0u1d_l1k3_70_d3v3l0p_GUI_v3rs10n_n3x7_t1m3}`
