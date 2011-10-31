#define F_READ "r"
#define F_WRITE "w"
#define F_READ_WRITE "r+"

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
       char telefone[9];
       char celular[9];
       float saldo;
       struct Data vencimento;
       int linha;
};

extern void fechaArquivo(FILE *f);

extern FILE* abreArquivo(char *caminho, char *modo);

extern void initCliente(struct Cliente *c);

extern int buildNome(struct Cliente *c);

extern int buildId(struct Cliente *c, int id);

extern int buildClienteById(struct Cliente *c, int id);

extern void exibirTodosDados();

extern void printCliente(struct Cliente *c);
