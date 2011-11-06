#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "pgc.h"

/* LE UM NUMERO E O RETORNA */
float leNumero(){
    float n;
    scanf("%f", &n);
    fflush(stdin);
    return n;
}

/* SETA A DATA PARA O DIA ATUAL */
void hojeIs(struct Data *h){
     time_t now = time(NULL);
     struct tm *t = localtime(&now);

     h->dia = t->tm_mday;
     h->mes = t->tm_mon+1;
     h->ano = t->tm_year+1900;
}

/* POSICIONA O CURSOR(PONTEIRO DE FILE) NO FINAL DO ARQUIVO */
void goToEOF(FILE *f){
     while(!feof(f)){
        char buffer[51] = { 0 };
        fgets(buffer, 50, f);
     }
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

/* APAGA ARQUIVO TEMPORARIO USADO NAS MANIPULAÇÕES DE DADOS */
void apagaArqTemporario(){
     char comando[] = "del database\\temp.pgc";
     system(comando);
}

/* ATRIBUI UM NOME AO CLIENTE */
void setNomeCliente(struct Cliente *c){
    do{
       printf("Nome: ");
       gets(c->nome);
    }while(strlen(c->nome) < 1);
}

/* ATRIBUI UM TELEFONE AO CLIENTE */
void setTelefoneCliente(struct Cliente *c){
    do{
       printf("Telefone(8 digitos): ");
       gets(c->telefone);
    }while(strlen(c->telefone) != 8);
}

/* ATRIBUI UM CELULAR AO CLIENTE */
void setCelularCliente(struct Cliente *c){
    do{
       printf("Celular(8 digitos): ");
       gets(c->celular);
    }while(strlen(c->celular) != 8);
}

/* ATRIBUI UM EMAIL AO CLIENTE */
void setEmailCliente(struct Cliente *c){
    do{
       printf("Email: ");
       gets(c->email);
    }while(strlen(c->email) < 1);
}

/* ATRIBUI UM SALDO AO CLIENTE */
void setSaldoCliente(struct Cliente *c){
     printf("Saldo(Ex.: 40.50): ");
     c->saldo = leNumero();
}

/* ATRIBUI UM PRAZO DE VENCIMENTO AO CLIENTE */
void setVencimentoCliente(struct Cliente *c){
     printf("Prazo maximo de pagamento\n");
     printf("Dia(dd): ");
     c->vencimento.dia = leNumero();
     printf("Mes(mm): ");
     c->vencimento.mes = leNumero();
     printf("Ano(aa): ");
     c->vencimento.ano = leNumero();
}

/* LE INFORMAÇÕES BÁSICAS DE UM CLIENTE
   RETURN 0 SE OCORREU ALGUM ERRO */
void leInformacoesBasicasCliente(struct Cliente *c){
    
    setNomeCliente(c);
    setTelefoneCliente(c);
    setCelularCliente(c);
    setEmailCliente(c);
}

/* ADICIONA UMA NOVA INFORMAÇÃO(LINHA) NA BASE DE DADOS COM CAMINHO 'ARQUIVO' */
int salvarNovoDado(char *arquivo, char *dado){
    FILE *f = abreArquivo(arquivo, F_READ_WRITE);
    
    if(!f) return 0;
    
    goToEOF(f);
    fputc('\n', f);
    fputs(dado, f);
    
    fechaArquivo(f);
    return 1;
}

/* ATUALIZA O ULTIMO ID INCREMENTANDO 1 E RETORNA O NOVO ÚLTIMO ID
   RETURN 0 = ERRO */
int atualizaID(){
    FILE *controleID = abreArquivo(DATABASE_ID_CONTROL , F_READ);
    int ultID;
    
    if(!controleID){
       printf("ERRO: problemas com o arquivo de controle de IDs\n<ENTER>");
       getchar();
       return 0;
    }

    fscanf(controleID, "%d", &ultID);
    ultID++;
    fechaArquivo(controleID);
    
    controleID = abreArquivo(DATABASE_ID_CONTROL, F_WRITE);
    if(!controleID){
       printf("ERRO: problemas com o arquivo de controle de IDs\n<ENTER>");
       getchar();
       return 0;
    }
    fprintf(controleID, "%d", ultID);
    
    fechaArquivo(controleID);
    return ultID;
}

/* SALVA O CLIENTE NA BASE DE DADOS E INCREMENTA O CONTROLE DE ID */
void salvarNovoCliente(struct Cliente *c){
     FILE *f = NULL;
     int id = 0;
     char IDARRAY[5] = { 0 };
     
     id = atualizaID();
     if(!id) return ;
     
     itoa(id, IDARRAY, 10);
     
     if(!salvarNovoDado(DATABASE_ID_CLIENTE, IDARRAY)){
        printf("ERRO: problemas ao tentar salvar nome\n<ENTER>");
        getchar();
        return ;
     }
     
     if(!salvarNovoDado(DATABASE_NOME_CLIENTE, c->nome)){
        printf("ERRO: problemas ao tentar salvar nome\n<ENTER>");
        getchar();
        return ;
     }
     
     if(!salvarNovoDado(DATABASE_TELEFONE_CLIENTE, c->telefone)){
        printf("ERRO: problemas ao tentar salvar telefone\n<ENTER>");
        getchar();
        return ;
     }
     
     if(!salvarNovoDado(DATABASE_CELULAR_CLIENTE, c->celular)){
        printf("ERRO: problemas ao tentar salvar celular\n<ENTER>");
        getchar();
        return ;
     }
     
     if(!salvarNovoDado(DATABASE_EMAIL_CLIENTE, c->email)){
        printf("ERRO: problemas ao tentar salvar email\n<ENTER>");
        getchar();
        return ;
     }
     
     //Caso específico para salvar saldo ( FLOAT )
     f = abreArquivo(DATABASE_SALDO_CLIENTE, F_READ_WRITE);
     if(!f){
        printf("ERRO: problemas ao tentar salvar saldo\n<ENTER>");
        getchar();
        return ;
     }
     goToEOF(f);
     fputc('\n', f);
     fprintf(f, "%f", c->saldo);
     
     //Caso específico para salvar o vencimento ( struct DATA )
     f = abreArquivo(DATABASE_VENCIMENTO_CLIENTE, F_READ_WRITE);
     if(!f){
        printf("ERRO: problemas ao tentar salvar prazo de vencimento\n<ENTER>");
        getchar();
        return ;
     }
     goToEOF(f);
     fputc('\n', f);
     fprintf(f, "%d %d %d", c->vencimento.dia, c->vencimento.mes, c->vencimento.ano);
     
     fechaArquivo(f);
     printf("\n\nCliente salvo com sucesso\n<ENTER>");
     getchar();
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
           fechaArquivo(f);
           
           c->id = id;
           return linha;
        }
     }
     
     fechaArquivo(f);
     
     return 0;
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
                
                if(aux[ultChar] == 10)
                   aux[ultChar] = 0;
                
                strcpy(c->nome, aux);
                
                fechaArquivo(f);
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
                
                fechaArquivo(f);                
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
                
                fechaArquivo(f);                
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
                
                fechaArquivo(f);
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
                
                fechaArquivo(f);
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
                
                fechaArquivo(f);
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
     
     f = abreArquivo(DATABASE_ID_CLIENTE, F_READ);
     if(!f) return ;
     
     while(!feof(f)){
        struct Cliente c;
        int id;
        fscanf(f, "%d", &id);
        
        if(buildClienteById(&c, id))
           printCliente(&c);
     }
     
     fechaArquivo(f);
}

/* USADA PARA FINALIZAR PESQUISA QUANDO UM ERRO GRAVE ACONTECER */
void limpaResultadoPesquisa(struct ResultadoPesquisa *r){
     printf("ERRO: ID nao existe para sua referencia\n");
     
     free(r->resultado);
     r->resultado = NULL;
     r->qtd = 0;
}

/* ADICIONA UM ID AO RESULTADO DA PESQUISA 
   RETORNA 1 SE OK E 0 SE DEU ERRO      */
int adicionaIdResultadoPesquisa(struct ResultadoPesquisa *r, int linha){    
     r->qtd++;
     
     if(r->qtd == 1){ //primeiro resultado da busca
        r->resultado = (int*) malloc(sizeof(int));
        r->resultado[0] = qualId(linha);
     }
        
     else{
        r->resultado = (int*) realloc(r->resultado, r->qtd * sizeof(int));
        r->resultado[r->qtd - 1] = qualId(linha);
     }
     
     if(!r->resultado[r->qtd-1]){ //ERRO GRAVE: NAO EXISTE ID PARA ESSA LINHA
        limpaResultadoPesquisa(r);
        return 0;
     }
     return 1;
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
       
       if( strstr(strupr(aux), pesquisa) )
          if(!adicionaIdResultadoPesquisa(r, linha)){
             fechaArquivo(f);
             return ;
          }
    
    }
    fechaArquivo(f);
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
        
        if(cond)
           if(!adicionaIdResultadoPesquisa(r, linha)){
              fechaArquivo(f);
              return ;
           }
     }
     fechaArquivo(f);
}

/* BUSCA CLIENTES COM PRAZO MAXIMO DE PAGAMENTO ATE DD/MM/AA */
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
                
        if(d.ano <= pesquisa.ano)
           if(((d.mes == pesquisa.mes) && (d.dia <= pesquisa.dia)) || (d.mes < pesquisa.mes))
              if(!adicionaIdResultadoPesquisa(r, linha)){
                 fechaArquivo(f);
                 return ;
              }
     }
     fechaArquivo(f);
}

/* Copia arquivo ORIGEM para DESTINO;
   Parâmetro LINHA exclui a linha da copia;
   LINHA = 0 copia o arquivo todo sem apagar nenhuma linha */
int copiaArquivo(FILE *origem, FILE *destino, int linha){
    int sucesso = 0,
        linhaAtual = 0,
        linhasCopiadas = 0;
    
    while(!feof(origem)){
       char aux[500] = { 0 };
       linhaAtual++;
       fgets(aux, 499, origem);
       
       if(linhaAtual == linha){ //Ignora a linha que será apagada
          sucesso = 1;
          continue;
       }
       else if(linhasCopiadas) fputc(10, destino);
       
       if(aux[strlen(aux) - 1] == 10) aux[strlen(aux) - 1] = 0;
       fputs(aux, destino);
       linhasCopiadas++;
    }
    if(linha == 0) return 1;
    
    return sucesso;
}

/* APAGA UMA LINHA DE UM ARQUIVO DA BASE DE DADOS */
int apagarRegistro(char *arquivo, int linha){
     int sucesso = 0;
     FILE *f = NULL;
     FILE *temp = NULL;
     
     f = abreArquivo(arquivo, F_READ);
     temp = abreArquivo(DATABASE_TEMP, F_WRITE);
     if((!temp) || (!f)) return 0;
     
     sucesso = copiaArquivo(f, temp, linha);
     
     fechaArquivo(f);
     fechaArquivo(temp);
     
     if(!sucesso) return 0;
     
     f = abreArquivo(arquivo, F_WRITE);
     temp = abreArquivo(DATABASE_TEMP, F_READ);
     if((!temp) || (!f)) return 0;
     
     sucesso = copiaArquivo(temp, f, 0);
     
     fechaArquivo(f);
     fechaArquivo(temp);
     apagaArqTemporario();
     
     if(!sucesso) return 0;
     return 1;
}

/* APAGA UM CLIENTE DA BASE DE DADOS */
void apagarCliente(int linha){
     
     if(!apagarRegistro(DATABASE_ID_CLIENTE, linha)){
        printf("ERRO: ID nao existe\n<ENTER>");
        getchar();
     }
     
     if(!apagarRegistro(DATABASE_NOME_CLIENTE, linha)){
        printf("ERRO GRAVE: Nome sem referencia\n<ENTER>");
        getchar();
     }
     
     if(!apagarRegistro(DATABASE_TELEFONE_CLIENTE, linha)){
        printf("ERRO GRAVE: Telefone sem referencia\n<ENTER>");
        getchar();
     }
     
     if(!apagarRegistro(DATABASE_CELULAR_CLIENTE, linha)){
        printf("ERRO GRAVE: Celular sem referencia\n<ENTER>");
        getchar();
     }
     
     if(!apagarRegistro(DATABASE_EMAIL_CLIENTE, linha)){
        printf("ERRO GRAVE: Email sem referencia\n<ENTER>");
        getchar();
     }
     
     if(!apagarRegistro(DATABASE_SALDO_CLIENTE, linha)){
        printf("ERRO GRAVE: Saldo sem referencia\n<ENTER>");
        getchar();
     }
     
     if(!apagarRegistro(DATABASE_VENCIMENTO_CLIENTE, linha)){
        printf("ERRO GRAVE: Vencimento sem referencia\n<ENTER>");
        getchar();
     }
}

/* ATUALIZA A LINHA DO CLIENTE COPIANDO PARA UM ARQUIVO TEMPORARIO */
int atualizarClienteTEMP(char info[], FILE *f, int linha){
    int sucesso = 0;
    int linhaAtual = 0;
    int linhasCopiadas = 0;
    FILE *temp = abreArquivo(DATABASE_TEMP, F_WRITE);
    
    if(!temp) return 0;
    
    while(!feof(f)){
       char aux[500] = { 0 };
       linhaAtual++;
       fgets(aux, 499, f);
       
       if(linhasCopiadas) fputc(10, temp);
       
       if(linhaAtual == linha){ //Aqui atualiza a informação
          fputs(info, temp);
          sucesso = 1;
          linhasCopiadas++;
          continue;
       }
       else if(aux[strlen(aux) - 1] == 10){
            aux[strlen(aux) - 1] = 0;
            fputs(aux, temp);
       }
       
       linhasCopiadas++;
    }
    
    fechaArquivo(f);
    fechaArquivo(temp);
    
    return sucesso;
}

/* ATUALIZA SALDO (CASO ESPECIFICO POR SE TRATAR DE UM FLOAT) */
void atualizaSaldo(struct Cliente *c){
     int linhaAtual = 0;
     int linhasCopiadas = 0;
     FILE *temp = abreArquivo(DATABASE_TEMP, F_WRITE);
     FILE *f = abreArquivo(DATABASE_SALDO_CLIENTE, F_READ);
     
     if((!f) || (!temp)) return ;
     
     while(!feof(f)){
       char aux[500] = { 0 };
       linhaAtual++;
       fgets(aux, 499, f);
       
       if(linhasCopiadas) fputc(10, temp);
       
       if(linhaAtual == c->linha){ //Aqui atualiza a informação
          fprintf(f, "%f", c->saldo);
          linhasCopiadas++;
          continue;
       }
       else if(aux[strlen(aux) - 1] == 10){
            aux[strlen(aux) - 1] = 0;
            fputs(aux, temp);
       }
       
       linhasCopiadas++;
    }
    fechaArquivo(f);
    fechaArquivo(temp);
    
    f = abreArquivo(DATABASE_SALDO_CLIENTE, F_WRITE);
    temp = abreArquivo(DATABASE_TEMP, F_READ);
     
    if((!f) || (!temp)){
       printf("ERRO: erro ao tentar abrir *f e *temp para repor a atualizacao\n<ENTER>");
       getchar();
       return ;
    }
    
    copiaArquivo(temp, f, 0);
    
    fechaArquivo(f);
    fechaArquivo(temp);
    apagaArqTemporario();
}

/* ATUALIZA VENCIMENTO (CASO ESPECIFICO POR SE TRATAR DE UMA STRUCT DATA) */
void atualizaVencimento(struct Cliente *c){
     int linhaAtual = 0;
     int linhasCopiadas = 0;
     FILE *temp = abreArquivo(DATABASE_TEMP, F_WRITE);
     FILE *f = abreArquivo(DATABASE_VENCIMENTO_CLIENTE, F_READ);
     
     if((!f) || (!temp)) return ;
     
     while(!feof(f)){
       char aux[500] = { 0 };
       linhaAtual++;
       fgets(aux, 499, f);
       
       if(linhasCopiadas) fputc(10, temp);
       
       if(linhaAtual == c->linha){ //Aqui atualiza a informação
          fprintf(f, "%d %d %d", c->vencimento.dia, c->vencimento.mes, c->vencimento.ano);
          linhasCopiadas++;
          continue;
       }
       else if(aux[strlen(aux) - 1] == 10){
            aux[strlen(aux) - 1] = 0;
            fputs(aux, temp);
       }
       
       linhasCopiadas++;
    }
    fechaArquivo(f);
    fechaArquivo(temp);
    
    f = abreArquivo(DATABASE_VENCIMENTO_CLIENTE, F_WRITE);
    temp = abreArquivo(DATABASE_TEMP, F_READ);
     
    if((!f) || (!temp)){
       printf("ERRO: erro ao tentar abrir *f e *temp para repor a atualizacao\n<ENTER>");
       getchar();
       return ;
    }
    
    copiaArquivo(temp, f, 0);
    
    fechaArquivo(f);
    fechaArquivo(temp);
    apagaArqTemporario();
}

/* ALTERA INFORMACOES DO CLIENTE DE ACORDO COM A OPÇÃO E ATUALIZA NA BASE DE DADOS */
void alterarInformacoesCliente(struct Cliente *c, int op){
     char *caminho, informacao[500] = { 0 };
     FILE *f = NULL;
     FILE *temp = NULL;
     
     if(op == 1){
        caminho = (char*) malloc(strlen(DATABASE_NOME_CLIENTE) * sizeof(char) + 1);
        memset(caminho, 0, strlen(DATABASE_NOME_CLIENTE) * sizeof(char) + 1);
        setNomeCliente(c);
        strcpy(caminho, DATABASE_NOME_CLIENTE);
        strcpy(informacao, c->nome);
     }
     
     if(op == 2){
        caminho = (char*) malloc(strlen(DATABASE_TELEFONE_CLIENTE) * sizeof(char) + 1);
        memset(caminho, 0, strlen(DATABASE_TELEFONE_CLIENTE) * sizeof(char) + 1);
        setTelefoneCliente(c);
        strcpy(caminho, DATABASE_TELEFONE_CLIENTE);
        strcpy(informacao, c->telefone);
     }
     
     if(op == 3){
        caminho = (char*) malloc(strlen(DATABASE_CELULAR_CLIENTE) * sizeof(char) + 1);
        memset(caminho, 0, strlen(DATABASE_CELULAR_CLIENTE) * sizeof(char) + 1);
        setCelularCliente(c);
        strcpy(caminho, DATABASE_CELULAR_CLIENTE);
        strcpy(informacao, c->celular);
     }
     
     if(op == 4){
        caminho = (char*) malloc(strlen(DATABASE_EMAIL_CLIENTE) * sizeof(char) + 1);
        memset(caminho, 0, strlen(DATABASE_EMAIL_CLIENTE) * sizeof(char) + 1);
        setEmailCliente(c);
        strcpy(caminho, DATABASE_EMAIL_CLIENTE);
        strcpy(informacao, c->email);
     }
     
     if(op == 5){ //Caso específico para saldo
        setSaldoCliente(c);
        atualizaSaldo(c);
        return ;
     }
     
     if(op == 6){ //Caso específico para data de vencimento
        setVencimentoCliente(c);
        atualizaVencimento(c);
        return ;
     }
     
     f = abreArquivo(caminho, F_READ);
     if(!f) return ;
     
     if(!atualizarClienteTEMP(informacao, f, c->linha)){
        printf("ERRO: erro ao tentar atualizar cliente para arquivo temporario\n<ENTER>");
        getchar();
        return ;
     }
     
     f = abreArquivo(caminho, F_WRITE);
     temp = abreArquivo(DATABASE_TEMP, F_READ);
     
     if((!f) || (!temp)){
        printf("ERRO: erro ao tentar abrir *f e *temp para repor a atualizacao\n<ENTER>");
        getchar();
        return ;
     }
     
     copiaArquivo(temp, f, 0);
     
     fechaArquivo(f);
     fechaArquivo(temp);
     free(caminho);
     apagaArqTemporario();
}
