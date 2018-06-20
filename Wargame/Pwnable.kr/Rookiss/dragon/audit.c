// gcc -o dragon dragon.c -m32 -no-pie -fno-pic -mpreferred-stack-boundary=2 -fstack-protector
#include <stdio.h>

struct Dragon {
	void (*printInfo)(struct Dragon* dragon);
	int info;
	char hp;
	char heal;
	short dummy;
	int damage;
};

struct Player {
	int class;
	int hp;
	int mana;
	void (*printInfo)(struct Player* dragon);
};

void PlayGame();
void FightDragon(int hero);
int PriestAttack(struct Player *player, struct Dragon *dragon);
int KnightAttack(struct Player *player, struct Dragon *dragon);
int PrintPlayerInfo(struct Player *player);
int PrintMonsterInfo(struct Dragon *dragon);
int GetChoice();
void SecretLevel();
int Count;

int main()
{
  setvbuf(stdout, 0, 2, 0);
  setvbuf(stdin, 0, 2, 0);
  puts("Welcome to Dragon Hunter!");
  PlayGame();
  return 0;
}

void PlayGame()
{
  int hero; // [sp+1Ch] [bp-Ch]@1

  while ( 1 )
  {
    while ( 1 )
    {
      puts("Choose Your Hero\n[ 1 ] Priest\n[ 2 ] Knight");
      hero = GetChoice();
      if ( hero != 1 && hero != 2 )
        break;
      FightDragon(hero);
    }
    if ( hero != 3 )
      break;
    SecretLevel();                              // 3
  }
}

void FightDragon(int hero)
{
  char toggle; // al@1
  void *dragon_4; // ST1C_4@10
  int v3; // [sp+10h] [bp-18h]@7
  struct Player *player; // [sp+14h] [bp-14h]@1
  struct Dragon *dragon; // [sp+18h] [bp-10h]@1

  player = malloc(0x10u);
  dragon = malloc(0x10u);
  toggle = Count++;
  if ( toggle & 1 )
  {
    dragon->info = 1;
    dragon->hp = 80;
    dragon->heal = 4;
    dragon->damage = 10;
    dragon->printInfo = PrintMonsterInfo;
    puts("Mama Dragon Has Appeared!");
  }
  else
  {
    dragon->info = 0;
    dragon->hp = 50;
    dragon->heal = 5;
    dragon->damage = 30;
    dragon->printInfo = PrintMonsterInfo;
    puts("Baby Dragon Has Appeared!");
  }
  if ( hero == 1 )                              // Priest
  {
    player->class = 1;
    player->hp = 42;
    player->mana = 50;
    player->printInfo = PrintPlayerInfo;
    v3 = PriestAttack(player, dragon);
  }
  else
  {
    if ( hero != 2 )                            // Knight
      return;
    player->class = 2;
    player->hp = 50;
    player->mana = 0;
    player->printInfo = PrintPlayerInfo;
    v3 = KnightAttack(player, dragon);
  }
  if ( v3 )
  {
    puts("Well Done Hero! You Killed The Dragon!");
    puts("The World Will Remember You As:");
    dragon_4 = malloc(0x10u);
    __isoc99_scanf("%16s", dragon_4);
    puts("And The Dragon You Have Defeated Was Called:");
    (dragon->printInfo)(dragon);
  }
  else
  {
    puts("\nYou Have Been Defeated!");
  }
  free(player);
}

int PriestAttack(struct Player *player, struct Dragon *dragon)
{
  int choice; // eax@1

  do
  {
    (dragon->printInfo)(dragon);
    (player->printInfo)(player);
    choice = GetChoice();
    switch ( choice )
    {
      case 2:
        puts("Clarity! Your Mana Has Been Refreshed");
        player->mana = 50;
        printf("But The Dragon Deals %d Damage To You!\n", dragon->damage);
        player->hp -= dragon->damage;
        printf("And The Dragon Heals %d HP!\n", dragon->heal);
        dragon->hp += dragon->heal;
        break;
      case 3:
        if ( player->mana <= 24 )
        {
          puts("Not Enough MP!");
        }
        else
        {
          puts("HolyShield! You Are Temporarily Invincible...");
          printf("But The Dragon Heals %d HP!\n", dragon->heal);
          dragon->hp += dragon->heal;
          player->mana -= 25;
        }
        break;
      case 1:
        if ( player->mana <= 9 )
        {
          puts("Not Enough MP!");
        }
        else
        {
          printf("Holy Bolt Deals %d Damage To The Dragon!\n", 20);
          dragon->hp -= 20;
          player->mana -= 10;
          printf("But The Dragon Deals %d Damage To You!\n", dragon->damage);
          player->hp -= dragon->damage;
          printf("And The Dragon Heals %d HP!\n", dragon->heal);
          dragon->hp += dragon->heal;
        }
        break;
    }
    if ( player->hp <= 0 )
    {
      free(dragon);
      return 0;
    }
  }
  while ( dragon->hp > 0 );
  free(dragon);
  return 1;
}

int KnightAttack(struct Player *player, struct Dragon *dragon)
{
  int choice; // eax@1

  do
  {
    (dragon->printInfo)(dragon);
    (player->printInfo)(player);
    choice = GetChoice();
    if ( choice == 1 )
    {
      printf("Crash Deals %d Damage To The Dragon!\n", 20);
      dragon->hp -= 20;
      printf("But The Dragon Deals %d Damage To You!\n", dragon->damage);
      player->hp -= dragon->damage;
      printf("And The Dragon Heals %d HP!\n", dragon->heal);
      dragon->hp += dragon->heal;
    }
    else if ( choice == 2 )
    {
      printf("Frenzy Deals %d Damage To The Dragon!\n", 40);
      dragon->hp -= 40;
      puts("But You Also Lose 20 HP...");
      player->hp -= 20;
      printf("And The Dragon Deals %d Damage To You!\n", dragon->damage);
      player->hp -= dragon->damage;
      printf("Plus The Dragon Heals %d HP!\n", dragon->heal);
      dragon->hp += dragon->heal;
    }
    if ( player->hp <= 0 )
    {
      free(dragon);
      return 0;
    }
  }
  while ( dragon->hp > 0 );
  free(dragon);
  return 1;
}

int PrintPlayerInfo(struct Player *player)
{
  int result; // eax@2

  if ( player->class == 1 )
  {
    result = printf("[ Priest ] %d HP / %d MP\n"
				"\t[ 1 ] Holy Bolt [ Cost : 10 MP ]\n"
				"\t\tDeals 20 Damage.\n"
				"\t[ 2 ] Clarity [ Cost : 0 MP ]\n"
				"\t\tRefreshes All Mana.\n"
				"\t[ 3 ] HolyShield [ Cost: 25 MP ]\n"
				"\t\tYou Become Temporarily Invincible.\n", player->hp, player->mana);
  }
  else
  {
    result = player->class;
    if ( player->class == 2 )
      result = printf("[ Knight ] %d HP / 0 Mana\n"
					"\t[ 1 ] Crash\n"
					"\t\tDeals 20 Damage.\n"
					"\t[ 2 ] Frenzy\n"
					"\t\tDeals 40 Damage, But You Lose 20 HP.\n", player->hp);
  }
  return result;
}

int PrintMonsterInfo(struct Dragon *dragon)
{
  int result; // eax@2

  if ( dragon->info )
    result = printf("[ Mama Dragon ] %d HP / 10 Damage / +4 Life Regeneration.\n", dragon->hp);
  else
    result = printf("[ Baby Dragon ] %d HP / 30 Damage / +5 Life Regeneration.\n", dragon->hp);
  return result;
}

int GetChoice()
{
  int v1; // [sp+1Ch] [bp-Ch]@1

  __isoc99_scanf("%d", &v1);
  while ( getchar() != 10 )
    ;
  return v1;
}

void SecretLevel()
{
  char s1[10]; // [sp+12h] [bp-16h]@1
  //int v1; // [sp+1Ch] [bp-Ch]@1

  //v1 = *MK_FP(__GS__, 20);
  printf("Welcome to Secret Level!\nInput Password : ");
  __isoc99_scanf("%10s", &s1);
  if ( strcmp(&s1, "Nice_Try_But_The_Dragons_Won't_Let_You!") )
  {
    puts("Wrong!\n");
    exit(-1);
  }
  system("/bin/sh");
}
