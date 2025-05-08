#include <iostream>
#include <cctype>
#include<math.h>
#include<string>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <vector>
using namespace std;

double numero(string str, int it){
    double inteiros = 0;
    int exp;
    double decimal = 0;
    double result = 1;
    string aux = "";
    if(it == 1 && str[it - 1] == '-'){
        result = -1;
    } else{
        if(str[it-1] == '-'){
            result = -1;
        }
        if(str[it] == '-'){
            result = -1;
            it++;
        }
    }
    while(it < str.size() && isdigit(str[it]) || str[it] == '.'){
        aux = "";
        if(isdigit(str[it])){
            while(isdigit(str[it])){
                aux += str[it];
                (it)++;
            }
            inteiros = stoi(aux, 0, 10);
        } else{
            if(str[it] == '.'){
                exp = 0;
                (it)++;
                if(isdigit(str[it])){
                    while(isdigit(str[it])){
                        aux += str[it];
                        (it)++;
                        exp++;
                    }
                    decimal = stoi(aux, 0, 10);
                    decimal = decimal * pow(10, (exp*(-1)));
                }
            }
        } 
    }
    result = result*(inteiros + decimal);
    return result;
}

int calcularInt(string str, int pos){
    int numero = 0;
    int exp;
    string aux = "";
    while(pos < str.size() && isdigit(str[pos])){
        if(isdigit(str[pos])){
            while(isdigit(str[pos])){
                aux += str[pos]; 
                pos++;
            }
            numero = stoi(aux, 0, 10);
        }
    }
    return numero;
}

int achar_n_vez(string str, char ch, int N) {
    int count = 0;
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == ch) {
            count++;
            if (count == N) {
                return i;
            }
        }
    }
    return -1;
}

string mudarEq(string str, int *maxX){
    int pos;
    pos = str.find("=",0);
    if(pos != string::npos){
        if(!ispunct(str[pos-1])){
            return str;
        } else{
            str.erase(str.begin() + pos);
        }
    }
    string str2 = "x" + to_string(*maxX) + "=";
    pos = str.find(">", 0);
    if(pos != string::npos){
        str.erase(str.begin() + pos);
        str2 = "-" + str2;
    } else{
        pos = str.find("<", 0);
        str.erase(str.begin() + pos);
        str2 = "+" + str2;
    }
    str.insert(pos, str2);
    (*maxX)++;
    return str;
}

int verificarMaiorX(vector<string> v){
    int j = 2;
    int maxX = -1, pos = 0, aux;
    for(int i = 0; i < v.size(); i++){
        while(pos != -1){   
            pos = achar_n_vez(v[i],'x',j);
            if(pos != -1){
                aux = calcularInt(v[i], pos + 1);
                if(aux > maxX){
                    maxX = aux;
                };
            }
            j++;
        }
        j = 1;
    }
    return maxX;
}



void realizaVariavelFolga(vector<string> *vec, int *max){
    for(int i = 0; i < (*vec).size(); i++){
        (*vec)[i].erase(std::remove((*vec)[i].begin(), (*vec)[i].end(), ' '), (*vec)[i].end());
    }
    *max = verificarMaiorX(*vec);
    (*max)++;
    for(int i = 0; i < (*vec).size(); i++){
        (*vec)[i] = mudarEq((*vec)[i], max);
    }
}
double transformaNumX(string str, int vez){
    vez++;
    int pos = str.find("x" + to_string(vez));
    if(pos == string::npos){
        return 0;
    }
    double num;
    pos--;
    if(isdigit(str[pos])){
        while(isdigit(str[pos]) || str[pos] == '.'){
            pos--;
        }
        num = numero(str, pos+1);
    } else{
        num = 1;
    }
    if(str[pos] == '-' &&  num > 0){
        num *= -1;
    }
    return num;
}
void porValoresMatriz(vector<string> vec, vector<vector<double>> *matrix, vector<double> *vetorLinha, vector<double> *vetorResult){
   for(int i = 0; i < (*matrix).size(); i++){
        for(int j = 0; j < (*matrix)[0].size(); j++){
            (*matrix)[i][j] = transformaNumX(vec[i+1], j);
        }
   }
    for(int i = 0; i < (*matrix)[0].size(); i++){
        (*vetorLinha)[i] = transformaNumX(vec[0], i);
   }
   for(int i = 0; i < (*vetorResult).size(); i++){
        (*vetorResult)[i] = numero(vec[i+1], vec[i+1].find('=') + 1);
   }
  
   cout << "Matriz A: " << endl;
   for(int i = 0; i < (*matrix).size(); i++){
        for(int j = 0; j < (*matrix)[0].size(); j++){
            cout << (*matrix)[i][j] << " "; 
        }
        cout << endl;
    }
    cout << endl;
    cout << "Matriz B: " << endl;
    cout << endl;
    cout << "Matriz b: " << endl;
    for(int i = 0; i < (*vetorResult).size(); i++){
        cout << (*vetorResult)[i] << endl;
   }
   cout << endl;
   cout << "Matriz C: " << endl;
   for(int i = 0; i < (*vetorLinha).size(); i++){
       cout << (*vetorLinha)[i] << " ";
   }
   cout << endl;    
}