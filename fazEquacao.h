#include <iostream>
#include <cctype>
#include<math.h>
#include<string>
#include <cstring>
using namespace std;

const double EulerConstant = std::exp(1.0);

struct fila {
    char info;
    int prior;
    fila *link;
};

fila *inicializaFP(fila *L){
    L = NULL;
    return L;
}

fila *insereFP(fila *L, char valor, int prior){
    fila *N, *P, *ANT;

    N = new fila;
    N->info = valor;
    N->prior = prior;

    if(L == NULL) {
        L = N;
        N->link = NULL;
    }else{
        P = L;

        while ((P != NULL) && (prior >= P->prior)) {
            ANT = P;
            P = P->link;
        }
        if (P == L) {
            N->link = L;
            L = N;
        }
        else {
            ANT->link = N;
            N->link = P;
        }
    }
    return L;
}

fila *removeFP(fila *L, char *n) {
	fila *AUX;

	if (L != NULL) {
		*n = L->info;
		AUX = L;
		L = L->link;
		delete AUX;
	}
	return L;
}
fila* analisaOperadores(string equacao){
    fila* F;
    F = inicializaFP(F);
    for(int i = 0; i < equacao.size(); i++){
        if(equacao[i] == 'c' || equacao[i] == 't' || equacao[i] == 'l'){
            F = insereFP(F, equacao[i], 0);
        } else{
            if(equacao[i] == 's' && equacao[i-2] != 'c'){
                F = insereFP(F, equacao[i], 0);
            } else{
                if(equacao[i] == '^'){
                    F = insereFP(F, equacao[i], 1);
                } else{
                    if(equacao[i] == '*' || equacao[i] == '/'){
                    F = insereFP(F, equacao[i], 2);
                    }else{
                        if(equacao[i] == '+' || equacao[i] == '-'){
                            if(i != 0 && equacao[i-1] != '*' && equacao[i-1] != '/' && equacao[i-1] != '^' && equacao[i-1] != 'c' && equacao[i-1] != 't' && equacao[i-1] != 'g' && equacao[i-1] != 'n' && equacao[i-1] != 's'){
                                F = insereFP(F, equacao[i], 3);
                            }
                        }
                    }
                }
            }
        }
    }
    return F;
}
double moduloEq(double r1, double r2){
    double sub = r2-r1;
    if(sub < 0){
        return (sub*(-1));
    }else{
        return sub;
    }
}

double moduloVar(double x){
    if(x < 0){
        return x * -1;
    } else{
        return x;
    }
}

double numero(string str, int *it){
    double inteiros = 0;
    int exp;
    double decimal = 0;
    double result = 1;
    string aux = "";
    if(*it == 1 && str[*it - 1] == '-'){
        result = -1;
    } else{
        if(str[*it] == '-'){
            result = -1;
            (*it)++;
        }
    }
    while(*it < str.size() && isdigit(str[*it]) || str[*it] == '.'){
        aux = "";
        if(isdigit(str[*it])){
            while(isdigit(str[*it])){
                aux += str[*it];
                (*it)++;
            }
            inteiros = stoi(aux, 0, 10);
        } else{
            if(str[*it] == '.'){
                exp = 0;
                (*it)++;
                if(isdigit(str[*it])){
                    while(isdigit(str[*it])){
                        aux += str[*it];
                        (*it)++;
                        exp++;
                    }
                    decimal = stoi(aux, 0, 10);
                    decimal = decimal * pow(10, (exp*(-1)));
                }
            }
        } 
    }
    result = result*(inteiros + decimal);
    return result;
}
double valorVariaveis(int *fim, string equacao, double x){
    switch(equacao[*fim]){
      case 'x':
            (*fim)++;
            if(equacao[(*fim)-1] == '-'){
                return (x*-1);
            } else{
                return x;
            }
            break;
        case 'e':
            (*fim)++;
            if(isalpha(equacao[(*fim)])){
                (*fim)++;
                return numero(equacao, fim);
            } else{
                if(equacao[(*fim)-2] == '-'){
                return (-1*EulerConstant);
                } else{
                    return EulerConstant;
                }
            }
            break;
        default:
            return numero(equacao, fim);
            break;
    }
}

string manipulaEquacao(string equacao, int pos, double x, char operador){
    int fim, aux;
    double result;
    double a, b;
    string novaString;
    aux = pos;
    pos--;
    while(((isdigit(equacao[pos])) || equacao[pos] == '.') && pos > 0){
        pos--;
    }
    if(!(isdigit(equacao[pos]) || equacao[pos] == 'x' || equacao[pos] == 'e')){
        pos++;
    }
    if(pos == 1 && equacao[pos-1] == '-'){
        pos = 0;
        fim = 1;
    } else{
        if(pos < 0){
            pos = 0;
        }
        fim = pos;
    }
    a = valorVariaveis(&fim, equacao, x);
    fim++;

    while(isalpha(equacao[fim]) && equacao[fim] != 'e' && equacao[fim] != 'x'){
        fim++;
    }
    if(equacao[fim] == '-' && operador == '-'){
        operador = '+';
        fim++;
    }
    while(!(isdigit(equacao[aux])) && equacao[aux] != '-'){
        aux++;
    }
    b = valorVariaveis(&fim, equacao, x);
    switch(operador){            
        case '+':
            result = a + b;
            break;
        case '-':
            result = a - b;
            break;
        case '*':
            result = a*b;
            break;
        case '/':
            if(b != 0){
                result = a/b;
            } else{
                equacao = "NAN";
            }
            break;
        case '^':
            result = pow(a,b);
            break;
        case 'c':
            aux = fim;
            while(!(isalpha(equacao[aux])) || equacao[aux] == 'x' || equacao[aux] == 'e'){
                aux--;
            }
            switch(equacao[aux]){
                case 'c':
                    result = asin(b);
                    break;
                case 't':
                    result = atan(b);
                    break;
                case 's':
                    result = cos(b);
                    break;
            }
            break;
        case 's':
            aux = fim;
            while(!(isalpha(equacao[aux])) || equacao[aux] == 'x' || equacao[aux] == 'e'){
                aux--;
            }
            switch(equacao[aux]){
                case 'c':
                    result = acos(b);
                    break;
                case 'n':
                    result = sin(b);
                    break;
            }
            break;
        case 't':
            aux = fim;
            while(!(isalpha(equacao[aux])) || equacao[aux] == 'x' || equacao[aux] == 'e'){
                aux--;
            }
            result = tan(b);
            break;
        case 'l':
            if(b <= 0){
                equacao = "NAN";
            } else{
                aux = fim;
                while(!(isalpha(equacao[aux])) || equacao[aux] == 'x' || equacao[aux] == 'e'){
                    aux--;
                }
                switch(equacao[aux]){
                    case 'g':
                        result = log10(b);
                        break;
                    case 'n':
                        result = log(b);
                        break;
                }
            }
            break;
        default:
            result = a;
            break;
    }
    if(equacao != "NAN"){
        novaString = to_string(result);
        equacao.replace(pos, (fim - pos) , novaString);
    }
    return equacao;
}
double realizaEquacao(string equacao, double x){
    int pos = 0, parenteses[2] = {0,0};
    char operador;
    int n = 0;
    double result;
    fila *F = inicializaFP(F);
    for(int i = 0; i < equacao.size(); i++){
        switch(equacao[i]){
            case '(':
                parenteses[0] = i;
                break;
            case ')':
                parenteses[1] = i;
                equacao.replace(parenteses[0], (parenteses[1] - parenteses[0] + 1), to_string(realizaEquacao(equacao.substr((parenteses[0]+1), (parenteses[1]) - parenteses[0] - 1), x)));
                i = -1;
                break;
        }
    }
    F = analisaOperadores(equacao);
     if(equacao[0] == 'x'){
        equacao.replace(0,1, to_string(x));
    }
    
     if(equacao[0] == 'x'){
        equacao.replace(0,1, to_string(x));
    }

        while(F != NULL){  
            pos = equacao.find('n');
            if(equacao[pos+1] != 'a'){          
                F = removeFP(F, &operador);
                pos = equacao.find(operador);
                if(pos == 0){
                    pos++;
                    while(equacao[pos] != operador && pos < equacao.size()){
                        pos++;
                    }
                    if(pos >= equacao.size()){
                        pos = 0;
                    }
                }
                equacao = manipulaEquacao(equacao, pos, x, operador);
            } else{
                return NAN;
            }
        }
    if(equacao == "NAN"){
        return NAN;
    } else{
        result = numero(equacao, &n);
        return result;
    }
}