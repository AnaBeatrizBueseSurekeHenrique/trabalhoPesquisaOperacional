#include <iostream>
#include "transformaMatrizEquacao.h"
#include "calcularMatrizB.h"
using namespace std;

int main(){
    char entrada;
    string s;
    ifstream f("entrada.txt");
    vector<string> vec;
    int i;
    cout << "Escolha o que quer utilizar:" <<endl;
    cout << "a. Tranformar txt em forma normal, e matrizes: " << endl;
    cout << "b. Calcular determinante de txt" << endl;
    cin >> entrada;
    cout << endl;
    vector<int> testVetor = {1,2,3, 4};
    if(entrada == 'a' || entrada == 'A'){
        int maxX;
        i = 0;
        while (getline(f, s)){
            if(!s.empty()){
                vec.insert(vec.begin() + i, s);
                i++;
            }
        }
        f.close();
        realizaVariavelFolga(&vec, &maxX);
        maxX--;
        i--;
        vector<vector<double>> matrix(i, vector<double>(maxX));
        vector<double> vetorLinha1(maxX);
        vector<double> vectorResult(vec.size() - 1);
        vector<vector<double>> matrizB(i, vector<double>(i));
        vector<double> coeficientesB (matrizB[0].size());
        vector<vector<double>> matrizN(i, vector<double> (maxX - i));
        vector<double> coeficientesN(matrizN[0].size());

        porValoresMatriz(vec, &matrix, &vetorLinha1, &vectorResult);
        vector<int> posicoes(matrix[0].size());
        for(int i = 0; i < posicoes.size(); i++){
            posicoes[i] = i;
        }
        posicoes = verificarMatrizCorreta(matrix, posicoes, matrizB.size(), matrizB[0].size());
        vector<int> posicoesN(posicoes.begin() + matrizB[0].size(), posicoes.end());
        matrizB = mudarPosicoes(matrix, posicoes, matrizB.size(), matrizB[0].size());
        mostrarMatriz(matrizB);
        matrizN = mudarPosicoes(matrix, posicoesN, matrizN.size(), matrizN[0].size());
        coeficientesB = mudarPosicoesVetor(vetorLinha1, posicoes, coeficientesB.size());

        coeficientesN = mudarPosicoesVetor(vetorLinha1, posicoesN, coeficientesN.size());

        mostrarMatriz(matrizN);

       // matrizB = verificarMatrizCorreta(matrix);
      //verificarMatrizCorreta(matrix);
        //mostrarMatriz(matrizB);
    } else{
        if(entrada == 'b' || entrada == 'B'){
            i = 0;
            vector<vector<double>> matriz;
            while(getline(f, s)){
                if(!s.empty()){
                    vec.insert(vec.begin() + i, s);
                    i++;
                }
            }
            transformarTxtEmMatriz(&matriz, vec);
            cout << calcularDeterminante(matriz);
        }
    }
}

/*
int main(){
    int main(){
    vector<vector<int>> matrix(3, vector<int>(3));
    matrix[0][0] = -2;
    matrix[0][1] = 3;
    matrix[0][2] = -1;
  //  matrix[0][3] = -2;
    matrix[1][0] = 1;
    matrix[1][1] =  -3;
    matrix[1][2] =  1;
 //   matrix[1][3] = -2;
    matrix[2][0] = -1;
    matrix[2][1] = 2;
    matrix[2][2] = -1;
 //   matrix[2][3] =  1;
  /*  matrix[3][0] = -2;
    matrix[3][1] =  2;
    matrix[3][2] = -3;
    matrix[3][3] = -1;

    mostrarMatriz(matrix);
    cout << calcularDeterminante(matrix);
}*/