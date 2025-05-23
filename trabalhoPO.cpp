#include <iostream>
#include "transformaMatrizEquacao.h"
#include "calcularMatrizB.h"
#include "calculosParaSimplex.h"
#include<math.h>
#include<string>
#include <cctype>


using namespace std;
vector<string> fase1(vector<string> vec);
int findN(string str,char ch, int n);
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
    //  posicoes = {2,3,4,0,1};
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
    vector<vector<double>> solucaoB(1, vector<double>(matrizB[0].size()));
    vector<vector<double>> multSimplex = multiplicadorSimplex(matrizB, coeficientesB);
    vector<vector<double>> custorN(1, vector<double>(coeficientesN.size()));
    vector<vector<double>> aN(matrizN.size(), vector<double>(1));
    vector<vector<double>> direcao(matrizB.size(), vector<double>(1));
    bool menorQZero = true;
    int posicaoSairBas;
    int aux;
    int posSairN;
    double funcObj;
    int iteracoes = 1;
    cout << endl << endl << endl;
    while(menorQZero){
        cout << "Numero de iteracoes: " << iteracoes << endl;
        cout << "Solucao Basica" << endl;
        solucaoB = solucaoBasica(matrizB, vectorResult);
        mostrarMatriz(solucaoB);
        funcObj = calculaFuncaoObjetivo(coeficientesB, solucaoB);
        cout << "Funcao objetivo: ";
        cout << funcObj << endl;
        cout << "Multiplicacao simplex: ";
        mostrarMatriz(multSimplex);
        cout << endl;
        custorN = custoRelativoN(coeficientesN, multSimplex, matrizN, &posSairN);
        if(custorN[0][posSairN] < 0){
            menorQZero = true;
        } else{
            menorQZero = false;
        }
        for(int i = 0; i < matrizN.size(); i++){
            aN[i][0] = matrizN[i][posSairN];
        }
        direcao = calculoDirecaoSimplex(matrizB, aN);
        cout << "Direcao simples: " << endl;
        mostrarMatriz(direcao);
        posicaoSairBas = determinacaoPasso(solucaoB, direcao);
        cout << "Posicao sair Basica: " << posicaoSairBas << endl;
        trocarColunasMatriz(&matrizB, &matrizN, posicaoSairBas, posSairN);
        trocarColunasMatriz(&coeficientesB, &coeficientesN, posicaoSairBas, posSairN);
        aux = posicoesN[posSairN];
        posicoesN[posSairN] = posicoes[posicaoSairBas];
        posicoes[posicaoSairBas] = aux;
        iteracoes++;
    }
    cout << "Acabou :D";
    cout << "Valor final: " << funcObj << endl;
}
int findN(string str,char ch, int n){
    int vezes = 0;
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == ch) {
            vezes++;
        }
        if (vezes == n)
            return i;
    }
    return -1;
}
vector<string> fase1(vector<string> vec){
    int pos = vec[0].find('a');
    if(pos != string::npos){
        vec[0].erase(vec[0].begin() + pos);
        vec[0].erase(vec[0].begin()+pos);
        vec[0].insert(pos, "in");
        string aux = vec[0];
        int i = 1;
        pos = findN(vec[0], '-', i);
        i++;
        while(pos != -1){
            aux[pos] = '+';
            pos = findN(vec[0], '-', i);
            i++;
        }
        i = 1;
        pos = findN(vec[0], '+', i);
        i++;
        while(pos != string::npos){
            aux[pos] = '-';
            pos = findN(vec[0], '+', i);
            i++;
        }
        vec[0] = aux;
        pos= vec[0].find('=');
        pos++;
        if(vec[0][pos] == ' '){
            pos++;
        }
        if(!ispunct(vec[0][pos])){
            vec[0].insert(pos, "-");
        } else{
            if(vec[0][pos] == '+'){
                vec[0].erase(vec[0].begin() + pos);
            }
        }
        cout << vec[0] << endl;
    }
    return vec;
}