#include <iostream>
#include <vector>

using namespace std;

void calculaInversa(vector<vector<double>>);
void mostrarMatriz(vector<vector<double>> vec);
int main(){
    vector<vector<double>> matrix(3, vector<double>(3));
    matrix[0][0] = 2;
    matrix[0][1] = 1;
    matrix[0][2] = 3;
    matrix[1][0] = -4;
    matrix[1][1] = 2;
    matrix[1][2] =  2;
    matrix[2][0] = 2;
    matrix[2][1] = 5;
    matrix[2][2] = 3;
    mostrarMatriz(matrix);
    calculaInversa(matrix);
}
void mostrarMatriz(vector<vector<double>> vec){
    for(int i = 0; i < vec.size(); i++){
        for(int j = 0; j < vec.size(); j++){
            cout << vec[i][j] << " ";
        }
        cout << endl;
    }
}
void calculaInversa(vector<vector<double>> vec){
    vector<vector<double>> identidade(vec.size(), vector<double>(vec.size()));
    for(int i = 0; i < vec.size(); i++){
        identidade[i][i] = 1;
    }
    mostrarMatriz(identidade);
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
       //mostrarMatriz(identidade);
        //for(int i)
    }
    mostrarMatriz(identidade);
    mostrarMatriz(vec);
}