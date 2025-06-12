#include <iostream>
#include "transformaMatrizEquacao.h"
#include "calcularMatrizB.h"
#include "calculosParaSimplex.h"
#include<math.h>
#include<string>
#include <cctype>
using namespace std;
vector<string> verifFase1(vector<string> vec);
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
    vec = verifFase1(vec);
    bool fase1 = false, fase2 = true;
    int pos;
    int qntdMaiorIgual = 0, qtndIgual = 0;
    for(int k = 1; k < vec.size(); k++){
        pos = vec[k].find('>');
        if(pos == string::npos){
            pos = vec[k].find('<');
            if(pos == string::npos){
                fase1 = true;
            }           
        } else{
            fase1 = true;
        }
    }
    vector<string> auxVec = vec;
    realizaVariavelFolga(&vec, &maxX);
    maxX--;
    i--;
    vector<int> varArtificais;
    for(int i = 0; i < vec.size(); i++){
        cout << vec[i] << endl;
    }
    if(fase1){
        maxX++;
        int auxMaxX = maxX;
        int pos;
        for(int i = 1; i < vec.size(); i++){
            pos = auxVec[i].find('=');
            if(auxVec[i][pos-1] == '>' || auxVec[i][pos-1] != '<'){
                pos = vec[i].find('=');
                vec[i].insert(pos, "+x" + to_string(maxX));
                varArtificais.push_back(maxX);
                maxX++;
            }
        }
        vec[0].erase(vec[0].begin()+vec[0].size()-1);
        for(int i = 0; i < vec.size(); i++){
            cout << vec[i] << endl;
        }
        maxX--;
    }
    vector<vector<double>> matrix(i, vector<double>(maxX));
    vector<vector<double>> vetorLinha1(1, vector<double>(maxX));
    vector<vector<double>> vectorResult(vec.size() - 1, vector<double>(1));
    vector<vector<double>> matrizB(i, vector<double>(i));
    vector<vector<double>> coeficientesB (1, vector<double>(matrizB[0].size()));
    vector<vector<double>> matrizN(i, vector<double> (maxX - i));
    vector<vector<double>> coeficientesN(1, vector<double>(matrizN[0].size()));
    porValoresMatriz(vec, &matrix, &vetorLinha1, &vectorResult);
    vector<int> posicoes(matrix[0].size());
    if(fase1){
        int pos = 0;
        for(int i = matrix[0].size()-1; i >= 0; i--){
            posicoes[pos] = i;
            pos++;
        }
    } else{
        for(int i = 0; i < posicoes.size(); i++){
            posicoes[i] = i;
        }
    }
    cout << "POSICOES:  ";
    for(int i = 0; i < posicoes.size();i++){
        cout << posicoes[i] << endl;
    }
    if(!fase1){     
        posicoes = verificarMatrizCorreta(matrix, posicoes, matrizB.size(), matrizB[0].size());
    }    
    vector<int> posicoesN(posicoes.begin() + matrizB[0].size(), posicoes.end());
    matrizB = mudarPosicoes(matrix, posicoes, matrizB.size(), matrizB[0].size());
    cout << "Matriz B" << endl;
    mostrarMatriz(matrizB);
    matrizN = mudarPosicoes(matrix, posicoesN, matrizN.size(), matrizN[0].size());
    coeficientesB = mudarPosicoes(vetorLinha1, posicoes, 1, coeficientesB[0].size());
    coeficientesN = mudarPosicoes(vetorLinha1, posicoesN, 1, coeficientesN[0].size());
    cout << endl << "Coeficient B " << endl;
    mostrarMatriz(coeficientesB);
    cout << "Matriz N" << endl;
    mostrarMatriz(matrizN);
    cout << endl << "Coeficient N " << endl;
    mostrarMatriz(coeficientesN);
    if(fase1){
        verificarIdentidadeB(&matrizB, &matrizN, &posicoes, &posicoesN, &coeficientesB, &coeficientesN);
    }
    cout << "MATRIZ B AGR:  " << endl;
    mostrarMatriz(matrizB);
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
    int itFase1 = 1;
    while(fase1){
        cout << "FASE1 PT 1 ";
        cout << itFase1 << endl;
        cout << "MATRIZ B : ";
        mostrarMatriz(matrizB);
        solucaoB = solucaoBasica(matrizB, vectorResult);
        cout << "SOLUCAO B: ";
        mostrarMatriz(solucaoB);
        multSimplex = multiplicadorSimplex(matrizB, coeficientesB);
        cout << "Mult Simplex" << endl;
        mostrarMatriz(multSimplex);
        custoRelativoN(coeficientesN, multSimplex, matrizN, &posSairN, &custorN);
        cout << "MATRIZ N ??????" << endl;
        mostrarMatriz(matrizN);
        if(custorN[0][posSairN] >= 0){
            fase1 = false;
            fase2 = true;
            for(int i = 0; i < posicoes.size(); i++){
                for(int j = 0; j < varArtificais.size(); j++){
                    if(posicoes[i] == varArtificais[j]){
                        fase2 = false;
                        break;
                    }
                }
            }
        }
        cout << "MATRIZ B:  "<<endl;
        mostrarMatriz(matrizB);
        verificarDiagonalB(&matrizB, &coeficientesB, posicoes);
        if(fase1){
            cout << "FASE1???????? ";
            bool direcaoMenorQZero = false;
            for(int i = 0; i < matrizN.size(); i++){
                aN[i][0] = matrizN[i][posSairN];
            }
            cout << "DIRECAO SIMPLEX:   " << endl;
            direcao = calculoDirecaoSimplex(matrizB, aN);
            mostrarMatriz(direcao);
            for(int i = 0; i < direcao.size(); i++){
                if(direcao[i][0] <= 0){
                    cout << "VALOR DIRECAO:  " << direcao[i][0] << endl;
                    direcaoMenorQZero = true;
                    break;
                }
            }
            if(direcaoMenorQZero){
                fase1 = false;
                fase2 = false;
            } else{
                posicaoSairBas = determinacaoPasso(solucaoB, direcao, &fase2);
                trocarColunasMatriz(&matrizB, &matrizN, posicaoSairBas, posSairN);
                trocarColunasMatriz(&coeficientesB, &coeficientesN, posicaoSairBas, posSairN);
                aux = posicoesN[posSairN];
                posicoesN[posSairN] = posicoes[posicaoSairBas];
                posicoes[posicaoSairBas] = aux;
                for(int i = 0; i < posicoes.size(); i++){
                    if(posicoes[i] > matrizB.size()){
                        fase1 = false;
                    } else{
                        fase1 = true;
                    }
                }
            }
        }
        verificarDiagonalB(&matrizB, &coeficientesB, posicoes);
        itFase1++;
       
    }
    verificarDiagonalB(&matrizB, &coeficientesB, posicoes);
    cout << endl << endl << endl;
    
    if(fase2){
        while(menorQZero){
            cout << "Numero de iteracoes: " << iteracoes << endl;
            cout << "Solucao Basica" << endl;
            solucaoB = solucaoBasica(matrizB, vectorResult);
            mostrarMatriz(solucaoB);
            funcObj = calculaFuncaoObjetivo(coeficientesB, solucaoB);
            cout << "Funcao objetivo: ";
            cout << funcObj << endl;
            cout << "Multiplicacao simplex: ";
            multSimplex = multiplicadorSimplex(matrizB,coeficientesB);
            mostrarMatriz(multSimplex);
            cout << endl;
            custoRelativoN(coeficientesN, multSimplex, matrizN, &posSairN, &custorN);
            if(custorN[0][posSairN] < 0){
                menorQZero = true;
            } else{
                menorQZero = false;
            }
            for(int i = 0; i < matrizN.size(); i++){
                aN[i][0] = matrizN[i][posSairN];
            }
            cout << "VALORES: " << custorN[0][posSairN] << endl;
            direcao = calculoDirecaoSimplex(matrizB, aN);
            cout << "Direcao simples: " << endl;
            mostrarMatriz(direcao);
            posicaoSairBas = determinacaoPasso(solucaoB, direcao, &fase2);
            cout << "Posicao sair Basica: " << posicaoSairBas << endl;
            if(menorQZero){
                trocarColunasMatriz(&matrizB, &matrizN, posicaoSairBas, posSairN);
                trocarColunasMatriz(&coeficientesB, &coeficientesN, posicaoSairBas, posSairN);
                aux = posicoesN[posSairN];
                posicoesN[posSairN] = posicoes[posicaoSairBas];
                posicoes[posicaoSairBas] = aux;
                iteracoes++;
                verificarDiagonalB(&matrizB, &coeficientesB, posicoes);
            }
        }
    } else{
        cout << "PROBLEMA INFACTIVEL";
    }
    cout << "Acabou :D";
    funcObj = calculaFuncaoObjetivo(coeficientesB, solucaoB);
    cout << "Valor final: " << funcObj << endl;
    cout << "Solucao B:   " << endl;
    mostrarMatriz(solucaoB);
    cout << "Coeficientes B: ";
    mostrarMatriz(coeficientesB);
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
vector<string> verifFase1(vector<string> vec){
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
    }
    for(int i = 1; i < vec.size(); i++){
        pos = vec[i].find('=');
        pos++;
        if(vec[i][pos] == ' '){
            pos++;
        }
        if(vec[i][pos] == '-'){
            vec[i].erase(vec[i].begin() + pos);
            int j = 1;
            while(pos != -1){
                pos = findN(vec[i], 'x', j);
                if(pos != -1){
                    pos--;
                    cout << "POS: " << vec[i][pos] << endl;
                    while(isdigit(vec[i][pos]) && pos > 0){
                        pos--;
                    }
                    cout << "POS ANTES: " << vec[i][pos] << endl;
                    if(vec[i][pos] == '-'){
                        if(pos == 0){
                            vec[i].erase(vec[i].begin());
                        } else{
                            vec[i][pos] = '+';
                        }
                    } else{
                        if(vec[i][pos] == '+'){
                            vec[i].erase(vec[i].begin() + pos);
                            
                        }
                        vec[i].insert(vec[i].begin() + pos, '-');
                    }
                }
                j++;
            }
            pos = vec[i].find('=');
            pos--;
            if(vec[i][pos] == '<'){
                vec[i][pos] = '>';
            } else{
                if(vec[i][pos] == '>'){
                    vec[i][pos] = '<';
                }
            }
        }
    }
    return vec;
}

// 1e-9