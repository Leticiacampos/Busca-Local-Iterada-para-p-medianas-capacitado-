/*UNIVERSIDADE FEDERAL DO RIO GRANDE DO NORTE
ESCOLA DE CIENCIAS E TECNOLOGIA
DISCIPLINA: LINGUAGEM DE PROGRAMAÇÃO
PROFESSOR: EVERTON SANTI
ALUNA: LETÍCIA CAMPOS DO NASCIMENTO */
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <time.h>
#include <unistd.h>
#define MAX 50
using namespace std;

struct dados
{
    int m,n,p;
    int capacidade_maxima[MAX];
    int custo[MAX][MAX];
};
struct solucao
{
    int alocacao_candidatos[MAX];
    int mediana[MAX];
    int total_alocados[MAX];
    int custo;
};
void funcao_abre_arquivo (char [], dados&);
void solucaoinicial(solucao&, dados&);
int custodeslocamento(solucao&, dados&);
void pertubacao(solucao&, dados&);
void buscalocal(solucao&, dados&);
void salvararquivo(char [], solucao&, dados&);
int main()
{
    clock_t tinicial, tfinal, testimado=0;
    float tempo, custo1, custo2;
    dados d;
    solucao s;
    char arquivo[MAX], arquivo1[MAX];
    cout << "Digite o nome do arquivo" << endl;
    cin >> arquivo;
    funcao_abre_arquivo(arquivo,d); // função que abre o arquivo
    solucaoinicial(s,d); // INICIO DO ALG. BLI
    buscalocal(s,d);
    custo1 = custodeslocamento(s,d);
    cout << "Digite a quantidade de tempo em segundo (s)" << endl;
    cin >> tempo;
    tinicial = clock();
    while(testimado<tempo)
    {
        pertubacao(s,d);
        buscalocal(s,d);
        custo2 = custodeslocamento(s,d);
        if(custo2<custo1)
        {
            custo1 = custo2;
        }
        tfinal = clock();
        testimado = ((float)tfinal - (float)tinicial)/(CLOCKS_PER_SEC); // transformação dos pulsos para segundo
    } // FINAL DO ALG. BLI
    cout << "Digite o nome do arquivo onde o resultado sera salvo" << endl;
    cin >> arquivo1;
    salvararquivo(arquivo1,s,d); //salva o arquivo
    return 0;
}
void funcao_abre_arquivo(char arquivo[MAX], dados &d)
{
    ifstream le(arquivo);
    //ifstream le("dados1.txt");
    if( !le.is_open() )
    {
        cout <<"Falha ao abrir o arquivo!" << endl;
        exit(1);
    }
    le >> d.n;
    le >> d.m;
    le >> d.p;
    for(int i=0; i<d.n; i++)
    {
        le>>d.capacidade_maxima[i];
    }
    for(int i=0; i<d.m; i++)
    {
        for(int j=0; j<d.n; j++)
        {
            le>>d.custo[i][j];
        }
    }
    le.close();
}
void solucaoinicial( solucao &s, dados &d)
{
    srand(time(NULL));
    int escolha = rand()%d.p;
    s.mediana[0]=((rand()%d.n)+1);
    for(int i=1; i<d.p; i++)
    {
        s.mediana[i]=((rand()%d.n)+1); // GERAÇÃO DE MEDIANAS SEM SEREM REPETIDAS
        for(int j=0; j<i; j++)
        {
            if(s.mediana[i]==s.mediana[j])
            {
                i=0;
                break;

            }
        }
    }
    for(int i=0; i<d.p; i++) //PREENCHE O VETOR TOTAL DE ALOCADOS POIS FUNCIONARÁ COMO UM CONTADOR
    {
        s.total_alocados[i]=0;
    }
    for(int i=0; i<d.m; i++)
    {
        escolha = rand()%d.p;
        while(!(s.total_alocados[escolha]<d.capacidade_maxima[s.mediana[escolha]-1]))
        {
            escolha=rand()%d.p;
        }
        s.alocacao_candidatos[i]=s.mediana[escolha];
        s.total_alocados[escolha]++;

    }

}
int custodeslocamento( solucao &s, dados &d)
{
    s.custo = 0;
    for(int i=0; i<d.m; i++)
    {
        s.custo = s.custo + d.custo[i][s.alocacao_candidatos[i]-1];
    }
    return s.custo;
}
void pertubacao(solucao &s, dados &d)  //introduzir uma pertubação em um local do vetor mediana
{
    srand(time(NULL));
    int valor;
    int escolha = rand()%d.p; // 0 a 2
    valor = rand()%d.n+1; // 1 a 5
    for(int i=0; i<d.p; i++) // 0 a 2
    {
        if(s.mediana[i]==valor) // verfica se os valores da medina são iguais a valor novo
        {
            i=-1;
            valor = rand()%d.n+1; // 1 a 5
        }
    }

    s.mediana[escolha]=valor; // Recebe valor de 1 a 5

    for(int i=0; i<d.p; i++)
    {
        s.total_alocados[i]=0;
    }
    for(int i=0; i<d.m; i++)
    {
        escolha = rand()%d.p;
        while(!(s.total_alocados[escolha]<d.capacidade_maxima[s.mediana[escolha]-1]))
        {
            escolha=rand()%d.p;
        }
        s.alocacao_candidatos[i]=s.mediana[escolha];
        s.total_alocados[escolha]++;

    }

}
void buscalocal(solucao &s,dados &d)
{
    solucao x=s;
    int guarda,custo1, custo2;
    for(int i=0; i <d.m; i++)
    {
        for(int j=i+1; j <d.m; j++)
        {
            if(x.alocacao_candidatos[i]!= x.alocacao_candidatos[j])
            {
                guarda = x.alocacao_candidatos[i];
                x.alocacao_candidatos[i] = x.alocacao_candidatos[j];
                x.alocacao_candidatos[j] = guarda;
                custo1 = custodeslocamento(s,d);
                custo2 = custodeslocamento(x,d);
                if(custo1 > custo2)
                {
                    s = x;
                }
            }
        }
    }
}
void salvararquivo(char arquivo1[MAX], solucao &s, dados &d)
{
    ofstream escreve(arquivo1);
    //ofstream escreve("saida1.txt");
    if(!escreve.is_open())
    {
        cout <<"Falha ao abrir o arquivo!" << endl;
        exit(1);
    }
    escreve << "CUSTO" << " " << s.custo << endl;
    escreve << "LOCAIS DE PROVA" << endl;
    for(int i=0; i<d.p ; i++)
    {
    escreve << s.mediana[i]  << " ";
    cout << endl;
    }
    escreve << "ALOCACAO" << endl;
    for(int i=0; i<d.m; i++)
    {
    escreve << "Candidato " << " " << i+1 << " " << "faz prova em" << " " << s.alocacao_candidatos[i] << endl;
    }
    escreve.close();
}

