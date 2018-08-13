from z3 import *

serial = '76876-77776'
len = 4
passwd = [BitVec('passwd%i'%i, 8)for i in range(len)]

s = Solver()

s.add(passwd[3] == ord('p'))

for i in range(len):
	s.add(passwd[i] >= ord('a'))
	s.add(passwd[i] <= ord('z'))

for i in range(len):
	for j in range(i+1,len):
		s.add(passwd[i] != passwd[j])

passwd_0_0 = (passwd[0] & 1) + 5
passwd_0_1 = ((passwd[0] >> 4) & 1) + 5
passwd_0_2 = ((passwd[0] >> 1) & 1) + 5
passwd_0_3 = ((passwd[0] >> 2) & 1) + 5
passwd_0_4 = ((passwd[0] >> 3) & 1) + 5

passwd_1_0 = (passwd[1] & 1) + 1
passwd_1_1 = ((passwd[1] >> 4) & 1) + 1
passwd_1_2 = ((passwd[1] >> 1) & 1) + 1
passwd_1_3 = ((passwd[1] >> 2) & 1) + 1
passwd_1_4 = ((passwd[1] >> 3) & 1) + 1

s.add(passwd_0_0 + passwd_1_3 == serial[0])
s.add(passwd_0_4 + passwd_1_4 == serial[1])
s.add(passwd_0_2 + passwd_1_1 == serial[2])
s.add(passwd_0_3 + passwd_1_0 == serial[3])
s.add(passwd_0_1 + passwd_1_2 == serial[4])

passwd_2_0 = (passwd[2] & 1) + 5
passwd_2_1 = ((passwd[2] >> 4) & 1) + 5
passwd_2_2 = ((passwd[2] >> 1) & 1) + 5
passwd_2_3 = ((passwd[2] >> 2) & 1) + 5
passwd_2_4 = ((passwd[2] >> 3) & 1) + 5

passwd_3_0 = (passwd[3] & 1) + 1
passwd_3_1 = ((passwd[3] >> 4) & 1) + 1
passwd_3_2 = ((passwd[3] >> 1) & 1) + 1
passwd_3_3 = ((passwd[3] >> 2) & 1) + 1
passwd_3_4 = ((passwd[3] >> 3) & 1) + 1

s.add(passwd_2_0 + passwd_3_3 == serial[6])
s.add(passwd_2_4 + passwd_3_4 == serial[7])
s.add(passwd_2_2 + passwd_3_1 == serial[8])
s.add(passwd_2_3 + passwd_3_0 == serial[9])
s.add(passwd_2_1 + passwd_3_2 == serial[10])

while True:
	if s.check() == sat:
		a = ''
		for i in range(4):
			a += chr(s.model()[passwd[i]].as_long())
		print(a)
		flag = map(ord, list(a))
		s.add(Not(And(passwd[0] == flag[0], passwd[1] == flag[1], passwd[2] == flag[2], passwd[3] == flag[3])))
	else: break
# CSimpleStringT Class: https://msdn.microsoft.com/en-us/library/sddk80xf.aspx
# itow_s : https://msdn.microsoft.com/ko-kr/library/0we9x30h.aspx
