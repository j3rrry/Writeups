'''
0x400ae0    test   rax, rax
b *0x400ae0
p *$rax = "MAIN=31173\x00"
4d41494e 3d333131 3733
p *$rax = 0x4e49414d
p *($rax+4) = 0x3131333d
p *($rax+8) = 0x003337
'''

# 0x402028    cmp    rax, rdx	; RDX = 0x12
passwd = ['a'] * 0x12

# 0x400ae0    test   rax, rax
gdb.Breakpoint('*0x400ae0').silent = True
gdb.execute('r <<< {}'.format(''.join(passwd)))

# MAIN=31173\x00
gdb.execute('p *$rax = 0x4e49414d', to_string=True)
gdb.execute('p *($rax+4) = 0x3131333d', to_string=True)
gdb.execute('p *($rax+8) = 0x3337', to_string=True)

# to not crash
fgets = gdb.Breakpoint('*fgets')
fgets.silent = True
gdb.execute('c')
fgets.delete()


# 0x402076    call   rax
gdb.Breakpoint('*0x402076').silent = True
gdb.Breakpoint('*0x402078').silent = True
for i in range(0x12):
	gdb.execute('c')
	gdb.execute('p $edi = 0', to_string=True)
	gdb.execute('ni')
	#eax = int(gdb.execute('p $eax', to_string=True).split()[-1])
	eax = int(gdb.parse_and_eval('$eax'))
	passwd[i] = chr(eax)
	print(''.join(passwd))
	gdb.execute('p $eax=0', to_string=True)
gdb.execute('q')
