from socket import *
import sys
from time import *
import threading
from gmpy2 import *
import random, string

#======================================================================================
# function "xgcd" tackes positive integers a, b as input
# and return a triple (g, x, y), such that ax + by = g = gcd(a, b).
def xgcd(b, n):
    x0, x1, y0, y1 = 1, 0, 0, 1
    while n != 0:
        q, b, n = b // n, n, b % n
        x0, x1 = x1, x0 - q * x1
        y0, y1 = y1, y0 - q * y1
    return  b, x0, y0

# An application of extended GCD algorithm to finding modular inverses:
def mulinv(b, n):
    g, x, _ = xgcd(b, n)
    if g == 1:
        return x % n
# - https://en.wikibooks.org/wiki/Algorithm_Implementation/Mathematics/Extended_Euclidean_algorithm
#=====================================================================================

def GeneratePrime(Base, randomST):
    while True:
        k1 = mpz_urandomb(randomST, 1023)
        k2 = k1 + mpz_urandomb(randomST, 1023)

        #Add Random Number to Bignumber, then find next prime number
        p1 = next_prime(Base+k1)
        p2 = next_prime(Base+k2)
        #Prime Checking
        if is_prime(p1, 100) and is_prime(p2, 100):
            return [p1, p2]

def GenerateKeys(p, q):
    e = 65537
    n = p * q
    pin = (p-1)*(q-1)
    d = mulinv(e, pin)
    return [e, d, n]


def MakeRandomString():
    return ''.join(random.choice(string.lowercase+string.uppercase+string.digits) for i in xrange(32))

def PrintIntro(conn):
    conn.send("ggggg\n")


Flag = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"

def ClinetHandle(conn):
    TimeLimit = 1
    randomST = random_state()
    #Make Big Number with 2048 bits
    BaseNumber = (mpz(2) ** mpz(2048)) + mpz_urandomb(randomST, 512)
    if True:
        PrintIntro(conn)
        conn.send("Generating Keys... Wait some seconds\n")
        sleep(1)

        p, q = GeneratePrime(BaseNumber, randomST)
        #sleep is for resting of server
        sleep(1)

        PublicKey, PrivateKey, N = GenerateKeys(p, q)
        #sleep is for resting of server
        sleep(1)
        
        Data = MakeRandomString()
        Data = int(Data.encode('hex'),16)

        EncryptedData = powmod(Data, PublicKey, N)
        conn.send("Key Generating is Ended\n")
        conn.send("Encrypted Data : %d\nPublic Key : %d\nN : %d\n" % (EncryptedData, PublicKey, N))
        
        s_time = time()
        Answer = conn.recv(1500)
        e_time = time()

        #Time Out
        if e_time - s_time > TimeLimit:
            conn.send("Time Out!!!!!\n")
            conn.close()
            return

        #Answer is OK
        if int(Answer) == Data:
            conn.send(Flag+"\n")
            conn.close()
            return
        conn.send("You are Wrong!!!!!!\n")
        conn.close() 
        return
    else:
        conn.close()
        print "SOCKET ERROR"
        return

PORT = 40002
serverSocket = socket()

try:
    serverSocket.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
    serverSocket.bind(('', PORT))
except error as msg:
    print 'Bind failed. Error Code : ' + str(msg[0]) + ' Message ' + msg[1]
    sys.exit()

serverSocket.listen(30)

while 1:
    try:
        conn, addr = serverSocket.accept()
    except socket.error:
        break
    print 'Connected with ' + addr[0] + ':' + str(addr[1])
    t = threading.Thread(target = ClinetHandle, args=(conn,))
    t.start()        

serverSocket.close()