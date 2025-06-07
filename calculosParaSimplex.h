#include<iostream>
#include <vector>
#include <climits>
#include"somaMultiplicacaoMatriz.h"
#include"calculaInversa.h"
using namespace std;
vector<vector<double>> multiplicadorSimplex(vector<vector<double>> matrizB, vector<vector<double>> cb){
    return (calculaMultiplicacao(cb, calculaInversa(matrizB)));
}

vector<vector<double>> solucaoBasica(vector<vector<double>> matrizB, vector<vector<double>> b){
    vector<vector<double>> inversaB(matrizB.size(), vector<double>(matrizB[0].size()));
    inversaB = calculaInversa(matrizB);
    cout << "MOSTRAR MATRIZ: INVERSA B:  " << endl;
    mostrarMatriz(inversaB);
    return (calculaMultiplicacao(inversaB, b));
}


double calculaFuncaoObjetivo(vector<vector<double>> cb, vector<vector<double>> xb){
    double func = 0;
    for(int j = 0; j < xb.size(); j++){
        cout << "CB:" << cb[0][j] << "XB: " << xb[j][0] << endl;
        func = func + cb[0][j]*xb[j][0];
    }
    return func;
}

void custoRelativoN(vector<vector<double>> cn, vector<vector<double>> multSimplex, vector<vector<double>> matrizN, int *posMud, vector<vector<double>> *custorN){
    vector<vector<double>> aN(matrizN.size(), vector<double>(1));
    double custNMin = INT_MAX;
    for(int j = 0; j < cn[0].size(); j++){
        for(int i = 0; i < matrizN.size(); i++){
            aN[i][0] = matrizN[i][j];
        }
        mostrarMatriz(aN);
        vector<vector<double>> aux = calculaMultiplicacao(multSimplex, aN);
        (*custorN)[0][j] = cn[0][j] - aux[0][0];
        if((*custorN)[0][j] < custNMin){
            custNMin = (*custorN)[0][j];
            *posMud = j;
        }
        cout << "CustoN: " << (*custorN)[0][j] << endl;
    }
}

vector<vector<double>> calculoDirecaoSimplex(vector<vector<double>> matrizB, vector<vector<double>> a){
    return(calculaMultiplicacao(calculaInversa(matrizB), a));
}

int determinacaoPasso(vector<vector<double>> xb, vector<vector<double>> y, bool *fase2){
    double custNMin = INT_MAX;
    double aux;
    int pos;
    for(int i = 0; i < xb.size(); i++){
        if(y[i][0] > 0){
            aux = xb[i][0]/y[i][0];
            if(aux < custNMin){
                custNMin = aux;
                pos = i;
            }
        } else{
            *fase2 = false;
        }
    }
    return pos;
}

void trocarColunasMatriz(vector<vector<double>> *matrizB, vector<vector<double>> *matrizN, int posB, int posN){
    vector<double> aux((*matrizB).size());
    for(int i = 0; i < (*matrizB).size(); i++){
        aux[i] = (*matrizB)[i][posB];
    }
    for(int i = 0; i < (*matrizB).size(); i++){
        (*matrizB)[i][posB] = (*matrizN)[i][posN];
    }
    for(int i = 0; i < (*matrizB).size(); i++){
        (*matrizN)[i][posN] = aux[i];
    }
    cout << "Nova B : " << endl;
    mostrarMatriz(*matrizB);
    cout << "Nova N : " << endl;
    mostrarMatriz(*matrizN);
}
