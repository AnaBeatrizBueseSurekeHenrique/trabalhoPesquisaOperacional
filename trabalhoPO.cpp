#include <iostream>
#include "transformaMatrizEquacao.h"
#include "calculoDeterminante.h"
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
        vector<vector<int>> matrix(i, vector<int>(maxX));
        vector<int> vetorLinha1(maxX);
        vector<int> vectorResult(vec.size() - 1);
        vector<vector<int>> matrizB(i, vector<int>(maxX-1));
        porValoresMatriz(vec, matrix, vetorLinha1, vectorResult,matrizB);
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