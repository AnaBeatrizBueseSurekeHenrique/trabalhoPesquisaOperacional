#include <iostream>
#include <vector>
#include <math.h>
#include "fazEquacao.h"

using namespace std;

void mostrarMatriz(vector<vector<double>> matrix){
    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix[0].size(); j++){
            cout << matrix[i][j] << " "; 
        }
        cout << endl;
    }
    cout << endl << endl;
}

double calcularDeterminante(vector<vector<double>> matrix){
    double resultado;
    
    if(matrix.size() > 2){
        resultado = 0;
        
        for(int k = 0; k < matrix.size(); k++){
            int valorI = 0;
            vector<vector<double>> aux(matrix.size()-1, vector<double>(matrix.size()-1));
            
            for(int i = 1; i < matrix.size(); i++){
                int valorJ = 0;
                for(int j = 0; j < matrix.size(); j++){
                    if(j != k){
                        aux[valorI][valorJ] = matrix[i][j];
                        valorJ++;
                    }  
                }
                valorI++;
            }        
            resultado += matrix[0][k]*pow(-1, k)* calcularDeterminante(aux);
        }
        
        return resultado;
    } else{
        resultado = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
        return resultado;
    }
}

vector<vector<double>> calculaMultiplicacao(vector<vector<double>> vecA, vector<vector<double>> vecB){
    vector<vector<double>> vecC(vecA.size(), vector<double>(vecB[0].size()));
    for(int i = 0; i < vecA.size(); i++){
        for(int j = 0; j < vecB[0].size(); j++){
            for(int k = 0; k < vecA[0].size(); k++){
                vecC[i][j] += (vecA[i][k] * vecB[k][j]);
            }
        }
    }
    return vecC;
}


vector<vector<double>> calculaInversa(vector<vector<double>> vec){
    vector<vector<double>> identidade(vec.size(), vector<double>(vec.size()));
    
    for(int i = 0; i < vec.size(); i++){
        identidade[i][i] = 1;
    }
    double pivo;
    
    for(int i = 0; i < vec.size(); i++){
        pivo = vec[i][i];
        
        for(int j = 0; j < vec.size(); j++){
            vec[i][j] = vec[i][j]/pivo;
            identidade[i][j] = identidade[i][j]/pivo;
        }
        
        for(int j = 0; j < vec.size(); j++){
            pivo = vec[j][i];
            if(j != i){
                for(int k = 0; k < vec.size(); k++){
                    vec[j][k] = vec[j][k] - vec[i][k]*pivo;
                    identidade[j][k] = identidade[j][k] - identidade[i][k]*pivo;
                }
            }
        }
        
    }
    return identidade;
}


