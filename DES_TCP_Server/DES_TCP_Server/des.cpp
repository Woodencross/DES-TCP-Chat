#include"des.h"

bitset<64> key;/*= str2bit("your key here")*/
bitset<48> subKey[16];
//初始置换IP
int IP[] = {
	58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6,
	64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17,  9, 1,
	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7
};

//逆初始置换IP-1
int IP_1[] = {
	40, 8, 48, 16, 56, 24, 64, 32,
	39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30,
	37, 5, 45, 13, 53, 21, 61, 29,
	36, 4, 44, 12, 52, 20, 60, 28,
	35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26,
	33, 1, 41,  9, 49, 17, 57, 25
};

//置换选择PC-1
int PC_1[] = {
	57, 49, 41, 33, 25, 17,  9,
	1, 58, 50, 42, 34, 26, 18,
	10,  2, 59, 51, 43, 35, 27,
	19, 11,  3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,
	7, 62, 54, 46, 38, 30, 22,
	14,  6, 61, 53, 45, 37, 29,
	21, 13,  5, 28, 20, 12,  4
};

//置换选择PC-2
int PC_2[] = {
	14, 17, 11, 24,  1,  5,  3, 28,
	15,  6, 21, 10, 23, 19, 12,  4,
	26,  8, 16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55, 30, 40,
	51, 45, 33, 48, 44, 49, 39, 56,
	34, 53, 46, 42, 50, 36, 29, 32
};

//左移表
int shift[] = { 1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1 };

//扩展置换E盒
int E[] = {
	32,  1,  2,  3,  4,  5,
	4,  5,  6,  7,  8,  9,
	8,  9, 10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32,  1
};

//S盒
bitset<4> S[][64] = {
	{
		0xe,0x0,0x4,0xf,0xd,0x7,0x1,0x4,
		0x2,0xe,0xf,0x2,0xb,0xd,0x8,0x1,
		0x3,0xa,0xa,0x6,0x6,0xc,0xc,0xb,
		0x5,0x9,0x9,0x5,0x0,0x3,0x7,0x8,
		0x4,0xf,0x1,0xc,0xe,0x8,0x8,0x2,
		0xd,0x4,0x6,0x9,0x2,0x1,0xb,0x7,
		0xf,0x5,0xc,0xb,0x9,0x3,0x7,0xe,
		0x3,0xa,0xa,0x0,0x5,0x6,0x0,0xd
	},
	{
		0xf,0x3,0x1,0xd,0x8,0x4,0xe,0x7,
		0x6,0xf,0xb,0x2,0x3,0x8,0x4,0xf,
		0x9,0xc,0x7,0x0,0x2,0x1,0xd,0xa,
		0xc,0x6,0x0,0x9,0x5,0xb,0xa,0x5,
		0x0,0xd,0xe,0x8,0x7,0xa,0xb,0x1,
		0xa,0x3,0x4,0xf,0xd,0x4,0x1,0x2,
		0x5,0xb,0x8,0x6,0xc,0x7,0x6,0xc,
		0x9,0x0,0x3,0x5,0x2,0xe,0xf,0x9
	},
	{
		0xa,0xd,0x0,0x7,0x9,0x0,0xe,0x9,
		0x6,0x3,0x3,0x4,0xf,0x6,0x5,0xa,
		0x1,0x2,0xd,0x8,0xc,0x5,0x7,0xe,
		0xb,0xc,0x4,0xb,0x2,0xf,0x8,0x1,
		0xd,0x1,0x6,0xa,0x4,0xd,0x9,0x0,
		0x8,0x6,0xf,0x9,0x3,0x8,0x0,0x7,
		0xb,0x4,0x1,0xf,0x2,0xe,0xc,0x3,
		0x5,0xb,0xa,0x5,0xe,0x2,0x7,0xc
	},
	{
		0x7,0xd,0xd,0x8,0xe,0xb,0x3,0x5,
		0x0,0x6,0x6,0xf,0x9,0x0,0xa,0x3,
		0x1,0x4,0x2,0x7,0x8,0x2,0x5,0xc,
		0xb,0x1,0xc,0xa,0x4,0xe,0xf,0x9,
		0xa,0x3,0x6,0xf,0x9,0x0,0x0,0x6,
		0xc,0xa,0xb,0xa,0x7,0xd,0xd,0x8,
		0xf,0x9,0x1,0x4,0x3,0x5,0xe,0xb,
		0x5,0xc,0x2,0x7,0x8,0x2,0x4,0xe
	},
	{
		0x2,0xe,0xc,0xb,0x4,0x2,0x1,0xc,
		0x7,0x4,0xa,0x7,0xb,0xd,0x6,0x1,
		0x8,0x5,0x5,0x0,0x3,0xf,0xf,0xa,
		0xd,0x3,0x0,0x9,0xe,0x8,0x9,0x6,
		0x4,0xb,0x2,0x8,0x1,0xc,0xb,0x7,
		0xa,0x1,0xd,0xe,0x7,0x2,0x8,0xd,
		0xf,0x6,0x9,0xf,0xc,0x0,0x5,0x9,
		0x6,0xa,0x3,0x4,0x0,0x5,0xe,0x3
	},
	{
		0xc,0xa,0x1,0xf,0xa,0x4,0xf,0x2,
		0x9,0x7,0x2,0xc,0x6,0x9,0x8,0x5,
		0x0,0x6,0xd,0x1,0x3,0xd,0x4,0xe,
		0xe,0x0,0x7,0xb,0x5,0x3,0xb,0x8,
		0x9,0x4,0xe,0x3,0xf,0x2,0x5,0xc,
		0x2,0x9,0x8,0x5,0xc,0xf,0x3,0xa,
		0x7,0xb,0x0,0xe,0x4,0x1,0xa,0x7,
		0x1,0x6,0xd,0x0,0xb,0x8,0x6,0xd
	},
	{
		0x4,0xd,0xb,0x0,0x2,0xb,0xe,0x7,
		0xf,0x4,0x0,0x9,0x8,0x1,0xd,0xa,
		0x3,0xe,0xc,0x3,0x9,0x5,0x7,0xc,
		0x5,0x2,0xa,0xf,0x6,0x8,0x1,0x6,
		0x1,0x6,0x4,0xb,0xb,0xd,0xd,0x8,
		0xc,0x1,0x3,0x4,0x7,0xa,0xe,0x7,
		0xa,0x9,0xf,0x5,0x6,0x0,0x8,0xf,
		0x0,0xe,0x5,0x2,0x9,0x3,0x2,0xc
	},
	{
		0xd,0x1,0x2,0xf,0x8,0xd,0x4,0x8,
		0x6,0xa,0xf,0x3,0xb,0x7,0x1,0x4,
		0xa,0xc,0x9,0x5,0x3,0x6,0xe,0xb,
		0x5,0x0,0x0,0xe,0xc,0x9,0x7,0x2,
		0x7,0x2,0xb,0x1,0x4,0xe,0x1,0x7,
		0x9,0x4,0xc,0xa,0xe,0x8,0x2,0xd,
		0x0,0xf,0x6,0xc,0xa,0x9,0xd,0x0,
		0xf,0x3,0x3,0x5,0x5,0x6,0x8,0xb
	}
};

//P盒
int P[] = {
	16,  7, 20, 21,
	29, 12, 28, 17,
	 1, 15, 23, 26,
	 5, 18, 31, 10,
	 2,  8, 24, 14,
	32, 27,  3,  9,
	19, 13, 30,  6,
	22, 11,  4, 25
};

//左移函数
bitset<28> ls(bitset<28> k, int shift) {
	bitset<28> temp;
	/*for (int i = 0; i < shift; i++) {
		temp[28 - shift + i] = k[i];
	}
	for (int i = 0; i < 28 - shift; i++) {
		temp[i] = k[i + shift];
	}*/
	temp = k << shift;
	temp |= (k >> 28 - shift);
	return temp;
}

//f函数
bitset<32> f(bitset<32> R, bitset<48> k) {
	//E盒扩展
	bitset<48> expandR;
	for (int i = 0; i < 48; i++) {
		expandR[47 - i] = R[32 - E[i]];
	}

	//与子密钥异或
	expandR ^= k;

	//S盒运算
	bitset<32> out = 0x0;
	bitset<6> s_in[8];
	int x = 47;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 6; j++) {
			s_in[i][5 - j] = expandR[x - j];
		}
		x -= 6;
	}
	for (int i = 0; i < 8; i++) {
		int index = (int)s_in[i].to_ulong();
		bitset<4> s = S[i][index];
		for (int j = 0; j < 4; j++) {
			out[31 - 4 * i - j] = s[3 - j];
		}
	}

	//P置换
	bitset<32> temp = out;
	for (int i = 0; i < 32; i++) {
		out[31 - i] = temp[32 - P[i]];
	}
	return out;
}

//子密钥生成函数
void genKeys() {
	bitset<56> theKey;//PC-1之后的key
	bitset<28> left;
	bitset<28> right;
	bitset<48> outKey;//PC-2之后的key

	//PC-1置换
	for (int i = 0; i < 56; i++) {
		theKey[55 - i] = key[64 - PC_1[i]];
	}

	for (int round = 0; round < 16; round++) {
		for (int i = 0; i < 28; i++) {
			left[27 - i] = theKey[55 - i];
			right[27 - i] = theKey[27 - i];
		}

		left = ls(left, shift[round]);
		right = ls(right, shift[round]);

		for (int i = 0; i < 28; i++) {
			theKey[55 - i] = left[27 - i];
			theKey[27 - i] = right[27 - i];
		}

		for (int i = 0; i < 48; i++) {
			outKey[47 - i] = theKey[56 - PC_2[i]];
		}
		subKey[round] = outKey;
		//cout << "K" << round + 1 << ":" << subKey[round] << endl;
	}
}

bitset<64> encrypt(bitset<64> plain)
{
	bitset<64> cypher;
	bitset<64> thePlain;
	bitset<32> left;
	bitset<32> right;
	bitset<32> newLeft;

	//IP置换
	for (int i = 0; i < 64; i++) {
		thePlain[63 - i] = plain[64 - IP[i]];
	}

	for (int i = 0; i < 32; i++) {
		left[31 - i] = thePlain[63 - i];
		right[31 - i] = thePlain[31 - i];
	}
	//cout << "IP:" << thePlain << endl;

	//16轮迭代
	for (int round = 0; round < 16; round++) {
		newLeft = right;
		right = left^f(right, subKey[round]);
		left = newLeft;
		//cout << "L" << round + 1 << ":" << left << endl << "R" << round + 1 << ":" << right << endl;
	}

	//合成R16L16
	for (int i = 0; i < 32; i++) {
		cypher[63 - i] = right[31 - i];
		cypher[31 - i] = left[31 - i];
	}
	//cout << "cypher:" << cypher << endl;

	//IP-1置换
	bitset<64> temp = cypher;
	for (int i = 0; i < 64; i++) {
		cypher[63 - i] = temp[64 - IP_1[i]];
	}

	return cypher;
}

bitset<64> decrypt(bitset<64> cypher)
{
	bitset<64> plain;
	bitset<64> theCypher;
	bitset<32> left;
	bitset<32> right;
	bitset<32> newLeft;

	for (int i = 0; i < 64; i++) {
		theCypher[63 - i] = cypher[64 - IP[i]];
	}

	for (int i = 0; i < 32; i++) {
		left[31 - i] = theCypher[63 - i];
		right[31 - i] = theCypher[31 - i];
	}

	for (int round = 0; round < 16; round++) {
		newLeft = right;
		right = left^f(right, subKey[15 - round]);
		left = newLeft;
		//cout << "L" << round + 1 << ":" << left << endl << "R" << round + 1 << ":" << right << endl;
	}

	for (int i = 0; i < 32; i++) {
		plain[63 - i] = right[31 - i];
		plain[31 - i] = left[31 - i];
	}

	bitset<64> temp = plain;
	for (int i = 0; i < 64; i++) {
		plain[63 - i] = temp[64 - IP_1[i]];
	}

	return plain;
}

string bit2str(bitset<64> bits)
{
	string str;
	int val = 0;
	if (bits.any() == false) {//全0
		//str = '\0';
	}
	else {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				val += bits[63 - 8 * i - j] * pow(2, 7 - j);
			}
			if (val != 0) {
				str += char(val);
			}
			val = 0;
		}
	}
	//cout << str.length() << endl;
	return str;
}

bitset<64> str2bit(string str)
{
	bitset<64> bits;
	if (str.length() == 8) {//字符串整8个，64bit
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				bits[63 - 8 * i - j] = (str[i] >> (7 - j)) & 1;
			}
		}
	}
	else {//补0
		int len = str.length() % 8;
		for (int i = 0; i < len; i++) {
			for (int j = 0; j < 8; j++) {
				bits[63 - 8 * i - j] = (str[i] >> (7 - j)) & 1;
			}
		}
		int zeroNum = 8 * (8 - len);
		for (int i = 0; i < zeroNum; i++) {
			bits[i] = 0;
		}
	}
	return bits;
}

string encrypt(string plain)
{
	int seg = plain.length() / 8;//几个64bit段
	int len = plain.length() % 8;//还剩多少
	string result;
	for (int i = 0; i < seg; i++) {
		string tempstr = plain.substr(i * 8, 8);
		bitset<64> tempbit = encrypt(str2bit(tempstr));
		result += tempbit.to_string();
	}
	if (len != 0) {
		result += encrypt(str2bit(plain.substr(seg * 8))).to_string();
	}
	return result;
}

string decrypt(string cypher)
{
	int seg = cypher.length() / 64;//几个64bit段,必定整除
	string result;
	for (int i = 0; i < seg; i++) {
		string s = cypher.substr(i * 64, 64);
		//cout << s << endl;
		bitset<64> b(s);
		b = decrypt(b);
		//cout << bit2str(b);
		result += bit2str(b);
	}
	return result;
}
