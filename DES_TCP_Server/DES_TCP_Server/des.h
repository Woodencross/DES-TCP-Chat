#pragma once

#include<iostream>
#include<bitset>
#include<string>
#include<string.h>
#include<math.h>
using namespace std;

extern bitset<64> key;//��ʼ64bit��Կ
extern bitset<48> subKey[16];//16��32bit����Կ

//��ʼ�û�IP
extern int IP[];
//���ʼ�û�IP-1
extern int IP_1[];
//�û�ѡ��PC-1
extern int PC_1[];
//�û�ѡ��PC-2
extern int PC_2[];
//���Ʊ�
extern int shift[];
//��չ�û�E��
extern int E[];
//S��
extern bitset<4> S[][64];
//P��
extern int P[];
//���ƺ���
extern bitset<28> ls(bitset<28> k, int shift);
//f����
extern bitset<32> f(bitset<32> R, bitset<48> k);
//����Կ���ɺ���
extern void genKeys();
//���ؼ��ܺ���
extern bitset<64> encrypt(bitset<64> plain);
//���ؽ��ܺ���
extern bitset<64> decrypt(bitset<64> cypher);
//����ת�ַ���
extern string bit2str(bitset<64> bits);
//�ַ���ת����
extern bitset<64> str2bit(string str);
//�ַ������ܺ���
extern string encrypt(string plain);
//�ַ������ܺ���
extern string decrypt(string cypher);