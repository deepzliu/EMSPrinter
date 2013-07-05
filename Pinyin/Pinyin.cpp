// Pinyin.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include <WinUser.h>
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}

#include   <iostream>
#include   <string>
using   namespace   std;
#include "pinyin.h"
static   char   convert(wchar_t   n);
static   bool   In(wchar_t   start,   wchar_t   end,   wchar_t   code);
int CN2Pinyin(string   sChinese);
int value(unsigned char *p);
int ShouZiMu();
int QuanPin();
int fetchString();

int main(int   argc, char *argv[])
{
	//ShouZiMu();
	QuanPin();
	//fetchString();
}
int fetchString(){
	string line = ",青海邦诚电子科技发展有限公司,龚艳萍,13619719946,西宁市西关大街49号永和大厦A幢1072室,810000,,";
	int find_pos = 0, cur_pos = 0;
	string substr;
	char str[10][256];
	for(int i = 0; i <10; i++){
		memset(str[i], 0, 256);
	}
	int n = 0;
	while(1){
		find_pos = line.find(',', cur_pos);
		if(find_pos != string::npos){
			if(cur_pos != find_pos){
				substr = line.substr(cur_pos, find_pos - cur_pos);
				memcpy(str[n], substr.c_str(), find_pos - cur_pos);
			}
			cur_pos = find_pos+1;
		}else{
			if(line.size() != cur_pos){
				substr = line.substr(cur_pos, line.size() - cur_pos);
				memcpy(str[n], substr.c_str(), line.size() - cur_pos);
			}
			break;
		}
		if(++n > 10) break;
	}
	line.clear();
	return 0;
}

int QuanPin(){
	string str, s;
	unsigned char c[256];
	while(1){
		cin >> str;
		memcpy(c, str.c_str(), str.size());
		str = getLetter(str, s);
		cout << str << endl;
		cout << s << endl;
		CharLowerBuffA((LPSTR)s.c_str(), s.size());
		cout << s << endl;
		s.clear();
	}
	return 0;	
}

int ShouZiMu(){
	//char buf[100];
	string str;
	while(1){
		//scanf("%s", buf);
		//str = buf;
		cin >> str;
		CN2Pinyin(str);
	}
	return 0;
}

int CN2Pinyin(string   sChinese)
{
	//string   sChinese   =   "我爱中国";   //   输入的字符串

	char   chr[3];
	wchar_t   wchr   =   0;
	int len = sChinese.length()/2;

	char*   buff   =   new   char[len];
	memset(buff,   0x00,   sizeof(char)*len+2);

	int i =0 ;
	for   (int  j   =   0;   i   <   len;   ++i)
	{
		memset(chr,   0x00,   sizeof(chr));
		chr[0]   =   sChinese[j++];
		chr[1]   =   sChinese[j++];
		chr[2]   =   '\0';

		//   单个字符的编码   如：'我'   =   0xced2
		wchr   =   0;
		wchr   =   (chr[0]   &   0xff)   <<   8;
		wchr   |=   (chr[1]   &   0xff);

		buff[i]   =   convert(wchr);
	}
	//buff[i] = 0;

	cout   <<   "pin   yin   =   ["   <<   buff   <<   "]"   <<   endl;

	delete[] buff;
	return   0;
}

char   convert(wchar_t   n)
{
	if   (In(0xB0A1,0xB0C4,n))   return   'a';
	if   (In(0XB0C5,0XB2C0,n))   return   'b';
	if   (In(0xB2C1,0xB4ED,n))   return   'c';
	if   (In(0xB4EE,0xB6E9,n))   return   'd';
	if   (In(0xB6EA,0xB7A1,n))   return   'e';
	if   (In(0xB7A2,0xB8c0,n))   return   'f';
	if   (In(0xB8C1,0xB9FD,n))   return   'g';
	if   (In(0xB9FE,0xBBF6,n))   return   'h';
	if   (In(0xBBF7,0xBFA5,n))   return   'j';
	if   (In(0xBFA6,0xC0AB,n))   return   'k';
	if   (In(0xC0AC,0xC2E7,n))   return   'l';
	if   (In(0xC2E8,0xC4C2,n))   return   'm';
	if   (In(0xC4C3,0xC5B5,n))   return   'n';
	if   (In(0xC5B6,0xC5BD,n))   return   'o';
	if   (In(0xC5BE,0xC6D9,n))   return   'p';
	if   (In(0xC6DA,0xC8BA,n))   return   'q';
	if   (In(0xC8BB,0xC8F5,n))   return   'r';
	if   (In(0xC8F6,0xCBF0,n))   return   's';
	if   (In(0xCBFA,0xCDD9,n))   return   't';
	if   (In(0xCDDA,0xCEF3,n))   return   'w';
	if   (In(0xCEF4,0xD188,n))   return   'x';
	if   (In(0xD1B9,0xD4D0,n))   return   'y';
	if   (In(0xD4D1,0xD7F9,n))   return   'z';
	return   '\0';
}

bool   In(wchar_t   start,   wchar_t   end,   wchar_t   code)
{
	if   (code   >=   start   &&   code   <=   end)
	{
		return   true;
	}
	return   false;
}


