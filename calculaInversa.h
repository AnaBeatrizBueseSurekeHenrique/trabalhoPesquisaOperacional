#include <iostream>
#include <vector>

using namespace std;

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