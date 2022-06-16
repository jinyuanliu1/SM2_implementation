#coding:utf-8

import secrets
from hashlib import sha256
import utils
# import sm3

from gmssl import sm3, func
A = 0
B = 7

G_X = 55066263022277343669578718895168534326250603453777594175500187360389116729240
G_Y = 32670510020758816978083085130507043184471273380659243275938904335757337482424

G = (G_X, G_Y)
# 有限域的阶
P = 115792089237316195423570985008687907853269984665640564039457584007908834671663
# 椭圆曲线的阶
N = 115792089237316195423570985008687907852837564279074904382605163141518161494337


def precompute(ID,a,b,G_X,G_Y,x_A,y_A):# 依次是：ID，椭圆曲线参数a、b,G点x、y,公钥x、y
    a=str(a)
    b=str(b)
    G_X=str(G_X)
    G_Y=str(G_Y)
    x_A=str(x_A)
    y_A=str(y_A)
    ENTL=str(utils.get_bit_num(ID))

    joint=ENTL+ID+a+b+G_X+G_Y+x_A+y_A
    # print(joint)
    # print(type(joint))
    joint_b=bytes(joint,encoding='utf-8')
    digest= sm3.sm3_hash(func.bytes_to_list(joint_b))
    return int(digest, 16)



def generate_key():
    private_key = int(secrets.token_hex(32), 16)
    # private_key=46073170035150554270537571857385364045777863195799403284151994371942719363254
    public_key = utils.elliptic_multiply(private_key, G)
    # print("Private Key: ",private_key)
    # print("Public Key: " ,public_key)
    return private_key, public_key





def sign(private_key, message,Z_A):#私钥签名
    _M=Z_A+message
    _M_b=bytes(_M,encoding='utf-8')
    e = sm3.sm3_hash(func.bytes_to_list(_M_b))#str
    e=int(e, 16)

    k = secrets.randbelow(P)#0~P的随机数
    # k=12581384025608555250890351613719061476129424215689402246915303910451212003013
    random_point = utils.elliptic_multiply(k, G)#P点=k*G

    print(random_point,'randpoint')
    r =( e+random_point[0] )% N
    s = (utils.mod_inverse(1+private_key, N) * (k - r*private_key))%N  #用私钥进行签名
    return (r, s)#以元组形式存在的签名




def verify(public_key,ID, message, signature):

    r=signature[0]
    s=signature[1]

    Z=precompute(ID,A,B,G_X,G_Y,public_key[0],public_key[1])

    _M=str(Z)+message
    _M_b=bytes(_M,encoding='utf-8')
    e=sm3.sm3_hash(func.bytes_to_list(_M_b))#str
    e=int(e, 16)



    t=(r+s) % N

    point=utils.elliptic_multiply(s ,G)
    point1=utils.elliptic_multiply(t ,  public_key)
    point=utils.elliptic_add(point,point1)

    x1=point[0]
    x2=point[1]
    R=(e+x1)%N
    print('r',r)
    print(R)

    return R==r



if __name__=='__main__':
    '''签名'''
    prikey, pubkey = generate_key()
    print('公钥：',pubkey)
    message = "hello,world!"
    ID='zrz'
    Z_A=precompute(ID,A,B,G_X,G_Y,pubkey[0],pubkey[1])
    signature = sign(prikey, message,str(Z_A))
    print("签名: ",signature)

    '''验证'''
    if verify(pubkey,ID,message,signature)==1:
        print('验证通过')


