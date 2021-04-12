#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <cstring>

using namespace std;

enum commandtype {
	RI, RR, J, RM
};

enum code {
	HALT = 0,
	SYSCALL = 1,
	ADD = 2,
	ADDI = 3,
	SUB = 4,
	SUBI = 5,
	MUL = 6,
	MULI = 7,
	DIV = 8,
	DIVI = 9,
	LC = 12,
	SHL = 13,
	SHLI = 14,
	SHR = 15,
	SHRI = 16,
	AND = 17,
	ANDI = 18,
	OR = 19,
	ORI = 20,
	XOR = 21,
	XORI = 22,
	NOT = 23,
	MOV = 24,
	ADDD = 32,
	SUBD = 33,
	MULD = 34,
	DIVD = 35,
	ITOD = 36,
	DTOI = 37,
	PUSH = 38,
	POP = 39,
	CALL = 40,
	CALLI = 41,
	RET = 42,
	CMP = 43,
	CMPI = 44,
	CMPD = 45,
	JMP = 46,
	JNE = 47,
	JEQ = 48,
	JLE = 49,
	JL = 50,
	JGE = 51,
	JG = 52,
	LOAD = 64,
	STORE = 65,
	LOAD2 = 66,
	STORE2 = 67,
	LOADR = 68,
	LOADR2 = 69,
	STORER = 70,
	STORER2 = 71
};

int main(){
	map<string, int> command_to_code = {
		{"halt", HALT},
		{"syscall",SYSCALL},
		{"add",ADD},
		{"addi",ADDI},
		{"sub",SUB},
		{"subi",SUBI},
		{"mul",MUL},
		{"muli",MULI},
		{"div",DIV},
		{"divi",DIVI},
		{"lc",LC},
		{"shl",SHL},
		{"shli",SHLI},
		{"shr",SHR},
		{"shri",SHRI},
		{"and",AND},
		{"andi",ANDI},
		{"or",OR},
		{"ori",ORI},
		{"xor",XOR},
		{"xori",XORI},
		{"not",NOT},
		{"mov",MOV},
		{"addd",ADDD},
		{"subd",SUBD},
		{"muld",MULD},
		{"divd",DIVD},
		{"itod",ITOD},
		{"dtoi",DTOI},
		{"push",PUSH},
		{"pop",POP},
		{"call",CALL},
		{"calli",CALLI},
		{"ret",RET},
		{"cmp",CMP},
		{"cmpi",CMPI},
		{"cmpd",CMPD},
		{"jmp",JMP},
		{"jne",JNE},
		{"jeq",JEQ},
		{"jle",JLE},
		{"jl",JL},
		{"jge",JGE},
		{"jg",JG},
		{"load",LOAD},
		{"store",STORE},
		{"load2",LOAD2},
		{"store2",STORE2},
		{"loadr",LOADR},
		{"loadr2",LOADR2},
		{"storer",STORER},
		{"storer2",STORER2}
	};
	map<int, commandtype> code_to_type = {
		{HALT,RI},
		{SYSCALL, RI},
		{ADD, RR},
		{ADDI, RI},
		{SUB, RR},
		{SUBI, RI},
		{MUL, RR},
		{MULI,RI},
		{DIV, RR},
		{DIVI,RI},
		{LC,RI},
		{SHL,RR},
		{SHLI,RI},
		{SHR,RR},
		{SHRI,RI},
		{AND,RR},
		{ANDI,RI},
		{OR,RR},
		{ORI,RI},
		{XOR,RR},
		{XORI,RI},
		{NOT,RI},
		{MOV,RR},
		{ADDD,RR},
		{SUBD,RR},
		{MULD,RR},
		{DIVD,RR},
		{ITOD,RR},
		{DTOI,RR},
		{PUSH,RI},
		{POP,RI},
		{CALL,RR},
		{CALLI,J},
		{RET,J},
		{CMP,RR},
		{CMPI,RI},
		{CMPD,RR},
		{JMP,J},
		{JNE,J},
		{JEQ,J},
		{JLE,J},
		{JL,J},
		{JGE,J},
		{JG,J},
		{LOAD,RM},
		{STORE,RM},
		{LOAD2,RM},
		{STORE2,RM},
		{LOADR,RR},
		{LOADR2,RR},
		{STORER,RR},
		{STORER2,RR}
	};
	map<string, int> register_to_code = {
		{"r0",0},
		{"r1",1},
		{"r2",2},
		{"r3",3},
		{"r4",4},
		{"r5",5},
		{"r6",6},
		{"r7",7},
		{"r8",8},
		{"r9",9},
		{"r10",10},
		{"r11",11},
		{"r12",12},
		{"r13",13},
		{"r14",14},
		{"r15",15},
		{"flags",16}
	};
	map<string, int> marks;
	/*
		flags:
			0 - !=, <
			1 - !=, >
			2 - ==
	*/
	vector < int > r(17, 0);
	vector < unsigned int > memory(1048576);
	r[14] = 1048575;

	int line_number = 0;
	//первый проход - заполнение словаря marks номерами строк
	ifstream fin("input.txt");
	while (true) {
		string command;
		line_number++;
		fin >> command;
		if (command == "")
			break;
		else if (command[command.size() - 1] == ':') {
			command.pop_back();
			marks[command] = line_number--;
		}
		else {
			char a[101];
			fin.getline(a, 100);
		}
	}
	fin.clear();
	line_number = 0;
	fin.seekg(0, fin.beg);
	//второй проход - ассемблирование
	while (true) {
		string command;
		line_number++;
		fin >> command;
		if (command == "")
			break;
		if (command[0] == ';') {
			char a[101];
			fin.getline(a, 100);
			line_number--;
			continue;
		}
		if (command[command.size() - 1] == ':') {
			line_number--;
			continue;
		}
		if (command == "end") {
			string tmp;
			fin >> tmp;
			r[15] = marks[tmp];
			continue;
		}
		if (command == "word") {
			continue;
		}
		else {
			int code = command_to_code[command];
			int word = 0;
			switch (code_to_type[code]) {
				case RM: {
					string reg_str;
					int ch;
					word |= code << 24;
					fin >> reg_str;
					int reg = register_to_code[reg_str];
					word |= reg << 20;
					fin >> ch;
					word |= ch;
					break;
				}
				case RR:{
					string reg1_str, reg2_str;
					int ch;
					word |= code << 24;
					fin >> reg1_str;
					int reg1 = register_to_code[reg1_str];
					word |= reg1 << 20;
					fin >> reg2_str;
					int reg2 = register_to_code[reg2_str];
					word |= reg2 << 16;
					if (code != 40) {
						fin >> ch;
						ch &= 65535;
						word |= ch;
					}
					break;
				}
				case RI: {
					string reg_str, ch_str;
					int ch;
					word |= code << 24;
					fin >> reg_str;
					int reg = register_to_code[reg_str];
					word |= reg << 20;
					fin >> ch_str;
					try {
						ch = marks.at(ch_str);
					}
					catch (exception & ex) {
						ch = atoi(ch_str.c_str());
					}
					ch &= 1048575;
					word |= ch;
					break;
				}
				case J: {
					word |= code << 24;
					string ch_str;
					int ch;
					fin >> ch_str;
					try {
						ch = marks.at(ch_str);
					}
					catch (exception & ex) {
						ch = atoi(ch_str.c_str());
					}
					word |= ch;
					break;
				}
			}
			memory[line_number] = word;
		}
	}

	memory[0] = -1;

	while (memory[r[15]] != 0) {
		unsigned int code = memory[r[15]] >> 24;
		switch (code_to_type[code]){
		case RM:{
			unsigned int reg = (memory[r[15]] >> 20) & 15;
			unsigned int ch = memory[r[15]] & 1048575;
			switch (code) {
			case 64:
				r[reg] = memory[ch];
				break;
			case 65:
				memory[ch] = r[reg];
				break;
			case 66:
				r[reg] = memory[ch];
				r[reg + 1] = memory[ch + 1];
				break;
			case 67:
				memory[ch] = r[reg];
				memory[ch + 1] = r[reg + 1];
				break;
			}
			break;
		}
		case RR: {
			unsigned int reg1 = (memory[r[15]] >> 20) & 15;
			unsigned int reg2 = (memory[r[15]] >> 16) & 15;
			unsigned int ch = memory[r[15]] & 65535;
			switch (code){
			case 2:
				r[reg1] += r[reg2] + ch;
				break;
			case 4:
				r[reg1] -= r[reg2] + ch;
				break;
			case 6:
			{
				int64_t res = (int64_t)r[reg1] * (int64_t)(r[reg2] + ch);
				r[reg1] = 0 | res;
				r[reg1 + 1] = 0 | (res >> 32);
				break; 
			}
			case 8: {
				int64_t delimoe = ((int64_t)r[reg1 + 1] << 32) | r[reg1];
				try {
					if (r[reg2] == 0)
						throw runtime_error("Division by zero!");
					int ost = delimoe % r[reg2];
					delimoe /= r[reg2];
					if (delimoe > 4294967296)
						throw runtime_error("Division by zero!");
					r[reg1] = delimoe;
					r[reg1 + 1] = ost;
				}
				catch (exception & ex) {
					cout << ex.what();
				}
				break;
			}
			case 13:
				r[reg1] = r[reg1] << r[reg2];
				break;
			case 15:
				r[reg1] = r[reg1] >> r[reg2];
				break;
			case 17:
				r[reg1] &= r[reg2];
				break;
			case 19:
				r[reg1] |= r[reg2];
				break;
			case 21:
				r[reg1] ^= r[reg2];
				break;
			case 24:
				r[reg1] = r[reg2] + ch;
				break;
			case 32: {
				int64_t dl1 = 0, dl2 = 0;
				double d1 = 0, d2 = 0;
				
				dl1 |= r[reg1];
				dl1 |= (int64_t)r[reg1 + 1] << 32;
				memcpy(&d1, &dl1, 8);

				dl2 |= r[reg2];
				dl2 |= (int64_t)r[reg2 + 1] << 32;
				memcpy(&d2, &dl2, 8);
				
				d1 += d2;
				memcpy(&dl1, &d1, 8);
				r[reg1] = dl1;
				r[reg1 + 1] = dl1 >> 32;
				break;
			}
			case 33: {
				int64_t dl1 = 0, dl2 = 0;
				double d1 = 0, d2 = 0;

				dl1 |= r[reg1];
				dl1 |= (int64_t)r[reg1 + 1] << 32;
				memcpy(&d1, &dl1, 8);

				dl2 |= r[reg2];
				dl2 |= (int64_t)r[reg2 + 1] << 32;
				memcpy(&d2, &dl2, 8);

				d1 -= d2;
				memcpy(&dl1, &d1, 8);
				r[reg1] = dl1;
				r[reg1 + 1] = dl1 >> 32;
				break;
			}
			case 34: {
				int64_t dl1 = 0, dl2 = 0;
				double d1 = 0, d2 = 0;

				dl1 |= r[reg1];
				dl1 |= (int64_t)r[reg1 + 1] << 32;
				memcpy(&d1, &dl1, 8);

				dl2 |= r[reg2];
				dl2 |= (int64_t)r[reg2 + 1] << 32;
				memcpy(&d2, &dl2, 8);

				d1 *= d2;
				memcpy(&dl1, &d1, 8);
				r[reg1] = dl1;
				r[reg1 + 1] = dl1 >> 32;
				break;
			}
			case 35: {
				int64_t dl1 = 0, dl2 = 0;
				double d1 = 0, d2 = 0;

				dl1 |= r[reg1];
				dl1 |= (int64_t)r[reg1 + 1] << 32;
				memcpy(&d1, &dl1, 8);

				dl2 |= r[reg2];
				dl2 |= (int64_t)r[reg2 + 1] << 32;
				memcpy(&d2, &dl2, 8);

				d1 /= d2;
				memcpy(&dl1, &d1, 8);
				r[reg1] = dl1;
				r[reg1 + 1] = dl1 >> 32;
				break;
			}
			case 36: {
				double d = (double)r[reg2];
				int64_t dl = 0;
				memcpy(&dl, &d, 8);
				memcpy(&r[reg1], &dl, 4);
				dl = dl >> 32;
				memcpy(&r[reg1 + 1], &dl , 4);
				break;
			}
			case 37: {
				int64_t dl = 0;
				double d = 0;

				dl |= r[reg2];
				dl |= (int64_t)r[reg2 + 1] << 32;
				memcpy(&d, &dl, 8);

				int64_t ans = 0;
				ans = (int64_t)d;
				if (ans > 2147483647)
					throw runtime_error("Переполнение");
				r[reg1] = ans;
				break;
			}
			case 40:
				memory[--r[14]] = r[15] + 1;
				r[15] = r[reg2] - 1;
				break;
			case 43:
				if (r[reg1] == r[reg2])
					r[16] = 2;
				else if (r[reg1] < r[reg2])
					r[16] = 0;
				else
					r[16] = 1;
				break;
			case 45: {
				int64_t dl1 = 0, dl2 = 0;
				double d1, d2;

				dl1 |= r[reg1];
				dl1 |= (int64_t)r[reg1 + 1] << 32;
				memcpy(&d1, &dl1, 8);

				dl2 |= r[reg2];
				dl2 |= (int64_t)r[reg2 + 1] << 32;
				memcpy(&d2, &dl2, 8);
				if (d1 == d2)
					r[16] = 2;
				else if (d1 < d2)
					r[16] = 0;
				else
					r[16] = 1;
				break;
			}
			case 68:
				r[reg1] = memory[r[reg2] + ch];
				break;
			case 69:
				memory[r[reg2] + ch] = r[reg1];
				break;
			case 70:
				r[reg1] = memory[r[reg2] + ch];
				r[reg1 + 1] = memory[r[reg2] + ch + 1];
				break;
			case 71:
				memory[r[reg2] + ch] = r[reg1];
				memory[r[reg2] + ch + 1] = r[reg1 + 1];
				break;
			}
			break; 
		}
		case RI: {
			unsigned int reg = (memory[r[15]] >> 20) & 15;
			unsigned int ch = memory[r[15]] & 1048575;
			switch (code) {
			case 0:
				return 0;
			case 1: {
				switch (ch) {
				case 0:
					return 0;
				case 100:
					scanf_s("%i", &r[reg]);
					break;
				case 101:{
					double d = 0;
					int64_t dl;
					scanf_s("%lf", &d);
					memcpy(&dl, &d, 8);
					r[reg] = dl;
					r[reg + 1] = dl >> 32;
					break;
				}
				case 102:
					printf("%i", r[reg]);
					break;
				case 103: {
					int64_t dl = 0;
					double d;

					dl |= r[reg];
					dl |= ((int64_t)r[reg + 1]) << 32;
					memcpy(&d, &dl, 8);

					printf("%lg", d);
					break;
				}
				case 104: {
					char c = 0;
					scanf_s("%c", &c);
					r[reg] = 0;
					r[reg] |= c;
					break;
				}
				case 105:
					printf("%c", r[reg]);
					break;
				}
			break;
			}
			case 3:
				r[reg] += ch;
				break;
			case 5:
				r[reg] -= ch;
				break;
			case 7: {
				int64_t res = (int64_t)r[reg] * (int64_t)ch;
				r[reg] = 0 | res;
				r[reg + 1] = 0 | (res >> 32);
				break;
			}
			case 9: {
				int64_t delimoe = ((int64_t)r[reg + 1] << 32) | r[reg];
				try {
					if (ch == 0)
						throw runtime_error("Division by zero!");
					int ost = delimoe % ch;
					delimoe /= ch;
					if (delimoe > 4294967296)
						throw runtime_error("Division by zero!");
					r[reg] = delimoe;
					r[reg + 1] = ost;
				}
				catch (exception & ex) {
					cout << ex.what();
				}
				break;
			}
			case 12:
				r[reg] = ch;
				break;
			case 14:
				r[reg] = r[reg] << ch;
				break;
			case 16:
				r[reg] = r[reg] >> ch;
				break;
			case 18:
				r[reg] = r[reg] & ch;
				break;
			case 20:
				r[reg] = r[reg] | ch;
				break;
			case 22:
				r[reg] = r[reg] ^ ch;
				break;
			case 23:
				r[reg] = ~r[reg];
				break;
			case 38:
				memory[--r[14]] = r[reg] + ch;
				break;
			case 39:
				r[reg] = memory[r[14]++];
				r[reg] += ch;
				break;
			case 44:
				if (r[reg] == (int)ch)
					r[16] = 2;
				else if (r[reg] > (int)ch)
					r[16] = 1;
				else
					r[16] = 0;
				break;
			}
 			break;
		}
		case J: {
			unsigned int ch = memory[r[15]] & 1048575;
			switch (code) {
			case 41:
				memory[--r[14]] = r[15] + 1;
				r[15] = ch - 1;
				break;
			case 42:
				r[15] = memory[r[14]] - 1;
				r[14] += 1 + ch;
				break;
			case 46:
				r[15] = ch - 1;
				break;
			case 47:
				if ((r[16] == 0) | (r[16] == 1))
					r[15] = ch - 1;
				break;
			case 48:
				if (r[16] == 2)
					r[15] = ch - 1;
				break;
			case 49:
				if ((r[16] == 0) | (r[16] == 2))
					r[15] = ch - 1;
				break;
			case 50:
				if (r[16] == 0)
					r[15] = ch - 1;
			case 51:
				if ((r[16] == 1) | (r[16] == 2))
					r[15] = ch - 1;
				break;
			case 52:
				if (r[16] == 1)
					r[15] = ch - 1;
				break;
			}
			break;
		}
		}
		r[15]++;
	}
}