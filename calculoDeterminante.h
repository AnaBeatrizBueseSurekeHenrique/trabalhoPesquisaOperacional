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

void transformarTxtEmMatriz(vector<vector<double>> *matrix, vector<string> s){
    vector<double> vec;
    for(int i = 0; i < s.size(); i++){
        int j = 0;
        int it = 0;
        while(it < s[i].size()){
            vec.insert(vec.begin() + j, numero(s[i], &it));
            if(s[i][it] == ' '){
                it++;
            }
            j++;
        }
        matrix->insert(matrix->begin() + i, vec);
    }
}


double calcularDeterminante(vector<vector<double>> matrix){
    if(matrix.size() > 2){
        double result = 0;
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
            result += matrix[0][k]*pow(-1, k)* calcularDeterminante(aux);
        }
        return result;
    } else{
        double result;
        result = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
        return result;
    }
}