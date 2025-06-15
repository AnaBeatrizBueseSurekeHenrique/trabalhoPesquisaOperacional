#include<iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <vector>
#include"transformaMatrizEquacao.h"
using namespace std;
vector<vector<double>> multiplicadorSimplex(vector<vector<double>> matrizB, vector<vector<double>> cb){
    return (calculaMultiplicacao(cb, calculaInversa(matrizB)));
}

vector<vector<double>> solucaoBasica(vector<vector<double>> matrizB, vector<vector<double>> b){
    return (calculaMultiplicacao(calculaInversa(matrizB), b));
}


double calculaFuncaoObjetivo(vector<vector<double>> cb, vector<vector<double>> xb){
    double func = 0;
    for(int j = 0; j < xb.size(); j++){
        func += cb[0][j]*xb[j][0];
    }
    return func;
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
            *posMud = j;
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
}

void removerVarArtificial(vector<vector<double>> *matrizN, vector<int> *posicoesN, vector<int> *posicoes, vector<vector<double>> *coeficientesN,  vector<vector<double>> *custorN,  vector<vector<double>> *aN, int auxMaxX){
    for(int i = 0; i < (*posicoesN).size(); i++){
        if((*posicoesN)[i] >= auxMaxX){
            for(int j = 0; j < (*matrizN).size(); j++){
                (*matrizN)[j].erase((*matrizN)[j].begin() + i);
            }
            (*coeficientesN)[0].erase((*coeficientesN)[0].begin() + i);
            (*posicoesN).erase((*posicoesN).begin() + i);
            (*custorN)[0].erase((*custorN)[0].begin()+i);
            (*aN).erase((*aN).begin() + i);
        }
    }
        cout << (*posicoes).size() << endl;

    while((*posicoes).size() > auxMaxX){
        posicoes->pop_back();
    }
}

void mostrarValorX(vector<int> posicoes, vector<vector<double>> xb){
    for(int i = 0; i < posicoes.size(); i++){
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