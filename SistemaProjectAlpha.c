#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define TF 100

// Structs
typedef struct {
	int dia;
	int mes;
	int ano;
} Data;

typedef struct {
	int  codigo;
	char nome[61];
	char cpf[12];
	Data dataNascimento;
	char telefone[16];
	char email[61];
	int  ativo;
} Aluno;

typedef struct {
	int  codigo;
	char nome[61];
	char cpf[12];
	char telefone[16];
	char funcao[31];
	int  ativo;
} Profissional;

typedef struct {
	int   codigo;
	char  nome[61];
	char  descricao[101];
	int   duracaoMin;
	int   capacidade;
	float valorAula;
	int   ativo;
} Atividade;

typedef struct {
	int   codigo;
	char  nome[31];
	int   meses;
	float valorMensal;
	float descontoPercent;
	int   ativo;
} Plano;

typedef struct {
	int   codigo;
	char  nome[61];
	int   qtdAtividades;
	int   codAtividades[5];
	float valor;
	int   ativo;
} Pacote;

typedef struct {
	int   codigo;
	int   codAluno;
	int   codPlano;
	int   codPacote;
	Data  dataInicio;
	Data  dataTermino;
	float valorTotal;
	int   situacao;          // 1 = ativa, 2 = cancelada
} Matricula;

typedef struct {
	int   codigo;
	int   codMatricula;
	int   mesRef;
	int   anoRef;
	float valor;
	Data  vencimento;
	Data  dataPagamento;
	int   situacao;          // 1 = pendente, 2 = paga, 3 = cancelada
} Cobranca;

typedef struct {
	int   codigo;
	int   codMatricula;
	Data  data;
	float valorPago;
	float valorDevolvido;
	float valorRetido;
} Cancelamento;

typedef struct {
	int  codigo;
	int  codAluno;
	int  codProfissional;
	char nome[31];
	char objetivo[61];
	char exercicios[10][41];
	Data dataCriacao;
	int  ativo;
} Treino;

typedef struct {
	int   codigo;
	int   codAluno;
	int   codProfissional;
	Data  data;
	float peso;
	float altura;
	float imc;
	float percGordura;
	char  observacoes[101];
	int   ativo;
} Avaliacao;

typedef struct {
	int codigo;
	int codProfissional;
	int codAtividade;
	int diaSemana;           // 1 a 7
	int horaInicio;
	int horaFim;
	int ativo;
} Agenda;

typedef struct {
	int   codigo;
	char  nome[61];
	char  tipo[31];
	int   quantidade;
	Data  dataAquisicao;
	float valor;
	int   situacao;          // 1 = bom, 2 = manutenção, 3 = inativo
	Data  dataUltimaManutencao;
	int   ativo;
} Equipamento;

typedef struct {
	int  codigo;
	int  codAluno;
	int  codAtividade;
	Data data;
	int  presente;
} Presenca;

typedef struct {
	int   codigo;
	char  descricao[61];
	char  categoria[31];
	float valor;
	Data  data;
} Gasto;

// Vetores e contadores
Aluno        alunos[TF];         int qtdAlunos = 0;
Profissional profissionais[TF];  int qtdProfissionais = 0;
Atividade    atividades[TF];     int qtdAtividades = 0;
Plano        planos[TF];         int qtdPlanos = 0;
Pacote       pacotes[TF];        int qtdPacotes = 0;
Matricula    matriculas[TF];     int qtdMatriculas = 0;
Cobranca     cobrancas[TF];      int qtdCobrancas = 0;
Cancelamento cancelamentos[TF];  int qtdCancelamentos = 0;
Treino       treinos[TF];        int qtdTreinos = 0;
Avaliacao    avaliacoes[TF];     int qtdAvaliacoes = 0;
Agenda       agendas[TF];        int qtdAgendas = 0;
Equipamento  equipamentos[TF];   int qtdEquipamentos = 0;
Presenca     presencas[TF];      int qtdPresencas = 0;
Gasto        gastos[TF];         int qtdGastos = 0;

// Protótipos
int  menu(void);
void altera(const char *textMensage);
void cadastrar(void);
void Gerenciar(void);
void frequencia(void);
void financeiro(void);

// Menu principal
int menu(void){
	int Opcoes;
	system("cls");
		printf("|========================================|\n");
		printf("|                                        |\n");
		printf("|        ESCOLHA AS OPÇÕES ABAIXO:       |\n");
		printf("|                                        |\n");
		printf("|========================================|\n\n");

		printf(" [1] - Cadastrar: \n");
		printf(" [2] - Gerenciar: \n");
		printf(" [3] - Controle de Frequência: \n");
		printf(" [4] - Controle Financeiro: \n");
		printf(" [5] - Encerrar Sistema: \n");
		printf("\n Opção Selecionada: ");
		Opcoes = 0;
		scanf("%d", &Opcoes);
		fflush(stdin);
		return Opcoes;
}

void altera(const char *textMensage) {
	int OpcoesAlteracao;

	do {
		system("cls");
		printf("|========================================|\n");
		printf("|                                        |\n");
		printf("|               GERENCIAR                |\n");
		printf("|                                        |\n");
		printf("|========================================|\n");

		printf("\n [1] - Alterar %s: \n", textMensage);
		printf(" [2] - Deletar %s: \n", textMensage);
		printf(" [3] - Relatório de %s: \n", textMensage);
		printf(" [4] - Voltar ao Sub Menu: \n");
		printf("\n Opção Selecionada: ");
		OpcoesAlteracao = 0;
		scanf("%d", &OpcoesAlteracao);
		fflush(stdin);

		switch(OpcoesAlteracao) {
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				printf("\n Voltando...");
				getch();
				break;

			default:
				printf("\n Opção Inválida!\n");
				getch();
				break;
		}
	} while(OpcoesAlteracao != 4);
}

//1 - FUNÇÃO DE CADASTRO
void cadastrar(void) {
	int OpcoesCadastro;

	do {
		system("cls");
		printf("|========================================|\n");
		printf("|                                        |\n");
		printf("|               CADASTRO                 |\n");
		printf("|                                        |\n");
		printf("|========================================|\n");

		printf(" [1] - Cadastro Aluno: \n");
		printf(" [2] - Cadastro Profissionais: \n\n");
		printf(" [3] - Cadastro de Treinos: \n");
		printf(" [4] - Cadastro de Planos: \n");
		printf(" [5] - Cadastro de Avaliações Físicas: \n");
		printf(" [6] - Cadastro de Pacote de Modalidades: \n\n");
		printf(" [7] - Cadastro de Agendamento: \n");
		printf(" [8] - Cadastro de Equipamentos: \n\n");
		printf(" [9] - Voltar ao Menu Inicial: \n");
		printf("\n Opção Selecionada: ");
		OpcoesCadastro = 0;
		scanf("%d", &OpcoesCadastro);
		fflush(stdin);

		switch(OpcoesCadastro) {
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				break;
			case 7:
				break;
			case 8:
				break;
			case 9:
				printf("\n Voltando...");
				getch();
				break;

			default:
				printf("\n Opção Inválida!");
				getch();
				break;
		}
	} while(OpcoesCadastro != 9);
}

//2 - FUNÇÃO DE GERENCIAR
void Gerenciar(void) {
	int OpcoesGerenciar;

	do {
		system("cls");
		printf("|========================================|\n");
		printf("|                                        |\n");
		printf("|              GERENCIAR                 |\n");
		printf("|                                        |\n");
		printf("|========================================|\n");

		printf("\n [1] - Gerenciar Alunos: \n");
		printf(" [2] - Gerenciar Profissionais: \n\n");
		printf(" [3] - Gerenciar Treinos: \n");
		printf(" [4] - Gerenciar Planos: \n");
		printf(" [5] - Gerenciar Matrículas: \n");
		printf(" [6] - Gerenciar Avaliações Físicas: \n\n");
		printf(" [7] - Gerenciar Agenda de Profissionais: \n");
		printf(" [8] - Gerenciar Equipamentos: \n");
		printf(" [9] - Gerenciar Pacote de Modalidades: \n\n");
		printf(" [10] - Voltar ao Menu Inicial: \n");
		printf("\n Opção Selecionada: ");
		OpcoesGerenciar = 0;
		scanf("%d", &OpcoesGerenciar);
		fflush(stdin);

		switch(OpcoesGerenciar)	{
			case 1:
				//Alunos
				altera("Alunos");
				break;

		//===========================================================*/

			case 2:
			  	//Profissionais
			  	altera("Profissionais");
		   		break;

		//===========================================================*/

			case 3:
			   	//Treinos
			   	altera("Treinos");
				break;

		//===========================================================*/

			case 4:
			   	//Planos
			   	altera("Planos");
				break;

		//===========================================================*/
		    case 5:
		    	//Matrículas
		    	altera("Matrículas");
		        break;

		//===========================================================*/

			case 6:
			   	//Avaliações Físicas
			   	altera("Avaliações Físicas");
				break;

		//===========================================================*/

			case 7:
			   	//Agenda de Profissionais
			   	altera("Agenda de Profissionais");
				break;

		//===========================================================*/

			case 8:
			   	//Equipamentos
			   	altera("Equipamentos");
				break;

		//===========================================================*/
			case 9:
			   	//Pacote de Modalidades
			   	altera("Pacote de Modalidades");
				break;


		//===========================================================*/
			case 10:
			  	printf("\n Voltando...");
				getch();
			   	break;

		//===========================================================*/

			default:
				printf("\n Opção Inválida!");
				getch();
				break;
		}
	} while(OpcoesGerenciar != 10);
}

//3 - FUNÇÃO LISTAMENTO DE FREQUÊNCIA
void frequencia(void) {
	int Opcoes;

	do {
		system("cls");
		printf("|========================================|\n");
		printf("|                                        |\n");
		printf("|        ESCOLHA AS OPÇÕES ABAIXO:       |\n");
		printf("|                                        |\n");
		printf("|========================================|\n\n");

		printf(" [1] - Listar Frequência de Alunos: \n");
		printf(" [2] - Fazer Chamada de Alunos: \n");
		printf(" [3] - Alterar Frequência de Alunos: \n");
		printf(" [4] - Voltar ao Menu Inicial: \n");
		printf("\n Opção Selecionada: ");
		Opcoes = 0;
		scanf("%d", &Opcoes);
		fflush(stdin);

		switch(Opcoes) {
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				printf("\n Voltando...");
				getch();
				break;

			default:
				printf("\n Opção Inválida!");
				getch();
				break;
		}
	} while(Opcoes != 4);
}

//4 - FUNÇÃO DE CONTROLE FINANCEIRO
void financeiro(void) {
	int Opcoes;

	do {
		system("cls");
		printf("|========================================|\n");
		printf("|                                        |\n");
		printf("|        ESCOLHA AS OPÇÕES ABAIXO:       |\n");
		printf("|                                        |\n");
		printf("|========================================|\n\n");

		printf(" [1] - Listar Faturamento Mensal: \n");
		printf(" [2] - Listar Faturamento por Planos: \n");
		printf(" [3] - Listar Gastos Mensais: \n");
		printf(" [4] - Pagamentos por Aluno: \n");
		printf(" [5] - Voltar ao Menu Inicial: \n");
		printf("\n Opção Selecionada: ");
		Opcoes = 0;
		scanf("%d", &Opcoes);
		fflush(stdin);

		switch(Opcoes) {
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				printf("\n Voltando...");
				getch();
				break;

			default:
				printf("\n Opção Inválida!");
				getch();
				break;
		}
	} while(Opcoes != 5);
}

int main() {
	setlocale(LC_ALL, "Portuguese");
	int Opcoes;

   	do{
    	Opcoes=menu();
		switch(Opcoes) {
			case 1:
		    	//Cadastro
				cadastrar();
			    break;

		//===========================================================*/

			case 2:
				//Gerenciar
				Gerenciar();
				break;

		//===========================================================*/

			case 3:
				//Controle de Frequência
				frequencia();
				break;

		//===========================================================*/

			case 4:
				//Controle Financeiro
				financeiro();
				break;

		//===========================================================*/

			case 5:
				printf("\n SISTEMA ENCERRADO! ");
				break;

		//===========================================================*/

			default:
				printf("\n Opção Inválida!\n");
				getch();
				break;
     	}
   	}while(Opcoes!=5);

   	return 0;
}
