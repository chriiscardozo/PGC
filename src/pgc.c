#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "pgc.h"

/* SETA A DATA PARA O DIA ATUAL */
void hojeIs(struct Data *h){
     time_t now = time(NULL);
     struct tm *t = localtime(&now);

     h->dia = t->tm_mday;
     h->mes = t->tm_mon+1;
     h->ano = t->tm_year+1900;
}

/* IMPRIME A STRUCT CLIENTE */
void printCliente(struct Cliente *c){
     printf("\nID: %d\n", c->id);
     printf("Nome: %s\n", c->nome);
     printf("Telefone: %s\tCelular: %s\n", c->telefone, c->celular);
     printf("Email: %s\n", c->email);
     printf("Saldo: %.2f\n", c->saldo);
     printf("Prazo de pagamento: %.2d/%.2d/%.2d\n",
             c->vencimento.dia, c->vencimento.mes, c->vencimento.ano);
}

/* INICIALIZA A STRUCT CLIENTE */
void initCliente(struct Cliente *c){
     
     memset(c->nome, 0, sizeof(char) * strlen(c->nome));
     memset(c->email, 0, sizeof(char) * strlen(c->email));
     memset(c->telefone, 0, sizeof(char) * strlen(c->telefone));
     memset(c->celular, 0, sizeof(char) * strlen(c->celular));
     
     c->id = 0;
     c->linha = 0;
     c->saldo = 0;
     c->vencimento.dia = 0;
     c->vencimento.mes = 0;
     c->vencimento.ano = 0;
}

/* INICIALIZA A STRUCT RESULTADOPESQUISA */
void initResultadoPesquisa(struct ResultadoPesquisa *r){
     r->qtd = 0;
     r->resultado = NULL;
}

/* ABRE ARQUIVO E RETORNA SEU PONTEIRO */
FILE * abreArquivo(char caminho[], char modo[]){
    FILE *f = fopen(caminho, modo);
    
    if(!f){
           printf("ERRO: Ocorreu um erro ao tentar ler o arquivo de dados\n<ENTER>");
           getchar();
           return 0;
    }
    
    return f;
}

/* FECHA UM ARQUIVO */
void fechaArquivo(FILE *f){
    if(fclose(f)){
        printf("ERRO: Problemas ao fechar o arquivo de dados\n<ENTER>");
        getchar();
     }
}

/* RECEBE A LINHA E RETORNA O ID REFERENTE A ESTA */
int qualId(int linha){
    FILE *f = NULL;
    int i;
    
    f = abreArquivo(DATABASE_ID_CLIENTE, F_READ);
    if(!f) return 0;
    
    for(i = 1; !feof(f); i++){
       int aux;
       
       fscanf(f, "%d", &aux);
       if(i == linha){
          fechaArquivo(f);
          return aux;
       }
    }
    
    fechaArquivo(f);
    
    return 0;
}


/* INICIALIZA O ID E A LINHA DO CLIENTE NA BASE DE DADOS */
int buildId(struct Cliente *c, int id){
     FILE *f = NULL;
     int linha = 0;
     
     f = abreArquivo(DATABASE_ID_CLIENTE, F_READ);
     if(!f) return 0;
     
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
     
     fechaArquivo(f);
     
     return 1;
}

/* SELECIONA NOME DO CLIENTE DE ACORDO COM A LINHA NA BASE DE DADOS */
int buildNome(struct Cliente *c){
    FILE *f = NULL;
    int linha = 0;
    
    f = abreArquivo(DATABASE_NOME_CLIENTE, F_READ);
    if(!f) return 0;
    
    while(!feof(f)){
       char aux[500] = { 0 };
       
       linha++;
       fgets(aux, 499, f);

       if(linha == c->linha){
                int ultChar = strlen(aux) - 1;
                
                if(aux[ultChar] == 10) aux[ultChar] = 0;
                
                strcpy(c->nome, aux);
                
                if(fclose(f)){
                   printf("Problemas ao fechar o arquivo de dados\n<ENTER>");
                   getchar();
                }
                
                return 1;
       }
    }
    
    fechaArquivo(f);
    
    return 1; 
}

/* SELECIONA TELEFONE DO CLIENTE DE ACORDO COM A LINHA NA BASE DE DADOS */
int buildTelefone(struct Cliente *c){
    FILE *f = NULL;
    int linha = 0;
    
    f = abreArquivo(DATABASE_TELEFONE_CLIENTE, F_READ);
    if(!f) return 0;
    
    while(!feof(f)){
       char aux[10] = { 0 };
       
       linha++;
       fgets(aux, 10, f);

       if(linha == c->linha){
                aux[8] = 0;
                strcpy(c->telefone, aux);
                
                if(fclose(f)){
                   printf("Problemas ao fechar o arquivo de dados\n<ENTER>");
                   getchar();
                }
                
                return 1;
       }
    }
    
    fechaArquivo(f);
    
    return 1; 
}

/* SELECIONA CELULAR DO CLIENTE DE ACORDO COM A LINHA NA BASE DE DADOS */
int buildCelular(struct Cliente *c){
    FILE *f = NULL;
    int linha = 0;
    
    f = abreArquivo(DATABASE_CELULAR_CLIENTE, F_READ);
    if(!f) return 0;
    
    while(!feof(f)){
       char aux[10] = { 0 };
       
       linha++;
       fgets(aux, 10, f);

       if(linha == c->linha){
                aux[8] = 0;
                strcpy(c->celular, aux);
                
                if(fclose(f)){
                   printf("Problemas ao fechar o arquivo de dados\n<ENTER>");
                   getchar();
                }
                
                return 1;
       }
    }
    
    fechaArquivo(f);
    
    return 1; 
}

/* SELECIONA EMAIL DO CLIENTE DE ACORDO COM A LINHA NA BASE DE DADOS */
int buildEmail(struct Cliente *c){
    FILE *f = NULL;
    int linha = 0;
    
    f = abreArquivo(DATABASE_EMAIL_CLIENTE, F_READ);
    if(!f) return 0;
    
    while(!feof(f)){
       char aux[500] = { 0 };
       
       linha++;
       fgets(aux, 499, f);

       if(linha == c->linha){
                int ultChar = strlen(aux) - 1;
                
                if(aux[ultChar] == 10) aux[ultChar] = 0;
                
                strcpy(c->email, aux);
                
                if(fclose(f)){
                   printf("Problemas ao fechar o arquivo de dados\n<ENTER>");
                   getchar();
                }
                
                return 1;
       }
    }
    
    fechaArquivo(f);
    
    return 1; 
}

/* SELECIONA SALDO DO CLIENTE DE ACORDO COM A LINHA NA BASE DE DADOS */
int buildSaldo(struct Cliente *c){
    FILE *f = NULL;
    int linha = 0;
    
    f = abreArquivo(DATABASE_SALDO_CLIENTE, F_READ);
    if(!f) return 0;
    
    while(!feof(f)){
       float aux = 0;
       
       linha++;
       fscanf(f, "%f", &aux);
       if(linha == c->linha){
                
                c->saldo = aux;
                
                if(fclose(f)){
                   printf("Problemas ao fechar o arquivo de dados\n<ENTER>");
                   getchar();
                }
                
                return 1;
       }
    }
    
    fechaArquivo(f);
    
    return 1; 
}

/* SELECIONA PRAZO DE VENCIMENTO DO CLIENTE DE ACORDO COM A LINHA NA BASE DE DADOS */
int buildVencimento(struct Cliente *c){
    FILE *f = NULL;
    int linha = 0;
    
    f = abreArquivo(DATABASE_VENCIMENTO_CLIENTE, F_READ);
    if(!f) return 0;
    
    while(!feof(f)){
       int d, m, a;
       d = m = a = 0;
       
       linha++;
       fscanf(f, "%d%d%d", &d, &m, &a);
       if(linha == c->linha){
                
                c->vencimento.dia = d;
                c->vencimento.mes = m;
                c->vencimento.ano = a;
                
                if(fclose(f)){
                   printf("Problemas ao fechar o arquivo de dados\n<ENTER>");
                   getchar();
                }
                
                return 1;
       }
    }
    
    fechaArquivo(f);
    
    return 1; 
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
        printf("ERRO: erro ao procurar nome(ID: %d)\n<ENTER>", c->id);
        getchar();
        return 0;
     }
     
     if(!buildTelefone(c)){
        printf("ERRO: erro ao procurar telefone\n<ENTER>");
        getchar();
        return 0;
     }
     
     if(!buildCelular(c)){
        printf("ERRO: erro ao procurar celular\n<ENTER>");
        getchar();
        return 0;
     }
     
     if(!buildEmail(c)){
        printf("ERRO: erro ao procurar Email\n<ENTER>");
        getchar();
        return 0;
     }
     
     if(!buildSaldo(c)){
        printf("ERRO: erro ao procurar saldo\n<ENTER>");
        getchar();
        return 0;
     }
     
     if(!buildVencimento(c)){
        printf("ERRO: erro ao procurar data de pagamento\n<ENTER>");
        getchar();
        return 0;
     }
     
     return 1;
}

/* EXIBE TODOS OS DADOS DE TODOS OS CLIENTES */
void exibirTodosDados(){
     FILE *f = NULL;
     
     f = fopen(DATABASE_ID_CLIENTE, F_READ);
     if(!f){
        printf("ERRO: Ocorreu um erro ao tentar ler a base de dados\n<ENTER>");
        getchar();
        return ;
     }
     
     while(!feof(f)){
        struct Cliente c;
        int id;
        fscanf(f, "%d", &id);
        
        if(buildClienteById(&c, id))
           printCliente(&c);
     }
     
     if(fclose(f)){
        printf("ERRO: Problemas ao fechar o arquivo de dados\n<ENTER>");
        getchar();
     }
}

/* PESQUISA UM NOME E RETORNA UM ARRAY ID DE OCORRENCIAS */
void buscarNome(struct ResultadoPesquisa *r, char *pesquisa){
    FILE *f = NULL;
    int linha = 0;
    
    f = abreArquivo(DATABASE_NOME_CLIENTE, F_READ);
    if(!f) return ;
    
    initResultadoPesquisa(r);
    
    while(!feof(f)){
       char aux[500] = { 0 };
       
       linha++;
       fgets(aux, 499, f);
       
       if( strstr(strupr(aux), pesquisa) ){
          
          r->qtd++;
          if(r->qtd == 1){ //primeiro resultado da busca
             r->resultado = (int*) malloc(sizeof(int));
             r->resultado[0] = qualId(linha);
             if(!r->resultado[0]){ //ERRO GRAVE: NAO EXISTE ID PARA ESSA LINHA
                printf("ERRO: ID nao existe para sua referencia\n");
                free(r->resultado);
                r->resultado = NULL;
                r->qtd = 0;
                return ;
             }
          }
          
          else{
             r->resultado = (int*) realloc(r->resultado, r->qtd * sizeof(int));
             r->resultado[r->qtd - 1] = qualId(linha);
             
             if(!r->resultado[r->qtd-1]){ //ERRO GRAVE: NAO EXISTE ID PARA ESSA LINHA
                printf("ERRO: ID nao existe para sua referencia\n");
                free(r->resultado);
                r->resultado = NULL;
                r->qtd = 0;
                return ;
             }
          }
       }
    }
}

/* BUSCA CLIENTES COM SALDO MAIOR( MODO = 1 ) OU MENOR( MODO = 2 ) QUE UM VALOR */
void buscarSaldo(struct ResultadoPesquisa *r, float valor, int modo){
     FILE *f = NULL;
     int linha = 0;
     
     f = abreArquivo(DATABASE_SALDO_CLIENTE, F_READ);
     if(!f) return ;
     
     initResultadoPesquisa(r);
     
     while(!feof(f)){
        int cond = 0;
        float aux;
        
        linha++;
        fscanf(f, "%f", &aux);
        
        if(modo == MAIOR_QUE){
                if(aux > valor) cond = 1;
        }
        else{
           if(aux < valor) cond = 1;
        }
        
        if(cond){
           
           r->qtd++;
           if(r->qtd == 1){ //primeiro resultado da busca
              r->resultado = (int*) malloc(sizeof(int));
              r->resultado[0] = qualId(linha);
              if(!r->resultado[0]){ //ERRO GRAVE: NAO EXISTE ID PARA ESSA LINHA
                 printf("ERRO: ID nao existe para sua referencia\n");
                 free(r->resultado);
                 r->resultado = NULL;
                 r->qtd = 0;
                 return ;
              }
           }
           
           else{
              r->resultado = (int*) realloc(r->resultado, r->qtd * sizeof(int));
              r->resultado[r->qtd - 1] = qualId(linha);
              
              if(!r->resultado[r->qtd-1]){ //ERRO GRAVE: NAO EXISTE ID PARA ESSA LINHA
                 printf("ERRO: ID nao existe para sua referencia\n");
                 free(r->resultado);
                 r->resultado = NULL;
                 r->qtd = 0;
                 return ;
              }
           }
        }
     }
}

void pesquisarPrazoAte(struct ResultadoPesquisa *r, struct Data pesquisa){
     FILE *f = NULL;
     int linha = 0;
     
     f = abreArquivo(DATABASE_VENCIMENTO_CLIENTE, F_READ);
     if(!f) return ;
     
     initResultadoPesquisa(r);
     
     while(!feof(f)){
        struct Data d;
        
        linha++;
        fscanf(f, "%d %d %d", &d.dia, &d.mes, &d.ano);
                
        if((d.ano <= pesquisa.ano) && (d.mes <= pesquisa.mes))
           if((d.mes < pesquisa.mes) || (d.dia < pesquisa.dia)){
               r->qtd++;
               if(r->qtd == 1){ //primeiro resultado da busca
                  r->resultado = (int*) malloc(sizeof(int));
                  r->resultado[0] = qualId(linha);
                  if(!r->resultado[0]){ //ERRO GRAVE: NAO EXISTE ID PARA ESSA LINHA
                     printf("ERRO: ID nao existe para sua referencia\n");
                     free(r->resultado);
                     r->resultado = NULL;
                     r->qtd = 0;
                     return ;
                  }
               }
               
               else{
                  r->resultado = (int*) realloc(r->resultado, r->qtd * sizeof(int));
                  r->resultado[r->qtd - 1] = qualId(linha);
                  
                  if(!r->resultado[r->qtd-1]){ //ERRO GRAVE: NAO EXISTE ID PARA ESSA LINHA
                     printf("ERRO: ID nao existe para sua referencia\n");
                     free(r->resultado);
                     r->resultado = NULL;
                     r->qtd = 0;
                     return ;
                  }
               }
           }
     }
}
