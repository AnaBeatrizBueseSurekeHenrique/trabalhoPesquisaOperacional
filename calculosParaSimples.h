#include<iostream>
#include <vector>
#include"somaMultiplicacaoMatriz.h"
#include"calculaInversa.h"
using namespace std;
vector<vector<double>> multiplicadorSimplex(vector<vector<double>> matrizB, vector<vector<double>> cb){
    return (calculaMultiplicacao(cb, calculaInversa(matrizB)));
}

vector<vector<double>> solucaoBasica(vector<vector<double>> matrizB, vector<vector<double>> b){
    return (calculaMultiplicacao(calculaInversa(matrizB), b));
}

vector<vector<double>> custoRelativoN(vector<vector<double>> cn, vector<vector<double>> multSimplex){
    vector<vector<double>> custorN(1, vector<double>(cn.size()));
    for(int j = 0; j < cn.size(); j++){
        custorN[0][j] = cn[0][j] - multSimplex[0][j];
        if(custorN[0][j] < 0){
        }
    }
}