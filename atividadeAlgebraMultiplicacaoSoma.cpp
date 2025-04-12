#include <iostream>
#include <vector>
//colocar print da entrada e da saida
using namespace std;
int main(){
    vector<vector<double>> matrizA;
    vector<vector<double>> matrizB;
    
    vector<vector<double>> matrizC(matrizA.size(), vector<double>(matrizB.size()));

   /* cout << "Insira o numero de colunas da matriz A: ";
    cin >> Ca;
    cout << "Insira o numero de linhas da matriz A: ";
    cin >> La;
    cout << "Insira o numero de colunas da matriz B: ";
    cin >> Cb;
    cout << "Insira o numero de linhas da matriz B: ";
    cin >> Lb;
    cout << "Insira os valores da Matriz A: " << endl;*/
    //int A[Ca][La], B[Cb][Lb], C[Ca][Lb], soma[Ca][La];
    if(matrizA.size() != matrizB[0].size()){
        cout << "Nao e possivel realizar as operacoes";
    } else{
        for(int i = 0; i < matrizA.size(); i++){
            for(int j = 0; j < matrizB[0].size(); j++){
                for(int k = 0; k < matrizA[0].size(); k++){
                    matrizC[i][j] += matrizA[i][k] * matrizB[k][j];
                }
            }
        }
    }
    return 0;
}
