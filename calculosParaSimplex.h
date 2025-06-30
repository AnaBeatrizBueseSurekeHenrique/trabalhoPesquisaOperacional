#include<iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <vector>
#include"transformaMatrizEquacao.h"
using namespace std;
vector<vector<double>> solucaoBasica(vector<vector<double>> matrizB, vector<vector<double>> b){
    return (calculaMultiplicacao(calculaInversa(matrizB), b));
}

vector<vector<double>> multiplicadorSimplex(vector<vector<double>> matrizB, vector<vector<double>> cb){

    return (calculaMultiplicacao(cb, calculaInversa(matrizB)));
}

vector<vector<double>> custoRelativoN(vector<vector<double>> cn, vector<vector<double>> multSimplex, vector<vector<double>> matrizN, int *posMud, vector<vector<double>> custorN){
    vector<vector<double>> aN(matrizN.size(), vector<double>(1));
    double custNMin = INT_MAX;
    
    for(int j = 0; j < custorN[0].size(); j++){
        for(int i = 0; i < matrizN.size(); i++){
            aN[i][0] = matrizN[i][j];
        }
        (custorN)[0][j] = cn[0][j] - calculaMultiplicacao(multSimplex, aN)[0][0];
        if((custorN)[0][j] < custNMin){
            custNMin = (custorN)[0][j];
            (*posMud) = j;
        }
    }

    return custorN;
}

vector<vector<double>> calculoDirecaoSimplex(vector<vector<double>> matrizB, vector<vector<double>> a){
    return(calculaMultiplicacao(calculaInversa(matrizB), a));
}

int determinacaoPasso(vector<vector<double>> xb, vector<vector<double>> y){
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

}
void trocarPosicoesBeN(vector<vector<double>> *matrizB, vector<vector<double>> *coeficientesB, vector<vector<double>> *matrizN,  vector<vector<double>> *coeficientesN, int posicaoSairBas, int posSairN, vector<int> *posicoes, vector<int> *posicoesN){
    int aux;                
   
    trocarColunasMatriz(matrizB, matrizN, posicaoSairBas, posSairN);
    trocarColunasMatriz(coeficientesB, coeficientesN, posicaoSairBas, posSairN);
    
    aux = (*posicoesN)[posSairN];
    (*posicoesN)[posSairN] = (*posicoes)[posicaoSairBas];
    (*posicoes)[posicaoSairBas] = aux;
}

void verificarDiagonalB(vector<vector<double>> *matrizB, vector<vector<double>> *coeficienteB, vector<int> *posicoes){
    int i = 0;
    int aux = i;
    
    while(i < (*matrizB).size()){
        if((*matrizB)[i][i] == 0){
            for(int k = 0; k < (*matrizB).size(); k++){
                if(i != k && k != aux){
                    if((*matrizB)[i][k] != 0 && (*matrizB)[i][i] == 0){
                        
                        if((*matrizB)[k][i] == 0){
                            aux = k;
                        }

                        trocarColunasMatriz(matrizB, matrizB, i, k);
                        trocarColunasMatriz(coeficienteB, coeficienteB, i, k);
                        
                        int aux = (*posicoes)[i];
                        (*posicoes)[i] = (*posicoes)[k];
                        (*posicoes)[k] = aux;
                        i = -1;
                        break;
                    }
                }
            } 
        }
        i++;
    }
}
void removerVarArtificial(vector<vector<double>> *matrizN, vector<int> *posicoesN, vector<int> *posicoes, vector<vector<double>> *coeficientesN,  vector<vector<double>> *custorN, int auxMaxX){
 int posN = (*posicoesN).size();
    for(int i = 0; i < posN; i++){
        if((*posicoesN)[i] >= auxMaxX){
            for(int j = 0; j < (*matrizN).size(); j++){
                (*matrizN)[j].erase((*matrizN)[j].begin() + i);
            }
            (*coeficientesN)[0].erase((*coeficientesN)[0].begin() + i);
           (*posicoesN).erase((*posicoesN).begin() + i);
           (*custorN)[0].erase((*custorN)[0].begin()+i);
           i = -1;
        }
    }
    while((*posicoes).size() > auxMaxX){
        posicoes->pop_back();
    }
}
double calculaFuncaoObjetivo(vector<vector<double>> cb, vector<vector<double>> xb){
    double func = 0;
    for(int j = 0; j < xb.size(); j++){
        func += cb[0][j]*xb[j][0];
    }
    return func;
}
void mostrarValorX(vector<int> posicoes, vector<vector<double>> xb, int maxValX){
    for(int i = 0; i < maxValX; i++){
        cout << "x" << (i+1) << "= ";
        int k = -1;
        
        do{
            k++;
        }while(k < xb.size() && posicoes[k] != i);
        
        if(k >= xb.size()){
            cout << "0" << endl;
        } else{
            cout << xb[k][0] << endl;
        }
    }
}