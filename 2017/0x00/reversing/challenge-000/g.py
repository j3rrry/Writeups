import gdb
import string

charset = string.ascii_lowercase
flag = ['a'] * 0xE

def exp_write():
    f = open('./exp', 'w')
    f.write(''.join(flag))
    f.close()

bp = gdb.Breakpoint('*0x0000000000400FF5')
bp.silent = True

for i in range(len(flag)):
    for c in charset:
        flag[i] = c
        exp_write()
        bp.ignore_count = i
        gdb.execute('r < ./exp')
        eflags = gdb.execute('p $eflags', to_string=True)
        if 'ZF' in eflags:
            break
gdb.execute('c')
gdb.execute('q')
# 0x00CTF{abbcdfinvidloz}
