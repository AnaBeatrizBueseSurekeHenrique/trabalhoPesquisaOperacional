#include <iostream>
#include <cctype>
#include<math.h>
#include<string>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <vector>
using namespace std;

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
int transformaNumXEmInt(string str, int vez){
    vez++;
    int pos = str.find("x" + to_string(vez));
    if(pos == string::npos){
        return 0;
    }
    int numero;
    pos--;
    if(isdigit(str[pos])){
        while(isdigit(str[pos])){
            pos--;
        }
        numero = calcularInt(str, pos+1);
    } else{
        numero = 1;
    }
    if(str[pos] == '-'){
        numero *= -1;
    }
    return numero;
}
void porValoresMatriz(vector<string> vec, vector<vector<int>> matrix, vector<int> vetorLinha, vector<int> vetorResult, vector<vector<int>> matrixB){
   for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix[0].size(); j++){
            matrix[i][j] = transformaNumXEmInt(vec[i+1], j);
        }
   }
   for(int i = 0; i < matrix[0].size(); i++){
        vetorLinha[i] = transformaNumXEmInt(vec[0], i);
   }
   for(int i = 0; i < vetorResult.size(); i++){
        vetorResult[i] = calcularInt(vec[i+1], vec[i+1].find('=') + 1);
   }
   for(int i = 0; i < matrixB.size(); i++){
        for(int j = 0; j < matrix[0].size(); j++){
            matrixB[i][j] = matrix[i][j];
        }
   }
   cout << "Matriz A: " << endl;
   for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix[0].size(); j++){
            cout << matrix[i][j] << " "; 
        }
        cout << endl;
    }
    cout << endl;
    cout << "Matriz B: " << endl;
    for(int i = 0; i < matrixB.size();i++){
        for(int j = 0; j < matrixB[0].size();j++){
            cout << matrixB[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << "Matriz b: " << endl;
    for(int i = 0; i < vetorResult.size(); i++){
        cout << vetorResult[i] << endl;
   }
   cout << endl;
    cout << "Matriz C: " << endl;
    for(int i = 0; i < vetorLinha.size(); i++){
        cout << vetorLinha[i] << " ";
    }    
}