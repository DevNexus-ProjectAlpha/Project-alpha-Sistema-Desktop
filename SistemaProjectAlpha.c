#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define TF_REG 100 // TF de Registros. Quando sistema estiver finalizado, melhorias futuras: vetor dinâmico e gravação em arquivos.
#define TF_MOV 1000 // TF de movimentações (TF é tamanho físico do vetor).

// Entidades do Gerenciar (Serve como auxiliar de verificação Ex: case).
enum { // define em sequência depois usa pra chamar a posição.
	ENT_ALUNO,          // ENT_ALUNO vale 0 é a posição de "Alunos" no nomesEntidades. o altera() usa pra reaproveitar o menu.
	ENT_PROFISSIONAL,   // 1
	ENT_TREINO,         // 2
	ENT_PLANO,          // 3
	ENT_MATRICULA,      // 4
	ENT_AVALIACAO,      // 5
	ENT_AGENDA,         // 6
	ENT_EQUIPAMENTO,    // 7
	ENT_PACOTE,         // 8
	ENT_ATIVIDADE       // 9
};

// Nomes das entidades, na mesma ordem do enum.
char nomesEntidades[10][31] = {
	"Alunos",
	"Profissionais",
	"Treinos",
	"Planos",
	"Matrículas",
	"Avaliações Físicas",
	"Agenda de Profissionais",
	"Equipamentos",
	"Pacote de Modalidades",
	"Atividades"
};

// Structs:
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
	float porcentagemDesconto;
	int   ativo;
} Plano;

typedef struct {
	int   codigo;
	char  nome[61];
	int   qtdAtivPacote;     // diferente do contador global.
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
	int   situacao;     // 1 = ativa, 2 = cancelada.
} Matricula;

typedef struct {
	int   codigo;
	int   codMatricula;
	int   mesRef;
	int   anoRef;
	float valor;
	Data  vencimento;
	Data  dataPagamento;
	int   situacao;   // 1 = pendente, 2 = paga, 3 = cancelada.
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
	int diaSemana;      
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
	int   situacao;   // 1 = bom, 2 = manutenção, 3 = inativo
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
	int   ativo;
} Gasto;

// OBS: São variáveis globais, se preferir mudar avisar no grupo e seguir as regras do relatorio.		
// Cada struct tem um vetor de TF e um contador de registros. 
// Struct    Vetores:			     	//  Contadores:				 // Está separado: Struct -> vetor[TF] -> contador.
Aluno        alunos[TF_REG];       		int qtdAlunos = 0;           // <-- qtd é quantidade, no caso é o contador de Alunos cadastrados.     
Profissional profissionais[TF_REG];     int qtdProfissionais = 0;	 
Atividade    atividades[TF_REG];   		int qtdAtividades = 0;
Plano        planos[TF_REG];       		int qtdPlanos = 0;
Pacote       pacotes[TF_REG];      		int qtdPacotes = 0;
Matricula    matriculas[TF_MOV];        int qtdMatriculas = 0;
Cobranca     cobrancas[TF_MOV];     	int qtdCobrancas = 0;
Cancelamento cancelamentos[TF_MOV];  	int qtdCancelamentos = 0;
Treino       treinos[TF_MOV];      		int qtdTreinos = 0;
Avaliacao    avaliacoes[TF_MOV];    	int qtdAvaliacoes = 0;
Agenda       agendas[TF_MOV];       	int qtdAgendas = 0;
Equipamento  equipamentos[TF_REG];  	int qtdEquipamentos = 0;
Presenca     presencas[TF_MOV];         int qtdPresencas = 0;
Gasto        gastos[TF_MOV];       		int qtdGastos = 0;

// Protótipos
int  menu(void);
void altera(int codEntidade);
void cadastrar(void);
void Gerenciar(void);
void frequencia(void);
void financeiro(void);
void operacoes(void);
void relatorios(void);

// Menu principal
int menu(void){
	int Opcoes;
	system("cls");
		printf("==============================================\n");
		printf("           ACADEMIA - PROJECT ALPHA\n");
		printf("           Sistema de Gerenciamento\n");
		printf("==============================================\n");
		printf("  MENU PRINCIPAL\n");
		printf("\n");
		printf("  [1]  Cadastros\n");
		printf("       Alunos, planos, pacotes e equipamentos\n");
		printf("  [2]  Gerenciar\n");
		printf("       Alterar, excluir e listar registros\n");
		printf("  [3]  Operações\n");
		printf("       Matrícula, recebimento e cancelamento\n");
		printf("  [4]  Controle de Frequência\n");
		printf("       Chamada e histórico de presença\n");
		printf("  [5]  Controle Financeiro\n");
		printf("       Faturamento, gastos e pagamentos\n");
		printf("  [6]  Relatórios\n");
		printf("       Consultas com filtros\n");
		printf("  [0]  Encerrar sistema\n");        // <--- OBS: encerrar padronizado com 0 em todos menus ok.
		printf("----------------------------------------------\n");
		printf("  Opção: ");
		Opcoes = 0;
		scanf("%d", &Opcoes);
		fflush(stdin);
		return Opcoes;
}

// Funções de Alteração
void alterarAluno() {
	int OpcoesAlteracao, matricula, i, achou;

	do {
		system("cls");
		printf("==============================================\n");
		printf("  ALTERAR - Alunos\n");
		printf("==============================================\n\n");
		printf("  [1]  Matrícula\n");
		printf("  [2]  Nome\n");
		printf("  [3]  CPF\n");
		printf("  [4]  Data Nascimento\n");
		printf("  [5]  Telefone\n");
		printf("  [6]  E-mail\n");
		printf("  [7]  Ativo\n");
		printf("  [0]  Voltar\n\n");
		printf("----------------------------------------------\n");
		printf("  Opção: ");
		scanf("%d", &OpcoesAlteracao);
		
		if(OpcoesAlteracao != 0) {
			printf("\nInfome a Matrícula do Aluno: ");
			scanf("%d", &matricula);
			
			achou = -1;
			for (i = 0; i < qtdAlunos && achou == -1; i++) {
				if (alunos[i].codigo == matricula) achou = i;
			}
			if(achou == -1) printf("\nAluno Não Encontrado!\n");
			else {
				fflush(stdin);
				switch(OpcoesAlteracao) {
					case 1:
						printf("Informe a Nova Matrícula: ");
						scanf("%d", &alunos[achou].codigo);
						break;
					case 2:
						printf("Informe o Novo Nome: ");
						fgets(alunos[achou].nome, 61, stdin);
						break;
					case 3:
						printf("Informe o Novo CPF: ");
						fgets(alunos[achou].cpf, 12, stdin);
						break;
					case 4:
						printf("Informe a Nova Data de Nascimento (DD MM AAAA): ");
						scanf("%d%d%d", &alunos[achou].dataNascimento.dia, &alunos[achou].dataNascimento.mes, &alunos[achou].dataNascimento.ano);
						break;
					case 5:
						printf("Informe o Novo Telefone: ");
						fgets(alunos[achou].telefone, 16, stdin);
						break;
					case 6:
						printf("Informe o Novo E-mail: ");
						fgets(alunos[achou].email, 61, stdin);
						break;
					case 7:
						printf("Informe a Situação (1- Ativo / 0- Inativo): ");
						scanf("%d", &alunos[achou].ativo);
						break;
					default:
						printf("\nOpção Inválida!");
						break;
				}
				printf("\nAlteração Realizada com Sucesso!\n");
			}
		}
		system("pause");
	} while(OpcoesAlteracao != 0);
}

void alterarProfissional() {
	int OpcoesAlteracao, matricula, i, achou;

	do {
		system("cls");
		printf("==============================================\n");
		printf("  ALTERAR - Profissionais\n");
		printf("==============================================\n\n");
		printf("  [1]  Matrícula\n");
		printf("  [2]  Nome\n");
		printf("  [3]  CPF\n");
		printf("  [4]  Telefone\n");
		printf("  [5]  Função\n");
		printf("  [6]  Ativo\n");
		printf("  [0]  Voltar\n\n");
		printf("----------------------------------------------\n");
		printf("  Opção: ");
		scanf("%d", &OpcoesAlteracao);

		if(OpcoesAlteracao != 0) {
			printf("\nInforme a Matrícula do Profissional: ");
			scanf("%d", &matricula);
	
			achou = -1;
			for (i = 0; i < qtdProfissionais && achou == -1; i++) {
				if (profissionais[i].codigo == matricula) achou = i;
			}
			if (achou == -1) printf("\nProfissional Não Encontrado!\n");
			else {
				fflush(stdin);
				switch (OpcoesAlteracao) {
					case 1:
						printf("Informe a Nova Matrícula: ");
						scanf("%d", &profissionais[achou].codigo);
						break;
					case 2:
						printf("Informe o Novo Nome: ");
						fgets(profissionais[achou].nome, 61, stdin);
						break;
					case 3:
						printf("Informe o Novo CPF: ");
						fgets(profissionais[achou].cpf, 12, stdin);
						break;
					case 4:
						printf("Informe o Novo Telefone: ");
						fgets(profissionais[achou].telefone, 16, stdin);
						break;
					case 5:
						printf("Informe a Nova Função: ");
						fgets(profissionais[achou].funcao, 31, stdin);
						break;
					case 6:
						printf("Informe a Situação (1- Ativo / 0- Inativo): ");
						scanf("%d", &profissionais[achou].ativo);
						break;
					default:
						printf("\nOpção Inválida!\n");
						break;
				}
				printf("\nAlteração Realizada com Sucesso!\n");
			}
		}
		system("pause");
	} while (OpcoesAlteracao != 0);
}

// Funções de Exclusão
void excluirAluno() {
	int matricula, achou, i;
	
	system("cls");
	printf("==============================================\n");
	printf("  EXCLUIR - Alunos\n");
	printf("==============================================\n\n");
	printf("Informe a Matrícula do Aluno: ");
	scanf("%d", &matricula);
	
	achou = -1;
	for (i = 0; i < qtdAlunos && achou == -1; i++) {
		if (alunos[i].codigo == matricula && alunos[i].ativo == 1) {
			achou = i;
			alunos[i].ativo = 0;
			printf("\nAluno Excluido com Sucesso!\n");
		}
	}
	if (achou == -1) printf("\nAluno Não Encontrado!\n");
	system("pause");
}

void excluirProfissional() {
	int matricula, achou, i;
	
	system("cls");
	printf("==============================================\n");
	printf("  EXCLUIR - Profissionais\n");
	printf("==============================================\n\n");
	printf("Informe a Matrícula do Profissional: ");
	scanf("%d", &matricula);
	
	achou = -1;
	for (i = 0; i < qtdProfissionais && achou == -1; i++) {
		if (profissionais[i].codigo == matricula && profissionais[i].ativo == 1) {
			achou = i;
			profissionais[i].ativo = 0;
			printf("\nProfissional Excluido com Sucesso!\n");
		}
	}
	if (achou == -1) printf("\nProfissional Não Encontrado!\n");
	system("pause");
}

// Funções de Listagem
void listarAlunos() {
	int i;
	
	system("cls");
	printf("==============================================\n");
	printf("  LISTAGEM - Alunos\n");
	printf("==============================================\n\n");
	
	for(i = 0; i < qtdAlunos; i++) {
		if(alunos[i].ativo) {
			printf("Matrícula: %d\n", alunos[i].codigo);
			printf("Nome: %s\n", alunos[i].nome);
			printf("CPF: %s\n", alunos[i].cpf);
			printf("Data de Nascimento: %d/%d/%d\n", alunos[i].dataNascimento.dia, alunos[i].dataNascimento.mes, alunos[i].dataNascimento.ano);
			printf("Telefone: %s\n", alunos[i].telefone);
			printf("E-mail: %s\n", alunos[i].email);
			printf("==============================================\n");
		}
	}
	system("pause");
}

void listarProfissionais() {
	int i;
	
	system("cls");
	printf("==============================================\n");
	printf("  LISTAGEM - Profissionais\n");
	printf("==============================================\n\n");
	
	for(i = 0; i < qtdProfissionais; i++) {
		if(profissionais[i].ativo) {
			printf("Matrícula: %d\n", profissionais[i].codigo);
			printf("Nome: %s\n", profissionais[i].nome);
			printf("CPF: %s\n", profissionais[i].cpf);
			printf("Telefone: %s\n", profissionais[i].telefone);
			printf("Função: %s\n", profissionais[i].funcao);
			printf("==============================================\n");
		}
	}
	system("pause");
}

void altera(int codEntidade) {
	int OpcoesAlteracao;

	do {
		system("cls");
		printf("==============================================\n");
		printf("  GERENCIAR - %s\n", nomesEntidades[codEntidade]);
		printf("==============================================\n");
		printf("\n");
		printf("  [1]  Alterar\n");
		printf("       Corrige os dados de um registro\n");
		printf("  [2]  Excluir\n");
		printf("       Desativa um registro do sistema\n");
		printf("  [3]  Relatório\n");
		printf("       Lista os registros cadastrados\n");
		printf("  [0]  Voltar ao submenu\n");
		printf("----------------------------------------------\n");
		printf("  Opção: ");
		OpcoesAlteracao = 0;
		scanf("%d", &OpcoesAlteracao);
		fflush(stdin);

		switch(OpcoesAlteracao) {
			case 1:
				// ================= ALTERAR: =================
				switch(codEntidade) {
					case ENT_ALUNO:
						if(!qtdAlunos) {
							printf("\nNenhum Aluno foi Cadastrado!\n");
							system("pause");
						} else alterarAluno();
						break;
					case ENT_PROFISSIONAL:
						if(!qtdProfissionais) {
							printf("\nNenhum Profissional foi Cadastrado!\n");
							system("pause");
						} else alterarProfissional();
						break;
					case ENT_TREINO:
						// alterarTreino();
						break;
					case ENT_PLANO:
						// alterarPlano();
						break;
					case ENT_MATRICULA:
						// alterarMatricula();
						break;
					case ENT_AVALIACAO:
						// alterarAvaliacao();
						break;
					case ENT_AGENDA:
						// alterarAgenda();
						break;
					case ENT_EQUIPAMENTO:
						// alterarEquipamento();
						break;
					case ENT_PACOTE:
						// alterarPacote();
						break;
					case ENT_ATIVIDADE:
						// alterarAtividade();
						break;
				}
				break;

			case 2:
				// ========== EXCLUIR: ============ (OBS: Matrícula não é excluída: chama o cancelamento)
				switch(codEntidade) {
					case ENT_ALUNO:
						if(!qtdAlunos) {
							printf("\nNenhum Aluno foi Cadastrado!\n");
							system("pause");
						} else excluirAluno();
						break;
					case ENT_PROFISSIONAL:
						if(!qtdProfissionais){
							printf("\nNenhum Profissional foi Cadastrado!\n");
							system("pause");
						}else excluirProfissional();
						break;
					case ENT_TREINO:
						// excluirTreino();
						break;
					case ENT_PLANO:
						// excluirPlano();
						break;
					case ENT_MATRICULA:
						// efetuarCancelamento();
						break;
					case ENT_AVALIACAO:
						// excluirAvaliacao();
						break;
					case ENT_AGENDA:
						// excluirAgenda();
						break;
					case ENT_EQUIPAMENTO:
						// excluirEquipamento();
						break;
					case ENT_PACOTE:
						// excluirPacote();
						break;
					case ENT_ATIVIDADE:
						// excluirAtividade();
						break;
				}
				break;

			case 3:
				// =========== RELATÓRIO / LISTAR: =============
				switch(codEntidade) {
					case ENT_ALUNO:
						if(!qtdAlunos) {
							printf("\nNenhum Aluno foi Cadastrado!\n");
							system("pause");
						}else listarAlunos();
						break;
					case ENT_PROFISSIONAL:
						if(!qtdProfissionais) {
							printf("\nNenhum Profissional foi Cadastrado!\n");
							system("pause");
						}else listarProfissionais();
						break;
					case ENT_TREINO:
						// listarTreinos();
						break;
					case ENT_PLANO:
						// listarPlanos();
						break;
					case ENT_MATRICULA:
						// listarMatriculas();
						break;
					case ENT_AVALIACAO:
						// listarAvaliacoes();
						break;
					case ENT_AGENDA:
						// listarAgenda();
						break;
					case ENT_EQUIPAMENTO:
						// listarEquipamentos();
						break;
					case ENT_PACOTE:
						// listarPacotes();
						break;
					case ENT_ATIVIDADE:
						// listarAtividades();
						break;
				}
				break;

			case 0:
				printf("\n Voltando...\n");
				getch();
				break;

			default:
				printf("\n Opção Inválida!\n");
				getch();
				break;
		}
	} while(OpcoesAlteracao != 0);
}

//1 - FUNÇÃO DE CADASTRO
void cadastrar(void) {
	int OpcoesCadastro;

	do {
		system("cls");
		printf("==============================================\n");
		printf("                  CADASTROS\n");
		printf("==============================================\n");
		printf("  PESSOAS\n");
		printf("  [1]  Alunos\n");
		printf("  [2]  Profissionais\n");
		printf("\n");
		printf("  SERVIÇOS E ACOMPANHAMENTO\n");
		printf("  [3]  Treinos\n");
		printf("  [4]  Planos\n");
		printf("  [5]  Avaliações físicas\n");
		printf("  [6]  Pacotes de modalidades\n");
		printf("  [7]  Agenda de profissionais\n");
		printf("\n");
		printf("  ESTRUTURA E ATIVIDADES\n");
		printf("  [8]  Equipamentos\n");
		printf("  [9]  Atividades\n");
		printf("\n");
		printf("  [0]  Voltar ao menu principal\n");
		printf("----------------------------------------------\n");
		printf("  Opção: ");
		OpcoesCadastro = 0;
		scanf("%d", &OpcoesCadastro);
		fflush(stdin);

		switch(OpcoesCadastro) {
			case 1:
				// Alunos
				printf("Informe a Matrícula do Aluno: ");
				scanf("%d", &alunos[qtdAlunos].codigo);
				
				fflush(stdin);
				printf("Informe o Nome do Aluno: ");
				fgets(alunos[qtdAlunos].nome, 61, stdin);
				
				fflush(stdin);
				printf("Informe o CPF do Aluno: ");
				fgets(alunos[qtdAlunos].cpf, 12, stdin);
				
				printf("Informe a Data de Nascimento (20 12 2000): ");
				scanf("%d%d%d", &alunos[qtdAlunos].dataNascimento.dia, &alunos[qtdAlunos].dataNascimento.mes, &alunos[qtdAlunos].dataNascimento.ano);
				
				fflush(stdin);
				printf("Informe o Número de Telefone: ");
				fgets(alunos[qtdAlunos].telefone, 21, stdin);
				
				fflush(stdin);
				printf("Informe um E-mail de Contato: ");
				fgets(alunos[qtdAlunos].email, 61, stdin);
				
				alunos[qtdAlunos].ativo = 1;
				
				qtdAlunos++;
				break;
			case 2:
				// Profissionais
				printf("Informe a Matrícula do Profissional: ");
				scanf("%d", &profissionais[qtdProfissionais].codigo);
				
				fflush(stdin);
				printf("Informe o Nome do Profissional: ");
				fgets(profissionais[qtdProfissionais].nome, 61, stdin);
				
				fflush(stdin);
				printf("Informe o CPF do Profissional: ");
				fgets(profissionais[qtdProfissionais].cpf, 12, stdin);
				
				fflush(stdin);
				printf("Informe o Telefone do Profissional: ");
				fgets(profissionais[qtdProfissionais].telefone, 21, stdin);
				
				fflush(stdin);
				printf("Informe a Função do Profissional: ");
				fgets(profissionais[qtdProfissionais].funcao, 31, stdin);
				
				profissionais[qtdProfissionais].ativo = 1;
				
				qtdProfissionais++;
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
				//Atividades
				break;

			case 0:
				printf("\n Voltando...");
				getch();
				break;

			default:
				printf("\n Opção Inválida!");
				getch();
				break;
		}
		system("pause");
	} while(OpcoesCadastro != 0);
}

//2 - FUNÇÃO DE GERENCIAR
void Gerenciar(void) {
	int OpcoesGerenciar;

	do {
		system("cls");
		printf("==============================================\n");
		printf("                  GERENCIAR\n");
		printf("==============================================\n");
		printf("  Escolha o cadastro a ser gerenciado\n");
		printf("\n");
		printf("  PESSOAS\n");
		printf("  [1]  Alunos\n");
		printf("  [2]  Profissionais\n");
		printf("\n");
		printf("  REGISTROS\n");
		printf("  [3]  Treinos\n");
		printf("  [4]  Planos\n");
		printf("  [5]  Matrículas\n");
		printf("  [6]  Avaliações físicas\n");
		printf("  [7]  Agenda de profissionais\n");
		printf("  [8]  Equipamentos\n");
		printf("  [9]  Pacotes de modalidades\n");
		printf("  [10] Atividades\n");
		printf("\n");
		printf("  [0]  Voltar ao menu principal\n");
		printf("----------------------------------------------\n");
		printf("  Opção: ");
		OpcoesGerenciar = 0;
		scanf("%d", &OpcoesGerenciar);
		fflush(stdin);

		switch(OpcoesGerenciar)	{
			case 1:
				//Alunos
				altera(ENT_ALUNO);
				break;

		//===========================================================*/

			case 2:
			  	//Profissionais
			  	altera(ENT_PROFISSIONAL);
		   		break;

		//===========================================================*/

			case 3:
			   	//Treinos
			   	altera(ENT_TREINO);
				break;

		//===========================================================*/

			case 4:
			   	//Planos
			   	altera(ENT_PLANO);
				break;

		//===========================================================*/
		    case 5:
		    	//Matrículas
		    	altera(ENT_MATRICULA);
		        break;

		//===========================================================*/

			case 6:
			   	//Avaliações Físicas
			   	altera(ENT_AVALIACAO);
				break;

		//===========================================================*/

			case 7:
			   	//Agenda de Profissionais
			   	altera(ENT_AGENDA);
				break;

		//===========================================================*/

			case 8:
			   	//Equipamentos
			   	altera(ENT_EQUIPAMENTO);
				break;

		//===========================================================*/
			case 9:
			   	//Pacote de Modalidades
			   	altera(ENT_PACOTE);
				break;


		//===========================================================*/
			case 10:
				//Atividades
				altera(ENT_ATIVIDADE);
				break;

		//===========================================================*/

			case 0:
			  	printf("\n Voltando...");
				getch();
			   	break;

		//===========================================================*/

			default:
				printf("\n Opção Inválida!");
				getch();
				break;
		}
	} while(OpcoesGerenciar != 0);
}

//3 - FUNÇÃO LISTAMENTO DE FREQUÊNCIA
void frequencia(void) {
	int Opcoes;

	do {
		system("cls");
		printf("==============================================\n");
		printf("            CONTROLE DE FREQUÊNCIA\n");
		printf("==============================================\n");
		printf("\n");
		printf("  [1]  Listar frequência\n");
		printf("       Consulta as presenças dos alunos\n");
		printf("  [2]  Fazer chamada\n");
		printf("       Registra a presença do dia\n");
		printf("  [3]  Alterar frequência\n");
		printf("       Corrige uma presença registrada\n");
		printf("  [0]  Voltar ao menu principal\n");
		printf("----------------------------------------------\n");
		printf("  Opção: ");
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
			case 0:
				printf("\n Voltando...");
				getch();
				break;

			default:
				printf("\n Opção Inválida!");
				getch();
				break;
		}
	} while(Opcoes != 0);
}

//4 - FUNÇÃO DE CONTROLE FINANCEIRO
void financeiro(void) {
	int Opcoes;

	do {
		system("cls");
		printf("==============================================\n");
		printf("             CONTROLE FINANCEIRO\n");
		printf("==============================================\n");
		printf("\n");
		printf("  [1]  Faturamento mensal\n");
		printf("       Total recebido em cada mês\n");
		printf("  [2]  Faturamento por plano\n");
		printf("       Receita separada por plano\n");
		printf("  [3]  Gastos mensais\n");
		printf("       Despesas lançadas no mês\n");
		printf("  [4]  Pagamentos por aluno\n");
		printf("       Histórico de pagamentos do aluno\n");
		printf("  [5]  Cadastrar gastos\n");
		printf("       Lança uma despesa da academia\n");
		printf("  [0]  Voltar ao menu principal\n");
		printf("----------------------------------------------\n");
		printf("  Opção: ");
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
				//Cadastrar gastos
				break;

			case 0:
				printf("\n Voltando...");
				getch();
				break;

			default:
				printf("\n Opção Inválida!");
				getch();
				break;
		}
	} while(Opcoes != 0);
}

//5 - FUNÇÃO DE OPERAÇÕES
void operacoes(void) {
	int Opcoes;

	do {
		system("cls");
		printf("==============================================\n");
		printf("                  OPERAÇÕES\n");
		printf("==============================================\n");
		printf("\n");
		printf("  [1]  Efetuar matrícula\n");
		printf("       Vincula aluno, plano e pacote\n");
		printf("  [2]  Efetuar recebimento\n");
		printf("       Dá baixa e gera o boleto seguinte\n");
		printf("  [3]  Efetuar cancelamento\n");
		printf("       Calcula a retenção e a devolução\n");
		printf("  [0]  Voltar ao menu principal\n");
		printf("----------------------------------------------\n");
		printf("  Opção: ");
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
			case 0:
				printf("\n Voltando...");
				getch();
				break;

			default:
				printf("\n Opção Inválida!");
				getch();
				break;
		}
	} while(Opcoes != 0);
}

//6 - FUNÇÃO DE RELATÓRIOS
void relatorios(void) {
	int Opcoes;

	do {
		system("cls");
		printf("==============================================\n");
		printf("                  RELATÓRIOS\n");
		printf("==============================================\n");
		printf("\n");
		printf("  [1]  Extrato de pagamentos\n");
		printf("       Por período, aluno e situação\n");
		printf("  [2]  Alunos por plano\n");
		printf("       Por plano e situação da matrícula\n");
		printf("  [3]  Alunos inadimplentes\n");
		printf("       Por mês de referência e plano\n");
		printf("  [4]  Matrículas por situação\n");
		printf("       Por situação, período e plano\n");
		printf("  [5]  Faturamento por plano e mês\n");
		printf("       Por ano e plano\n");
		printf("  [6]  Frequência\n");
		printf("       Por aluno, período e atividade\n");
		printf("  [0]  Voltar ao menu principal\n");
		printf("----------------------------------------------\n");
		printf("  Opção: ");
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
				break;
			case 6:
				break;
			case 0:
				printf("\n Voltando...");
				getch();
				break;

			default:
				printf("\n Opção Inválida!");
				getch();
				break;
		}
	} while(Opcoes != 0);
}

int main() {
	setlocale(LC_ALL, "Portuguese");
	int Opcoes;

	do {
		Opcoes = menu();
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
				//Operações
				operacoes();
				break;

		//===========================================================*/

			case 4:
				//Controle de Frequência
				frequencia();
				break;

		//===========================================================*/

			case 5:
				//Controle Financeiro
				financeiro();
				break;

		//===========================================================*/

			case 6:
				//Relatórios
				relatorios();
				break;

		//===========================================================*/

			case 0:
				printf("\n SISTEMA ENCERRADO! ");
				break;

		//===========================================================*/

			default:
				printf("\n Opção Inválida!\n");
				getch();
				break;
		}
	} while(Opcoes != 0);

	return 0;
}
