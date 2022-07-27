# SM2 impl sm2 with RFC6979
# SM2 加解密
# 原理
# 加密算法


![image](https://user-images.githubusercontent.com/75195549/181188020-2e40746b-b919-475c-b093-8f95acbcd0c2.png)



# 解密算法



![image](https://user-images.githubusercontent.com/75195549/181188058-fbb95e2c-ee68-4b61-a785-4efdbab16987.png)


# 获取公私钥

![image](https://user-images.githubusercontent.com/75195549/181188167-2776929e-8ecd-4f28-b522-617b3535db4d.png)




我们在SM2的加解密中要用到SM3作为哈希函数，所以先用python实现SM3。以标准文档中椭圆曲线标准参数为例，参数长度为256bit

add（）函数为椭圆曲线上的点加


![image](https://user-images.githubusercontent.com/75195549/181185546-a6bec49b-0fc2-4f2f-b41a-f64602dfb399.png)



mul_add（）为椭圆曲线上的点乘




当然，按照原理我们还实现了加密和解密函数

![image](https://user-images.githubusercontent.com/75195549/181187817-8aa790d2-4364-46e6-8482-184503fb8529.png)



![image](https://user-images.githubusercontent.com/75195549/181187864-55a7524f-f585-4a5a-a43d-4f90bb1d9b14.png)














之后根据SM2的原理实现加解密功能，其中须生成随机参数时，将该参数设置为标准文档中的值（若需要随机生成，只需修改注释）

该样例为对m="liujinyuan"加解密
输出加密之后的结果m1,m2,m3以及解密之后的结果：liujinyuan


![image](https://user-images.githubusercontent.com/75195549/181188480-6f9f87c0-b9d4-4bb7-a28f-59de26432ebb.png)



# SM2 密钥交换实现


哈希函数以及椭圆曲线上运算与SM2加解密相同。




Alice随机生成一个ra计算[ra]G发送给bill，bill随机生成一个rb计算[rb]G发送给Alice


对于BILL，可以使用Alice的ID以及公钥计算Za，同样可以计算自己的Zb，之后利用Alice发送的Ra按照密钥交换 协议可以计算一个密钥。


对于Alice，同样可以计算Za和Zb，并利用Bill的Rb按照密钥交换协议计算一个密钥。


二者计算的密钥相同，从而达到密钥交换。


首先定义了一系列基础操作


![image](https://user-images.githubusercontent.com/75195549/181188785-5f191af6-054d-4c17-aaa1-acd8b22284f8.png)


定义填充规则





![image](https://user-images.githubusercontent.com/75195549/181188847-112aaab4-b6f1-4f55-89b8-9377a27945bb.png)




定义int类型到字符串类型的转化

![image](https://user-images.githubusercontent.com/75195549/181188954-45a73772-42c6-4118-a0e0-fe7199e9029d.png)



我们还实现了最重要的密钥交换函数（A同理）


![image](https://user-images.githubusercontent.com/75195549/181189151-63be67cf-a2d4-4970-a6dc-df9adb17caab.png)


最终我们进行密钥的交换


![image](https://user-images.githubusercontent.com/75195549/181189257-7dae2872-6658-415f-80ec-ae4984f219ef.png)




# 结果展示

![image](https://user-images.githubusercontent.com/75195549/181189347-4d5a192b-f854-487a-ab2c-8cd6525c1929.png)




