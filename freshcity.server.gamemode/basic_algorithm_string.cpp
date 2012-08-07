/*
 * Copyright 2012 TennenColl
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "basic_algorithm_string.h"
#include <Windows.h>
#include <vector>
#include <exception>
#include <string>
#include <sstream>

typedef unsigned int uint;

// GBK Encoding
std::string GBKToUTF8(const std::string& strGBK) {
    std::string strOutUTF8;
    WCHAR * str1;
    int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
    str1 = new WCHAR[n];
    MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);
    n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
    char * str2 = new char[n];
    WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
    strOutUTF8 = str2;
    delete[]str1;
    str1 = NULL;
    delete[]str2;
    str2 = NULL;
    return strOutUTF8;
}

std::string UTF8ToGBK(const std::string& strUTF8) {
    int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
    unsigned short * wszGBK = new unsigned short[len + 1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUTF8.c_str(), -1, (LPWSTR)wszGBK, len);
    len = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
    char *szGBK = new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte(CP_ACP,0, (LPCWSTR)wszGBK, -1, szGBK, len, NULL, NULL);
    std::string strTemp(szGBK);
    delete[]szGBK;
    delete[]wszGBK;
    return strTemp;
}

// Wide-char string converting
std::wstring string2wstring(const std::string& str) {
	std::wstring result;
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	TCHAR* buffer = new TCHAR[len + 1];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';
	result.append(buffer);
	delete[] buffer;
	return result;
}

std::string wstring2string(const std::wstring& src) {  
	std::string result;
	int len = WideCharToMultiByte(CP_ACP, 0, src.c_str(), src.size(), NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, src.c_str(), src.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	result.append(buffer);
	delete[] buffer;
	return result;
}

// Hashing
//jagatsastry.nitk@gmail.com 9th April  '09
//Implementation of SHA 256
std::string fromDecimal(uint n, int b) {
	std::string chars="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	std::string result="";
	
	while(n>0) {
		result=chars.at(n%b)+result;
		n/=b;
	}
	
	return result;
}

uint K[]= {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};
   
void makeblock(std::vector<uint>& ret, std::string p_msg) {
	uint cur=0;
	int ind=0;
	for(uint i=0; i<p_msg.size(); i++) {
		cur = (cur<<8) | (unsigned char)p_msg[i];
		
		if(i%4==3) {
			ret.at(ind++)=cur;
			cur=0;
		}
	}
}
   
class Block {
public:
	std::vector<uint> msg;
	Block():msg(16, 0) {}
	Block(std::string p_msg):msg(16, 0) {
		makeblock(msg, p_msg);
	}
};

void split(std::vector<Block>& blks, std::string& msg) {
	for(uint i=0; i<msg.size(); i+=64) {
		try {
			makeblock(blks[i/64].msg, msg.substr(i, 64));
		} catch(...) {}
	}
}

std::string mynum(uint x) {
	std::string ret;
	for(uint i=0; i<4; i++)
		ret+=char(0);
	
	for(uint i=4; i>=1; i--) {	//big endian machine assumed
		ret += ((char*)(&x))[i-1];
	}

	return ret;
}

#define shr(x,n) ((x & 0xFFFFFFFF) >> n)
#define rotr(x,n) (shr(x,n) | (x << (32 - n)))

uint ch(uint x, uint y, uint z) {
	return (x&y) ^ (~x&z);
}

uint maj(uint x, uint y, uint z) {
	return (x&y) ^ (y&z) ^ (z&x);
}

uint fn0(uint x) {
	return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22);
}

uint fn1(uint x) {
	return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25);
}

uint sigma0(uint x) {
	return rotr(x, 7) ^ rotr(x, 18) ^ shr(x, 3);
}

uint sigma1(uint x) {
	return rotr(x, 17) ^ rotr(x, 19) ^ shr(x, 10);
}

std::string sha256(std::string msg_arr) {
	std::string msg;
	msg=msg_arr;
	msg_arr += (char)(1<<7);
	uint cur_len = msg.size()*8 + 8;
	uint reqd_len = ((msg.size()*8)/512+1) *512;
	uint pad_len = reqd_len - cur_len - 64;
	
	std::string pad(pad_len/8, char(0));
	msg_arr += pad;
	std::string len_str(mynum(msg.size()*8));
	msg_arr = msg_arr + len_str;	
	uint num_blk = msg_arr.size()*8/512;
	std::vector<Block> M(num_blk, Block());
	split(M, msg_arr);
	
	uint H[]={0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

	for(uint i=0; i<num_blk; i++) {
		std::vector<uint> W(64, 0);
		for(uint t=0; t<16; t++) {
			W[t] = M[i].msg[t];
		}
		
		for(uint t=16; t<64; t++) {
			W[t] = sigma1(W[t-2]) + W[t-7] + sigma0(W[t-15]) + W[t-16];
		}
		
		uint work[8];
		for(uint i=0; i<8; i++)
			work[i] = H[i];
		
		for(uint t=0; t<64; t++) {
			uint t1, t2;
			t1 = work[7] + fn1(work[4]) + ch(work[4], work[5], work[6]) + K[t] + W[t];
			t2 = fn0(work[0]) + maj(work[0], work[1], work[2]);
			work[7] = work[6];
			work[6] = work[5];
			work[5] = work[4];
			work[4] = work[3] + t1; 
			work[3] = work[2]; 
			work[2] = work[1];
			work[1] = work[0];
			work[0] = t1 + t2;
		}
		
		for(uint i=0; i<8; i++) {
			H[i] = work[i] + H[i];
		}
	}
	
	std::stringstream dest;
	for(uint i=0; i<8; i++)
		dest << fromDecimal(H[i], 16);

	return dest.str();
}
