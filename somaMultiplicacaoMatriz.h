#include <iostream>
#include <vector>
using namespace std;

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

void calculaSoma(vector<vector<double>> vecA, vector<vector<double>> vecB, vector<vector<double>>*soma){
    for(int i = 0; i < (*soma).size(); i++){
        for(int j = 0; j < (*soma)[0].size(); j++){
        (*soma)[i][j] = vecA[i][j] + vecB[i][j];
        }
    }
}

void calculaSub(vector<vector<double>> vecA, vector<vector<double>> vecB, vector<vector<double>>*soma){
    for(int i = 0; i < (*soma).size(); i++){
        for(int j = 0; j < (*soma)[0].size(); j++){
        (*soma)[i][j] = vecA[i][j] - vecB[i][j];
        }
    }
}
