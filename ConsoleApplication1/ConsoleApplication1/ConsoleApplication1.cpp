

#include "pch.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <math.h>
#include <malloc.h>



using namespace std;

enum OperationType
{
	_none, _add, _sub, _mul, _div, _pow, _sin, _cos, _tg, _ctg
};

const int w = 1000;
const int h = 1000;


int space = 5;

const int fsize = w * h * 3;

unsigned char *img;


string str;
char* fx=(char*)&str;


bool *xs;
double *values;
OperationType *operations;


int v_length=0;
int o_length=0;



void UpdateX(double x) {
	for (int i = 0; i < v_length; i++) {
		if (xs[i]) {
			values[i] = x;
		}
	}
}



int ReadNumber(int indx,double* Res) {
	if (fx[indx] == 88 || fx[indx] == 120) {
		xs[v_length] = true;
		v_length += 1;
		*Res = 0;
		indx++;
		cout << 'x';
		return indx;
	}
	xs[v_length] = false;
	
	
	double val=0; 
	double mval=0;

	while (fx[indx] >= 48 && fx[indx] <= 57 || fx[indx] == 44 || fx[indx] == 46 || fx[indx] == 120 || fx[indx] == 88) {
		
			if (fx[indx] == 120 || fx[indx] == 88) {
				Res[0] = true;
			}
			if (fx[indx] == 44 || fx[indx] == 46) {
				mval = -1;
			}
			else {
				if (mval) {
					val += (fx[indx] - 48) * (double)pow(10, mval);
					mval -= 1;
				}
				else {
					val = val * 10 + (fx[indx]-48);
				}
			}
		
	
		indx += 1;
		if (fx[indx]==NULL) {
			break;
		}
	}
	cout << val;
	*Res = val;
	v_length += 1;
	return indx;
}

int ReadOper(int indx, OperationType* Res) {
	OperationType val = _none;
	double mval = 0;
	/*if (fx[indx]=='cos') {
		val = _none;
		indx += 3;
		values[v_length] = 0;
		v_length += 1;
		cout << 'cos';
	}
	else if (fx[indx] == 'sin') {
		val = _none;
		indx += 3;
		values[v_length] = 0;
		v_length += 1;
		cout << 'sin';
	}
	else if (fx[indx] == 'ctg') {
		val = _none;
		indx += 3;
		values[v_length] = 0;
		v_length += 1;
		cout << 'ctg';
	}
	else if (fx[indx] == 'tg') {
		val = _none;
		indx += 2;
		cout << 'tg';
	}
	else*/ if (fx[indx] == '+') {
		val = _add;
		indx += 1;
		cout << '+';
	}
	else if (fx[indx] == '-') {
		val = _sub;
		indx += 1;
		cout << '-';
	}
	else if (fx[indx] == '/') {
		val = _div;
		indx += 1;
		cout << '/';
	}
	else if (fx[indx] == '*') {
		val = _mul;
		indx += 1;
		cout << '*';
	} else if (fx[indx] == '^') {
		val = _pow;
		indx += 1;
		cout << '^';
	}
	
	*Res = val;
	o_length += 1;
	return indx;
}

void SetOperations(int indx) {
	if (fx[indx] >= 48 && fx[indx] <= 57 || fx[indx] == 88 || fx[indx] == 120) {
		indx = ReadNumber(indx,&values[v_length]);
	} else {
		indx = ReadOper(indx,&operations[o_length]);
	}
	if (fx[indx]==NULL) {
		return;
	}
	SetOperations(indx);
}

int DelVal(double* array, int l, int indx) {
	l -= 1;
	double* newArray = (double*)malloc(sizeof(double)*l);
	for (int i = 0; i < indx; i++) {
		newArray[i] = array[i];
	}
	for (int i = indx; i < l; i++) {
		newArray[i] = array[i+1];
	}
	array = newArray;
	return l;
}

int DelOper(OperationType* array, int l, int indx) {
	l-=1;
	OperationType* newArray = (OperationType*)malloc(sizeof(OperationType)*l);
	for (int i = 0; i < indx; i++) {
		newArray[i] = array[i];
	}
	for (int i = indx; i < l; i++) {
		newArray[i] = array[i+1];
	}
	array = newArray;
	return l;
}

double Calculate(double x) {
	UpdateX(x);
	int v=v_length;
	int o=o_length;
	
	double* l_values=(double*)malloc(sizeof(double)*v_length);
	for (int i = 0; i < v_length; i++) {
		l_values[i] = values[i];
	}

	OperationType* l_operations = (OperationType*)malloc(sizeof(OperationType)*o_length);
	for (int i = 0; i < o_length; i++) {
		l_operations[i] = operations[i];
	}

	while (v!=1) {
		int opertodo=0;
		int v_opertodo=0;
		for (int i = 0; i < o; i++) {
			if (l_operations[i] > v_opertodo) {
				v_opertodo = l_operations[i];
				opertodo = i;
			}
		}
		if (v_opertodo==1) {
		//+
			l_values[opertodo] += l_values[opertodo + 1];
			v = DelVal(l_values, v, opertodo + 1);
			o = DelOper(l_operations, o, opertodo);
		}
		else if (v_opertodo == 2) {
			l_values[opertodo] -= l_values[opertodo + 1];
			v = DelVal(l_values, v, opertodo + 1);
			o = DelOper(l_operations, o, opertodo);
		//-
		}
		else if (v_opertodo == 3) {
			l_values[opertodo] *= l_values[opertodo + 1];
			v = DelVal(l_values, v, opertodo + 1);
			o = DelOper(l_operations, o, opertodo);
		//*
		}
		else if (v_opertodo == 4) {
			l_values[opertodo] /= l_values[opertodo + 1];
			v = DelVal(l_values, v, opertodo + 1);
			o = DelOper(l_operations, o, opertodo);
		// /
		}if (v_opertodo == 5) {
			l_values[opertodo] = pow(l_values[opertodo],l_values[opertodo + 1]);
			v = DelVal(l_values, v, opertodo + 1);
			o = DelOper(l_operations, o, opertodo);
			// ^
		}
	}
	double res=l_values[0];
	if (v != 0) {
		//free(l_values);
	}
	if (l_operations!=NULL) {
		//free(l_operations);
	}
	return res;
}




void DrawP(double _x, double _y, unsigned char R, unsigned char G, unsigned char B) {
	int x = (int)round(_x);
	int y = (int)round(_y);
	int position;

	if (x > w / 2 || -x > w / 2 || y >= w / 2 || -y >= w / 2) {
		return;
	}



	position = (y + w / 2) * w * 3 + (x + w / 2) * 3;
	if (position < w*w * 3) {
	img[position] = (unsigned char)B;
	img[position + 1] = (unsigned char)G;
	img[position + 2] = (unsigned char)R;
	}/*
	if (position + w * 3 < w*w * 3) {
		img[position + w * 3] = (unsigned char)B;
		img[position + w * 3 + 1] = (unsigned char)G;
		img[position + w * 3 + 2] = (unsigned char)R;
	}
	if (position - w * 3 >= 0) {
		img[position - w * 3] = (unsigned char)B;
		img[position - w * 3 + 1] = (unsigned char)G;
		img[position - w * 3 + 2] = (unsigned char)R;
	}
	if (position - 3 >= 0) {
		img[position - 3] = (unsigned char)B;
		img[position - 2] = (unsigned char)G;
		img[position - 1] = (unsigned char)R;
	}
	if (position + 3 < w * w * 3) {
		img[position + 3] = (unsigned char)B;
		img[position + 4] = (unsigned char)G;
		img[position + 5] = (unsigned char)R;
	}*/
	//offset on y
	//val on x
}


void DrawF() {
	cout << Calculate(6);
	for (double x = -(w / 2); x < w; x += 0.002f) {
		DrawP(x , Calculate(x/space), 255, 0, 0);
	}
	
}

void DrawX() {


	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < w; i++) {
			int temp = ((w / 2) + w * i ) * 3;
			img[temp] = (unsigned char)255;
			img[temp + 1] = (unsigned char)255;
			img[temp + 2] = (unsigned char)255;
		}
	}


	for (int i = 0; i < w * 3; i++) {
		int temp = i + ((w * w * 3) / 2);
		img[temp] = (unsigned char)255;
		img[temp + 1] = (unsigned char)255;
		img[temp + 2] = (unsigned char)255;
	}

	for (int i = 0; i < w/2; i+=space) {
		DrawP((space+i), 0, 255, 255, 255);
		DrawP(-(space +i), 0, 255, 255, 255);
	}
	for (int i = 0; i < w / 2; i += space) {
		DrawP(0, (space +i), 255, 255, 255);
		DrawP(0, -(space +i), 255, 255, 255);
	}

}

void saveBMP() {
	FILE *f;

	unsigned char fheader[14] = { 'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0 };
	unsigned char iheader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0 };

	fheader[2] = (unsigned char)((fsize + 54));
	fheader[3] = (unsigned char)((fsize + 54) >> 8);
	fheader[4] = (unsigned char)((fsize + 54) >> 16);
	fheader[5] = (unsigned char)((fsize + 54) >> 24);

	iheader[4] = (unsigned char)(w);
	iheader[5] = (unsigned char)(w >> 8);
	iheader[6] = (unsigned char)(w >> 16);
	iheader[7] = (unsigned char)(w >> 24);
	iheader[8] = (unsigned char)((h));
	iheader[9] = (unsigned char)((h) >> 8);
	iheader[10] = (unsigned char)((h) >> 16);
	iheader[11] = (unsigned char)((h) >> 24);

	fopen_s(&f, "img.bmp", "wb");
	if (f==NULL) {
		return;
	}
	fwrite(fheader, 1, 14, f);
	fwrite(iheader, 1, 40, f);
	
	fwrite(img, 1, fsize, f);
	
	
	return;
}

void Correct() {

	int size = sizeof(fx)/sizeof(char*);
	for (int i = 0; i < size;i++) {
		if (fx[i]==32) {

			for (int j = 0; j > size - i;j++) {
				fx[j] = fx[j + 1];
			}
			size--;
			i--;
		}
	}
}

int main()
{
	values = (double*)malloc(sizeof(double) * 32);
	operations = (OperationType*)malloc(sizeof(OperationType) * 32);
	xs= (bool*)malloc(sizeof(bool) * 32);

	v_length = 0;
	o_length = 0;

	cout << "Space=";
	cin >> space;

	cout << "f(x)=";
	cin >> str;

	Correct();

	img = (unsigned char*)malloc(fsize * sizeof(unsigned char));
	if (img) {
		for (int i = 0; i < fsize; i++) {
			img[i] = (unsigned char)0;
		}
	}

	SetOperations(0);
	DrawX();
	DrawF();
	saveBMP();
	free(img);
	cin >> str;
	return(0);
}

/*
TODO:
-Wygląd
-Tabelka
-Input
-SuperSampling 
-Czyszczenie kodu
-Udaskonalanie 
-Czyszczenie kodu jeszcze raz
*/


//Kamera jest ustawiona w punkcie (0,0,0) i patrzy w stronę (,,,)

// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln
