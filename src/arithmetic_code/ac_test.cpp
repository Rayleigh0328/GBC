#include<iostream>
#include<fstream>
#include<cstdio>
#include<vector>
#include<queue>

#include "ac_encoder.h"
#include "ac_decoder.h"

using namespace std;

int main()
{
	ifstream fin;
	fin.open("ac_input");

	AC_encoder ac_enc;
	queue<bool> buf;
	while (!buf.empty()) buf.pop();

	cout << "Encode begins:" << endl;
	int symbol;
	while (fin>>symbol)
	{
		cout << "encode " << symbol << endl;
		ac_enc.encode_symbol(symbol, buf);
	}

	cout << "AC codeword length: " << buf.size() << endl;
	ofstream fout;
	fout.open("ac_output");
	
	AC_decoder ac_dec;
	vector<int> output;
	ac_dec.decode(buf,output);
	for (int i=0;i<output.size();++i)
		fout<<output[i]<<endl;

	return 0;
}
