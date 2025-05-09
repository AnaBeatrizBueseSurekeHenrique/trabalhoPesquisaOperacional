#include <iostream>
#include "transformaMatrizEquacao.h"
#include "calcularMatrizB.h"
#include "calculosParaSimples.h"
using namespace std;
int main(){
    string s;
    ifstream f("entrada.txt");
    vector<string> vec;
    int i;
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
    vector<vector<double>> vetorLinha1(1, vector<double>(maxX));
    vector<vector<double>> vectorResult(vec.size() - 1, vector<double>(1));
    vector<vector<double>> matrizB(i, vector<double>(i));
    vector<vector<double>> coeficientesB (1, vector<double>(matrizB[0].size()));
    vector<vector<double>> matrizN(i, vector<double> (maxX - i));
    vector<vector<double>> coeficientesN(1, vector<double>(matrizN[0].size()));
    porValoresMatriz(vec, &matrix, &vetorLinha1, &vectorResult);
    vector<int> posicoes(matrix[0].size());
    for(int i = 0; i < posicoes.size(); i++){
        posicoes[i] = i;
    }
    posicoes = verificarMatrizCorreta(matrix, posicoes, matrizB.size(), matrizB[0].size());
    vector<int> posicoesN(posicoes.begin() + matrizB[0].size(), posicoes.end());
    matrizB = mudarPosicoes(matrix, posicoes, matrizB.size(), matrizB[0].size());
    cout << "Matriz B" << endl;
    mostrarMatriz(matrizB);
    matrizN = mudarPosicoes(matrix, posicoesN, matrizN.size(), matrizN[0].size());
    coeficientesB = mudarPosicoes(vetorLinha1,posicoes, 1, coeficientesB[0].size());
    cout << endl << "Coeficient B " << endl;
    mostrarMatriz(coeficientesB);
    coeficientesN = mudarPosicoes(vetorLinha1, posicoesN, 1, coeficientesN[0].size());
    cout << "Matriz N" << endl;
    mostrarMatriz(matrizN);
    cout << endl << "Coeficient N " << endl;
    mostrarMatriz(coeficientesN);
    vector<vector<double>> multSimples = multiplicadorSimplex(matrizB, coeficientesB);
    cout << "Multiplicacao simples" << endl;
    mostrarMatriz(multSimples);
    cout << "Solucao Basica" << endl;
    vector<vector<double>> solucaoB = solucaoBasica(matrizB, vectorResult);
    mostrarMatriz(solucaoB);
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