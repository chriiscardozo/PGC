#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgc.h"

#define F_READ "r"
#define F_WRITE "w"
#define F_READ_WRITE "rw"

#define DATABASE_ID_CLIENTE "database\\idCliente.pgc"
#define DATABASE_NOME_CLIENTE "database\\nomeCliente.pgc"
#define DATABASE_EMAIL_CLIENTE "database\\emailCliente.pgc"
#define DATABASE_CELULAR_CLIENTE "database\\celularCliente.pgc"
#define DATABASE_TELEFONE_CLIENTE "database\\telefoneCliente.pgc"
#define DATABASE_SALDO_CLIENTE "database\\saldoCliente.pgc"
#define DATABASE_VENCIMENTO_CLIENTE "database\\vencimentoCliente.pgc"

struct Data{
       int dia;
       int mes;
       int ano;
};

struct Cliente{
       int id;
       char nome[500];
       char email[500];
       char telefone[8];
       char celular[8];
       float saldo;
       struct Data vencimento;
       int linha;
};

/* INICIALIZA A STRUCT CLIENTE */
void initCliente(struct Cliente *c){
     c->id = 0;
     
     memset(c->nome, 0, sizeof(char) * strlen(c->nome));
     memset(c->email, 0, sizeof(char) * strlen(c->email));
     memset(c->telefone, 0, sizeof(char) * strlen(c->telefone));
     memset(c->celular, 0, sizeof(char) * strlen(c->celular));
     
     c->linha = 0;
     c->saldo = 0;
     c->vencimento.dia = 0;
     c->vencimento.mes = 0;
     c->vencimento.ano = 0;     
}

/* SELECIONA NOME DO CLIENTE DE ACORDO COM A LINHA NA BASE DE DADOS */
int buildNome(struct Cliente *c){
    FILE *f = NULL;
    int linha = 0;
    
    f = fopen(DATABASE_NOME_CLIENTE, F_READ);
    if(!f){
       printf("Ocorreu um erro ao tentar ler a base de dados\n<ENTER>");
       getchar();
       return 0;
    }
    
    while(!feof(f)){
       char aux[500];
       
       linha++;
       fgets(aux, 499, f);

       if(linha == c->linha){
                strcpy(c->nome, aux);
                
                if(fclose(f)){
                   printf("Problemas ao fechar o arquivo de dados\n<ENTER>");
                   getchar();
                }
                
                return 1;
       }
    }
    
    if(fclose(f)){
       printf("Problemas ao fechar o arquivo de dados\n<ENTER>");
       getchar();
    }
    
    return 0; 
}

/* INICIALIZA O ID E A LINHA DO CLIENTE NA BASE DE DADOS */
int buildId(struct Cliente *c, int id){
     FILE *f = NULL;
     int linha = 0;
     
     f = fopen(DATABASE_ID_CLIENTE, F_READ);
     if(!f){
            printf("Ocorreu um erro ao tentar ler a base de dados\n<ENTER>");
            getchar();
            return 0;
     }
     
     while(!feof(f)){
        int aux;
        
        linha++;
        fscanf(f, "%d", &aux);
        
        if(aux == id){
           if(fclose(f)){
              printf("Problemas ao fechar o arquivo de dados\n<ENTER>");
              getchar();
           }
           c->id = id;
           return linha;
        }
     }
     
     if(fclose(f)){
        printf("Problemas ao fechar o arquivo de dados\n<ENTER>");
        getchar();
     }
     
     return 0;
}

/* CONSTROI INFORMAÇÕES DO CLIENTE PELO DO ID*/
int buildClienteById(struct Cliente *c, int id){
     
     initCliente(c);
     
     c->linha = buildId(c, id);
     
     if(!c->linha){
        printf("ERRO: Nao foi encontrado cliente com este ID\n<ENTER>");
        getchar();
        return 0;
     }
     
     if(!buildNome(c)){
        printf("ERRO: erro ao procurar nome\n<ENTER>");
        getchar();
        return 0;
     }
     
     return 1;
}
