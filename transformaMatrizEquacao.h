#include <iostream>
#include <cctype>
#include<math.h>
#include<string>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <vector>
#include "calculosMatriz.h"
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
    int qntd = 0;
    for (int i = 0; i < str.size(); i++) {        
        if (str[i] == ch) {
            qntd++;
            if (qntd == N) {
                return i;
            }
        }
    }
    return -1;
}

string mudarEq(string str, int *maxX){
    int pos;

    pos = str.find("=");
    if(pos != string::npos){
        if(!ispunct(str[pos-1])){
            // caso o sistema nao for de desigualdade, nenhuma variavel de folga será adicionada.
            return str;
        } else{
            str.erase(str.begin() + pos);
        }
    }
    
    string str2 = "x" + to_string(*maxX) + "=";
    pos = str.find(">", 0);
    
    if(pos != string::npos){
        //caso a desigualdade for >, adiciona -x
        str.erase(str.begin() + pos);
        str2 = "-" + str2;
    } else{
        // caso contrario, adiciona de +X
        pos = str.find("<", 0);
        str.erase(str.begin() + pos);
        str2 = "+" + str2;
    }

    str.insert(pos, str2);
    //aumenta o valor do maior de x
    
    (*maxX)++;
    return str;
}

int verificarMaiorX(vector<string> v){
    int j = 1;
    int maxX = -1, pos, aux;

    for(int i = 0; i < v.size(); i++){

        do{   
            pos = achar_n_vez(v[i],'x',j);
            if(pos != -1){
                //percorre cada linha do sistema e encontra o maior valor de x.
                aux = calcularInt(v[i], pos + 1);
                if(aux > maxX){
                    maxX = aux;
                };
            }

            j++;
        
        }while (pos != -1);

        j = 1;
    }
    return maxX;
}

void realizaVariavelFolga(vector<string> *vec, int *max){
    //: remove os espacos em cada linha
    for(int i = 0; i < (*vec).size(); i++){
        (*vec)[i].erase(std::remove((*vec)[i].begin(), (*vec)[i].end(), ' '), (*vec)[i].end());
    }
    // encontra a quantidade de xs no sistema.

    *max = verificarMaiorX(*vec);
    (*max)++;
    
    for(int i = 0; i < (*vec).size(); i++){
        //adiciona as variaveis de folga nas equacoes.
        (*vec)[i] = mudarEq((*vec)[i], max);
    }
}
double transformaNumX(string str, int vez){
    //procura o x para o transformar em numero, caso nao encontrar, retornara zero
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
    
    // caso o valor for negativo, o multiplicara por menos um.
    if(str[pos] == '-' && num > 0){
        num *= -1;
    }
    
    return num;
}
void porValoresMatriz(vector<string> vec, vector<vector<double>> *matrizA, vector<vector<double>> *vetorLinha, vector<vector<double>> *vetorResult){
    //matriz que armazena todos os valores das restrições.
    for(int i = 0; i < (*matrizA).size(); i++){
        for(int j = 0; j < (*matrizA)[0].size(); j++){
            (*matrizA)[i][j] = transformaNumX(vec[i+1], j+1);
        }
    }
    // armazena os coeficientes de x.    
    for(int i = 0; i < (*matrizA)[0].size(); i++){
        (*vetorLinha)[0][i] = transformaNumX(vec[0], i+1);
    }
    //armazena o valor dos resultados.
    for(int i = 0; i < (*vetorResult).size(); i++){
        (*vetorResult)[i][0] = numero(vec[i+1], vec[i+1].find('=') + 1);
    }
}


vector<vector<double>> mudarPosicoes(vector<vector<double>> matrizA, vector<int> pos, int tamI, int tamJ){
    vector<vector<double>> novaMatriz(tamI, vector<double>(tamJ));
    for(int i = 0; i < tamI; i++){
        for(int j = 0; j < tamJ; j++){
            novaMatriz[i][j] = matrizA[i][pos[j]];
        }
    }
    return novaMatriz;
}

vector<int> verificarMatrizCorreta(vector<vector<double>> matrizA, vector<int> vec, int tamI, int tamJ){
    vector<vector<double>> novaMatriz(tamI, vector<double>(tamJ));
    double detMatriz;
    
    do {
        //permuta sequenciamente as posicoes da matriz A, e verifica se seu determinante é diferente de 0, caso for,
        //ele retornara essa sequencia, caso nao for, permutara novamente.
        novaMatriz = mudarPosicoes(matrizA, vec, tamI, tamJ);
        detMatriz = calcularDeterminante(novaMatriz);
        if(detMatriz != 0){ 
            return vec;
        }

    } while (next_permutation(vec.begin(), vec.end()));
    
    return {};
}
