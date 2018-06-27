import re
import string

chset = string.ascii_lowercase+string.digits+'{}?_;)'+string.ascii_uppercase
flag = 'dimi{ca1cul4t3d'#_inv3rs3?_0r_us3d_z3?_0h_y0u_ar3_4_F0Ol_;)}'
length = len(flag)

bp1 = gdb.Breakpoint('*rand')
bp1.silent = True
for i in range(0x74//2-len(flag)):
    flag_ = re.sub('[;)]', lambda m:{';':r'\;', ')':r'\)'}[m.group()], flag)
    for c in chset:
        bp1.ignore_count = length
        gdb.execute('r <<< {}'.format(flag_+'\\'+c))
        gdb.execute('ni {}'.format(21))
        key = gdb.execute('x/hd $rip+0x200719+{}'.format(length * 2), to_string=True).split()[-1]
        key = int(key)
        dx = gdb.parse_and_eval('$dx')
        dx = int(dx)
        if key == dx:
            flag += c
            length += 1
            break
    sys.stderr.write(flag+'\n')
gdb.execute('q')
