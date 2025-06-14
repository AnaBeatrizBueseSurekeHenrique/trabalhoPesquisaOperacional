#include<iostream>
#include <vector>
#include <climits>
#include"transformaMatrizEquacao.h"
using namespace std;
vector<vector<double>> multiplicadorSimplex(vector<vector<double>> matrizB, vector<vector<double>> cb){
    return (calculaMultiplicacao(cb, calculaInversa(matrizB)));
}

vector<vector<double>> solucaoBasica(vector<vector<double>> matrizB, vector<vector<double>> b){
    cout << "HORA CALCULO INVERSA: " << endl;
    vector<vector<double>> inversaB(matrizB.size(), vector<double>(matrizB[0].size()));
    inversaB = calculaInversa(matrizB);
    cout << "MOSTRAR INVERSA:  " << endl;
    mostrarMatriz(inversaB);
    return (calculaMultiplicacao(inversaB, b));
}


double calculaFuncaoObjetivo(vector<vector<double>> cb, vector<vector<double>> xb){
    double func = 0;
    for(int j = 0; j < xb.size(); j++){
        cout << "VALORES: " << cb[0][j] << " " << xb[j][0] << endl;
        func = func + cb[0][j]*xb[j][0];
    }
    return func;
}

vector<vector<double>> custoRelativoN(vector<vector<double>> cn, vector<vector<double>> multSimplex, vector<vector<double>> matrizN, int *posMud, vector<vector<double>> custorN){
    vector<vector<double>> aN(matrizN.size(), vector<double>(1));
    double custNMin = INT_MAX;
    for(int j = 0; j < cn[0].size(); j++){
        cout << "J" << endl;
        for(int i = 0; i < matrizN.size(); i++){
            aN[i][0] = matrizN[i][j];
        }
        mostrarMatriz(aN);
        vector<vector<double>> aux = calculaMultiplicacao(multSimplex, aN);
        (custorN)[0][j] = cn[0][j] - aux[0][0];
        if((custorN)[0][j] < custNMin){
            custNMin = (custorN)[0][j];
            *posMud = j;
        }
        cout << "CustoN: " << (custorN)[0][j] << endl;
    }
    return custorN;
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
void trocarLinhaMatriz(vector<vector<double>> *matrizB, int pos1, int pos2){
    vector<double> aux((*matrizB)[0].size());
   for(int i = 0; i < (*matrizB).size(); i++){
        aux[i] = (*matrizB)[pos1][i];
    }
    for(int i = 0; i < (*matrizB).size(); i++){
        (*matrizB)[pos1][i] = (*matrizB)[pos2][i];
    }
    for(int i = 0; i < (*matrizB).size(); i++){
        (*matrizB)[pos2][i] = aux[i];
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
    mostrarMatriz(*matrizB);
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
                          mostrarMatriz(*matrizB);
                          break;
                    }
                }
           } 
        }
        i++;
    }
    mostrarMatriz(*matrizB);
}
void verificarIdentidadeB(vector<vector<double>> *matrizB, vector<vector<double>> *matrizN, vector<int> *posicoes, vector<int> *posicoesN, vector<vector<double>> *coeficientesB, vector<vector<double>> *coeficientesN){
    vector<int> posIdentidade(matrizB[0].size());
    vector<int> posIdentidaN(matrizN[0].size());
    for(int i = 0; i < posIdentidade.size(); i++){
        posIdentidade[i] = -1;
    }
    for(int i = 0; i < posIdentidaN.size(); i++){
        posIdentidaN[i] = -1;
    }
   
    for(int i = 0; i < (*matrizB).size(); i++){
        bool identidade = true;
        int aux; 
        int qntd1 = 0;
        for(int j = 0; j < (*matrizB).size(); j++){
            if((*matrizB)[j][i] == 1){
                qntd1++;
                aux = j;
            } else{
                if((*matrizB)[j][i] != 0){
                    identidade = false;
                }
            }
            cout << "VALOR::  " << qntd1 << endl;
            
        }
        if(qntd1 == 0 || qntd1 > 1){
            identidade = false;
        }
        if(identidade){
            posIdentidade[i] = aux;
        }
    }
    
    for(int i = 0; i < (*matrizN)[0].size(); i++){
        bool identidade = true;
        int aux;
        int qntd1 = 0;
        for(int j = 0; j < (*matrizN).size(); j++){
            if((*matrizN)[j][i] == 1){
                qntd1++;
                aux = j;
            } else{
                if((*matrizN)[j][i] != 0){
                    identidade = false;
                }
            }
        }
        if(qntd1 == 0 || qntd1 > 1){
            identidade = false;
        }
        if(!identidade){
            posIdentidaN[i] = -1;
        } else{
            posIdentidaN[i] = aux;
        }
    }

    bool trocar;
    int valTroca;
    for(int i = 0; i < posIdentidade.size(); i++){
        if(posIdentidade[i] == -1){
            for(int k = 0; k < posIdentidaN.size(); k++){
                trocar = false;

                if(posIdentidaN[k] != -1){
                    trocar = true;
                    for(int j = 0; j < posIdentidade.size(); j++){
                        if(posIdentidade[j] == posIdentidaN[k]){
                            trocar = false;
                            break;
                        }
                    }
                    if(trocar){
                        valTroca = k;
                        break;
                    }
                }
            }
            if(trocar){
                trocarPosicoesBeN(matrizB, coeficientesB, matrizN, coeficientesN, i, valTroca, posicoes, posicoesN);
                posIdentidade[i] = posIdentidaN[valTroca];
                posIdentidaN[valTroca] = -1;
            }
        }
    }
    verificarDiagonalB(matrizB, coeficientesB, posicoes);
    cout << "MOSTRAR VALOR:  " << endl;
    mostrarMatriz(*matrizB);
    mostrarMatriz(*matrizN);
    /*const auto duplicate = std::adjacent_find(posIdentidade.begin(), posIdentidade.end());

    if (duplicate != posIdentidade.end())
        std::cout << "Duplicate element = " << *duplicate << "\n";
    */
}

void removerVarArtificial(vector<vector<double>> *matrizN, vector<int> *posicoesN, vector<vector<double>> *coeficientesN, int auxMaxX){
    cout << "MATRIZ N NESTE MOMENTO: " << auxMaxX << " " << endl;
    mostrarMatriz(*matrizN);
    cout << "POSICOES N: " << endl;
    for(int i = 0; i < (*posicoesN).size();i++){
        cout << (*posicoesN)[i] << " ";
    }
    for(int i = 0; i < (*posicoesN).size(); i++){
        if((*posicoesN)[i] >= auxMaxX){
            for(int j = 0; j < (*matrizN).size(); j++){
                (*matrizN)[j].erase((*matrizN)[j].begin() + i);
            }
            (*coeficientesN)[0].erase((*coeficientesN)[0].begin() + i);
            (*posicoesN).erase((*posicoesN).begin() + i);
        }
    }
    cout << "MATRIZ N NESTE MOMENTO: " << auxMaxX << " " << endl;
    mostrarMatriz(*matrizN);
      cout << "POSICOES N: " << endl;
    for(int i = 0; i < (*posicoesN).size();i++){
        cout << (*posicoesN)[i] << " ";
    }

}