#pragma once

#include<iostream>
#include<bitset>
#include<string>
#include<string.h>
#include<math.h>
using namespace std;

extern bitset<64> key;//初始64bit密钥
extern bitset<48> subKey[16];//16个32bit子密钥

//初始置换IP
extern int IP[];
//逆初始置换IP-1
extern int IP_1[];
//置换选择PC-1
extern int PC_1[];
//置换选择PC-2
extern int PC_2[];
//左移表
extern int shift[];
//扩展置换E盒
extern int E[];
//S盒
extern bitset<4> S[][64];
//P盒
extern int P[];
//左移函数
extern bitset<28> ls(bitset<28> k, int shift);
//f函数
extern bitset<32> f(bitset<32> R, bitset<48> k);
//子密钥生成函数
extern void genKeys();
//比特加密函数
extern bitset<64> encrypt(bitset<64> plain);
//比特解密函数
extern bitset<64> decrypt(bitset<64> cypher);
//比特转字符串
extern string bit2str(bitset<64> bits);
//字符串转比特
extern bitset<64> str2bit(string str);
//字符串加密函数
extern string encrypt(string plain);
//字符串解密函数
extern string decrypt(string cypher);