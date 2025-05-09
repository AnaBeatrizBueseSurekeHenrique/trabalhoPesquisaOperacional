#include "calculoDeterminante.h"
#include <algorithm>
vector<vector<double>> atribuirValor(vector<vector<double>> matrizA, vector<vector<double>> matrizB, vector<int> pos){
    int iteracoes = 0;
    int i;
    while(iteracoes < matrizA.size()){
        i = pos[iteracoes];

        for(int j = 0; j < matrizA.size(); j++){
            matrizB[iteracoes][j] = matrizA[i][j];
        }
        iteracoes++;
    }
    mostrarMatriz(matrizB);
    return matrizB;
}

void mostrarVetor(vector<double> vetor){
    for(int i = 0; i < vetor.size(); i++){
        cout << vetor[i] << " ";
    }
    cout << endl;
}

vector<vector<double>> mudarPosicoes(vector<vector<double>> matrizA, vector<int> pos, int tamI, int tamJ){
    vector<vector<double>> novaMatriz(tamI, vector<double>(tamJ));
    for(int i = 0; i < tamI; i++){
        for(int j = 0; j < tamJ; j++){
            novaMatriz[i][j] = matrizA[i][pos[j]];
        }
    }
    return novaMatriz;
}

vector<double> mudarPosicoesVetor(vector<double> vetorA, vector<int> pos, int tamI){
    vector<double> novoVetor(tamI);
    for(int i = 0; i < tamI; i++){
        novoVetor[i] = vetorA[pos[i]];
    }
    return novoVetor;
}

vector<int> verificarMatrizCorreta(vector<vector<double>> matrizA, vector<int> vec, int tamI, int tamJ){
    vector<vector<double>> novaMatriz(tamI, vector<double>(tamJ));
    double detMatriz;
    do {
        novaMatriz = mudarPosicoes(matrizA, vec, tamI, tamJ);
        detMatriz = calcularDeterminante(novaMatriz);
        if(detMatriz != 0){
            return vec;
        }
    } while (next_permutation(vec.begin(), vec.end()));
    return {};
}
 