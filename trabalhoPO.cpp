#include <iostream>
#include "calculosParaSimplex.h"
#include<math.h>
#include<string>
#include <cctype>

using namespace std;

vector<string> verifFase1(vector<string> vec, bool *maximizar);
void mostrarVetor(vector<string> vec);

int main(){
    string s;
    bool haSolucao, maximizar = false, fase1 = false, fase2 = true;

    ifstream f("entrada.txt");
    vector<string> vec;
    int qntdLinhas = 0, maxX, pos;
   
    while (getline(f, s)){
        if(!s.empty()){
            vec.insert(vec.begin() + qntdLinhas, s);
            qntdLinhas++;
        }
    }
    f.close();
    
    vec = verifFase1(vec, &maximizar);
    qntdLinhas--;
    pos = vec[qntdLinhas].find(',');
    
    if(pos != string::npos){
        vec.erase(vec.begin() + qntdLinhas);
        qntdLinhas--;
    }
    
    //verifica se ha necessidade para a fase 1
    int k = 1;
    while(k < vec.size() && !fase1){
        pos = vec[k].find('>');
        if(pos == string::npos){
            pos = vec[k].find('<');
            if(pos == string::npos){
                fase1 = true;
            }           
        } else{
            fase1 = true;
        }
        k++;
    }
    
    vector<string> auxVec = vec;
    int maxValXEquacao = verificarMaiorX(vec);
    realizaVariavelFolga(&vec, &maxX);
    maxX--;
    
    cout << "Adicionadas as variaveis de folga, sistema atual: " << endl;
    mostrarVetor(vec);
    // necessario para a fase1, encontra o valor maximo de x antes das variaveis artificiais serem adicionadas.
    int auxMaxXOriginal = maxX;
    int qntdDesigualdade = 0;
    if(fase1){
        maxX++;
        for(int i = 1; i < vec.size(); i++){
            pos = auxVec[i].find('<');
            if(pos == string::npos){
                qntdDesigualdade++;
            }
        }
        for(int i = 1; i < vec.size(); i++){
            
            pos = auxVec[i].find('<');
            
            // caso sistema não tiver desigualdade menor que, adicionara artificial
            
            if(pos == string::npos || qntdDesigualdade > 1){
                pos = vec[i].find('=');
                vec[i].insert(pos, "+x" + to_string(maxX));
                maxX++;
            }
        }
        maxX--;
    }
    vector<vector<double>> matrizA(qntdLinhas, vector<double>(maxX));
    vector<vector<double>> matrizB(qntdLinhas, vector<double>(qntdLinhas));
    vector<vector<double>> matrizN(qntdLinhas, vector<double> (maxX - qntdLinhas));
    vector<vector<double>> coeficientesB (1, vector<double>(matrizB[0].size()));
    vector<vector<double>> coeficientesN(1, vector<double>(matrizN[0].size()));
    vector<vector<double>> vetorLinha1(1, vector<double>(maxX));
    vector<vector<double>> vectorResult(vec.size() - 1, vector<double>(1));
    vector<vector<double>> solucaoB(1, vector<double>(matrizB[0].size()));
    vector<vector<double>> multSimplex (1, vector<double>(matrizB[0].size()));
    vector<vector<double>> direcao(matrizB.size(), vector<double>(1));
    vector<vector<double>> aN(matrizN.size(), vector<double>(1));
    vector<vector<double>> custorN(1, vector<double>(coeficientesN[0].size()));


   //transforma o sistema em matrizes: 

    porValoresMatriz(vec, &matrizA, &vetorLinha1, &vectorResult);
    vector<int> posicoes(matrizA[0].size());
    if(fase1){

        int j = 0;
        bool identidade;
        int i = maxValXEquacao;
        if(qntdDesigualdade > 1){
            //caso A
            int val = 0;
            for(int i = 0; i < matrizB.size(); i++){
                posicoes[i] = auxMaxXOriginal + i;
            }
            for(int i = 0; i < auxMaxXOriginal; i++){
                posicoes[i+matrizB.size()] = val;
                val++; 
            }
        } else{
            //caso B
            int val = 0;
            for(int i = 0; i < maxValXEquacao; i++){
                posicoes[i+matrizB.size()] = val;
                val++;
             }
              for(int i = 0; i < posicoes.size(); i++){
        }
            int j = 0;
            int i = maxValXEquacao;
            while(j < matrizB.size()){
                if(matrizA[j][i] == -1){
                    posicoes[matrizB.size()+val] = i;
                    val++;
                } else{
                    if(matrizA[j][i] == 1){
                        posicoes[j] = i;
                        j++; 
                    }
                }
                if(i >= posicoes.size()){
                    i = maxValXEquacao;
                } else{
                    i++;
                }
            }
        }

    }
    if(!fase1){
        for(int i = 0; i < matrizA[0].size(); i++){
            posicoes[i] = i;
        }
        // encontra uma matriz com determinante diferente de zero paera a matriz B.     
        posicoes = verificarMatrizCorreta(matrizA, posicoes, matrizB.size(), matrizB[0].size());
    }    
  

    vector<int> posicoesN(posicoes.begin() + matrizB[0].size(), posicoes.end());

    while(posicoes.size() > matrizB.size()){
        posicoes.erase(posicoes.end() - 1);
    }

    matrizB = mudarPosicoes(matrizA, posicoes, matrizB.size(), matrizB[0].size());

    matrizN = mudarPosicoes(matrizA, posicoesN, matrizN.size(), matrizN[0].size());

    if(fase1){
        for(int i = 0; i < matrizB[0].size(); i++){
            if(posicoes[i] >= auxMaxXOriginal){
                coeficientesB[0][i] = 1;
            }
        }
    } else{
        coeficientesB = mudarPosicoes(vetorLinha1, posicoes, 1, coeficientesB[0].size());
        coeficientesN = mudarPosicoes(vetorLinha1, posicoesN, 1, coeficientesN[0].size());
    }
    int posicaoSairBas, posSairN, iteracoes = 1;
    if(fase1){
        cout << "Inicio fase 1: " << endl;
    }
    while(fase1 && iteracoes < 100){
        cout << "Iteracao numero " << iteracoes << endl;
        solucaoB = solucaoBasica(matrizB, vectorResult);
        multSimplex = multiplicadorSimplex(matrizB, coeficientesB);
        custorN = custoRelativoN(coeficientesN, multSimplex, matrizN, &posSairN, custorN);
        
        if(custorN[0][posSairN] >= 0){
            fase1 = false;
            fase2 = true;
            for(int i = 0; i < matrizB[0].size(); i++){
                if(posicoes[i] > auxMaxXOriginal){
                    fase2 = false;
                    cout << "Há variaveis artificais na básica!";
                    haSolucao = false;
                    break;
                }
            }
            removerVarArtificial(&matrizN, &posicoesN, &posicoes, &coeficientesN, &custorN, auxMaxXOriginal);
            coeficientesB = mudarPosicoes(vetorLinha1, posicoes, 1, coeficientesB[0].size());
            coeficientesN = mudarPosicoes(vetorLinha1, posicoesN, 1, coeficientesN[0].size());
            cout << "Custos maiores que zero! " << endl;
            break;
        }
         for(int i = 0; i < matrizN.size(); i++){
            aN[i][0] = matrizN[i][posSairN];
        }
        direcao = calculoDirecaoSimplex(matrizB, aN); 
        fase1 = false;
        fase2 = false;
        for(int i = 0; i < direcao.size(); i++){
            if(direcao[i][0] > 0){
                fase1 = true;
                fase2 = true;
                break;
            }
        }
        if(fase1){
            posicaoSairBas = determinacaoPasso(solucaoB, direcao);
            trocarPosicoesBeN(&matrizB, &coeficientesB, &matrizN, &coeficientesN, posicaoSairBas, posSairN, &posicoes, &posicoesN);
            fase2 = true;
            for(int i = 0; i < matrizB[0].size(); i++){
                if(posicoes[i] > auxMaxXOriginal){
                    fase2 = false;
                    break;
                }
            }
            if(fase2){    
                removerVarArtificial(&matrizN, &posicoesN, &posicoes, &coeficientesN, &custorN, auxMaxXOriginal);
                coeficientesB = mudarPosicoes(vetorLinha1, posicoes, 1, coeficientesB[0].size());
                coeficientesN = mudarPosicoes(vetorLinha1, posicoesN, 1, coeficientesN[0].size());
                cout << "Nao ha mais variaveis artificiais na basica! ";
                fase1 = false;
            }
            verificarDiagonalB(&matrizB, &coeficientesB, &posicoes);
            iteracoes++;
        }
    }

    if(iteracoes >= 100){
        fase2 = false;
    }
    iteracoes = 1;
   
    // veririca se ha valor igual a zero na diagonal de B, se ha, ira trocar as posicoes.
    verificarDiagonalB(&matrizB, &coeficientesB, &posicoes);

    if(!fase2){
        cout << "Problema infactivel!";
    } else{
        cout << endl << "Inicio fase 2: " << endl << endl;
    }
    while(fase2 && iteracoes < 100){
        cout << endl << "Iteracao numero: " << iteracoes << endl;
        solucaoB = solucaoBasica(matrizB, vectorResult);
        multSimplex = multiplicadorSimplex(matrizB,coeficientesB);

        custorN = custoRelativoN(coeficientesN, multSimplex, matrizN, &posSairN, custorN);

        if(custorN[0][posSairN] < 0){
            fase2 = true;
        } else{
            fase2 = false;
            for(int i = 0; i < solucaoB.size();i++){
                if(solucaoB[i][0] < 0){
                    fase2 = true;
                }
            }
            cout << endl << endl;
            if(!fase2){
                haSolucao = true;
            }
        }
       
        if(fase2){
            for(int i = 0; i < matrizN.size(); i++){
                aN[i][0] = matrizN[i][posSairN];
             }
            direcao = calculoDirecaoSimplex(matrizB, aN);
            
            fase2 = false;
            for(int i = 0; i < direcao.size(); i++){
                if(direcao[i][0] > 0){
                    fase2 = true;
                    break;
                }
                haSolucao = false;
            }
        }
        if(fase2){
            posicaoSairBas = determinacaoPasso(solucaoB, direcao);
            trocarPosicoesBeN(&matrizB, &coeficientesB, &matrizN, &coeficientesN, posicaoSairBas, posSairN, &posicoes, &posicoesN);
            iteracoes++;
            verificarDiagonalB(&matrizB, &coeficientesB, &posicoes);
        }
        mostrarValorX(posicoes, solucaoB, auxMaxXOriginal);
        
    } 
    if(haSolucao){
        cout << "Solucao otima encontrada!" << endl;
        double funcObj = calculaFuncaoObjetivo(coeficientesB, solucaoB);
        if(abs(funcObj) < 1e-9){
            funcObj = 0;
        }
        if(maximizar){
            funcObj = funcObj*-1;
        }
        
        cout << "Valor final: " << funcObj << endl;
    } else{
        cout << "Problema infactivel";
    }
    return 0;
}
void mostrarVetor(vector<string> vec){
    for(int i = 0; i < vec.size(); i++){
        cout << vec[i] << endl;
    }
}
vector<string> verifFase1(vector<string> vec, bool *maximizar){
    //verifica se o problema e de maximizacao
    int pos = vec[0].find('a');
    
    if(pos != string::npos){
        (*maximizar) = true;
        cout << "PROBLEMA DE MAXMIZACAO ENCONTRADO! Transformando em de minimizacao..." << endl << endl;
        vec[0].erase(vec[0].begin() + pos);
        vec[0].erase(vec[0].begin()+pos);
        vec[0].insert(pos, "in");
        // transforma em problema de minimizacao
        
        string aux = vec[0];
        int i = 1;
        pos = achar_n_vez(vec[0], '-', i);
        i++;

        while(pos != -1){
            //encontra -, e os transforma em +
            aux[pos] = '+';
            pos = achar_n_vez(vec[0], '-', i);
            i++;
        }

        i = 1;
        pos = achar_n_vez(vec[0], '+', i);
        i++;
        while(pos != string::npos){
            //encontra os +, e os transforma em -
            aux[pos] = '-';
            pos = achar_n_vez(vec[0], '+', i);
            i++;
        }

        vec[0] = aux;
        pos= vec[0].find('=');
        pos++;

        if(vec[0][pos] == ' '){
            pos++;
        }
        //checa se havia sinal no primeiro x da primeira linha, se nao ha, insere um - antes dele.
        if(!ispunct(vec[0][pos])){
            vec[0].insert(pos, "-");
        } else{
            if(vec[0][pos] == '+'){
                vec[0].erase(vec[0].begin() + pos);
            }
        }
    }
    //apos a verificao da primeira linha, verifica se ha valor negativo apos o sinal de =
    
    for(int i = 1; i < vec.size(); i++){
        pos = vec[i].find('=');
        pos++;
        if(vec[i][pos] == ' '){
            pos++;
        }
        if(vec[i][pos] == '-'){
            cout << "Valor negativo no vetor b encontrado! Invertendo os sinais....";
            vec[i].erase(vec[i].begin() + pos);
            int j = 1;
            //se ha, ele multiplicara a linha por -1, invertendo os sinais.
            while(pos != -1){
                pos = achar_n_vez(vec[i], 'x', j);
                if(pos != -1){
                    pos--;
                    while(isdigit(vec[i][pos]) && pos > 0){
                        pos--;
                    }
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
    cout << "Sistema atual: " << endl;
    mostrarVetor(vec);
    cout << endl << "---------------" << endl << endl;
    return vec;
}