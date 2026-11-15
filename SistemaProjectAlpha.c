#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>

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
	char cpf[16];
	Data dataNascimento;
	char telefone[16];
	char email[61];
	int  ativo;
} Aluno;

typedef struct {
	int  codigo;
	char nome[61];
	char cpf[15];
	char telefone[17];
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
	int  quantidade;
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
	float vTotal;
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
Aluno        alunos;
Profissional profissionais;
Atividade    atividades;
Plano        planos;
Pacote       pacotes;
Matricula    matriculas;
Cobranca     cobrancas;
Cancelamento cancelamentos;
Treino       treinos;
Avaliacao    avaliacoes;
Agenda       agendas;
Equipamento  equipamentos;
Presenca     presencas;
Gasto        gastos;

// Protótipos
int  menu(void);
void altera(int codEntidade);
void cadastrar(void);
void Gerenciar(void);
void frequencia(void);
void financeiro(void);
void operacoes(void);
void relatorios(void);
void alterarAvaliacao(void);
void excluirAvaliacao(void);
void listarAvaliacoes(void);
int existeAlunoAvaliacao(int codigo);
int existeProfissionalAvaliacao(int codigo);
int existeAtividade(int codigo);
void cadastrarAtividade(void);
void alterarAtividade(void);
void excluirAtividade(void);
void listarAtividades(void);
void cadastrarPlano(void);
float calcularValorPlano(Plano p);
void alterarPlano(void);
void excluirPlano(void);
void listarPlanos(void);
int  buscarPlano(int codigo);
int  existePlano(int codigo);
int  planoTemMatricula(int codPlano);
int  escolherTipoPlano(Plano *p);
int  tipoPlanoRepetido(FILE *arq, char nome[], int codIgnorar);
void mostrarPlano(Plano p);
void validarCodigo(int cod);
void cadastrarTreino(void);
void alterarTreino(void);
void excluirTreino(void);
void listarTreinos(void);
void cadastrarEquipamento(void);
void alterarEquipamento(void);
void excluirEquipamento(void);
void listarEquipamentos(void);
void relatorioAlfabetico(void);
void efetuarMatricula(void);
void efetuarRecebimento(void);
void efetuarCancelamento(void);
void listarMatriculas(void);
void avisoMatricula(void);
int  buscarPacote(int codigo);
int  buscarMatricula(int codigo);
int  alunoTemMatriculaAtiva(int codAluno);
void gerarCobranca(int codMatricula, float valor, Data vencimento);
int  proximoCodigo(char arquivo[], int tamanho);
void fazerChamada(void);
void listarFrequencia(void);
void alterarFrequencia(void);
void faturamentoMensal(void);
void faturamentoPorPlano(void);
void gastosMensais(void);
void pagamentosPorAluno(void);
void cadastrarGasto(void);
void cadastrarPacote(void);
void alterarPacote(void);
void excluirPacote(void);
void listarPacotes(void);
void cadastrarAgenda(void);
void alterarAgenda(void);
void excluirAgenda(void);
void listarAgenda(void);
void extratoPagamentos(void);
void alunosPorPlano(void);
void alunosInadimplentes(void);
void matriculasPorSituacao(void);
void faturamentoPlanoMes(void);
int  nomeDaAtividade(int codigo, char destino[]);
int  atividadeEmPacoteAtivo(int codAtividade);
int  cpfDuplicado(char cpf[], int codEnt, int codigo);

// Função de Busca
int busca(FILE *arq, int cod, int codEnt) {
	rewind(arq);
	
	Aluno tempAluno;
	Profissional tempProfissional;
	Avaliacao tempAvaliacao;
	Atividade tempAtividade;
	Plano tempPlano;
	Treino tempTreino;
	Equipamento tempEquipamento;
	Matricula tempMatricula;
	Pacote tempPacote;
	Agenda tempAgenda;
	
	switch(codEnt) {
		case ENT_ALUNO:
			fread(&tempAluno, sizeof(Aluno), 1, arq);
			while(!feof(arq) && tempAluno.codigo != cod) {
				fread(&tempAluno, sizeof(Aluno), 1, arq);
			}
			if(!feof(arq)) return (ftell(arq) - sizeof(Aluno));
			else return -1;   
			break;
		case ENT_PROFISSIONAL:
			fread(&tempProfissional, sizeof(Profissional), 1, arq);
			while(!feof(arq) && tempProfissional.codigo != cod) {
				fread(&tempProfissional, sizeof(Profissional), 1, arq);
			}
			if(!feof(arq)) return (ftell(arq) - sizeof(Profissional));
			else return -1;   
			break;
			case ENT_AVALIACAO:
            while(fread(&tempAvaliacao, sizeof(Avaliacao), 1, arq) == 1)
                {
                    if(tempAvaliacao.codigo == cod)
                        return (int)(ftell(arq) - sizeof(Avaliacao));
                }
                return -1;
            break;
		case ENT_ATIVIDADE:
			while(fread(&tempAtividade, sizeof(Atividade), 1, arq) == 1) {
				if(tempAtividade.codigo == cod) return (int)(ftell(arq) - sizeof(Atividade));
			}
			return -1;
			break;
		case ENT_PLANO:
			while(fread(&tempPlano, sizeof(Plano), 1, arq) == 1) {
				if(tempPlano.codigo == cod) return (int)(ftell(arq) - sizeof(Plano));
			}
			return -1;
			break;
		case ENT_TREINO:
			while(fread(&tempTreino, sizeof(Treino), 1, arq) == 1) {
				if(tempTreino.codigo == cod) return (int)(ftell(arq) - sizeof(Treino));
			}
			return -1;
			break;
		case ENT_EQUIPAMENTO:
			while(fread(&tempEquipamento, sizeof(Equipamento), 1, arq) == 1) {
				if(tempEquipamento.codigo == cod) return (int)(ftell(arq) - sizeof(Equipamento));
			}
			return -1;
			break;
		case ENT_MATRICULA:
			while(fread(&tempMatricula, sizeof(Matricula), 1, arq) == 1) {
				if(tempMatricula.codigo == cod) return (int)(ftell(arq) - sizeof(Matricula));
			}
			return -1;
			break;
		case ENT_PACOTE:
			while(fread(&tempPacote, sizeof(Pacote), 1, arq) == 1) {
				if(tempPacote.codigo == cod) return (int)(ftell(arq) - sizeof(Pacote));
			}
			return -1;
			break;
		case ENT_AGENDA:
			while(fread(&tempAgenda, sizeof(Agenda), 1, arq) == 1) {
				if(tempAgenda.codigo == cod) return (int)(ftell(arq) - sizeof(Agenda));
			}
			return -1;
			break;
	}
	return -1;
}

int existeAlunoAvaliacao(int codigo) {
	FILE *arq;
	Aluno temp;
	int pos, achou = 0;

	arq = fopen("alunos.bin", "rb");
	if(arq == NULL) return 0;

	pos = busca(arq, codigo, ENT_ALUNO);
	if(pos != -1) {
		fseek(arq, pos, 0);
		fread(&temp, sizeof(Aluno), 1, arq);
		if(temp.ativo == 1) achou = 1;
	}
	fclose(arq);
	return achou;
}

int existeProfissionalAvaliacao(int codigo) {
	FILE *arq;
	Profissional temp;
	int pos, achou = 0;

	arq = fopen("profissionais.bin", "rb");
	if(arq == NULL) return 0;

	pos = busca(arq, codigo, ENT_PROFISSIONAL);
	if(pos != -1) {
		fseek(arq, pos, 0);
		fread(&temp, sizeof(Profissional), 1, arq);
		if(temp.ativo == 1) achou = 1;
	}
	fclose(arq);
	return achou;
}

int nomeDoAluno(int codigo, char destino[]) {
	FILE *arq;
	Aluno temp;
	int pos, achou = 0;

	arq = fopen("alunos.bin", "rb");
	if(arq == NULL) return 0;

	pos = busca(arq, codigo, ENT_ALUNO);
	if(pos != -1) {
		fseek(arq, pos, 0);
		fread(&temp, sizeof(Aluno), 1, arq);
		strcpy(destino, temp.nome);
		achou = 1;
	}
	fclose(arq);
	return achou;
}

int nomeDoProfissional(int codigo, char destino[]) {
	FILE *arq;
	Profissional temp;
	int pos, achou = 0;

	arq = fopen("profissionais.bin", "rb");
	if(arq == NULL) return 0;

	pos = busca(arq, codigo, ENT_PROFISSIONAL);
	if(pos != -1) {
		fseek(arq, pos, 0);
		fread(&temp, sizeof(Profissional), 1, arq);
		strcpy(destino, temp.nome);
		achou = 1;
	}
	fclose(arq);
	return achou;
}

int temProfissionalAtivo(void) 
{
    FILE *arq;
    Profissional temp;
    int encontrou = 0;

    arq = fopen("profissionais.bin", "rb");

    if(arq == NULL)
        return 0;

    while(fread(&temp, sizeof(Profissional), 1, arq) == 1) {
        if(temp.ativo == 1)
            encontrou = 1;
    }

    fclose(arq);

    return encontrou;
}

int temAlunoAtivo(void) {
    FILE *arq;
    Aluno temp;
    int encontrou = 0;

    arq = fopen("alunos.bin", "rb");

    if(arq == NULL)
        return 0;

    while(fread(&temp, sizeof(Aluno), 1, arq) == 1) {
        if(temp.ativo == 1)
            encontrou = 1;
    }

    fclose(arq);

    return encontrou;
}
// Funções de Validação
int validaExcedeu(char texto[]) {
	fflush(stdin);
    if(texto[strlen(texto)-1] != '\n') return 1;
	else {
		texto[strlen(texto)-1] = '\0';
		return 0;
	} 
}

void validarCodigo(int cod) {
	if(cod < 1 || cod > 1000) {
		printf("O Código Deve Estar Entre 1 e 1000!\n\n");
	}
}

int validaCPF(char cpf[]) {
	int i;
	if(strlen(cpf) == 14) {
		if(cpf[3] == '.' && cpf[7] == '.' && cpf[11] == '-') {
			for(i = 0; i < 14; i++) {
				if(i != 3 && i != 7 && i != 11) {
					if(!isdigit(cpf[i])) return 0;
				}
			}
			return 1;
		} else return 0;
		
	}
	return 0;
}

int validaTelefone(char tel[]) {
	int i;
    if(strlen(tel) == 14) { // Formato: (XX) XXXX-XXXX
        if(tel[0] == '(' && tel[3] == ')' && tel[4] == ' ' && tel[9] == '-') {
            for(i = 0; i < 14; i++) {
                if(i != 0 && i != 3 && i != 4 && i != 9) {
                    if(!isdigit(tel[i])) return 0;
                }
            }
            return 1;
        }
    } else if(strlen(tel) == 15) { // Formato: (XX) XXXXX-XXXX
        if(tel[0] == '(' && tel[3] == ')' && tel[4] == ' ' && tel[10] == '-') {
            for(i = 0; i < 15; i++) {
                if(i != 0 && i != 3 && i != 4 && i != 10) {
                    if(!isdigit(tel[i])) return 0;
                }
            }
            return 1;
        }
    }
    return 0;
}

int validaData(Data d) {
	if (d.ano < 1900 || d.ano > 2100) return 0; // Valida o Ano
    if (d.mes < 1 || d.mes > 12) return 0; // Valida o Mês

    // Valida os Dias Baseado no Mês
    if (d.mes == 1 || d.mes == 3 || d.mes == 5 || d.mes == 7 || d.mes == 8 || d.mes == 10 || d.mes == 12) {
        if (d.dia < 1 || d.dia > 31) return 0;
    } else if (d.mes == 4 || d.mes == 6 || d.mes == 9 || d.mes == 11) {
        if (d.dia < 1 || d.dia > 30) return 0;
    } else if (d.mes == 2) {
        // Validação Ano Bissexto Fevereiro
        int bissexto = 0;
        if ((d.ano % 4 == 0 && d.ano % 100 != 0) || (d.ano % 400 == 0)) bissexto = 1;

        if (bissexto && (d.dia < 1 || d.dia > 29)) return 0;
        if (!bissexto && (d.dia < 1 || d.dia > 28)) return 0;
    }

    return 1;
}

int validaEmail(char email[]) {
	int i, contArroba = 0, contPonto = 0;
    
    for (i = 0; i < strlen(email); i++) {
        char c = email[i];

        if (c == '@') {
            contArroba++;
            if (contArroba > 1) return 0;
        } else if (contArroba == 0) { // Antes do @
            if (!isalnum(c) && c != '-' && c != '.') return 0;
        } else { // Depois do @
            if (c == '.') contPonto++;
            else if (!isalnum(c)) return 0;
        }
    }

    if (contArroba == 1 && contPonto >= 1) return 1;
    return 0;
}

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
void alterarAluno(void) {
	FILE *arq;
	int OpcoesAlteracao, matricula, i, pos, tempPos;

	do {
		system("cls");
		fflush(stdin);
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
			arq = fopen("alunos.bin", "rb+");
			if(arq == NULL) printf("Erro na Abertura do Arquivo!\n\n");
			else {
				printf("\nInfome a Matrícula do Aluno: ");
				scanf("%d", &matricula);
				
				pos = busca(arq, matricula, ENT_ALUNO);
				if(pos != -1) {
					fseek(arq, pos, 0);
					fread(&alunos, sizeof(Aluno), 1, arq);
					fseek(arq, pos, 0);
					
					fflush(stdin);
					switch(OpcoesAlteracao) {
						case 1:
							do {
								do {
									printf("Informe a Matrícula do Aluno: ");
									scanf("%d", &alunos.codigo);
									if(alunos.codigo < 1 || alunos.codigo > 1000) printf("A Matrícula Deve Estar Entre 1 e 1000!\n\n");
								} while(alunos.codigo < 1 || alunos.codigo > 1000);
								
								tempPos = busca(arq, alunos.codigo, ENT_ALUNO);
								if(tempPos != -1) printf("Matrícula Já Cadastrada!\n\n");
							} while(tempPos != -1);
							fwrite(&alunos, sizeof(Aluno), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						case 2:
							printf("Informe o Novo Nome: ");
							fgets(alunos.nome, 61, stdin);
							if(validaExcedeu(alunos.nome) == 1) printf("Somente os Primeiros 60 Caracteres Serão Cadastrados!\n\n");
							fwrite(&alunos, sizeof(Aluno), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						case 3:
							do {
								printf("Informe o Novo CPF: ");
								fgets(alunos.cpf, 15, stdin);
								alunos.cpf[15] = '\0';
								
								if(validaCPF(alunos.cpf) == 0) printf("Formato de CPF Inválido! \nInsira este Formato XXX.XXX.XXX-XX \nPreencha no Lugar do X Somente Números!\n\n");
							} while(validaCPF(alunos.cpf) != 1 || cpfDuplicado(alunos.cpf, ENT_ALUNO, alunos.codigo));
							fwrite(&alunos, sizeof(Aluno), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						case 4:
							do {
								printf("Informe a Nova Data de Nascimento (DD MM AAAA): ");
								scanf("%d%d%d", &alunos.dataNascimento.dia, &alunos.dataNascimento.mes, &alunos.dataNascimento.ano);
								
								if(validaData(alunos.dataNascimento) == 0) printf("Insira Uma Data Valida! \nAno Entre 1900 e 2100 \nMês Entre 1 e 12 \nE os Dias Válidos de Cada Mês\n\n");
							} while(validaData(alunos.dataNascimento) != 1);
							fwrite(&alunos, sizeof(Aluno), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						case 5:
							do {
								printf("Informe o Novo Telefone: ");
								fgets(alunos.telefone, 17, stdin);
								alunos.telefone[strlen(alunos.telefone)-1] = '\0';
								
								if(validaTelefone(alunos.telefone) == 0) printf("Formato de Telefone Inválido! \nInsira este Formato (XX) XXXX-XXXX ou (XX) XXXXX-XXXX \nPreencha no Lugar do X Somente Números!\n\n");
							} while(validaTelefone(alunos.telefone) != 1);
							fwrite(&alunos, sizeof(Aluno), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						case 6:
							do {
								printf("Informe o Novo E-mail: ");
								fgets(alunos.email, 61, stdin);
								if(validaExcedeu(alunos.email) == 1) printf("Somente os Primeiros 60 Caracteres Serão Cadastrados!\n\n");
								
								if(validaEmail(alunos.email) == 0) printf("Email Deve conter um @ e um . depois Dele \nAntes do @ só pode Conter - ou _ \nDepois do @ Apenas .\n\n");
							} while(validaEmail(alunos.email) != 1);
							fwrite(&alunos, sizeof(Aluno), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						case 7:
							do {
								printf("Informe a Situação (1- Ativo / 0- Inativo): ");
								scanf("%d", &alunos.ativo);
								
								if(alunos.ativo != 1 && alunos.ativo != 0) printf("Informe Uma Situação Válida!\n\n");
							} while(alunos.ativo != 1 && alunos.ativo != 0);
							fwrite(&alunos, sizeof(Aluno), 1, arq);
							printf("\nAlteração Realizada com Sucesso!\n");
							break;
						default:
							printf("\nOpção Inválida!");
							break;
					}
				} else printf("Aluno não Encontrado!\n\n");
			}
			fclose(arq);
		}
		system("pause");
	} while(OpcoesAlteracao != 0);
}

void alterarProfissional(void) {
	FILE *arq;
	int OpcoesAlteracao, matricula, i, pos;

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
			arq = fopen("profissionais.bin", "rb+");
			if(arq == NULL) printf("Erro na Abertura do Arquivo!\n\n");
			else {
				printf("\nInforme a Matrícula do Profissional: ");
				scanf("%d", &matricula);
		
				pos = busca(arq, matricula, ENT_PROFISSIONAL);
				if(pos != -1) {
					fseek(arq, pos, 0);
					fread(&profissionais, sizeof(Profissional), 1, arq);
					fseek(arq, pos, 0);
					
					fflush(stdin);
					switch (OpcoesAlteracao) {
						case 1:
							do {
								do {
									printf("Informe a Matrícula do Profissional: ");
									scanf("%d", &profissionais.codigo);
									if(profissionais.codigo < 1 || profissionais.codigo > 1000) printf("A Matrícula Deve Estar Entre 1 e 1000!\n\n");
								} while(profissionais.codigo < 1 || profissionais.codigo > 1000);
								
								pos = busca(arq, profissionais.codigo, ENT_PROFISSIONAL);
								if(pos != -1) printf("Matrícula Já Cadastrada!\n\n");
							} while(pos != -1);
							fwrite(&profissionais, sizeof(Profissional), 1, arq);
							printf("\nAlteração Realizada com Sucesso!\n");
							break;
						case 2:
							printf("Informe o Novo Nome: ");
							fgets(profissionais.nome, 61, stdin);
							if(validaExcedeu(profissionais.nome) == 1) printf("Somente os Primeiros 60 Caracteres Serão Cadastrados!\n\n");
							fwrite(&profissionais, sizeof(Profissional), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						case 3:
							do {
								printf("Informe o Novo CPF: ");
								fgets(profissionais.cpf, 15, stdin);
								profissionais.cpf[15] = '\0';
								
								if(validaCPF(profissionais.cpf) == 0) printf("Formato de CPF Inválido! \nInsira este Formato XXX.XXX.XXX-XX \nPreencha no Lugar do X Somente Números!\n\n");
							} while(validaCPF(profissionais.cpf) != 1 || cpfDuplicado(profissionais.cpf, ENT_PROFISSIONAL, profissionais.codigo));
							fwrite(&profissionais, sizeof(Profissional), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						case 4:
							do {
								printf("Informe o Novo Telefone: ");
								fgets(profissionais.telefone, 17, stdin);
								profissionais.telefone[strlen(profissionais.telefone)-1] = '\0'; // Substitui \n por \0
								
								if(validaTelefone(profissionais.telefone) == 0) printf("Formato de Telefone Inválido! \nInsira este Formato (XX) XXXX-XXXX ou (XX) XXXXX-XXXX \nPreencha no Lugar do X Somente Números!\n\n");
							} while(validaTelefone(profissionais.telefone) != 1);
							fwrite(&profissionais, sizeof(Profissional), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						case 5:
							printf("Informe a Nova Função: ");
							fgets(profissionais.funcao, 31, stdin);
							if(validaExcedeu(profissionais.funcao) == 1) printf("Somente os Primeiros 30 Caracteres Serão Cadastrados!\n\n");
							fwrite(&profissionais, sizeof(Profissional), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						case 6:
							do {
								printf("Informe a Situação (1- Ativo / 0- Inativo): ");
							scanf("%d", &profissionais.ativo);
								
								if(profissionais.ativo != 1 && profissionais.ativo != 0) printf("Informe Uma Situação Válida!\n\n");
							} while(profissionais.ativo != 1 && profissionais.ativo != 0);
							fwrite(&profissionais, sizeof(Profissional), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						default:
							printf("\nOpção Inválida!\n");
							break;
					}
				} else printf("\nProfissional Não Encontrado!\n");
			}
			fclose(arq);
		}
		system("pause");
	} while (OpcoesAlteracao != 0);
}
void alterarAvaliacao(void) {
	FILE *arq;
	int opcao, codigo, pos, aux, alterou;

	do {
		system("cls");
		printf("==============================================\n");
		printf("       ALTERAR - AVALIAÇÕES FÍSICAS\n");
		printf("==============================================\n\n");
		printf("  [1]  Aluno\n");
		printf("  [2]  Profissional\n");
		printf("  [3]  Data\n");
		printf("  [4]  Peso\n");
		printf("  [5]  Altura\n");
		printf("  [6]  Percentual de Gordura\n");
		printf("  [7]  Observações\n");
		printf("  [8]  Ativo\n");
		printf("  [0]  Voltar\n\n");
		printf("----------------------------------------------\n");
		printf("  Opção: ");
		scanf("%d", &opcao);

		if(opcao != 0) {
			arq = fopen("avaliacoes.bin", "rb+");
			if(arq == NULL) printf("\nNenhuma Avaliação Cadastrada!\n\n");
			else {
				printf("\nInforme o Código da Avaliação: ");
				scanf("%d", &codigo);

				pos = busca(arq, codigo, ENT_AVALIACAO);
				if(pos != -1) {
					fseek(arq, pos, 0);
					fread(&avaliacoes, sizeof(Avaliacao), 1, arq);
					alterou = 1;

					switch(opcao) {
						case 1:
							printf("Informe o Novo Código do Aluno: ");
							scanf("%d", &aux);
							if(existeAlunoAvaliacao(aux)) avaliacoes.codAluno = aux;
							else {
								printf("Aluno Não Encontrado ou Inativo!\n\n");
								alterou = 0;
							}
							break;
						case 2:
							printf("Informe o Novo Código do Profissional: ");
							scanf("%d", &aux);
							if(existeProfissionalAvaliacao(aux)) avaliacoes.codProfissional = aux;
							else {
								printf("Profissional Não Encontrado ou Inativo!\n\n");
								alterou = 0;
							}
							break;
						case 3:
							do {
								printf("Informe a Nova Data (DD MM AAAA): ");
								scanf("%d%d%d", &avaliacoes.data.dia, &avaliacoes.data.mes, &avaliacoes.data.ano);
								if(validaData(avaliacoes.data) == 0) printf("Data Inválida!\n\n");
							} while(validaData(avaliacoes.data) != 1);
							break;
						case 4:
							do {
								printf("Informe o Novo Peso (KG): ");
								scanf("%f", &avaliacoes.peso);
								if(avaliacoes.peso < 1 || avaliacoes.peso > 500) printf("O Peso Deve Estar Entre 1 e 500 KG!\n\n");
							} while(avaliacoes.peso < 1 || avaliacoes.peso > 500);
							break;
						case 5:
							do {
								printf("Informe a Nova Altura (M): ");
								scanf("%f", &avaliacoes.altura);
								if(avaliacoes.altura < 0.5 || avaliacoes.altura > 2.5) printf("A Altura Deve Estar Entre 0.5 e 2.5 M!\n\n");
							} while(avaliacoes.altura < 0.5 || avaliacoes.altura > 2.5);
							break;
						case 6:
							do {
								printf("Informe o Novo Percentual de Gordura: ");
								scanf("%f", &avaliacoes.percGordura);
								if(avaliacoes.percGordura < 0 || avaliacoes.percGordura > 100) printf("Percentual Inválido! (0 a 100)\n\n");
							} while(avaliacoes.percGordura < 0 || avaliacoes.percGordura > 100);
							break;
						case 7:
							fflush(stdin);
							printf("Informe as Novas Observações: ");
							fgets(avaliacoes.observacoes, 101, stdin);
							if(validaExcedeu(avaliacoes.observacoes) == 1) printf("Somente os Primeiros 100 Caracteres Serão Cadastrados!\n\n");
							break;
						case 8:
							do {
								printf("Informe a Situação (1- Ativo / 0- Inativo): ");
								scanf("%d", &avaliacoes.ativo);
								if(avaliacoes.ativo != 1 && avaliacoes.ativo != 0) printf("Informe Uma Situação Válida!\n\n");
							} while(avaliacoes.ativo != 1 && avaliacoes.ativo != 0);
							break;
						default:
							printf("\nOpção Inválida!\n");
							alterou = 0;
							break;
					}

					if(alterou) {
						avaliacoes.imc = avaliacoes.peso / (avaliacoes.altura * avaliacoes.altura);
						fseek(arq, pos, 0);
						fwrite(&avaliacoes, sizeof(Avaliacao), 1, arq);
						printf("\nAlteração Realizada com Sucesso!\n");
					}
				} else printf("\nAvaliação Não Encontrada!\n");
				fclose(arq);
			}
		}
		system("pause");
	} while(opcao != 0);
}
// Funções de Exclusão
void excluirAluno(void) {
	FILE *arq, *temp;
	int matricula, pos, i;
	
	system("cls");
	printf("==============================================\n");
	printf("  EXCLUIR - Alunos\n");
	printf("==============================================\n\n");
	
	arq = fopen("alunos.bin", "rb");
	if(arq == NULL) printf("Erro na Abertura do Arquivo!\n\n");
	else {
		printf("Informe a Matrícula do Aluno: ");
		scanf("%d", &matricula);
		
		pos = busca(arq, matricula, ENT_ALUNO);
		if(pos != -1) {
			fseek(arq, pos, 0);
			fread(&alunos, sizeof(Aluno), 1, arq);
			printf("==============================================\n");
			printf("Matrícula: %d\n", alunos.codigo);
			printf("Nome: %s", alunos.nome);
			printf("CPF: %s", alunos.cpf);
			printf("Data de Nascimento: %d/%d/%d\n", alunos.dataNascimento.dia, alunos.dataNascimento.mes, alunos.dataNascimento.ano);
			printf("Telefone: %s", alunos.telefone);
			printf("E-mail: %s", alunos.email);
			printf("Ativo: ");
			if(alunos.ativo == 1) printf("Sim\n");
			else printf("Não\n");
			printf("==============================================\n");
			if(alunoTemMatriculaAtiva(matricula) == 1) {
				printf("Não é possível excluir: o aluno possui matrícula ativa!\n");
			} else {
			printf("Deseja excluir? S/N: ");
			if(toupper(getche()) == 'S') {
				temp = fopen("auxiliar.bin", "wb");
				rewind(arq);
				while(fread(&alunos, sizeof(Aluno), 1, arq) == 1)
				if(alunos.codigo != matricula) fwrite(&alunos, sizeof(Aluno), 1, temp);
				fclose(arq);
				fclose(temp);
				remove("alunos.bin");
				rename("auxiliar.bin", "alunos.bin");
				printf("\nO Aluno Foi Excluido com Sucesso!\n");
			}
			}
		} else printf("\nAluno Não Encontrado!\n");
	}
	fclose(arq);
	system("pause");
}

void excluirProfissional(void) {
	FILE *arq, *temp;
	int matricula, pos, i;
	
	system("cls");
	printf("==============================================\n");
	printf("  EXCLUIR - Profissionais\n");
	printf("==============================================\n\n");
	
	arq = fopen("profissionais.bin", "rb");
	if(arq == NULL) printf("Erro na Abertura do Arquivo!\n\n");
	else {
		printf("Informe a Matrícula do Profissional: ");
		scanf("%d", &matricula);
		
		pos = busca(arq, matricula, ENT_PROFISSIONAL);
		if(!feof(arq)) {
			fseek(arq, pos, 0);
			fread(&profissionais, sizeof(Profissional), 1, arq);
			printf("==============================================\n");
			printf("Matrícula: %d\n", profissionais.codigo);
			printf("Nome: %s", profissionais.nome);
			printf("CPF: %s", profissionais.cpf);
			printf("Telefone: %s", profissionais.telefone);
			printf("Função: %s", profissionais.funcao);
			printf("Ativo: ");
			if(profissionais.ativo == 1) printf("Sim\n");
			else printf("Não\n");
			printf("==============================================\n");
			printf("Deseja excluir? S/N: ");
			if(toupper(getche()) == 'S') {
			    temp = fopen("auxiliar.bin", "wb");
			    rewind(arq);
			    while(fread(&profissionais, sizeof(Profissional), 1, arq) == 1)
			    if(profissionais.codigo != matricula) fwrite(&profissionais, sizeof(Profissional), 1, temp);
			    fclose(arq);
			    fclose(temp);
				remove("profissionais.bin");
				rename("auxiliar.bin", "profissionais.bin");
				printf("\nO Profissional Foi Excluido com Sucesso!\n");
			}
		} else printf("\nProfissional Não Encontrado!\n");
	}
	fclose(arq);
	system("pause");
}

void excluirAvaliacao(void) 
{
    FILE *arq, *temp;
    int codigo, pos, confirmou = 0;
    char nomeA[61], nomeP[61];
    system("cls");
    printf("==============================================\n");
    printf("       EXCLUIR - AVALIAÇÕES FÍSICAS\n");
    printf("==============================================\n\n");
    arq = fopen("avaliacoes.bin", "rb");
    if(arq == NULL) 
    {
        printf("Nenhuma Avaliação Cadastrada!\n");
    }
    else 
    {
        printf("Informe o Código da Avaliação: ");
        scanf("%d", &codigo);
        pos = busca(arq, codigo, ENT_AVALIACAO);
        if(pos != -1) 
        {
            fseek(arq, pos, 0);
            fread(&avaliacoes, sizeof(Avaliacao), 1, arq);
            printf("\nCódigo: %d\n", avaliacoes.codigo);
            if(!nomeDoAluno(avaliacoes.codAluno, nomeA)) strcpy(nomeA, "(não encontrado)");
            if(!nomeDoProfissional(avaliacoes.codProfissional, nomeP)) strcpy(nomeP, "(não encontrado)");
            printf("Aluno: %d - %s\n", avaliacoes.codAluno, nomeA);
            printf("Profissional: %d - %s\n", avaliacoes.codProfissional, nomeP);
            printf("Data: %02d/%02d/%d\n", avaliacoes.data.dia, avaliacoes.data.mes, avaliacoes.data.ano);
            printf("Peso: %.2f KG\n", avaliacoes.peso);
            printf("Altura: %.2f M\n", avaliacoes.altura);
            printf("IMC: %.2f\n", avaliacoes.imc);
            printf("Gordura: %.2f%%\n", avaliacoes.percGordura);
            printf("Observações: %s\n", avaliacoes.observacoes);
            printf("\nDeseja excluir? S/N: ");
            if(toupper(getche()) == 'S')
                confirmou = 1;
            if(confirmou) 
            {
                temp = fopen("auxiliar.bin", "wb");
                if(temp == NULL) 
                {
                    printf("\nErro ao Criar Arquivo Temporário!\n");
                }
                else 
                {
                    rewind(arq);
                    while(fread(&avaliacoes, sizeof(Avaliacao), 1, arq) == 1) 
                        if(avaliacoes.codigo != codigo) 
                            fwrite(&avaliacoes, sizeof(Avaliacao), 1, temp);
                    fclose(arq);
                    arq = NULL;
                    fclose(temp);
                    remove("avaliacoes.bin");
                    rename("auxiliar.bin", "avaliacoes.bin");
                    printf("\nAvaliação Excluída com Sucesso!\n");
                }
            }
        }
        else 
            printf("\nAvaliação Não Encontrada!\n");
        if(arq != NULL)
            fclose(arq);
    }
    system("pause");
}
// Funções de Listagem
void listarAlunos(void) {
	FILE *arq;
	int i;
	
	system("cls");
	printf("==============================================\n");
	printf("  LISTAGEM - Alunos\n");
	printf("==============================================\n");
	
	arq = fopen("alunos.bin", "rb");
	if(arq == NULL) printf("Erro na Abertura do Arquivo!\n\n");
	else {
		while(fread(&alunos, sizeof(Aluno), 1, arq) == 1) {
			printf("Matrícula: %d\n", alunos.codigo);
			printf("Nome: %s\n", alunos.nome);
			printf("CPF: %s\n", alunos.cpf);
			printf("Data de Nascimento: %d/%d/%d\n", alunos.dataNascimento.dia, alunos.dataNascimento.mes, alunos.dataNascimento.ano);
			printf("Telefone: %s\n", alunos.telefone);
			printf("E-mail: %s\n", alunos.email);
			printf("Ativo: ");
			if(alunos.ativo == 1) printf("Sim\n");
			else printf("Não\n");
			printf("==============================================\n");
		}
	}
	fclose(arq);
	system("pause");
}

void listarProfissionais(void) {
	FILE *arq;
	int i;
	
	system("cls");
	printf("==============================================\n");
	printf("  LISTAGEM - Profissionais\n");
	printf("==============================================\n");
	
	arq = fopen("profissionais.bin", "rb");
	if(arq == NULL) printf("Erro na Abertura do Arquivo!\n\n");
	else {
		while(fread(&profissionais, sizeof(Profissional), 1, arq) == 1) {
			printf("Matrícula: %d\n", profissionais.codigo);
			printf("Nome: %s\n", profissionais.nome);
			printf("CPF: %s\n", profissionais.cpf);
			printf("Telefone: %s\n", profissionais.telefone);
			printf("Função: %s\n", profissionais.funcao);
			printf("Ativo: ");
			if(profissionais.ativo == 1) printf("Sim\n");
			else printf("Não\n");
			printf("==============================================\n");
		}
	}
	fclose(arq);
	system("pause");
}

void listarAvaliacoes(void) {
	FILE *arq;
	int codigoAluno = 0, encontrou = 0, opc;
	char nomeA[61], nomeP[61];

	system("cls");
	printf("==============================================\n");
	printf("    HISTÓRICO DE AVALIAÇÕES FÍSICAS\n");
	printf("==============================================\n\n");
	arq = fopen("avaliacoes.bin", "rb");
	if(arq == NULL) printf("\nNenhuma Avaliação Cadastrada!\n");
	else {
		do {
			printf("==============================================\n");
			printf("    SELECIONAR POR CÓDIGO OU LISTA COMPLETA\n");
			printf("==============================================\n\n");
			printf("[1] : Código do aluno\n");
			printf("[2] : Lista completa\n");
			printf("Opção: ");
			scanf("%d", &opc);
			if(opc != 1 && opc != 2) printf("\nOpção Inválida!\n\n");
		} while(opc != 1 && opc != 2);

		if(opc == 1) {
			printf("Informe o Código do Aluno: ");
			scanf("%d", &codigoAluno);
		}

		while(fread(&avaliacoes, sizeof(Avaliacao), 1, arq) == 1) {
			if(opc == 2 || (avaliacoes.codAluno == codigoAluno && avaliacoes.ativo == 1)) {
				encontrou = 1;
				if(!nomeDoAluno(avaliacoes.codAluno, nomeA)) strcpy(nomeA, "(não encontrado)");
				if(!nomeDoProfissional(avaliacoes.codProfissional, nomeP)) strcpy(nomeP, "(não encontrado)");
				printf("\n==============================================\n");
				printf("Código da Avaliação: %d\n", avaliacoes.codigo);
				printf("Aluno: %d - %s\n", avaliacoes.codAluno, nomeA);
				printf("Profissional: %d - %s\n", avaliacoes.codProfissional, nomeP);
				printf("Data: %02d/%02d/%d\n", avaliacoes.data.dia, avaliacoes.data.mes, avaliacoes.data.ano);
				printf("Peso: %.2f KG\n", avaliacoes.peso);
				printf("Altura: %.2f M\n", avaliacoes.altura);
				printf("IMC: %.2f\n", avaliacoes.imc);
				printf("Percentual de Gordura: %.2f%%\n", avaliacoes.percGordura);
				printf("Observações: %s\n", avaliacoes.observacoes);
				printf("Ativo: ");
				if(avaliacoes.ativo == 1) printf("Sim\n");
				else printf("Não\n");
				printf("==============================================\n");
			}
		}

		if(!encontrou) {
			if(opc == 1) printf("\nNenhuma Avaliação Encontrada Para Este Aluno!\n");
			else printf("\nNenhuma Avaliação Cadastrada!\n");
		}
		fclose(arq);
	}
	system("pause");
}

// ===================== INÍCIO DO MENU PLANOS (parte da Ana) =====================
// Mostra o menu de tipos e preenche nome, meses e desconto do plano recebido.
// O desconto NÃO é digitado: sai do tipo (Mensal 0%, Trimestral 5%, Semestral 10%, Anual 15%).
// Devolve 1 se escolheu um tipo válido e 0 se não.
int escolherTipoPlano(Plano *p) {
	int opcao;

	printf("\nTIPOS DE PLANO\n");
	printf("  [1] Mensal      (1 mês,   0%% de desconto)\n");
	printf("  [2] Trimestral  (3 meses, 5%% de desconto)\n");
	printf("  [3] Semestral   (6 meses, 10%% de desconto)\n");
	printf("  [4] Anual       (12 meses, 15%% de desconto)\n");
	printf("Escolha: ");
	scanf("%d", &opcao);

	switch(opcao) {
		case 1:
			strcpy(p->nome, "Mensal");
			p->meses = 1;
			p->porcentagemDesconto = 0;
			return 1;
		case 2:
			strcpy(p->nome, "Trimestral");
			p->meses = 3;
			p->porcentagemDesconto = 5;
			return 1;
		case 3:
			strcpy(p->nome, "Semestral");
			p->meses = 6;
			p->porcentagemDesconto = 10;
			return 1;
		case 4:
			strcpy(p->nome, "Anual");
			p->meses = 12;
			p->porcentagemDesconto = 15;
			return 1;
	}
	printf("Opção inválida!\n");
	return 0;
}

// Confere se já existe OUTRO plano com o mesmo tipo (nome). O arquivo chega ABERTO
// (esta função não abre nem fecha). codIgnorar é o código do próprio plano que está
// sendo alterado (no cadastro, passar 0). Depois dela o ponteiro do arquivo fica no
// fim: quem chamou precisa dar fseek antes de gravar.
int tipoPlanoRepetido(FILE *arq, char nome[], int codIgnorar) {
	Plano tempPlano;

	rewind(arq);
	while(fread(&tempPlano, sizeof(Plano), 1, arq) == 1) {
		if(strcmp(tempPlano.nome, nome) == 0 && tempPlano.codigo != codIgnorar) return 1;
	}
	return 0;
}

// Mostra um plano na tela, um campo por linha e uma linha de ===== no final
// (mesmo formato da listagem de Alunos).
void mostrarPlano(Plano p) {
	printf("Código: %d\n", p.codigo);
	printf("Tipo: %s\n", p.nome);
	printf("Meses: %d\n", p.meses);
	printf("Valor mensal: R$ %.2f\n", p.valorMensal);
	printf("Desconto: %.0f%%\n", p.porcentagemDesconto);
	printf("Valor total: R$ %.2f\n", calcularValorPlano(p));
	printf("Ativo: ");
	if(p.ativo == 1) printf("Sim\n");
	else printf("Não\n");
	printf("==============================================\n");
}

// Calcula o valor total do plano já com desconto:
// valor mensal x meses x (1 - desconto/100). Ex.: 100 x 3 x 0,95 = 285,00.
float calcularValorPlano(Plano p) {
	float total;

	total = p.valorMensal * p.meses;
	total = total - (total * p.porcentagemDesconto / 100);

	return total;
}

// Procura o plano pelo código em planos.bin. Se achar, deixa o registro preenchido
// na variável global planos e devolve a posição em bytes. Se não achar (ou se o
// arquivo ainda não existir), devolve -1. Abre e fecha o arquivo sozinha.
// Uso: a Matrícula chama buscarPlano(cod) e depois calcularValorPlano(planos).
int buscarPlano(int codigo) {
	FILE *arq;
	int pos = -1;

	arq = fopen("planos.bin", "rb");
	if(arq != NULL) {
		pos = busca(arq, codigo, ENT_PLANO);
		if(pos != -1) {
			fseek(arq, pos, SEEK_SET);
			fread(&planos, sizeof(Plano), 1, arq);
		}
		fclose(arq);
	}
	return pos;
}

// Devolve 1 se existe plano com esse código e 0 se não existe. (Para Matrícula e
// Pacotes validarem vínculo.) Abre, busca, fecha.
int existePlano(int codigo) {
	FILE *arq;
	int existe = 0;

	arq = fopen("planos.bin", "rb");
	if(arq != NULL) {
		if(busca(arq, codigo, ENT_PLANO) != -1) existe = 1;
		fclose(arq);
	}
	return existe;
}

// Devolve 1 se alguma matrícula usa esse plano (lê matriculas.bin do início ao fim).
// Se matriculas.bin ainda não existe, devolve 0. Usada para bloquear a exclusão.
int planoTemMatricula(int codPlano) {
	FILE *arq;
	Matricula tempMatricula;
	int achou = 0;

	arq = fopen("matriculas.bin", "rb");
	if(arq != NULL) {
		while(achou == 0 && fread(&tempMatricula, sizeof(Matricula), 1, arq) == 1) {
			if(tempMatricula.codPlano == codPlano && tempMatricula.situacao == 1) achou = 1;
		}
		fclose(arq);
	}
	return achou;
}

// CADASTRAR PLANO: pede código (1 a 1000, sem repetir), tipo (o desconto e os meses
// saem do tipo, sem repetir o tipo) e valor mensal (maior que zero). Grava no fim de
// planos.bin com ativo = 1.
void cadastrarPlano(void) {
	FILE *arq;
	int codigo, pos = -1, ok;

	system("cls");
	printf("==============================================\n");
	printf("  CADASTRAR - Planos\n");
	printf("==============================================\n\n");

	arq = fopen("planos.bin", "ab+");
	if(arq == NULL) {
		printf("Erro na Abertura do Arquivo!\n\n");
	} else {
		do {
			printf("Informe o código do plano (1 a 1000): ");
			scanf("%d", &codigo);
			if(codigo < 1 || codigo > 1000) {
				printf("O Código Deve Estar Entre 1 e 1000!\n\n");
			} else {
				pos = busca(arq, codigo, ENT_PLANO);
				if(pos != -1) printf("Código Já Cadastrado!\n\n");
			}
		} while(codigo < 1 || codigo > 1000 || pos != -1);
		planos.codigo = codigo;

		do {
			ok = escolherTipoPlano(&planos);
		} while(ok == 0);

		if(tipoPlanoRepetido(arq, planos.nome, 0) == 1) {
			printf("\nEste tipo de plano já está cadastrado!\n");
		} else {
			do {
				printf("\nInforme o valor mensal: R$ ");
				scanf("%f", &planos.valorMensal);
				if(planos.valorMensal <= 0 || planos.valorMensal > 100000) {
					printf("O valor deve ser maior que zero e menor que 100000!\n");
				}
			} while(planos.valorMensal <= 0 || planos.valorMensal > 100000);

			planos.ativo = 1;

			fseek(arq, 0, SEEK_END);       // depois de ler, volta ao fim para gravar
			fwrite(&planos, sizeof(Plano), 1, arq);

			printf("\nPlano cadastrado com sucesso!\n");
			printf("Valor total: R$ %.2f\n", calcularValorPlano(planos));
		}
		fclose(arq);
	}
}

// LISTAR PLANOS: lê planos.bin do início ao fim mostrando cada plano.
// Se o arquivo não existe ou está vazio, avisa e volta ao menu.
void listarPlanos(void) {
	FILE *arq;
	int total = 0;

	system("cls");
	printf("==============================================\n");
	printf("  LISTAGEM - Planos\n");
	printf("==============================================\n");

	arq = fopen("planos.bin", "rb");
	if(arq == NULL) printf("Nenhum plano cadastrado!\n\n");
	else {
		while(fread(&planos, sizeof(Plano), 1, arq) == 1) {
			mostrarPlano(planos);
			total++;
		}
		if(total == 0) printf("Nenhum plano cadastrado!\n\n");
		else printf("Total de planos: %d\n", total);
		fclose(arq);
	}
	system("pause");
}

// ALTERAR PLANO: escolhe o que mudar (tipo, valor mensal ou ativo), pede o código,
// acha o registro e regrava NA MESMA POSIÇÃO. O código não pode ser alterado.
// Mudar o tipo muda também meses e desconto (o desconto nunca é digitado).
void alterarPlano(void) {
	FILE *arq;
	Plano novo;
	int opcao, codigo, pos, gravar;

	do {
		system("cls");
		printf("==============================================\n");
		printf("  ALTERAR - Planos\n");
		printf("==============================================\n\n");
		printf("  [1]  Tipo do plano\n");
		printf("  [2]  Valor mensal\n");
		printf("  [3]  Ativo\n");
		printf("  [0]  Voltar\n\n");
		printf("----------------------------------------------\n");
		printf("  Opção: ");
		opcao = -1;
		scanf("%d", &opcao);

		if(opcao != 0) {
			if(opcao < 1 || opcao > 3) {
				printf("\nOpção Inválida!\n\n");
			} else {
				arq = fopen("planos.bin", "rb+");
				if(arq == NULL) {
					printf("\nNenhum plano cadastrado!\n\n");
				} else {
					printf("\nInforme o código do plano: ");
					scanf("%d", &codigo);
					pos = busca(arq, codigo, ENT_PLANO);
					if(pos == -1) {
						printf("\nPlano Não Encontrado!\n\n");
					} else {
						fseek(arq, pos, SEEK_SET);
						fread(&planos, sizeof(Plano), 1, arq);
						gravar = 0;

						switch(opcao) {
							case 1:
								novo = planos;
								if(escolherTipoPlano(&novo) == 1) {
									if(tipoPlanoRepetido(arq, novo.nome, planos.codigo) == 1) {
										printf("\nEste tipo de plano já está cadastrado!\n");
									} else {
										strcpy(planos.nome, novo.nome);
										planos.meses = novo.meses;
										planos.porcentagemDesconto = novo.porcentagemDesconto;
										gravar = 1;
									}
								}
								break;
							case 2:
								do {
									printf("Informe o Novo Valor Mensal: R$ ");
									scanf("%f", &planos.valorMensal);
									if(planos.valorMensal <= 0 || planos.valorMensal > 100000) {
										printf("O valor deve ser maior que zero e menor que 100000!\n\n");
									}
								} while(planos.valorMensal <= 0 || planos.valorMensal > 100000);
								gravar = 1;
								break;
							case 3:
								do {
									printf("Informe a Situação (1- Ativo / 0- Inativo): ");
									scanf("%d", &planos.ativo);
									if(planos.ativo != 1 && planos.ativo != 0) printf("Informe Uma Situação Válida!\n\n");
								} while(planos.ativo != 1 && planos.ativo != 0);
								gravar = 1;
								break;
						}

						if(gravar == 1) {
							fseek(arq, pos, SEEK_SET);     // volta para o início do registro
							fwrite(&planos, sizeof(Plano), 1, arq);
							printf("\nAlteração Realizada com Sucesso!\n");
						}
					}
					fclose(arq);
				}
			}
			system("pause");
		}
	} while(opcao != 0);
}

// EXCLUIR PLANO (exclusão física, como na Base): mostra o plano, bloqueia se existe
// matrícula com ele, pede S/N e só depois mexe nos arquivos: copia todos os planos,
// menos o excluído, para auxiliar.bin; apaga planos.bin e renomeia o auxiliar.
void excluirPlano(void) {
	FILE *arq, *temp;
	Plano tempPlano;
	int codigo, pos;

	system("cls");
	printf("==============================================\n");
	printf("  EXCLUIR - Planos\n");
	printf("==============================================\n\n");

	arq = fopen("planos.bin", "rb");
	if(arq == NULL) {
		printf("Nenhum plano cadastrado!\n\n");
	} else {
		printf("Informe o código do plano: ");
		scanf("%d", &codigo);
		pos = busca(arq, codigo, ENT_PLANO);
		if(pos == -1) {
			printf("\nPlano Não Encontrado!\n");
			fclose(arq);
		} else {
			fseek(arq, pos, SEEK_SET);
			fread(&planos, sizeof(Plano), 1, arq);
			printf("==============================================\n");
			mostrarPlano(planos);

			if(planoTemMatricula(codigo) == 1) {
				printf("Não é possível excluir: existe matrícula com este plano!\n");
				fclose(arq);
			} else {
				printf("Deseja excluir? S/N: ");
				if(toupper(getche()) == 'S') {
					temp = fopen("auxiliar.bin", "wb");
					if(temp == NULL) {
						printf("\nErro ao criar o arquivo auxiliar!\n");
						fclose(arq);
					} else {
						rewind(arq);
						while(fread(&tempPlano, sizeof(Plano), 1, arq) == 1) {
							if(tempPlano.codigo != codigo) fwrite(&tempPlano, sizeof(Plano), 1, temp);
						}
						fclose(temp);
						fclose(arq);
						remove("planos.bin");
						rename("auxiliar.bin", "planos.bin");
						printf("\nO Plano Foi Excluído com Sucesso!\n");
					}
				} else {
					printf("\nExclusão Cancelada!\n");
					fclose(arq);
				}
			}
		}
	}
	system("pause");
}
// ===================== FIM DO MENU PLANOS =====================

// ===================== MENU ATIVIDADES (parte do Luiggi) =====================
int existeAtividade(int codigo) {
	FILE *arq;
	Atividade temp;
	int pos, achou = 0;

	arq = fopen("atividades.bin", "rb");
	if(arq == NULL) return 0;

	pos = busca(arq, codigo, ENT_ATIVIDADE);
	if(pos != -1) {
		fseek(arq, pos, 0);
		fread(&temp, sizeof(Atividade), 1, arq);
		if(temp.ativo == 1) achou = 1;
	}
	fclose(arq);
	return achou;
}

int nomeAtividadeExiste(char nome[], int codIgnorar) {
	FILE *arq;
	Atividade temp;
	int achou = 0;

	arq = fopen("atividades.bin", "rb");
	if(arq == NULL) return 0;

	while(!achou && fread(&temp, sizeof(Atividade), 1, arq) == 1) {
		if(temp.codigo != codIgnorar && stricmp(temp.nome, nome) == 0) achou = 1;
	}
	fclose(arq);
	return achou;
}

void cadastrarAtividade(void) {
	FILE *arq;
	int pos, repetido;

	arq = fopen("atividades.bin", "ab+");
	if(arq == NULL) printf("Erro na Abertura do Arquivo!\n\n");
	else {
		do {
			do {
				printf("Informe o Código da Atividade: ");
				scanf("%d", &atividades.codigo);
				if(atividades.codigo < 1 || atividades.codigo > 1000) printf("O Código Deve Estar Entre 1 e 1000!\n\n");
			} while(atividades.codigo < 1 || atividades.codigo > 1000);

			pos = busca(arq, atividades.codigo, ENT_ATIVIDADE);
			if(pos != -1) printf("Código Já Cadastrado!\n\n");
		} while(pos != -1);

		do {
			fflush(stdin);
			printf("Informe o Nome da Atividade: ");
			fgets(atividades.nome, 61, stdin);
			if(validaExcedeu(atividades.nome) == 1) printf("Somente os Primeiros 60 Caracteres Serão Cadastrados!\n\n");
			repetido = nomeAtividadeExiste(atividades.nome, 0);
			if(repetido) printf("Já Existe uma Atividade com Esse Nome!\n\n");
		} while(repetido);

		fflush(stdin);
		printf("Informe uma Breve Descrição da Atividade: ");
		fgets(atividades.descricao, 101, stdin);
		if(validaExcedeu(atividades.descricao) == 1) printf("Somente os Primeiros 100 Caracteres Serão Cadastrados!\n\n");

		do {
			printf("Informe a Duração da Atividade (Minutos): ");
			scanf("%d", &atividades.duracaoMin);
			if(atividades.duracaoMin < 1) printf("A Duração Deve Ser Maior que Zero!\n\n");
		} while(atividades.duracaoMin < 1);

		do {
			printf("Informe o Número de Vagas: ");
			scanf("%d", &atividades.capacidade);
			if(atividades.capacidade < 1) printf("O Número de Vagas Deve Ser Maior que Zero!\n\n");
		} while(atividades.capacidade < 1);

		do {
			printf("Informe o Valor da Aula: R$ ");
			scanf("%f", &atividades.valorAula);
			if(atividades.valorAula <= 0) printf("O Valor Deve Ser Maior que Zero!\n\n");
		} while(atividades.valorAula <= 0);

		atividades.ativo = 1;
		fseek(arq, 0, SEEK_END);
		fwrite(&atividades, sizeof(Atividade), 1, arq);
		fclose(arq);
		printf("\n==============================================\n");
		printf("     ATIVIDADE CADASTRADA COM SUCESSO!\n");
		printf("==============================================\n");
	}
}

void alterarAtividade(void) {
	FILE *arq;
	int opcao, codigo, pos, repetido;

	do {
		system("cls");
		printf("==============================================\n");
		printf("  ALTERAR - Atividades\n");
		printf("==============================================\n\n");
		printf("  [1]  Nome\n");
		printf("  [2]  Descrição\n");
		printf("  [3]  Duração\n");
		printf("  [4]  Vagas\n");
		printf("  [5]  Valor da Aula\n");
		printf("  [6]  Ativo\n");
		printf("  [0]  Voltar\n\n");
		printf("----------------------------------------------\n");
		printf("  Opção: ");
		scanf("%d", &opcao);

		if(opcao != 0) {
			arq = fopen("atividades.bin", "rb+");
			if(arq == NULL) printf("\nNenhuma Atividade Cadastrada!\n\n");
			else {
				printf("\nInforme o Código da Atividade: ");
				scanf("%d", &codigo);

				pos = busca(arq, codigo, ENT_ATIVIDADE);
				if(pos != -1) {
					fseek(arq, pos, 0);
					fread(&atividades, sizeof(Atividade), 1, arq);
					fseek(arq, pos, 0);

					fflush(stdin);
					switch(opcao) {
						case 1:
							do {
								printf("Informe o Novo Nome: ");
								fgets(atividades.nome, 61, stdin);
								if(validaExcedeu(atividades.nome) == 1) printf("Somente os Primeiros 60 Caracteres Serão Cadastrados!\n\n");
								repetido = nomeAtividadeExiste(atividades.nome, atividades.codigo);
								if(repetido) printf("Já Existe uma Atividade com Esse Nome!\n\n");
							} while(repetido);
							fwrite(&atividades, sizeof(Atividade), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						case 2:
							printf("Informe a Nova Descrição: ");
							fgets(atividades.descricao, 101, stdin);
							if(validaExcedeu(atividades.descricao) == 1) printf("Somente os Primeiros 100 Caracteres Serão Cadastrados!\n\n");
							fwrite(&atividades, sizeof(Atividade), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						case 3:
							do {
								printf("Informe a Nova Duração (Minutos): ");
								scanf("%d", &atividades.duracaoMin);
								if(atividades.duracaoMin < 1) printf("A Duração Deve Ser Maior que Zero!\n\n");
							} while(atividades.duracaoMin < 1);
							fwrite(&atividades, sizeof(Atividade), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						case 4:
							do {
								printf("Informe o Novo Número de Vagas: ");
								scanf("%d", &atividades.capacidade);
								if(atividades.capacidade < 1) printf("O Número de Vagas Deve Ser Maior que Zero!\n\n");
							} while(atividades.capacidade < 1);
							fwrite(&atividades, sizeof(Atividade), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						case 5:
							do {
								printf("Informe o Novo Valor da Aula: R$ ");
								scanf("%f", &atividades.valorAula);
								if(atividades.valorAula <= 0) printf("O Valor Deve Ser Maior que Zero!\n\n");
							} while(atividades.valorAula <= 0);
							fwrite(&atividades, sizeof(Atividade), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						case 6:
							do {
								printf("Informe a Situação (1- Ativo / 0- Inativo): ");
								scanf("%d", &atividades.ativo);
								if(atividades.ativo != 1 && atividades.ativo != 0) printf("Informe Uma Situação Válida!\n\n");
							} while(atividades.ativo != 1 && atividades.ativo != 0);
							fwrite(&atividades, sizeof(Atividade), 1, arq);
							printf("Alteração Realizada com Sucesso!\n");
							break;
						default:
							printf("\nOpção Inválida!\n");
							break;
					}
				} else printf("\nAtividade Não Encontrada!\n");
				fclose(arq);
			}
		}
		system("pause");
	} while(opcao != 0);
}

void excluirAtividade(void) {
	FILE *arq, *temp;
	int codigo, pos;

	system("cls");
	printf("==============================================\n");
	printf("  EXCLUIR - Atividades\n");
	printf("==============================================\n\n");

	arq = fopen("atividades.bin", "rb");
	if(arq == NULL) printf("Nenhuma Atividade Cadastrada!\n\n");
	else {
		printf("Informe o Código da Atividade: ");
		scanf("%d", &codigo);

		pos = busca(arq, codigo, ENT_ATIVIDADE);
		if(pos != -1) {
			fseek(arq, pos, 0);
			fread(&atividades, sizeof(Atividade), 1, arq);
			printf("==============================================\n");
			printf("Código: %d\n", atividades.codigo);
			printf("Nome: %s\n", atividades.nome);
			printf("Descrição: %s\n", atividades.descricao);
			printf("Duração: %d Minutos\n", atividades.duracaoMin);
			printf("Vagas: %d\n", atividades.capacidade);
			printf("Valor da Aula: R$ %.2f\n", atividades.valorAula);
			printf("Ativo: ");
			if(atividades.ativo == 1) printf("Sim\n");
			else printf("Não\n");
			printf("==============================================\n");
			if(atividadeEmPacoteAtivo(codigo) == 1) {
				printf("Não é possível excluir: a atividade faz parte de pacote ativo!\n");
			} else {
			printf("Deseja excluir? S/N: ");
			if(toupper(getche()) == 'S') {
				temp = fopen("auxiliar.bin", "wb");
				if(temp == NULL) printf("\nErro ao Criar Arquivo Temporário!\n");
				else {
					rewind(arq);
					while(fread(&atividades, sizeof(Atividade), 1, arq) == 1)
						if(atividades.codigo != codigo) fwrite(&atividades, sizeof(Atividade), 1, temp);
					fclose(arq);
					arq = NULL;
					fclose(temp);
					remove("atividades.bin");
					rename("auxiliar.bin", "atividades.bin");
					printf("\nAtividade Excluída com Sucesso!\n");
				}
			}
			}
		} else printf("\nAtividade Não Encontrada!\n");
		if(arq != NULL) fclose(arq);
	}
	system("pause");
}

void listarAtividades(void) {
	FILE *arq;
	int encontrou = 0;

	system("cls");
	printf("==============================================\n");
	printf("  LISTAGEM - Atividades\n");
	printf("==============================================\n");

	arq = fopen("atividades.bin", "rb");
	if(arq == NULL) printf("\nNenhuma Atividade Cadastrada!\n\n");
	else {
		while(fread(&atividades, sizeof(Atividade), 1, arq) == 1) {
			encontrou = 1;
			printf("Código: %d\n", atividades.codigo);
			printf("Nome: %s\n", atividades.nome);
			printf("Descrição: %s\n", atividades.descricao);
			printf("Duração: %d Minutos\n", atividades.duracaoMin);
			printf("Vagas: %d\n", atividades.capacidade);
			printf("Valor da Aula: R$ %.2f\n", atividades.valorAula);
			printf("Ativo: ");
			if(atividades.ativo == 1) printf("Sim\n");
			else printf("Não\n");
			printf("==============================================\n");
		}
		if(!encontrou) printf("\nNenhuma Atividade Cadastrada!\n\n");
		fclose(arq);
	}
	system("pause");
}
// =================== FIM DO MENU ATIVIDADES ===================

// ===== MENU TREINOS E EQUIPAMENTOS (parte do Igor) =====
void alterarTreino() {
	FILE *arq;
	FILE *arqAlunos;
	FILE *arqProfissionais;
	
	int i, opcao, pos, posProfissional;
	int numExerc, opExerc, indice, duplicado, exibir;

	do {
		system("cls");
		printf("==============================================\n");
		printf("  ALTERAR - Treinos\n");
		printf("==============================================\n\n");
		printf("  [1]  Código do Profissional\n");
		printf("  [2]  Nome do Treino\n");
		printf("  [3]  Exercícios\n");
		printf("  [4]  Objetivos\n");
		printf("  [0]  Voltar\n\n");
		printf("----------------------------------------------\n");
		printf("  Opção: ");
		scanf("%d", &opcao);
	
		if(opcao !=0) {
			arq = fopen("treinos.bin", "rb+");
		
			if(arq == NULL) {
				printf("Erro na Abertura do Arquivo!\n\n");
				system("pause");
				system("cls");
				return;
			} else {
				system("cls");	
				switch(opcao) {
					case 1:
						// verifica se o codigo do treino existe.
						do {	
							printf("Informe o Código do Treino: ");
							scanf("%d", &treinos.codigo);
							validarCodigo(treinos.codigo);
								
						} while(treinos.codigo < 1 || treinos.codigo > 1000);
							
						pos = busca(arq, treinos.codigo, ENT_TREINO);
						if(pos == -1) {
							printf("[ERRO] Código não encontrado!\n\n");
							fclose(arq);
							
							system("pause");
							system("cls");
							return;
						} 
						
						fseek(arq, pos, SEEK_SET);
						fread(&treinos, sizeof(Treino), 1, arq);
							
						arqProfissionais = fopen("profissionais.bin", "rb");
						if(arqProfissionais == NULL) {
							printf("Erro na Abertura do Arquivo!\n\n");
							
							fclose(arq);
							system("pause");
							system("cls");
							return;
						}
						
						// exibe as informações do profissional.
						posProfissional = busca(arqProfissionais, treinos.codProfissional, ENT_PROFISSIONAL);
						if(posProfissional != -1) {
							fseek(arqProfissionais, posProfissional, SEEK_SET);
							fread(&profissionais, sizeof(Profissional), 1, arqProfissionais);
							printf("  Profissional Atual: %d | %s\n\n", profissionais.codigo, profissionais.nome);
						}
						
						do {
							printf("Novo Código do Profissional: ");
							scanf("%d", &treinos.codProfissional);
							validarCodigo(treinos.codProfissional);
							
						} while(treinos.codProfissional < 1 || treinos.codProfissional > 1000);
					
						// verifica se o codigo do profissional existe.
						posProfissional = busca(arqProfissionais, treinos.codProfissional, ENT_PROFISSIONAL);
						if(posProfissional == -1) {
							printf("[ERRO] Profissional não Encontrado!\n\n");
							fclose(arq);
							fclose(arqProfissionais);
							system("pause");
							system("cls");
							return;
						}
						
						fseek(arq, pos, SEEK_SET);
						fwrite(&treinos, sizeof(Treino), 1, arq);
						printf("\n[SUCESSO] Alteração Realizada com Sucesso!\n");
					
						fclose(arqProfissionais);
						system("pause");
						system("cls");	
					break;
					case 2:
						// verifica se o codigo do treino existe.
						do {	
							printf("Informe o Código do Treino: ");
							scanf("%d", &treinos.codigo);
							validarCodigo(treinos.codigo);
								
						} while(treinos.codigo < 1 || treinos.codigo > 1000);
							
						pos = busca(arq, treinos.codigo, ENT_TREINO);
						if(pos == -1) {
							printf("[ERRO] Código não encontrado!\n\n");
							
							fclose(arq);
							system("pause");
							system("cls");
							return;
						} 
						
						// lê as informações especificas do respectivo treino.
						fseek(arq, pos, SEEK_SET);
						fread(&treinos, sizeof(Treino), 1, arq);
						
						printf("  Nome do Treino: %s\n", treinos.nome);
				
						getchar();
						printf("Informe o Novo Nome do Treino: ");
						fgets(treinos.nome, 31, stdin);
						if(validaExcedeu(treinos.nome) == 1) printf("Somente os Primeiros 30 Caracteres Serão Cadastrados!\n\n");
						
						fseek(arq, pos, SEEK_SET);
						fwrite(&treinos, sizeof(Treino), 1, arq);
						printf("\n[SUCESSO] Alteração Realizada com Sucesso!\n");
					
						system("pause");
						system("cls");	
					break;
					case 3:
						system("cls");
						printf("==============================================\n");
						printf("  ALTERAR - Treinos\n");
						printf("==============================================\n\n");
						printf("  [1]  Alterar Todos os Exercícios\n");
						printf("  [2]  Alterar Exercício Específico\n");
						printf("  [3]  Adicionar Exercício\n");
						printf("  [4]  Remover Exercício\n");
						printf("  [0]  Voltar\n\n");
						printf("----------------------------------------------\n");
						printf("  Opção: ");
						scanf("%d", &opExerc);
						
						switch(opExerc) {
							case 1:
								// verifica se o codigo do treino existe.
								system("cls");
								do {	
									printf("Informe o Código do Treino: ");
									scanf("%d", &treinos.codigo);
									getchar();
									validarCodigo(treinos.codigo);
										
								} while(treinos.codigo < 1 || treinos.codigo > 1000);
									
								pos = busca(arq, treinos.codigo, ENT_TREINO);
								if(pos == -1) {
									printf("[ERRO] Código não encontrado!\n\n");
									
									fclose(arq);
									system("pause");
									system("cls");
									return;
								} 
								
								// lê as informações especificas do respectivo treino.
								fseek(arq, pos, SEEK_SET);
								fread(&treinos, sizeof(Treino), 1, arq);
								
								printf("\nExercícios:\n");
								for(i=0; i<treinos.quantidade; i++) {
									printf("  %d) Exercício: %s\n", i+1, treinos.exercicios[i]);
								}
								
								printf("\nNovos Exercícios:\n");
								for(i=0; i<treinos.quantidade; i++) {
									printf("  %d) Exercício: ", i+1);
								    fgets(treinos.exercicios[i], 41, stdin);
								    if(validaExcedeu(treinos.exercicios[i]) == 1) printf("Somente os Primeiros 40 Caracteres Serão Cadastrados!\n\n");
								}
								
								fseek(arq, pos, SEEK_SET);
								fwrite(&treinos, sizeof(Treino), 1, arq);
								printf("\n[SUCESSO] Alteração Realizada com Sucesso!\n");
							
								system("pause");
								system("cls");	
							break;
							case 2:
								// verifica se o codigo do treino existe.
								system("cls");
								do {	
									printf("Informe o Código do Treino: ");
									scanf("%d", &treinos.codigo);
									getchar();
									validarCodigo(treinos.codigo);
										
								} while(treinos.codigo < 1 || treinos.codigo > 1000);
									
								pos = busca(arq, treinos.codigo, ENT_TREINO);
								if(pos == -1) {
									printf("[ERRO] Código não encontrado!\n\n");
									
									fclose(arq);
									system("pause");
									system("cls");
									return;
								} 
								
								// lê as informações especificas do respectivo treino.
								fseek(arq, pos, SEEK_SET);
								fread(&treinos, sizeof(Treino), 1, arq);
								
								printf("\nExercícios:\n");
								for(i=0; i<treinos.quantidade; i++) {
									printf("  %d) Exercício: %s\n", i+1, treinos.exercicios[i]);
								}
								
								printf("\nInforme o Número do Exercício a Alterar: ");
								scanf("%d", &numExerc);
								getchar();
								
								if(numExerc>=1 && numExerc<=treinos.quantidade) {
									indice = numExerc - 1;
									printf("  %d) Exercício: %s\n", indice+1, treinos.exercicios[indice]);
											
									printf("\nNovo Exercício: ");
									fgets(treinos.exercicios[indice], 41, stdin);
									if(validaExcedeu(treinos.exercicios[indice]) == 1) printf("Somente os Primeiros 40 Caracteres Serão Cadastrados!\n\n");
									
									fseek(arq, pos, SEEK_SET);
									fwrite(&treinos, sizeof(Treino), 1, arq);
									printf("\n[SUCESSO] Alteração Realizada com Sucesso!\n");
								
									system("pause");
									system("cls");	
								} else {
									printf("\n[AVISO] Número Inválido!\n\n");
									
									fclose(arq);
									system("pause");
									system("cls");
									return;
								}
								
							break;
							case 3:
								// verifica se o codigo do treino existe.
								system("cls");
								
								do {	
									printf("Informe o Código do Treino: ");
									scanf("%d", &treinos.codigo);
									getchar();
									validarCodigo(treinos.codigo);
										
								} while(treinos.codigo < 1 || treinos.codigo > 1000);
									
								pos = busca(arq, treinos.codigo, ENT_TREINO);
								if(pos == -1) {
									printf("[ERRO] Código não encontrado!\n\n");
									
									fclose(arq);
									system("pause");
									system("cls");
									return;
								} 
								
								// lê as informações especificas do respectivo treino.
								fseek(arq, pos, SEEK_SET);
								fread(&treinos, sizeof(Treino), 1, arq);
								
								if(treinos.quantidade >= 10) {
									printf("[AVISO] Quantidade Máxima de Exercícios Atingida!\n\n");
									fclose(arq); 
									
									system("pause");
									system("cls");
									return;
								} 
								
								printf("\nExercícios:\n");
								for(i=0; i<treinos.quantidade; i++) {
									printf("  %d) Exercício: %s\n", i+1, treinos.exercicios[i]);
								}
								
								do {
									if(exibir == 1) {
										printf("Informe o Código do Treino: %d\n", treinos.codigo);
									
										printf("\nExercícios:\n");
										for(i=0; i<treinos.quantidade; i++) {
											printf("  %d) Exercício: %s\n", i+1, treinos.exercicios[i]);
										}
									}
									
									duplicado = 0;
									exibir = 0;
									
									printf("\nNovo Exercício: ");
									fgets(treinos.exercicios[treinos.quantidade], 41, stdin);
									if(validaExcedeu(treinos.exercicios[treinos.quantidade]) == 1) printf("Somente os Primeiros 40 Caracteres Serão Cadastrados!\n\n");
								
									for(i=0; i<treinos.quantidade; i++) {
										if(stricmp(treinos.exercicios[treinos.quantidade], treinos.exercicios[i]) == 0) {
											printf("\n[AVISO] Exercício já cadastrado!\n\n");
											duplicado = 1;
											exibir = 1;
											
											system("pause");
											system("cls");
										}
									}
								} while(duplicado == 1);
								treinos.quantidade++;
								
								fseek(arq, pos, SEEK_SET);
								fwrite(&treinos, sizeof(Treino), 1, arq);
								printf("\n[SUCESSO] Alteração Realizada com Sucesso!\n");
							
								system("pause");
								system("cls");
							break;
							case 4:
								// verifica se o codigo do treino existe.
								system("cls");
								do {	
									printf("Informe o Código do Treino: ");
									scanf("%d", &treinos.codigo);
									getchar();
									validarCodigo(treinos.codigo);
										
								} while(treinos.codigo < 1 || treinos.codigo > 1000);
									
								pos = busca(arq, treinos.codigo, ENT_TREINO);
								if(pos == -1) {
									printf("[ERRO] Código não encontrado!\n\n");
									
									fclose(arq);
									system("pause");
									system("cls");
									return;
								} 
								
								// lê as informações especificas do respectivo treino.
								fseek(arq, pos, SEEK_SET);
								fread(&treinos, sizeof(Treino), 1, arq);
								
								if(treinos.quantidade == 1) {
									printf("[AVISO] O Treino deve Possuir pelo menos 1 Exercício Cadastrado!\n\n");
									fclose(arq); 
									
									system("pause");
									system("cls");
									return;
								} 
								
								printf("\nExercícios:\n");
								for(i=0; i<treinos.quantidade; i++) {
									printf("  %d) Exercício: %s\n", i+1, treinos.exercicios[i]);
								}
								
								printf("\nInforme o Número do Exercício a Remover: ");
								scanf("%d", &numExerc);
								getchar();
									
								if(numExerc>=1 && numExerc<=treinos.quantidade) {
									indice = numExerc - 1;
									printf("  %d) Exercício: %s\n", indice+1, treinos.exercicios[indice]);
												
									printf("\nDeseja Remover o Exercício? (S/N): ");
									if(toupper(getche()) == 'S') {
										for(i=indice; i<=treinos.quantidade-2; i++) {
											strcpy(treinos.exercicios[i], treinos.exercicios[i+1]);
										}
										treinos.quantidade --;
											
										fseek(arq, pos, SEEK_SET);
										fwrite(&treinos, sizeof(Treino), 1, arq);
										printf("\n\n[SUCESSO] Alteração Realizada com Sucesso!\n");
										
										fclose(arq);
										system("pause");
										system("cls");
										return;	
									} else {
										printf("\n[AVISO] Exclusão Cancelada!\n");
										
										fclose(arq);
										system("pause");
										system("cls");
										return;
									}
									
								} else {
									printf("\n[AVISO] Número Inválido!\n\n");
									
									fclose(arq);
									system("pause");
									system("cls");
									return;
								}
							break;
						}
					break;
					case 4:
						// verifica se o codigo do treino existe.
						do {
							do {	
							printf("Informe o Código do Treino: ");
							scanf("%d", &treinos.codigo);
							validarCodigo(treinos.codigo);
								
							} while(treinos.codigo < 1 || treinos.codigo > 1000);
								
							pos = busca(arq, treinos.codigo, ENT_TREINO);
							if(pos == -1) {
								printf("[ERRO] Código não encontrado!\n\n");
								
								fclose(arq);
								system("pause");
								system("cls");
							} 
						} while(pos == -1);
						
						// lê as informações especificas do respectivo treino.
						fseek(arq, pos, SEEK_SET);
						fread(&treinos, sizeof(Treino), 1, arq);
						
						printf("  Objetivo: %s\n", treinos.objetivo);
				
						getchar();
						printf("Informe o Novo Objetivo: ");
						fgets(treinos.objetivo, 61, stdin);
						if(validaExcedeu(treinos.objetivo) == 1) printf("Somente os Primeiros 60 Caracteres Serão Cadastrados!\n\n");
						
						fseek(arq, pos, SEEK_SET);
						fwrite(&treinos, sizeof(Treino), 1, arq);
						printf("\n[SUCESSO] Alteração Realizada com Sucesso!\n");
					
						system("pause");
						system("cls");	
					break;
					default:
						printf("\n Opção Inválida!");
					break;
				}
				fclose(arq);
			}	
		}
	} while(opcao !=0);	
}

// Função Alterar Equipamento
void alterarEquipamento() {
	FILE *arq;
	Equipamento equipamentos;
	
	int pos, opcao;
	
	do {
		system("cls");
		printf("==============================================\n");
		printf("  ALTERAR - Equipamentos\n");
		printf("==============================================\n\n");
		printf("  [1]  Nome do Equipamento\n"); // Ok
		printf("  [2]  Tipo do Equipamento\n"); // Ok
		printf("  [3]  Data da Aquisição\n"); // Ok
		printf("  [4]  Data da Última Manutenção\n"); // Ok
		printf("  [5]  Situação\n"); // Ok
		printf("  [6]  Valor\n");
		printf("  [7]  Quantidade\n");
		printf("  [0]  Voltar\n\n");
		printf("----------------------------------------------\n");
		printf("  Opção: ");
		scanf("%d", &opcao);
		
		if(opcao !=0) {
			arq = fopen("equipamentos.bin", "rb+");
		
			if(arq == NULL) {
				printf("Erro na Abertura do Arquivo!\n\n");
				system("pause");
				system("cls");
				return;
			} else {
				system("cls");	
				switch(opcao) {
					
					case 1:
						// verifica se o codigo do treino existe.
						do {	
							printf("Informe o Código do Equipamento: ");
							scanf("%d", &equipamentos.codigo);
							validarCodigo(equipamentos.codigo);
								
						} while(equipamentos.codigo < 1 || equipamentos.codigo > 1000);
							
						pos = busca(arq, equipamentos.codigo, ENT_EQUIPAMENTO);
						if(pos == -1) {
							printf("[ERRO] Código não encontrado!\n\n");
							
							fclose(arq);
							system("pause");
							system("cls");
							return;
						} 
						
						// lê as informações especificas do respectivo treino.
						fseek(arq, pos, SEEK_SET);
						fread(&equipamentos, sizeof(Equipamento), 1, arq);
						
						printf("  Nome do Equipamento: %s\n", equipamentos.nome);
				
						getchar();
						printf("Informe o Novo Nome do Equipamento: ");
						fgets(equipamentos.nome, 31, stdin);
						if(validaExcedeu(equipamentos.nome) == 1) printf("Somente os Primeiros 30 Caracteres Serão Cadastrados!\n\n");
						
						fseek(arq, pos, SEEK_SET);
						fwrite(&equipamentos, sizeof(Equipamento), 1, arq);
						printf("\n[SUCESSO] Alteração Realizada com Sucesso!\n");
					
						system("pause");
						system("cls");	
					break;
					case 2:
						// verifica se o codigo do treino existe.
						do {	
							printf("Informe o Código do Equipamento: ");
							scanf("%d", &equipamentos.codigo);
							validarCodigo(equipamentos.codigo);
								
						} while(equipamentos.codigo < 1 || equipamentos.codigo > 1000);
							
						pos = busca(arq, equipamentos.codigo, ENT_EQUIPAMENTO);
						if(pos == -1) {
							printf("[ERRO] Código não encontrado!\n\n");
							
							fclose(arq);
							system("pause");
							system("cls");
							return;
						} 
						
						// lê as informações especificas do respectivo treino.
						fseek(arq, pos, SEEK_SET);
						fread(&equipamentos, sizeof(Equipamento), 1, arq);
						
						printf("  Tipo do Equipamento: %s\n", equipamentos.tipo);
				
						getchar();
						printf("Informe o Novo Tipo do Equipamento: ");
						fgets(equipamentos.tipo, 31, stdin);
						if(validaExcedeu(equipamentos.tipo) == 1) printf("Somente os Primeiros 30 Caracteres Serão Cadastrados!\n\n");
						
						fseek(arq, pos, SEEK_SET);
						fwrite(&equipamentos, sizeof(Equipamento), 1, arq);
						printf("\n[SUCESSO] Alteração Realizada com Sucesso!\n");
					
						system("pause");
						system("cls");
					break;
					case 3:
						// verifica se o codigo do treino existe.
						do {	
							printf("Informe o Código do Equipamento: ");
							scanf("%d", &equipamentos.codigo);
							validarCodigo(equipamentos.codigo);
								
						} while(equipamentos.codigo < 1 || equipamentos.codigo > 1000);
							
						pos = busca(arq, equipamentos.codigo, ENT_EQUIPAMENTO);
						if(pos == -1) {
							printf("[ERRO] Código não encontrado!\n\n");
							
							fclose(arq);
							system("pause");
							system("cls");
							return;
						} 
						
						// lê as informações especificas do respectivo treino.
						fseek(arq, pos, SEEK_SET);
						fread(&equipamentos, sizeof(Equipamento), 1, arq);
						
						printf("  Data de Aquisição: %d/%d/%d\n\n", equipamentos.dataAquisicao.dia, equipamentos.dataAquisicao.mes, equipamentos.dataAquisicao.ano);
				
						printf("Informe a Nova Data de Aquisição (DD MM AAAA): ");
						scanf("%d%d%d", &equipamentos.dataAquisicao.dia, &equipamentos.dataAquisicao.mes, &equipamentos.dataAquisicao.ano);
						
						fseek(arq, pos, SEEK_SET);
						fwrite(&equipamentos, sizeof(Equipamento), 1, arq);
						printf("\n[SUCESSO] Alteração Realizada com Sucesso!\n");
					
						system("pause");
						system("cls");
					break;
					case 4:
						// verifica se o codigo do treino existe.
						do {	
							printf("Informe o Código do Equipamento: ");
							scanf("%d", &equipamentos.codigo);
							validarCodigo(equipamentos.codigo);
								
						} while(equipamentos.codigo < 1 || equipamentos.codigo > 1000);
							
						pos = busca(arq, equipamentos.codigo, ENT_EQUIPAMENTO);
						if(pos == -1) {
							printf("[ERRO] Código não encontrado!\n\n");
							
							fclose(arq);
							system("pause");
							system("cls");
							return;
						} 
						
						// lê as informações especificas do respectivo treino.
						fseek(arq, pos, SEEK_SET);
						fread(&equipamentos, sizeof(Equipamento), 1, arq);
						
						printf("  Data da Última Manutenção: %d/%d/%d\n\n", equipamentos.dataUltimaManutencao.dia, equipamentos.dataUltimaManutencao.mes, equipamentos.dataUltimaManutencao.ano);
				
						printf("Informe a Nova Data de Manutenção (DD MM AAAA): ");
						scanf("%d%d%d", &equipamentos.dataUltimaManutencao.dia, &equipamentos.dataUltimaManutencao.mes, &equipamentos.dataUltimaManutencao.ano);
						
						fseek(arq, pos, SEEK_SET);
						fwrite(&equipamentos, sizeof(Equipamento), 1, arq);
						printf("\n[SUCESSO] Alteração Realizada com Sucesso!\n");
					
						system("pause");
						system("cls");
					break;
					case 5:
						// verifica se o codigo do treino existe.
						do {	
							printf("Informe o Código do Equipamento: ");
							scanf("%d", &equipamentos.codigo);
							validarCodigo(equipamentos.codigo);
								
						} while(equipamentos.codigo < 1 || equipamentos.codigo > 1000);
							
						pos = busca(arq, equipamentos.codigo, ENT_EQUIPAMENTO);
						if(pos == -1) {
							printf("[ERRO] Código não encontrado!\n\n");
							
							fclose(arq);
							system("pause");
							system("cls");
							return;
						} 
						
						// lê as informações especificas do respectivo treino.
						fseek(arq, pos, SEEK_SET);
						fread(&equipamentos, sizeof(Equipamento), 1, arq);
						
						printf("\nSituação: ");
						printf("\n  Opção: %d", equipamentos.situacao);
				
						printf("\n\nNova Situação: 1 = Bom | 2 = Manutenção | 3 = Inativo");
						printf("\n  Opção: ");
						scanf("%d", &equipamentos.situacao);
						
						fseek(arq, pos, SEEK_SET);
						fwrite(&equipamentos, sizeof(Equipamento), 1, arq);
						printf("\n[SUCESSO] Alteração Realizada com Sucesso!\n");
					
						system("pause");
						system("cls");
					break;
					case 6:
						// verifica se o codigo do treino existe.
						do {	
							printf("Informe o Código do Equipamento: ");
							scanf("%d", &equipamentos.codigo);
							validarCodigo(equipamentos.codigo);
								
						} while(equipamentos.codigo < 1 || equipamentos.codigo > 1000);
							
						pos = busca(arq, equipamentos.codigo, ENT_EQUIPAMENTO);
						if(pos == -1) {
							printf("[ERRO] Código não encontrado!\n\n");
							
							fclose(arq);
							system("pause");
							system("cls");
							return;
						} 
						
						// lê as informações especificas do respectivo treino.
						fseek(arq, pos, SEEK_SET);
						fread(&equipamentos, sizeof(Equipamento), 1, arq);
						
						printf("  Valor: %.2f\n\n", equipamentos.valor);
				
				        equipamentos.vTotal = equipamentos.valor * equipamentos.quantidade;
				
						printf("Informe o Novo Valor: ");
						scanf("%f", &equipamentos.valor);
						
						fseek(arq, pos, SEEK_SET);
						fwrite(&equipamentos, sizeof(Equipamento), 1, arq);
						printf("\n[SUCESSO] Alteração Realizada com Sucesso!\n");
					
						system("pause");
						system("cls");
					break;
					case 7:
						// verifica se o codigo do treino existe.
						do {	
							printf("Informe o Código do Equipamento: ");
							scanf("%d", &equipamentos.codigo);
							validarCodigo(equipamentos.codigo);
								
						} while(equipamentos.codigo < 1 || equipamentos.codigo > 1000);
							
						pos = busca(arq, equipamentos.codigo, ENT_EQUIPAMENTO);
						if(pos == -1) {
							printf("[ERRO] Código não encontrado!\n\n");
							
							fclose(arq);
							system("pause");
							system("cls");
							return;
						} 
						
						// lê as informações especificas do respectivo treino.
						fseek(arq, pos, SEEK_SET);
						fread(&equipamentos, sizeof(Equipamento), 1, arq);
						
						printf("  Quantidade: %d\n\n", equipamentos.quantidade);
				
						printf("Informe a Nova Quantidade: ");
						scanf("%d", &equipamentos.quantidade);
						
						equipamentos.vTotal = equipamentos.valor * equipamentos.quantidade;
						
						fseek(arq, pos, SEEK_SET);
						fwrite(&equipamentos, sizeof(Equipamento), 1, arq);
						printf("\n[SUCESSO] Alteração Realizada com Sucesso!\n");
					
						system("pause");
						system("cls");
					break;
					default:
						printf("\n Opção Inválida!");
					break;
				}
				
				fclose(arq);
			}	
		}
	} while(opcao !=0);	
}

// Funções de Exclusão
void excluirTreino() {
	FILE *arq, *temp;
	FILE *arqProfissional, *arqAlunos;
	
	int i, pos, codigo=0;
	
	system("cls");
	printf("==============================================\n");
	printf("  EXCLUIR - Treinos\n");
	printf("==============================================\n\n");
	
	arq = fopen("treinos.bin", "rb");
	if(arq == NULL) {
		printf("Erro na Abertura do Arquivo!\n\n");
	} else {
		// verifica se o codigo do treino existe.
		do {	
			printf("Informe o Código do Treino: ");
			scanf("%d", &treinos.codigo);
		    validarCodigo(treinos.codigo);
						
		} while(treinos.codigo < 1 || treinos.codigo > 1000);
							
		pos = busca(arq, treinos.codigo, ENT_TREINO);
		if(pos == -1) {
			printf("[ERRO] Código não encontrado!\n\n");
							
			fclose(arq);
			system("pause");
			system("cls");
			return;
		} 
		
		// lê as informações especificas do respectivo treino.
		fseek(arq, pos, SEEK_SET);
		fread(&treinos, sizeof(Treino), 1, arq);
		
		codigo = treinos.codigo;
		
		printf("\n==============================================\n");
		printf("Código do Treino: %d\n\n", treinos.codigo);
		
		arqAlunos = fopen("alunos.bin", "rb");
		printf("Código do Aluno: %d\n", treinos.codAluno);
		if(arqAlunos == NULL) {
			printf("Erro na Abertura do Arquivo!\n\n");
		} else {
			rewind(arqAlunos);
			while(fread (&alunos, sizeof(Aluno), 1, arqAlunos) == 1) {
				if(alunos.codigo == treinos.codAluno) {
				    printf("  Nome do Aluno: %s\n\n", alunos.nome);
				}
			}
		}
		
		arqProfissional = fopen("profissionais.bin", "rb");	
		printf("Código do Profissional: %d\n", treinos.codProfissional);
		if(arqProfissional == NULL) {
			printf("Erro na Abertura do Arquivo!\n\n");
		} else {
			rewind(arqProfissional);
			while(fread (&profissionais, sizeof(Profissional), 1, arqProfissional) == 1) {
				if(profissionais.codigo == treinos.codProfissional) {
					printf("  Nome do Profissional: %s\n\n", profissionais.nome);
				}
			}
		}
				
		printf("Nome do Treino: %s\n", treinos.nome);
		printf("Objetivo do Treino: %s\n", treinos.objetivo);
				
		printf("Exercícios:\n");
		for(i=0; i<treinos.quantidade; i++) {
			printf("  %d) Exercício: %s\n", i+1, treinos.exercicios[i]);
		}
					
		printf("\nCriado em: %d/%d/%d\n", treinos.dataCriacao.dia, treinos.dataCriacao.mes, treinos.dataCriacao.ano);
		printf("Ativo: ");
		if(treinos.ativo == 1) {
			printf("Sim\n");
		} else {
			printf("Não\n");
		}
		printf("==============================================\n");
		
		printf("\nDeseja Excluir o Treino? (S/N): ");
		if(toupper(getche()) == 'S') {
			temp = fopen("auxiliar.bin", "wb");
			if(temp == NULL) {
				printf("Erro na Abertura do Arquivo!\n\n");
			} else {
				rewind(arq);
			
				while(fread(&treinos, sizeof(Treino), 1, arq) == 1) {
					if(treinos.codigo != codigo) {
						fwrite(&treinos, sizeof(Treino), 1, temp);
					}
				}
				fclose(arq);
				fclose(temp);
				if(arqAlunos != NULL) fclose(arqAlunos);
				if(arqProfissional != NULL) fclose(arqProfissional);
				remove("treinos.bin");
				rename("auxiliar.bin", "treinos.bin");
				printf("\n\n[SUCESSO]Treino Excluído com Sucesso!\n");
			} 						
		} else {
			printf("\n\n[AVISO] Exclusão Cancelada!\n");
				
			if(arqAlunos != NULL) fclose(arqAlunos);
			if(arqProfissional != NULL) fclose(arqProfissional);					
			fclose(arq);
			system("pause");
			system("cls");
			return;
		}
		system("pause");
	}
}

void excluirEquipamento() {
	FILE *arq, *temp;
	Equipamento equipamentos;
	
	int pos, codigo;
	
	system("cls");
	printf("==============================================\n");
	printf("  EXCLUIR - Equipamentos\n");
	printf("==============================================\n\n");
	
	arq = fopen("equipamentos.bin", "rb");
	if(arq == NULL) {
		printf("Erro na Abertura do Arquivo!\n\n");
	} else {
		// verifica se o codigo do treino existe.
		do {	
			printf("Informe o Código do Equipamento: ");
			scanf("%d", &equipamentos.codigo);
		    validarCodigo(equipamentos.codigo);
						
		} while(equipamentos.codigo < 1 || equipamentos.codigo > 1000);
							
		pos = busca(arq, equipamentos.codigo, ENT_EQUIPAMENTO);
		if(pos == -1) {
			printf("[ERRO] Código não encontrado!\n\n");
							
			fclose(arq);
			system("pause");
			system("cls");
			return;
		} 
		codigo = equipamentos.codigo;
		
		// lê as informações especificas do respectivo treino.
		fseek(arq, pos, SEEK_SET);
		fread(&equipamentos, sizeof(Equipamento), 1, arq);
		
		printf("\n==============================================\n");
		printf("Código do Equipamento: %d\n\n", equipamentos.codigo);
				
		printf("Nome do Equipamento: %s\n", equipamentos.nome);
		printf("Tipo do Equipamento: %s\n", equipamentos.tipo);
		printf("Data de Aquisição: %d/%d/%d\n\n", equipamentos.dataAquisicao.dia, equipamentos.dataAquisicao.mes, equipamentos.dataAquisicao.ano);
				
		printf("Quantidade: %d\n", equipamentos.quantidade);
		printf("Valor do Equipamento: %.2f\n\n", equipamentos.valor);
				
		printf("Situação: \n");
		printf("  Opção: ");
		if(equipamentos.situacao == 1) {
			printf("Bom\n\n");
		} else if(equipamentos.situacao == 2) {
			printf("Manutenção\n\n");
		} else if(equipamentos.situacao == 3) {
			printf("Inativo\n\n");
		}
		
		printf("Data da Última Manutenção: %d/%d/%d\n", equipamentos.dataUltimaManutencao.dia, equipamentos.dataUltimaManutencao.mes, equipamentos.dataUltimaManutencao.ano);
				
		printf("Ativo: ");
		if(equipamentos.ativo == 1) {
			printf("Sim\n");
		} else {
			printf("Não\n");
		}
		printf("\n==============================================\n");
		
		printf("\nDeseja Excluir o Equipamento? (S/N): ");
		if(toupper(getche()) == 'S') {
			temp = fopen("auxiliar.bin", "wb");
			if(temp == NULL) {
				printf("Erro na Abertura do Arquivo!\n\n");
			} else {
				rewind(arq);
			
				while(fread(&equipamentos, sizeof(Equipamento), 1, arq) == 1) {
					if(equipamentos.codigo != codigo) {
						fwrite(&equipamentos, sizeof(Equipamento), 1, temp);
					}
				}
				fclose(arq);
				fclose(temp);
	
				remove("equipamentos.bin");
				rename("auxiliar.bin", "equipamentos.bin");
				printf("\n\n[SUCESSO] Equipamento Excluído com Sucesso!\n");
			} 						
		} else {
			printf("\n\n[AVISO] Exclusão Cancelada!\n");
									
			fclose(arq);
			system("pause");
			system("cls");
			return;
		}
		system("pause");
	}
}

// Funções de Listagem
void listarTreinos() {
	FILE *arq;
	arq = fopen("treinos.bin", "rb");
	
	FILE *arqAluno;
	arqAluno = fopen("alunos.bin", "rb");
	
	FILE *arqProfissional;
	arqProfissional = fopen("profissionais.bin", "rb");
	
	int i;
	system("cls");
	printf("==============================================\n");
	printf("  LISTAGEM - Treinos\n");
	printf("==============================================\n");
	
	if(arq == NULL) {
		printf("Erro na Abertura do Arquivo!\n\n");
	} else {
		
		fseek(arq, 0, SEEK_END);
		if(ftell(arq) == 0) {
		    printf("\n[ERRO] Nenhum Dado foi Encontrado!\n\n");
			system("pause");
			system("cls");
			return;
		} else {
			
			fseek(arq, 0, SEEK_SET); // Retorna ao inicio do arquivo
			while(fread(&treinos, sizeof(Treino), 1, arq) == 1) {
				printf("Código do Treino: %d\n\n", treinos.codigo);
				printf("Código do Aluno: %d\n", treinos.codAluno);
				
				if(arqAluno == NULL) {
					printf("Erro na Abertura do Arquivo!\n\n");
				} else {
					rewind(arqAluno);
					while(fread (&alunos, sizeof(Aluno), 1, arqAluno) == 1) {
						if(alunos.codigo == treinos.codAluno) {
							printf("  Nome do Aluno: %s\n\n", alunos.nome);
						}
					}
				}
				
				printf("Código do Profissional: %d\n", treinos.codProfissional);
				
				if(arqProfissional == NULL) {
					printf("Erro na Abertura do Arquivo!\n\n");
				} else {
					rewind(arqProfissional);
					while(fread (&profissionais, sizeof(Profissional), 1, arqProfissional) == 1) {
						if(profissionais.codigo == treinos.codProfissional) {
							printf("  Nome do Profissional: %s\n\n", profissionais.nome);
						}
					}
				}
				
				printf("Nome do Treino: %s\n", treinos.nome);
				printf("Objetivo do Treino: %s\n", treinos.objetivo);
				
				printf("Exercícios:\n");
				for(i=0; i<treinos.quantidade; i++) {
					printf("  %d) Exercício: %s\n", i+1, treinos.exercicios[i]);
				}
					
				printf("\nCriado em: %d/%d/%d\n", treinos.dataCriacao.dia, treinos.dataCriacao.mes, treinos.dataCriacao.ano);
	
				printf("Ativo: ");
				if(treinos.ativo == 1) {
					printf("Sim\n");
				} else {
				    printf("Não\n");
				}
				printf("==============================================\n");
			}
	    }
	}
	
	fclose(arq);
	system("pause");
}

// Listar Equipamentos
void listarEquipamentos() {
	FILE *arq;
	Equipamento equipamentos;
	
	int i, encontrados, opcao;
	
	system("cls");
	arq = fopen("equipamentos.bin", "rb");
	if(arq == NULL) {
		printf("Erro na Abertura do Arquivo!\n\n");
	} else  {
		fseek(arq, 0, SEEK_END);
		if(ftell(arq) == 0) {
		    printf("\n[ERRO] Nenhum Dado foi Encontrado!\n\n");
			system("pause");
			system("cls");
			return;
		} else {
			do {
			    system("cls");
			    printf("==============================================\n");
			    printf("  LISTAGEM - Equipamentos\n");
			    printf("==============================================\n\n");
			    printf("  [1]  Listar Equipamentos Bons\n");
			    printf("  [2]  Listar Equipamentos em Manutenção\n");
			    printf("  [3]  Listar Equipamentos Inativos\n");
			    printf("  [4]  Listar Todos os Equipamentos\n");
			    printf("  [0]  Voltar\n\n");
			    printf("----------------------------------------------\n");
			    printf("  Opção: ");
			    scanf("%d", &opcao);
			
			    system("cls");
			    switch (opcao) {
			        case 1:
			            encontrados = 0;
			            fseek(arq, 0, SEEK_SET);
			            while (fread(&equipamentos, sizeof(Equipamento), 1, arq) == 1) {
			                if (equipamentos.situacao == 1) {
			                    printf("Código do Equipamento: %d\n\n", equipamentos.codigo);
			
			                    printf("Nome do Equipamento: %s\n", equipamentos.nome);
			                    printf("Tipo do Equipamento: %s\n", equipamentos.tipo);
			                    printf("Data de Aquisição: %d/%d/%d\n\n", equipamentos.dataAquisicao.dia, equipamentos.dataAquisicao.mes, equipamentos.dataAquisicao.ano);
			
			                    printf("Quantidade: %d\n", equipamentos.quantidade);
			                    printf("Valor do Equipamento: %.2f\n", equipamentos.valor);
			                    printf("Valor Total dos Equipamentos: R$%.2f\n\n", equipamentos.vTotal);
			
			                    printf("Situação: Bom\n\n");
			
			                    printf("Data da Última Manutenção: %d/%d/%d\n", equipamentos.dataUltimaManutencao.dia, equipamentos.dataUltimaManutencao.mes, equipamentos.dataUltimaManutencao.ano);
			
			                    printf("Ativo: %s\n", equipamentos.ativo == 1 ? "Sim" : "Não");
			                    printf("==============================================\n");
			
			                    encontrados++;
			                }
			            }
			            if (encontrados == 0) {
			                printf("[ERRO] Nenhum equipamento cadastrado.\n\n");
			            }
			            system("pause");
			            break;
			
			        case 2:
			            encontrados = 0;
			            fseek(arq, 0, SEEK_SET);
			            while (fread(&equipamentos, sizeof(Equipamento), 1, arq) == 1) {
			                if (equipamentos.situacao == 2) {
			                    printf("Código do Equipamento: %d\n\n", equipamentos.codigo);
			
			                    printf("Nome do Equipamento: %s\n", equipamentos.nome);
			                    printf("Tipo do Equipamento: %s\n", equipamentos.tipo);
			                    printf("Data de Aquisição: %d/%d/%d\n\n", equipamentos.dataAquisicao.dia, equipamentos.dataAquisicao.mes, equipamentos.dataAquisicao.ano);
			
			                    printf("Quantidade: %d\n", equipamentos.quantidade);
			                    printf("Valor do Equipamento: %.2f\n", equipamentos.valor);
			                    printf("Valor Total dos Equipamentos: R$%.2f\n\n", equipamentos.vTotal);
			
			                    printf("Situação: Manutenção\n\n");
			
			                    printf("Data da Última Manutenção: %d/%d/%d\n", equipamentos.dataUltimaManutencao.dia, equipamentos.dataUltimaManutencao.mes, equipamentos.dataUltimaManutencao.ano);
			
			                    printf("Ativo: %s\n", equipamentos.ativo == 1 ? "Sim" : "Não");
			                    printf("==============================================\n");
			
			                    encontrados++;
			                }
			            }
			            if (encontrados == 0) {
			                printf("[ERRO] Nenhum equipamento cadastrado.\n\n");
			            }
			            system("pause");
			            break;
			
			        case 3:
			            encontrados = 0;
			            fseek(arq, 0, SEEK_SET);
			            while (fread(&equipamentos, sizeof(Equipamento), 1, arq) == 1) {
			                if (equipamentos.situacao == 3) {
			                    printf("Código do Equipamento: %d\n\n", equipamentos.codigo);
			
			                    printf("Nome do Equipamento: %s\n", equipamentos.nome);
			                    printf("Tipo do Equipamento: %s\n", equipamentos.tipo);
			                    printf("Data de Aquisição: %d/%d/%d\n\n", equipamentos.dataAquisicao.dia, equipamentos.dataAquisicao.mes, equipamentos.dataAquisicao.ano);
			
			                    printf("Quantidade: %d\n", equipamentos.quantidade);
			                    printf("Valor do Equipamento: %.2f\n", equipamentos.valor);
			                    printf("Valor Total dos Equipamentos: R$%.2f\n\n", equipamentos.vTotal);
			
			                    printf("Situação: Inativo\n\n");
			
			                    printf("Data da Última Manutenção: %d/%d/%d\n", equipamentos.dataUltimaManutencao.dia, equipamentos.dataUltimaManutencao.mes, equipamentos.dataUltimaManutencao.ano);
			
			                    printf("Ativo: %s\n", equipamentos.ativo == 1 ? "Sim" : "Não");
			                    printf("==============================================\n");
			
			                    encontrados++;
			                }
			            }
			            if (encontrados == 0) {
			                printf("[ERRO] Nenhum equipamento cadastrado.\n\n");
			            }
			            system("pause");
			            break;
			
			        case 4:
			            encontrados = 0;
			            fseek(arq, 0, SEEK_SET);
			            while (fread(&equipamentos, sizeof(Equipamento), 1, arq) == 1) {
			                printf("Código do Equipamento: %d\n\n", equipamentos.codigo);
			
			                printf("Nome do Equipamento: %s\n", equipamentos.nome);
			                printf("Tipo do Equipamento: %s\n", equipamentos.tipo);
			                printf("Data de Aquisição: %d/%d/%d\n\n", equipamentos.dataAquisicao.dia, equipamentos.dataAquisicao.mes, equipamentos.dataAquisicao.ano);
			
			                printf("Quantidade: %d\n", equipamentos.quantidade);
			                printf("Valor do Equipamento: %.2f\n", equipamentos.valor);
			                printf("Valor Total dos Equipamentos: R$%.2f\n\n", equipamentos.vTotal);
			
			                printf("Situação: ");
			                if (equipamentos.situacao == 1) {
			                    printf("Bom\n\n");
			                } else if (equipamentos.situacao == 2) {
			                    printf("Manutenção\n\n");
			                } else if (equipamentos.situacao == 3) {
			                    printf("Inativo\n\n");
			                }
			
			                printf("Data da Última Manutenção: %d/%d/%d\n", equipamentos.dataUltimaManutencao.dia, equipamentos.dataUltimaManutencao.mes, equipamentos.dataUltimaManutencao.ano);
			
			                printf("Ativo: %s\n", equipamentos.ativo == 1 ? "Sim" : "Não");
			                printf("==============================================\n");
			
			                encontrados++;
			            }
			            if (encontrados == 0) {
			                printf("[ERRO] Nenhum equipamento cadastrado.\n\n");
			            }
			            system("pause");
			            break;
			
			        case 0:
			            break;
			
			        default:
			            printf("Opção inválida!\n");
			            system("pause");
			    }
			} while (opcao != 0);
		}
		fclose(arq);
		system("pause");
		system("cls");
	}
}

// Cadastrar Treinos
void cadastrarTreino() {
	int i, quant, pos, ok;
	
	FILE *arq;
	
	system("cls");
	if(!temAlunoAtivo()) {
		printf("\nNão Existem Alunos Ativos Cadastrados!\n\n");
		system("pause");
		return;
	}
	if(!temProfissionalAtivo()) {
		printf("\nNão Existem Profissionais Ativos Cadastrados!\n\n");
		system("pause");
		return;
	}
	arq = fopen("treinos.bin", "ab+");
	if(arq == NULL) {
		printf("Erro na Abertura do Arquivo!\n\n");
	} else {
		
		// verifica se o codigo do treino existe.
		do {
			do {	
				printf("Informe o Código do Treino: ");
				scanf("%d", &treinos.codigo);
			    validarCodigo(treinos.codigo);
							
			} while(treinos.codigo < 1 || treinos.codigo > 1000);
								
			pos = busca(arq, treinos.codigo, ENT_TREINO);
			if(pos != -1) printf("Código já Cadastrado!\n\n");
		} while(pos != -1);
		
		// Verifica se o aluno existe ou não
		do {
			printf("Informe o Código do Aluno: ");
			scanf("%d", &treinos.codAluno);
			ok = existeAlunoAvaliacao(treinos.codAluno);
			if(!ok) printf("Aluno não Encontrado ou Inativo!\n\n");
		} while(!ok);
		
		// Verifica se o profissional existe ou não
		do {
			printf("Informe o Código do Profissional: ");
			scanf("%d", &treinos.codProfissional);
			ok = existeProfissionalAvaliacao(treinos.codProfissional);
			if(!ok) printf("Profissional não Encontrado ou Inativo!\n\n");
		} while(!ok);
		
		fflush(stdin);
		printf("Informe o Nome do Treino: ");
		fgets(treinos.nome, 31, stdin);
		if(validaExcedeu(treinos.nome) == 1) printf("Somente os Primeiros 30 Caracteres Serão Cadastrados!\n\n");
		
		fflush(stdin);
		printf("Informe o Objetivo do Treino: ");
		fgets(treinos.objetivo, 61, stdin);
		if(validaExcedeu(treinos.objetivo) == 1) printf("Somente os Primeiros 60 Caracteres Serão Cadastrados!\n\n");
		
		do {
			printf("\nInforme Quantos Exercícios Deseja Cadastrar: ");
			scanf("%d", &treinos.quantidade);
			getchar();
			
			if(treinos.quantidade>0 && treinos.quantidade <=10) {
				for(i=0; i<treinos.quantidade; i++) {
					printf("  %d) Exercício: ", i+1);
				    fgets(treinos.exercicios[i], 41, stdin);
				    if(validaExcedeu(treinos.exercicios[i]) == 1) printf("Somente os Primeiros 40 Caracteres Serão Cadastrados!\n\n");
				}
			} else {
				printf("[AVISO] A Quantidade Máxima de Exercícios por Treino é 10!!\n\n");
			}
		} while(treinos.quantidade<=0 || treinos.quantidade>10);
		
		do {
			printf("\nInforme a Data de Criação (DD MM AAAA): ");
			scanf("%d%d%d", &treinos.dataCriacao.dia, &treinos.dataCriacao.mes, &treinos.dataCriacao.ano);
			if(validaData(treinos.dataCriacao) == 0) printf("Insira Uma Data Valida!\n");
		} while(validaData(treinos.dataCriacao) != 1);
		
		treinos.ativo = 1;
	
		fwrite(&treinos, sizeof(Treino), 1, arq);
		printf("\n[SUCESSO] Treino cadastrado!\n");
		system("pause");
		system("cls");
		fclose(arq);
	}
}

// Cadastrar Equipamentos
void cadastrarEquipamento() {
	FILE *arq;
	Equipamento equipamentos;
	
	int pos;
	
	system("cls");
	arq = fopen("equipamentos.bin", "ab+");
	if(arq == NULL) {
		printf("Erro na Abertura do Arquivo!\n\n");
	} else  {
		// verifica se o codigo do treino existe.
		do {	
			printf("Informe o Código do Equipamento: ");
			scanf("%d", &equipamentos.codigo);
		    validarCodigo(equipamentos.codigo);
						
		} while(equipamentos.codigo < 1 || equipamentos.codigo > 1000);
							
		pos = busca(arq, equipamentos.codigo, ENT_EQUIPAMENTO);
		if(pos != -1) {
			printf("[ERRO] Código já cadastrado!\n\n");
							
			fclose(arq);
			system("pause");
			system("cls");
			return;
		}
		
		fflush(stdin);
		printf("\nInforme o Nome do Equipamento: ");
		fgets(equipamentos.nome, 61, stdin);
		if(validaExcedeu(equipamentos.nome) == 1) printf("Somente os Primeiros 60 Caracteres Serão Cadastrados!\n\n");
		
		fflush(stdin);
		printf("Informe o Tipo do Equipamento: ");
		fgets(equipamentos.tipo, 31, stdin);
		if(validaExcedeu(equipamentos.tipo) == 1) printf("Somente os Primeiros 30 Caracteres Serão Cadastrados!\n\n");
		
		printf("Informe a Data de Aquisição (DD MM AAAA): ");
		scanf("%d%d%d", &equipamentos.dataAquisicao.dia, &equipamentos.dataAquisicao.mes, &equipamentos.dataAquisicao.ano);
		
		do {
			printf("\nInforme a Quantidade: ");
		    scanf("%d", &equipamentos.quantidade);
		    
		    if(equipamentos.quantidade <= 0) {
		    	printf("[ERRO] A Quantidade deve ser Superior à 0!\n\n");
		    }
		} while(equipamentos.quantidade <= 0);
		
		do {
			printf("Informe o Valor do Equipamento: ");
			scanf("%f", &equipamentos.valor);
		    
		    if(equipamentos.valor <= 0) {
		    	printf("[ERRO] O valor deve ser Superior à 0!\n\n");
		    }
		} while(equipamentos.valor <= 0);
		
		printf("\nSituação: 1 = Bom | 2 = Manutenção | 3 = Inativo\n");
		printf("  Opção: ");
		scanf("%d", &equipamentos.situacao);
		
		printf("\nInforme da Última Manutenção (DD MM AAAA): ");
		scanf("%d%d%d", &equipamentos.dataUltimaManutencao.dia, &equipamentos.dataUltimaManutencao.mes, &equipamentos.dataUltimaManutencao.ano);
		
		equipamentos.vTotal = equipamentos.valor * equipamentos.quantidade;
		equipamentos.ativo = 1;
		
		fwrite(&equipamentos, sizeof(Equipamento), 1, arq);
		printf("\n[SUCESSO] Equipamento cadastrado!\n");
		system("pause");
		system("cls");
		fclose(arq);
	}
}

// ===== FIM DO MENU TREINOS E EQUIPAMENTOS =====

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
						alterarAluno();
						break;
					case ENT_PROFISSIONAL:
						alterarProfissional();
						break;
					case ENT_TREINO:
						alterarTreino();
						break;
					case ENT_PLANO:
						alterarPlano();
						break;
					case ENT_MATRICULA:
						avisoMatricula();
						break;
					case ENT_AVALIACAO:
						alterarAvaliacao();
						break;
					case ENT_AGENDA:
						alterarAgenda();
						break;
					case ENT_EQUIPAMENTO:
						alterarEquipamento();
						break;
					case ENT_PACOTE:
						alterarPacote();
						break;
					case ENT_ATIVIDADE:
						alterarAtividade();
						break;
				}
				break;

			case 2:
				// ========== EXCLUIR: ============ (OBS: Matrícula não é excluída: chama o cancelamento)
				switch(codEntidade) {
					case ENT_ALUNO:
						excluirAluno();
						break;
					case ENT_PROFISSIONAL:
						excluirProfissional();
						break;
					case ENT_TREINO:
						excluirTreino();
						break;
					case ENT_PLANO:
						excluirPlano();
						break;
					case ENT_MATRICULA:
						efetuarCancelamento();
						break;
					case ENT_AVALIACAO:
						excluirAvaliacao();
						break;
					case ENT_AGENDA:
						excluirAgenda();
						break;
					case ENT_EQUIPAMENTO:
						excluirEquipamento();
						break;
					case ENT_PACOTE:
						excluirPacote();
						break;
					case ENT_ATIVIDADE:
						excluirAtividade();
						break;
				}
				break;

			case 3:
				// =========== RELATÓRIO / LISTAR: =============
				switch(codEntidade) {
					case ENT_ALUNO:
						listarAlunos();
						break;
					case ENT_PROFISSIONAL:
						listarProfissionais();
						break;
					case ENT_TREINO:
						listarTreinos();
						break;
					case ENT_PLANO:
						listarPlanos();
						break;
					case ENT_MATRICULA:
						listarMatriculas();
						break;
					case ENT_AVALIACAO:
						listarAvaliacoes();
						break;
					case ENT_AGENDA:
						listarAgenda();
						break;
					case ENT_EQUIPAMENTO:
						listarEquipamentos();
						break;
					case ENT_PACOTE:
						listarPacotes();
						break;
					case ENT_ATIVIDADE:
						listarAtividades();
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
	FILE *arq;
	int OpcoesCadastro, pos, ok;

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
				arq = fopen("alunos.bin", "ab+");
				if(arq == NULL) printf("Erro na Abertura do Arquivo\n\n");
				else {
					do {
						do {
							printf("Informe a Matrícula do Aluno: ");
						scanf("%d", &alunos.codigo);
							if(alunos.codigo < 1 || alunos.codigo > 1000) printf("A Matrícula Deve Estar Entre 1 e 1000!\n\n");
						} while(alunos.codigo < 1 || alunos.codigo > 1000);
						
						pos = busca(arq, alunos.codigo, ENT_ALUNO);
						if(pos != -1) printf("Matrícula Já Cadastrada!\n\n");
					} while(pos != -1);
					
					fflush(stdin);
					printf("Informe o Nome do Aluno: ");
					fgets(alunos.nome, 61, stdin);
					if(validaExcedeu(alunos.nome) == 1) printf("Somente os Primeiros 60 Caracteres Serão Cadastrados!\n\n");
					
					do {
						fflush(stdin);
						printf("Informe o CPF do Aluno: ");
						fgets(alunos.cpf, 15, stdin);
						alunos.cpf[15] = '\0';
						
						if(validaCPF(alunos.cpf) == 0) printf("Formato de CPF Inválido! \nInsira este Formato XXX.XXX.XXX-XX \nPreencha no Lugar do X Somente Números!\n\n");
					} while(validaCPF(alunos.cpf) != 1 || cpfDuplicado(alunos.cpf, ENT_ALUNO, alunos.codigo));
				
					do {
						printf("Informe a Data de Nascimento (DD MM AAAA): ");
						scanf("%d%d%d", &alunos.dataNascimento.dia, &alunos.dataNascimento.mes, &alunos.dataNascimento.ano);
								
						if(validaData(alunos.dataNascimento) == 0) printf("Insira Uma Data Valida! \nAno Entre 1900 e 2100 \nMês Entre 1 e 12 \nE os Dias Válidos de Cada Mês\n\n");
					} while(validaData(alunos.dataNascimento) != 1);
					
					do {
						fflush(stdin);
						printf("Informe o Número de Telefone: ");
						fgets(alunos.telefone, 17, stdin);
						alunos.telefone[strlen(alunos.telefone)-1] = '\0';
						
						if(validaTelefone(alunos.telefone) == 0) printf("Formato de Telefone Inválido! \nInsira este Formato (XX) XXXX-XXXX ou (XX) XXXXX-XXXX \nPreencha no Lugar do X Somente Números!\n\n");
					} while(validaTelefone(alunos.telefone) != 1);
					
					do {
						fflush(stdin);
						printf("Informe um E-mail de Contato: ");
						fgets(alunos.email, 61, stdin);
						if(validaExcedeu(alunos.email) == 1) printf("Somente os Primeiros 60 Caracteres Serão Cadastrados!\n\n");
						
						if(validaEmail(alunos.email) == 0) printf("Email Deve conter um @ e um . depois Dele \nAntes do @ só pode Conter - ou _ \nDepois do @ Apenas .\n\n");
					} while(validaEmail(alunos.email) != 1);
					
					alunos.ativo = 1;
					fwrite(&alunos, sizeof(Aluno), 1, arq);
				}
				fclose(arq);
				break;
			case 2:
				// Profissionais
				arq = fopen("profissionais.bin", "ab+");
				if(arq == NULL) printf("Erro na Abertura do Arquivo\n\n");
				else {
					do {
						do {
							printf("Informe a Matrícula do Profissional: ");
							scanf("%d", &profissionais.codigo);
							if(profissionais.codigo < 1 || profissionais.codigo > 1000) printf("A Matrícula Deve Estar Entre 1 e 1000!\n\n");
						} while(profissionais.codigo < 1 || profissionais.codigo > 1000);
						
						pos = busca(arq, profissionais.codigo, ENT_PROFISSIONAL);
						if(pos != -1) printf("Matrícula Já Cadastrada!\n\n");
					} while(pos != -1);
					
					fflush(stdin);
					printf("Informe o Nome do Profissional: ");
					fgets(profissionais.nome, 61, stdin);
					if(validaExcedeu(profissionais.nome) == 1) printf("Somente os Primeiros 60 Caracteres Serão Cadastrados!\n\n");
					
					do {
						fflush(stdin);
						printf("Informe o CPF do Profissional: ");
						fgets(profissionais.cpf, 15, stdin);
						profissionais.cpf[15] = '\0';
						
						if(validaCPF(profissionais.cpf) == 0) printf("Formato de CPF Inválido! \nInsira este Formato XXX.XXX.XXX-XX \nPreencha no Lugar do X Somente Números!\n\n");
					} while(validaCPF(profissionais.cpf) != 1 || cpfDuplicado(profissionais.cpf, ENT_PROFISSIONAL, profissionais.codigo));
					
					do {
						fflush(stdin);
						printf("Informe o Telefone do Profissional: ");
						fgets(profissionais.telefone, 17, stdin);
						profissionais.telefone[strlen(profissionais.telefone)-1] = '\0'; // Substitui \n por \0
						
						if(validaTelefone(profissionais.telefone) == 0) printf("Formato de Telefone Inválido! \nInsira este Formato (XX) XXXX-XXXX ou (XX) XXXXX-XXXX \nPreencha no Lugar do X Somente Números!\n\n");
					} while(validaTelefone(profissionais.telefone) != 1);
					
					fflush(stdin);
					printf("Informe a Função do Profissional: ");
					fgets(profissionais.funcao, 31, stdin);
					if(validaExcedeu(profissionais.funcao) == 1) printf("Somente os Primeiros 30 Caracteres Serão Cadastrados!\n\n");
					
					profissionais.ativo = 1;
					fwrite(&profissionais, sizeof(Profissional), 1, arq);
				}
				fclose(arq);
				break;
			case 3:
				//Treinos
				cadastrarTreino();
				break;
			case 4:
				//Planos
				cadastrarPlano();
				break;
			case 5:
				if(!temAlunoAtivo()) 
					printf("\nNão Existem Alunos Ativos Cadastrados!\n\n");
				else 
                if(!temProfissionalAtivo()) 
					printf("\nNão Existem Profissionais Ativos Cadastrados!\n\n");
				else 
                {
					arq = fopen("avaliacoes.bin", "ab+");
					if(arq == NULL) printf("Erro na Abertura do Arquivo!\n\n");
					else {
						do {
							do {
								printf("Informe o Código da Avaliação: ");
								scanf("%d", &avaliacoes.codigo);
								if(avaliacoes.codigo < 1 || avaliacoes.codigo > 1000) printf("O Código Deve Estar Entre 1 e 1000!\n\n");
							} while(avaliacoes.codigo < 1 || avaliacoes.codigo > 1000);

							pos = busca(arq, avaliacoes.codigo, ENT_AVALIACAO);
							if(pos != -1) printf("Código Já Cadastrado!\n\n");
						} while(pos != -1);

						do {
							printf("Informe o Código do Aluno: ");
							scanf("%d", &avaliacoes.codAluno);
							ok = existeAlunoAvaliacao(avaliacoes.codAluno);
							if(!ok) printf("Aluno Não Encontrado ou Inativo!\n\n");
						} while(!ok);

						do {
							printf("Informe o Código do Profissional: ");
							scanf("%d", &avaliacoes.codProfissional);
							ok = existeProfissionalAvaliacao(avaliacoes.codProfissional);
							if(!ok) printf("Profissional Não Encontrado ou Inativo!\n\n");
						} while(!ok);

						do {
							printf("Informe a Data da Avaliação (DD MM AAAA): ");
							scanf("%d%d%d", &avaliacoes.data.dia, &avaliacoes.data.mes, &avaliacoes.data.ano);
							if(validaData(avaliacoes.data) == 0) printf("Data Inválida!\n\n");
						} while(validaData(avaliacoes.data) != 1);

						do {
							printf("Informe o Peso (KG): ");
							scanf("%f", &avaliacoes.peso);
							if(avaliacoes.peso < 1 || avaliacoes.peso > 500) printf("O Peso Deve Estar Entre 1 e 500 KG!\n\n");
						} while(avaliacoes.peso < 1 || avaliacoes.peso > 500);

						do {
							printf("Informe a Altura (M): ");
							scanf("%f", &avaliacoes.altura);
							if(avaliacoes.altura < 0.5 || avaliacoes.altura > 2.5) printf("A Altura Deve Estar Entre 0.5 e 2.5 M!\n\n");
						} while(avaliacoes.altura < 0.5 || avaliacoes.altura > 2.5);
						avaliacoes.imc = avaliacoes.peso / (avaliacoes.altura * avaliacoes.altura);
						do {
							fflush(stdin);
							printf("Informe o Percentual de Gordura: ");
							scanf("%f", &avaliacoes.percGordura);
							if(avaliacoes.percGordura < 0 || avaliacoes.percGordura > 100) printf("Percentual Inválido! (0 a 100)\n\n");
						} while(avaliacoes.percGordura < 0 || avaliacoes.percGordura > 100);
						fflush(stdin);
						printf("Observações: ");
						fgets(avaliacoes.observacoes, 101, stdin);
						if(validaExcedeu(avaliacoes.observacoes) == 1) 
							printf("Somente os Primeiros 100 Caracteres Serão Cadastrados!\n\n");
						avaliacoes.ativo = 1;
						fseek(arq, 0, SEEK_END);
						fwrite(&avaliacoes, sizeof(Avaliacao), 1, arq);
						fclose(arq);
						printf("\n==============================================\n");
						printf("     AVALIAÇÃO CADASTRADA COM SUCESSO!\n");
						printf("==============================================\n");
						printf("IMC: %.2f\n\n", avaliacoes.imc);
					}
				}
				break;
			case 6:
				//Pacotes de Modalidades
				cadastrarPacote();
				break;
			case 7:
				//Agenda de Profissionais
				cadastrarAgenda();
				break;
			case 8:
				//Equipamentos
				cadastrarEquipamento();
				break;
			case 9:
				//Atividades
				cadastrarAtividade();
				break;

			case 0:
				printf("\n  Voltando...\n");
				break;

			default:
				printf("\n Opção Inválida!");
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
				listarFrequencia();
				break;
			case 2:
				fazerChamada();
				break;
			case 3:
				alterarFrequencia();
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
				faturamentoMensal();
				break;
			case 2:
				faturamentoPorPlano();
				break;
			case 3:
				gastosMensais();
				break;
			case 4:
				pagamentosPorAluno();
				break;
			case 5:
				cadastrarGasto();
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
				efetuarMatricula();
				break;
			case 2:
				efetuarRecebimento();
				break;
			case 3:
				efetuarCancelamento();
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

// ===== OPERAÇÕES: MATRÍCULA, RECEBIMENTO E CANCELAMENTO (regras do ERS da Project Alpha) =====

// Último dia do mês (considera ano bissexto).
int ultimoDiaMes(int mes, int ano) {
	switch(mes) {
		case 4:
		case 6:
		case 9:
		case 11:
			return 30;
		case 2:
			if((ano % 4 == 0 && ano % 100 != 0) || ano % 400 == 0) return 29;
			return 28;
	}
	return 31;
}

// Soma meses a uma data. Se o dia não existir no mês novo (ex.: 31/02), usa o último dia do mês.
Data somarMeses(Data d, int meses) {
	Data r;
	int total;

	total = (d.mes - 1) + meses;
	r.ano = d.ano + total / 12;
	r.mes = total % 12 + 1;
	r.dia = d.dia;
	if(r.dia > ultimoDiaMes(r.mes, r.ano)) r.dia = ultimoDiaMes(r.mes, r.ano);
	return r;
}

// Transforma a data em número (AAAAMMDD) para poder comparar duas datas.
int dataNumero(Data d) {
	return d.ano * 10000 + d.mes * 100 + d.dia;
}

// Código automático: quantidade de registros do arquivo + 1 (usado onde o código não é digitado).
int proximoCodigo(char arquivo[], int tamanho) {
	FILE *arq;
	int qtde = 0;

	arq = fopen(arquivo, "rb");
	if(arq != NULL) {
		fseek(arq, 0, SEEK_END);
		qtde = ftell(arq) / tamanho;
		fclose(arq);
	}
	return qtde + 1;
}

// Procura o pacote pelo código. Se achar, deixa o registro na variável global pacotes
// e devolve a posição em bytes. Se não achar (ou se o arquivo não existir), devolve -1.
int buscarPacote(int codigo) {
	FILE *arq;
	int pos = -1;

	arq = fopen("pacotes.bin", "rb");
	if(arq != NULL) {
		pos = busca(arq, codigo, ENT_PACOTE);
		if(pos != -1) {
			fseek(arq, pos, SEEK_SET);
			fread(&pacotes, sizeof(Pacote), 1, arq);
		}
		fclose(arq);
	}
	return pos;
}

// Procura a matrícula pelo código. Se achar, deixa o registro na variável global matriculas
// e devolve a posição em bytes. Se não achar (ou se o arquivo não existir), devolve -1.
int buscarMatricula(int codigo) {
	FILE *arq;
	int pos = -1;

	arq = fopen("matriculas.bin", "rb");
	if(arq != NULL) {
		pos = busca(arq, codigo, ENT_MATRICULA);
		if(pos != -1) {
			fseek(arq, pos, SEEK_SET);
			fread(&matriculas, sizeof(Matricula), 1, arq);
		}
		fclose(arq);
	}
	return pos;
}

// Devolve 1 se o aluno já tem uma matrícula ativa (o ERS permite só uma por aluno).
int alunoTemMatriculaAtiva(int codAluno) {
	FILE *arq;
	Matricula temp;
	int achou = 0;

	arq = fopen("matriculas.bin", "rb");
	if(arq != NULL) {
		while(achou == 0 && fread(&temp, sizeof(Matricula), 1, arq) == 1) {
			if(temp.codAluno == codAluno && temp.situacao == 1) achou = 1;
		}
		fclose(arq);
	}
	return achou;
}

// Grava uma cobrança pendente no fim de cobrancas.bin (o código é automático).
void gerarCobranca(int codMatricula, float valor, Data vencimento) {
	FILE *arq;
	Cobranca nova;

	nova.codigo = proximoCodigo("cobrancas.bin", sizeof(Cobranca));
	nova.codMatricula = codMatricula;
	nova.mesRef = vencimento.mes;
	nova.anoRef = vencimento.ano;
	nova.valor = valor;
	nova.vencimento = vencimento;
	nova.dataPagamento.dia = 0;
	nova.dataPagamento.mes = 0;
	nova.dataPagamento.ano = 0;
	nova.situacao = 1;

	arq = fopen("cobrancas.bin", "ab+");
	if(arq == NULL) {
		printf("Erro na Abertura do Arquivo!\n\n");
	} else {
		fwrite(&nova, sizeof(Cobranca), 1, arq);
		fclose(arq);
	}
}

// EFETUAR MATRÍCULA: vincula aluno, plano e pacote. O valor total é
// (valor mensal do plano + acréscimo do pacote) x meses x (1 - desconto/100).
// A data de término é a de início + meses do plano e a primeira cobrança sai na hora.
void efetuarMatricula(void) {
	FILE *arq;
	Matricula nova;
	int pos, ok;
	float mensal, cheio, desconto, parcela;
	char nomeA[61];

	system("cls");
	printf("==============================================\n");
	printf("  EFETUAR MATRÍCULA\n");
	printf("==============================================\n\n");

	if(!temAlunoAtivo()) {
		printf("Não Existem Alunos Ativos Cadastrados!\n\n");
		system("pause");
		return;
	}

	arq = fopen("matriculas.bin", "ab+");
	if(arq == NULL) {
		printf("Erro na Abertura do Arquivo!\n\n");
		system("pause");
		return;
	}

	do {
		do {
			printf("Informe o Código da Matrícula (1 a 1000): ");
			scanf("%d", &nova.codigo);
			if(nova.codigo < 1 || nova.codigo > 1000) printf("O Código Deve Estar Entre 1 e 1000!\n\n");
		} while(nova.codigo < 1 || nova.codigo > 1000);

		pos = busca(arq, nova.codigo, ENT_MATRICULA);
		if(pos != -1) printf("Código Já Cadastrado!\n\n");
	} while(pos != -1);

	// aluno: precisa existir, estar ativo e não ter outra matrícula ativa
	do {
		printf("Informe o Código do Aluno (0 para cancelar): ");
		scanf("%d", &nova.codAluno);
		if(nova.codAluno == 0) {
			fclose(arq);
			return;
		}
		ok = existeAlunoAvaliacao(nova.codAluno);
		if(!ok) printf("Aluno Não Encontrado ou Inativo!\n\n");
		else if(alunoTemMatriculaAtiva(nova.codAluno)) {
			printf("Este Aluno Já Possui Matrícula Ativa!\n\n");
			ok = 0;
		}
	} while(!ok);

	// plano: precisa existir e estar ativo (fica preenchido em planos)
	do {
		printf("Informe o Código do Plano (0 para cancelar): ");
		scanf("%d", &nova.codPlano);
		if(nova.codPlano == 0) {
			fclose(arq);
			return;
		}
		pos = buscarPlano(nova.codPlano);
		ok = 1;
		if(pos == -1) {
			printf("Plano Não Encontrado!\n\n");
			ok = 0;
		} else if(planos.ativo != 1) {
			printf("Plano Inativo!\n\n");
			ok = 0;
		}
	} while(!ok);

	// pacote: precisa existir e estar ativo (fica preenchido em pacotes)
	do {
		printf("Informe o Código do Pacote (0 para cancelar): ");
		scanf("%d", &nova.codPacote);
		if(nova.codPacote == 0) {
			fclose(arq);
			return;
		}
		pos = buscarPacote(nova.codPacote);
		ok = 1;
		if(pos == -1) {
			printf("Pacote Não Encontrado!\n\n");
			ok = 0;
		} else if(pacotes.ativo != 1) {
			printf("Pacote Inativo!\n\n");
			ok = 0;
		}
	} while(!ok);

	do {
		printf("Informe a Data de Início (DD MM AAAA): ");
		scanf("%d%d%d", &nova.dataInicio.dia, &nova.dataInicio.mes, &nova.dataInicio.ano);
		if(validaData(nova.dataInicio) == 0) printf("Insira Uma Data Valida!\n\n");
	} while(validaData(nova.dataInicio) != 1);

	nova.dataTermino = somarMeses(nova.dataInicio, planos.meses);

	mensal = planos.valorMensal + pacotes.valor;
	cheio = mensal * planos.meses;
	desconto = cheio * planos.porcentagemDesconto / 100;
	nova.valorTotal = cheio - desconto;
	parcela = nova.valorTotal / planos.meses;
	nova.situacao = 1;

	nomeDoAluno(nova.codAluno, nomeA);
	printf("\n==============================================\n");
	printf("Aluno: %d - %s\n", nova.codAluno, nomeA);
	printf("Plano: %s (%d mês(es))\n", planos.nome, planos.meses);
	printf("Pacote: %s\n", pacotes.nome);
	printf("Valor mensal do plano: R$ %.2f\n", planos.valorMensal);
	printf("Acréscimo do pacote: R$ %.2f\n", pacotes.valor);
	printf("Valor sem desconto: R$ %.2f\n", cheio);
	printf("Desconto: %.0f%% (- R$ %.2f)\n", planos.porcentagemDesconto, desconto);
	printf("Valor total: R$ %.2f\n", nova.valorTotal);
	printf("Valor de cada mensalidade: R$ %.2f\n", parcela);
	printf("Início: %02d/%02d/%d\n", nova.dataInicio.dia, nova.dataInicio.mes, nova.dataInicio.ano);
	printf("Término: %02d/%02d/%d\n", nova.dataTermino.dia, nova.dataTermino.mes, nova.dataTermino.ano);
	printf("==============================================\n");
	printf("Confirma a matrícula? S/N: ");
	if(toupper(getche()) == 'S') {
		fseek(arq, 0, SEEK_END);
		fwrite(&nova, sizeof(Matricula), 1, arq);
		fclose(arq);
		gerarCobranca(nova.codigo, parcela, nova.dataInicio);
		printf("\n\nMatrícula efetuada com sucesso!\n");
		printf("A primeira cobrança foi gerada (vencimento em %02d/%02d/%d).\n", nova.dataInicio.dia, nova.dataInicio.mes, nova.dataInicio.ano);
	} else {
		fclose(arq);
		printf("\n\nMatrícula cancelada!\n");
	}
	system("pause");
}

// LISTAR MATRÍCULAS (usada no Gerenciar > Matrículas > Relatório).
void listarMatriculas(void) {
	FILE *arq;
	Matricula m;
	char nomeA[61];
	int total = 0;

	system("cls");
	printf("==============================================\n");
	printf("  LISTAGEM - Matrículas\n");
	printf("==============================================\n");

	arq = fopen("matriculas.bin", "rb");
	if(arq == NULL) {
		printf("Nenhuma matrícula cadastrada!\n\n");
	} else {
		while(fread(&m, sizeof(Matricula), 1, arq) == 1) {
			if(!nomeDoAluno(m.codAluno, nomeA)) strcpy(nomeA, "(não encontrado)");
			printf("Código da Matrícula: %d\n", m.codigo);
			printf("Aluno: %d - %s\n", m.codAluno, nomeA);
			if(buscarPlano(m.codPlano) != -1) printf("Plano: %s\n", planos.nome);
			else printf("Plano: %d\n", m.codPlano);
			if(buscarPacote(m.codPacote) != -1) printf("Pacote: %s\n", pacotes.nome);
			else printf("Pacote: %d\n", m.codPacote);
			printf("Início: %02d/%02d/%d\n", m.dataInicio.dia, m.dataInicio.mes, m.dataInicio.ano);
			printf("Término: %02d/%02d/%d\n", m.dataTermino.dia, m.dataTermino.mes, m.dataTermino.ano);
			printf("Valor total: R$ %.2f\n", m.valorTotal);
			printf("Situação: ");
			if(m.situacao == 1) printf("Ativa\n");
			else printf("Cancelada\n");
			printf("==============================================\n");
			total++;
		}
		fclose(arq);
		if(total == 0) printf("Nenhuma matrícula cadastrada!\n\n");
		else printf("Total de matrículas: %d\n", total);
	}
	system("pause");
}

// Matrícula não é alterada: valor e cobranças já foram lançados. Para encerrar use o Cancelamento.
void avisoMatricula(void) {
	system("cls");
	printf("==============================================\n");
	printf("  Matrículas\n");
	printf("==============================================\n\n");
	printf("A matrícula não pode ser alterada, porque guarda\n");
	printf("valores e cobranças já lançados.\n\n");
	printf("Para encerrar o vínculo use a opção Excluir deste\n");
	printf("menu ou Operações > Efetuar cancelamento.\n\n");
	system("pause");
}

// EFETUAR RECEBIMENTO: dá baixa na cobrança pendente da matrícula, grava a data do pagamento
// e gera a cobrança do mês seguinte (se a matrícula está ativa e o término ainda não chegou).
void efetuarRecebimento(void) {
	FILE *arq;
	Matricula m;
	Cobranca c;
	Data dataPag, proximo;
	int codigo, posMat, posCob, achou, n;
	char nomeA[61];

	system("cls");
	printf("==============================================\n");
	printf("  EFETUAR RECEBIMENTO\n");
	printf("==============================================\n\n");

	printf("Informe o Código da Matrícula: ");
	scanf("%d", &codigo);
	posMat = buscarMatricula(codigo);
	if(posMat == -1) {
		printf("\nMatrícula Não Encontrada!\n\n");
		system("pause");
		return;
	}
	m = matriculas;
	if(m.situacao != 1) {
		printf("\nMatrícula Cancelada! Não é possível receber.\n\n");
		system("pause");
		return;
	}

	arq = fopen("cobrancas.bin", "rb+");
	if(arq == NULL) {
		printf("\nNenhuma cobrança cadastrada!\n\n");
		system("pause");
		return;
	}

	achou = 0;
	while(achou == 0 && fread(&c, sizeof(Cobranca), 1, arq) == 1) {
		if(c.codMatricula == codigo && c.situacao == 1) achou = 1;
	}
	if(achou == 0) {
		printf("\nNão há cobrança pendente para esta matrícula!\n\n");
		fclose(arq);
		system("pause");
		return;
	}
	posCob = ftell(arq) - sizeof(Cobranca);

	if(!nomeDoAluno(m.codAluno, nomeA)) strcpy(nomeA, "(não encontrado)");
	printf("\n==============================================\n");
	printf("Matrícula: %d\n", m.codigo);
	printf("Aluno: %d - %s\n", m.codAluno, nomeA);
	printf("Cobrança: %d\n", c.codigo);
	printf("Referência: %02d/%d\n", c.mesRef, c.anoRef);
	printf("Vencimento: %02d/%02d/%d\n", c.vencimento.dia, c.vencimento.mes, c.vencimento.ano);
	printf("Valor: R$ %.2f\n", c.valor);
	printf("==============================================\n");
	printf("Confirma o recebimento? S/N: ");
	if(toupper(getche()) != 'S') {
		printf("\n\nRecebimento cancelado!\n");
		fclose(arq);
		system("pause");
		return;
	}

	printf("\n\n");
	do {
		printf("Informe a Data do Pagamento (DD MM AAAA): ");
		scanf("%d%d%d", &dataPag.dia, &dataPag.mes, &dataPag.ano);
		if(validaData(dataPag) == 0) printf("Insira Uma Data Valida!\n\n");
	} while(validaData(dataPag) != 1);

	c.situacao = 2;
	c.dataPagamento = dataPag;
	fseek(arq, posCob, SEEK_SET);
	fwrite(&c, sizeof(Cobranca), 1, arq);
	fclose(arq);
	printf("\nRecebimento registrado com sucesso!\n");

	// próxima cobrança: mês seguinte ao da cobrança paga (dezembro vira janeiro do outro ano)
	n = (c.anoRef - m.dataInicio.ano) * 12 + (c.mesRef - m.dataInicio.mes) + 1;
	proximo = somarMeses(m.dataInicio, n);
	if(dataNumero(proximo) < dataNumero(m.dataTermino)) {
		gerarCobranca(m.codigo, c.valor, proximo);
		printf("Cobrança de %02d/%d gerada (vencimento em %02d/%02d/%d).\n", proximo.mes, proximo.ano, proximo.dia, proximo.mes, proximo.ano);
	} else {
		printf("Esta era a última mensalidade do plano. Nenhuma nova cobrança foi gerada.\n");
	}
	system("pause");
}

// EFETUAR CANCELAMENTO: plano mensal só suspende a próxima cobrança, sem devolução.
// Trimestral, semestral e anual: retém 30% do valor pago e devolve 70%.
// A matrícula vira cancelada e as cobranças pendentes viram canceladas.
void efetuarCancelamento(void) {
	FILE *arq;
	Matricula m;
	Cobranca c;
	Cancelamento canc;
	int codigo, posMat;
	long pos;
	float pago = 0;
	char nomeA[61];

	system("cls");
	printf("==============================================\n");
	printf("  EFETUAR CANCELAMENTO\n");
	printf("==============================================\n\n");

	printf("Informe o Código da Matrícula: ");
	scanf("%d", &codigo);
	posMat = buscarMatricula(codigo);
	if(posMat == -1) {
		printf("\nMatrícula Não Encontrada!\n\n");
		system("pause");
		return;
	}
	m = matriculas;
	if(m.situacao != 1) {
		printf("\nEsta Matrícula Já Está Cancelada!\n\n");
		system("pause");
		return;
	}
	if(buscarPlano(m.codPlano) == -1) {
		printf("\nPlano da Matrícula Não Encontrado!\n\n");
		system("pause");
		return;
	}

	// soma o que o aluno já pagou
	arq = fopen("cobrancas.bin", "rb");
	if(arq != NULL) {
		while(fread(&c, sizeof(Cobranca), 1, arq) == 1) {
			if(c.codMatricula == codigo && c.situacao == 2) pago = pago + c.valor;
		}
		fclose(arq);
	}

	canc.codMatricula = codigo;
	canc.valorPago = pago;
	if(planos.meses == 1) {
		canc.valorDevolvido = 0;
		canc.valorRetido = 0;
	} else {
		canc.valorRetido = pago * 0.30;
		canc.valorDevolvido = pago * 0.70;
	}

	if(!nomeDoAluno(m.codAluno, nomeA)) strcpy(nomeA, "(não encontrado)");
	printf("\n==============================================\n");
	printf("Matrícula: %d\n", m.codigo);
	printf("Aluno: %d - %s\n", m.codAluno, nomeA);
	printf("Plano: %s\n", planos.nome);
	printf("Valor pago até agora: R$ %.2f\n", canc.valorPago);
	if(planos.meses == 1) {
		printf("Plano mensal: sem devolução, só suspende a próxima cobrança.\n");
	} else {
		printf("Valor retido (30%%): R$ %.2f\n", canc.valorRetido);
		printf("Valor devolvido (70%%): R$ %.2f\n", canc.valorDevolvido);
	}
	printf("==============================================\n");
	printf("Confirma o cancelamento? S/N: ");
	if(toupper(getche()) != 'S') {
		printf("\n\nCancelamento desfeito!\n");
		system("pause");
		return;
	}

	printf("\n\n");
	do {
		printf("Informe a Data do Cancelamento (DD MM AAAA): ");
		scanf("%d%d%d", &canc.data.dia, &canc.data.mes, &canc.data.ano);
		if(validaData(canc.data) == 0) printf("Insira Uma Data Valida!\n\n");
	} while(validaData(canc.data) != 1);

	// grava o cancelamento
	canc.codigo = proximoCodigo("cancelamentos.bin", sizeof(Cancelamento));
	arq = fopen("cancelamentos.bin", "ab+");
	if(arq == NULL) {
		printf("Erro na Abertura do Arquivo!\n\n");
		system("pause");
		return;
	}
	fwrite(&canc, sizeof(Cancelamento), 1, arq);
	fclose(arq);

	// matrícula passa a cancelada (regrava na mesma posição)
	arq = fopen("matriculas.bin", "rb+");
	if(arq != NULL) {
		m.situacao = 2;
		fseek(arq, posMat, SEEK_SET);
		fwrite(&m, sizeof(Matricula), 1, arq);
		fclose(arq);
	}

	// cobranças pendentes passam a canceladas
	arq = fopen("cobrancas.bin", "rb+");
	if(arq != NULL) {
		pos = ftell(arq);
		while(fread(&c, sizeof(Cobranca), 1, arq) == 1) {
			if(c.codMatricula == codigo && c.situacao == 1) {
				c.situacao = 3;
				fseek(arq, pos, SEEK_SET);
				fwrite(&c, sizeof(Cobranca), 1, arq);
				fseek(arq, pos + sizeof(Cobranca), SEEK_SET);
			}
			pos = ftell(arq);
		}
		fclose(arq);
	}

	printf("\nCancelamento efetuado com sucesso!\n");
	system("pause");
}

// ===== FREQUÊNCIA =====

// Copia o nome da atividade para destino. Devolve 1 se achou e 0 se não achou.
int nomeDaAtividade(int codigo, char destino[]) {
	FILE *arq;
	Atividade temp;
	int pos, achou = 0;

	arq = fopen("atividades.bin", "rb");
	if(arq == NULL) return 0;

	pos = busca(arq, codigo, ENT_ATIVIDADE);
	if(pos != -1) {
		fseek(arq, pos, SEEK_SET);
		fread(&temp, sizeof(Atividade), 1, arq);
		strcpy(destino, temp.nome);
		achou = 1;
	}
	fclose(arq);
	return achou;
}

// Devolve 1 se o aluno tem matrícula ativa e a atividade faz parte do pacote dessa matrícula.
int alunoTemAtividade(int codAluno, int codAtividade) {
	FILE *arq;
	Matricula temp;
	int i, achou = 0;

	arq = fopen("matriculas.bin", "rb");
	if(arq != NULL) {
		while(achou == 0 && fread(&temp, sizeof(Matricula), 1, arq) == 1) {
			if(temp.codAluno == codAluno && temp.situacao == 1) {
				if(buscarPacote(temp.codPacote) != -1) {
					for(i = 0; i < pacotes.qtdAtivPacote; i++) {
						if(pacotes.codAtividades[i] == codAtividade) achou = 1;
					}
				}
			}
		}
		fclose(arq);
	}
	return achou;
}

// Devolve 1 se o aluno já tem presença lançada nessa atividade nesse dia.
int presencaJaRegistrada(int codAluno, int codAtividade, Data data) {
	FILE *arq;
	Presenca temp;
	int achou = 0;

	arq = fopen("presencas.bin", "rb");
	if(arq != NULL) {
		while(achou == 0 && fread(&temp, sizeof(Presenca), 1, arq) == 1) {
			if(temp.codAluno == codAluno && temp.codAtividade == codAtividade && dataNumero(temp.data) == dataNumero(data)) achou = 1;
		}
		fclose(arq);
	}
	return achou;
}

// Conta os alunos presentes na atividade nesse dia (para respeitar a capacidade máxima).
int contarPresentes(int codAtividade, Data data) {
	FILE *arq;
	Presenca temp;
	int total = 0;

	arq = fopen("presencas.bin", "rb");
	if(arq != NULL) {
		while(fread(&temp, sizeof(Presenca), 1, arq) == 1) {
			if(temp.codAtividade == codAtividade && dataNumero(temp.data) == dataNumero(data) && temp.presente == 1) total++;
		}
		fclose(arq);
	}
	return total;
}

// FAZER CHAMADA: escolhe a atividade e o dia e lança a presença aluno por aluno.
// Só entra aluno com matrícula ativa cujo pacote tem a atividade. Respeita a capacidade da atividade.
void fazerChamada(void) {
	FILE *arq;
	Atividade ativ;
	Presenca nova;
	Data data;
	int codAtividade, codAluno, presente, pos;
	char nomeA[61];

	system("cls");
	printf("==============================================\n");
	printf("  FAZER CHAMADA\n");
	printf("==============================================\n\n");

	do {
		printf("Informe o Código da Atividade (0 para cancelar): ");
		scanf("%d", &codAtividade);
		if(codAtividade == 0) return;
		if(!existeAtividade(codAtividade)) printf("Atividade Não Encontrada ou Inativa!\n\n");
	} while(!existeAtividade(codAtividade));

	arq = fopen("atividades.bin", "rb");
	pos = busca(arq, codAtividade, ENT_ATIVIDADE);
	fseek(arq, pos, SEEK_SET);
	fread(&ativ, sizeof(Atividade), 1, arq);
	fclose(arq);

	do {
		printf("Informe a Data da Aula (DD MM AAAA): ");
		scanf("%d%d%d", &data.dia, &data.mes, &data.ano);
		if(validaData(data) == 0) printf("Insira Uma Data Valida!\n\n");
	} while(validaData(data) != 1);

	printf("\nAtividade: %s (capacidade: %d alunos)\n", ativ.nome, ativ.capacidade);

	do {
		printf("\nInforme o Código do Aluno (0 para encerrar a chamada): ");
		scanf("%d", &codAluno);
		if(codAluno != 0) {
			if(!existeAlunoAvaliacao(codAluno)) {
				printf("Aluno Não Encontrado ou Inativo!\n");
			} else if(!alunoTemAtividade(codAluno, codAtividade)) {
				printf("Aluno Sem Matrícula Ativa com Esta Atividade no Pacote!\n");
			} else if(presencaJaRegistrada(codAluno, codAtividade, data)) {
				printf("A Presença Deste Aluno Já Foi Registrada Neste Dia!\n");
			} else {
				nomeDoAluno(codAluno, nomeA);
				printf("Aluno: %s\n", nomeA);
				do {
					printf("Presente? (1- Sim / 0- Não): ");
					scanf("%d", &presente);
					if(presente != 1 && presente != 0) printf("Informe 1 ou 0!\n");
				} while(presente != 1 && presente != 0);

				if(presente == 1 && contarPresentes(codAtividade, data) >= ativ.capacidade) {
					printf("Capacidade Máxima da Atividade Atingida!\n");
				} else {
					nova.codigo = proximoCodigo("presencas.bin", sizeof(Presenca));
					nova.codAluno = codAluno;
					nova.codAtividade = codAtividade;
					nova.data = data;
					nova.presente = presente;

					arq = fopen("presencas.bin", "ab+");
					if(arq == NULL) {
						printf("Erro na Abertura do Arquivo!\n\n");
					} else {
						fwrite(&nova, sizeof(Presenca), 1, arq);
						fclose(arq);
						printf("Presença registrada!\n");
					}
				}
			}
		}
	} while(codAluno != 0);

	printf("\nChamada encerrada!\n");
	system("pause");
}

// LISTAR FREQUÊNCIA (também é o relatório de frequência por aluno, período e atividade).
// Código 0 em aluno ou atividade significa "todos".
void listarFrequencia(void) {
	FILE *arq;
	Presenca p;
	Data inicio, fim;
	int codAluno, codAtividade, total = 0, presentes = 0;
	char nomeA[61], nomeAt[61];

	system("cls");
	printf("==============================================\n");
	printf("  LISTAGEM - Frequência\n");
	printf("==============================================\n\n");

	printf("Informe o Código do Aluno (0 para todos): ");
	scanf("%d", &codAluno);
	printf("Informe o Código da Atividade (0 para todas): ");
	scanf("%d", &codAtividade);
	do {
		printf("Informe a Data Inicial (DD MM AAAA): ");
		scanf("%d%d%d", &inicio.dia, &inicio.mes, &inicio.ano);
		if(validaData(inicio) == 0) printf("Insira Uma Data Valida!\n\n");
	} while(validaData(inicio) != 1);
	do {
		printf("Informe a Data Final (DD MM AAAA): ");
		scanf("%d%d%d", &fim.dia, &fim.mes, &fim.ano);
		if(validaData(fim) == 0) printf("Insira Uma Data Valida!\n\n");
		else if(dataNumero(fim) < dataNumero(inicio)) {
			printf("A Data Final Não Pode Ser Antes da Inicial!\n\n");
			fim.ano = 0;
		}
	} while(validaData(fim) != 1);

	printf("\n==============================================\n");
	arq = fopen("presencas.bin", "rb");
	if(arq == NULL) {
		printf("Nenhuma presença registrada!\n\n");
	} else {
		while(fread(&p, sizeof(Presenca), 1, arq) == 1) {
			if((codAluno == 0 || p.codAluno == codAluno) && (codAtividade == 0 || p.codAtividade == codAtividade)
			   && dataNumero(p.data) >= dataNumero(inicio) && dataNumero(p.data) <= dataNumero(fim)) {
				if(!nomeDoAluno(p.codAluno, nomeA)) strcpy(nomeA, "(não encontrado)");
				if(!nomeDaAtividade(p.codAtividade, nomeAt)) strcpy(nomeAt, "(não encontrada)");
				printf("Código: %d\n", p.codigo);
				printf("Data: %02d/%02d/%d\n", p.data.dia, p.data.mes, p.data.ano);
				printf("Aluno: %d - %s\n", p.codAluno, nomeA);
				printf("Atividade: %d - %s\n", p.codAtividade, nomeAt);
				printf("Presença: ");
				if(p.presente == 1) printf("Presente\n");
				else printf("Faltou\n");
				printf("==============================================\n");
				total++;
				if(p.presente == 1) presentes++;
			}
		}
		fclose(arq);
		if(total == 0) printf("Nenhum registro encontrado!\n\n");
		else printf("Total de registros: %d | Presenças: %d | Faltas: %d\n", total, presentes, total - presentes);
	}
	system("pause");
}

// ALTERAR FREQUÊNCIA: corrige a presença (presente/faltou) de um registro, pelo código da presença.
void alterarFrequencia(void) {
	FILE *arq, *arqAtiv;
	Presenca p;
	Atividade ativ;
	int codigo, pos, novo, posAtiv;
	char nomeA[61];

	system("cls");
	printf("==============================================\n");
	printf("  ALTERAR - Frequência\n");
	printf("==============================================\n\n");

	arq = fopen("presencas.bin", "rb+");
	if(arq == NULL) {
		printf("Nenhuma presença registrada!\n\n");
		system("pause");
		return;
	}

	printf("Informe o Código da Presença: ");
	scanf("%d", &codigo);
	rewind(arq);
	pos = -1;
	while(pos == -1 && fread(&p, sizeof(Presenca), 1, arq) == 1) {
		if(p.codigo == codigo) pos = ftell(arq) - sizeof(Presenca);
	}

	if(pos == -1) {
		printf("\nPresença Não Encontrada!\n\n");
	} else {
		if(!nomeDoAluno(p.codAluno, nomeA)) strcpy(nomeA, "(não encontrado)");
		printf("\nAluno: %d - %s\n", p.codAluno, nomeA);
		printf("Data: %02d/%02d/%d\n", p.data.dia, p.data.mes, p.data.ano);
		printf("Situação atual: ");
		if(p.presente == 1) printf("Presente\n");
		else printf("Faltou\n");

		do {
			printf("Informe a Nova Situação (1- Presente / 0- Faltou): ");
			scanf("%d", &novo);
			if(novo != 1 && novo != 0) printf("Informe 1 ou 0!\n");
		} while(novo != 1 && novo != 0);

		if(novo == 1 && p.presente == 0) {
			// falta virando presença: confere a capacidade da atividade
			arqAtiv = fopen("atividades.bin", "rb");
			if(arqAtiv != NULL) {
				posAtiv = busca(arqAtiv, p.codAtividade, ENT_ATIVIDADE);
				if(posAtiv != -1) {
					fseek(arqAtiv, posAtiv, SEEK_SET);
					fread(&ativ, sizeof(Atividade), 1, arqAtiv);
					if(contarPresentes(p.codAtividade, p.data) >= ativ.capacidade) {
						printf("Capacidade Máxima da Atividade Atingida!\n");
						novo = p.presente;
					}
				}
				fclose(arqAtiv);
			}
		}

		if(novo != p.presente) {
			p.presente = novo;
			fseek(arq, pos, SEEK_SET);
			fwrite(&p, sizeof(Presenca), 1, arq);
			printf("\nAlteração Realizada com Sucesso!\n");
		}
	}
	fclose(arq);
	system("pause");
}

// ===== CONTROLE FINANCEIRO =====

// Soma das mensalidades recebidas no mês/ano (pela data do pagamento).
float somaRecebido(int mes, int ano) {
	FILE *arq;
	Cobranca c;
	float total = 0;

	arq = fopen("cobrancas.bin", "rb");
	if(arq != NULL) {
		while(fread(&c, sizeof(Cobranca), 1, arq) == 1) {
			if(c.situacao == 2 && c.dataPagamento.mes == mes && c.dataPagamento.ano == ano) total = total + c.valor;
		}
		fclose(arq);
	}
	return total;
}

// Soma do valor retido (receita) nos cancelamentos do mês/ano.
float somaRetido(int mes, int ano) {
	FILE *arq;
	Cancelamento c;
	float total = 0;

	arq = fopen("cancelamentos.bin", "rb");
	if(arq != NULL) {
		while(fread(&c, sizeof(Cancelamento), 1, arq) == 1) {
			if(c.data.mes == mes && c.data.ano == ano) total = total + c.valorRetido;
		}
		fclose(arq);
	}
	return total;
}

// Soma do valor devolvido (saída) nos cancelamentos do mês/ano.
float somaDevolvido(int mes, int ano) {
	FILE *arq;
	Cancelamento c;
	float total = 0;

	arq = fopen("cancelamentos.bin", "rb");
	if(arq != NULL) {
		while(fread(&c, sizeof(Cancelamento), 1, arq) == 1) {
			if(c.data.mes == mes && c.data.ano == ano) total = total + c.valorDevolvido;
		}
		fclose(arq);
	}
	return total;
}

// Devolve o código do plano da matrícula (0 se a matrícula não existe).
int codPlanoDaMatricula(int codMatricula) {
	FILE *arq;
	Matricula m;
	int pos, codPlano = 0;

	arq = fopen("matriculas.bin", "rb");
	if(arq != NULL) {
		pos = busca(arq, codMatricula, ENT_MATRICULA);
		if(pos != -1) {
			fseek(arq, pos, SEEK_SET);
			fread(&m, sizeof(Matricula), 1, arq);
			codPlano = m.codPlano;
		}
		fclose(arq);
	}
	return codPlano;
}

// FATURAMENTO MENSAL: receita de cada mês do ano = mensalidades recebidas + valores retidos em cancelamentos.
void faturamentoMensal(void) {
	int ano, mes;
	float recebido, retido, totalAno = 0;

	system("cls");
	printf("==============================================\n");
	printf("  FATURAMENTO MENSAL\n");
	printf("==============================================\n\n");

	do {
		printf("Informe o Ano (1900 a 2100): ");
		scanf("%d", &ano);
		if(ano < 1900 || ano > 2100) printf("Ano Inválido!\n\n");
	} while(ano < 1900 || ano > 2100);

	printf("\n Mês | Mensalidades |    Retenções |        Total\n");
	printf("----------------------------------------------\n");
	for(mes = 1; mes <= 12; mes++) {
		recebido = somaRecebido(mes, ano);
		retido = somaRetido(mes, ano);
		printf("  %02d | R$ %9.2f | R$ %9.2f | R$ %9.2f\n", mes, recebido, retido, recebido + retido);
		totalAno = totalAno + recebido + retido;
	}
	printf("----------------------------------------------\n");
	printf("Faturamento total de %d: R$ %.2f\n\n", ano, totalAno);
	system("pause");
}

// FATURAMENTO POR PLANO: receita do ano separada por plano (mensalidades + retenções).
void faturamentoPorPlano(void) {
	FILE *arqPlanos, *arq;
	Plano p;
	Cobranca c;
	Cancelamento canc;
	int ano, total = 0;
	float recebido, retido, totalGeral = 0;

	system("cls");
	printf("==============================================\n");
	printf("  FATURAMENTO POR PLANO\n");
	printf("==============================================\n\n");

	do {
		printf("Informe o Ano (1900 a 2100): ");
		scanf("%d", &ano);
		if(ano < 1900 || ano > 2100) printf("Ano Inválido!\n\n");
	} while(ano < 1900 || ano > 2100);

	arqPlanos = fopen("planos.bin", "rb");
	if(arqPlanos == NULL) {
		printf("\nNenhum plano cadastrado!\n\n");
		system("pause");
		return;
	}

	printf("\n==============================================\n");
	while(fread(&p, sizeof(Plano), 1, arqPlanos) == 1) {
		recebido = 0;
		retido = 0;

		arq = fopen("cobrancas.bin", "rb");
		if(arq != NULL) {
			while(fread(&c, sizeof(Cobranca), 1, arq) == 1) {
				if(c.situacao == 2 && c.dataPagamento.ano == ano && codPlanoDaMatricula(c.codMatricula) == p.codigo) recebido = recebido + c.valor;
			}
			fclose(arq);
		}
		arq = fopen("cancelamentos.bin", "rb");
		if(arq != NULL) {
			while(fread(&canc, sizeof(Cancelamento), 1, arq) == 1) {
				if(canc.data.ano == ano && codPlanoDaMatricula(canc.codMatricula) == p.codigo) retido = retido + canc.valorRetido;
			}
			fclose(arq);
		}

		printf("Plano: %d - %s\n", p.codigo, p.nome);
		printf("Mensalidades recebidas: R$ %.2f\n", recebido);
		printf("Valores retidos: R$ %.2f\n", retido);
		printf("Total do plano: R$ %.2f\n", recebido + retido);
		printf("==============================================\n");
		totalGeral = totalGeral + recebido + retido;
		total++;
	}
	fclose(arqPlanos);

	if(total == 0) printf("Nenhum plano cadastrado!\n\n");
	else printf("Faturamento total de %d: R$ %.2f\n\n", ano, totalGeral);
	system("pause");
}

// GASTOS MENSAIS: despesas lançadas no mês + valores devolvidos em cancelamentos (saídas).
void gastosMensais(void) {
	FILE *arq;
	Gasto g;
	int mes, ano, total = 0;
	float somaGastos = 0, devolvido;

	system("cls");
	printf("==============================================\n");
	printf("  GASTOS MENSAIS\n");
	printf("==============================================\n\n");

	do {
		printf("Informe o Mês (1 a 12): ");
		scanf("%d", &mes);
		if(mes < 1 || mes > 12) printf("Mês Inválido!\n\n");
	} while(mes < 1 || mes > 12);
	do {
		printf("Informe o Ano (1900 a 2100): ");
		scanf("%d", &ano);
		if(ano < 1900 || ano > 2100) printf("Ano Inválido!\n\n");
	} while(ano < 1900 || ano > 2100);

	printf("\n==============================================\n");
	arq = fopen("gastos.bin", "rb");
	if(arq != NULL) {
		while(fread(&g, sizeof(Gasto), 1, arq) == 1) {
			if(g.ativo == 1 && g.data.mes == mes && g.data.ano == ano) {
				printf("Código: %d\n", g.codigo);
				printf("Descrição: %s\n", g.descricao);
				printf("Categoria: %s\n", g.categoria);
				printf("Data: %02d/%02d/%d\n", g.data.dia, g.data.mes, g.data.ano);
				printf("Valor: R$ %.2f\n", g.valor);
				printf("==============================================\n");
				somaGastos = somaGastos + g.valor;
				total++;
			}
		}
		fclose(arq);
	}
	if(total == 0) printf("Nenhum gasto lançado neste mês!\n");

	devolvido = somaDevolvido(mes, ano);
	printf("\nGastos lançados: R$ %.2f\n", somaGastos);
	printf("Devoluções de cancelamentos: R$ %.2f\n", devolvido);
	printf("Total de saídas em %02d/%d: R$ %.2f\n\n", mes, ano, somaGastos + devolvido);
	system("pause");
}

// PAGAMENTOS POR ALUNO: histórico das cobranças de todas as matrículas do aluno.
void pagamentosPorAluno(void) {
	FILE *arqMat, *arqCob;
	Matricula m;
	Cobranca c;
	int codAluno, achouMat = 0;
	float pago = 0, pendente = 0;
	char nomeA[61];

	system("cls");
	printf("==============================================\n");
	printf("  PAGAMENTOS POR ALUNO\n");
	printf("==============================================\n\n");

	printf("Informe o Código do Aluno: ");
	scanf("%d", &codAluno);
	if(!nomeDoAluno(codAluno, nomeA)) {
		printf("\nAluno Não Encontrado!\n\n");
		system("pause");
		return;
	}

	printf("\nAluno: %d - %s\n", codAluno, nomeA);
	printf("==============================================\n");
	arqMat = fopen("matriculas.bin", "rb");
	if(arqMat != NULL) {
		while(fread(&m, sizeof(Matricula), 1, arqMat) == 1) {
			if(m.codAluno == codAluno) {
				achouMat = 1;
				printf("Matrícula %d (", m.codigo);
				if(m.situacao == 1) printf("ativa)\n");
				else printf("cancelada)\n");

				arqCob = fopen("cobrancas.bin", "rb");
				if(arqCob != NULL) {
					while(fread(&c, sizeof(Cobranca), 1, arqCob) == 1) {
						if(c.codMatricula == m.codigo) {
							printf("  Ref. %02d/%d | Venc. %02d/%02d/%d | R$ %.2f | ", c.mesRef, c.anoRef, c.vencimento.dia, c.vencimento.mes, c.vencimento.ano, c.valor);
							if(c.situacao == 2) {
								printf("Paga em %02d/%02d/%d\n", c.dataPagamento.dia, c.dataPagamento.mes, c.dataPagamento.ano);
								pago = pago + c.valor;
							} else if(c.situacao == 1) {
								printf("Pendente\n");
								pendente = pendente + c.valor;
							} else {
								printf("Cancelada\n");
							}
						}
					}
					fclose(arqCob);
				}
				printf("----------------------------------------------\n");
			}
		}
		fclose(arqMat);
	}
	if(achouMat == 0) printf("Este aluno não possui matrícula!\n\n");
	else {
		printf("Total pago: R$ %.2f\n", pago);
		printf("Total pendente: R$ %.2f\n\n", pendente);
	}
	system("pause");
}

// CADASTRAR GASTOS: lança uma despesa da academia (o código é automático).
void cadastrarGasto(void) {
	FILE *arq;
	Gasto novo;

	system("cls");
	printf("==============================================\n");
	printf("  CADASTRAR - Gastos\n");
	printf("==============================================\n\n");

	arq = fopen("gastos.bin", "ab+");
	if(arq == NULL) {
		printf("Erro na Abertura do Arquivo!\n\n");
		system("pause");
		return;
	}

	novo.codigo = proximoCodigo("gastos.bin", sizeof(Gasto));

	fflush(stdin);
	printf("Informe a Descrição do Gasto: ");
	fgets(novo.descricao, 61, stdin);
	if(validaExcedeu(novo.descricao) == 1) printf("Somente os Primeiros 60 Caracteres Serão Cadastrados!\n\n");

	printf("Informe a Categoria (ex.: Água, Luz, Manutenção): ");
	fgets(novo.categoria, 31, stdin);
	if(validaExcedeu(novo.categoria) == 1) printf("Somente os Primeiros 30 Caracteres Serão Cadastrados!\n\n");

	do {
		printf("Informe o Valor: R$ ");
		scanf("%f", &novo.valor);
		if(novo.valor <= 0) printf("O Valor Deve Ser Maior que Zero!\n\n");
	} while(novo.valor <= 0);

	do {
		printf("Informe a Data do Gasto (DD MM AAAA): ");
		scanf("%d%d%d", &novo.data.dia, &novo.data.mes, &novo.data.ano);
		if(validaData(novo.data) == 0) printf("Insira Uma Data Valida!\n\n");
	} while(validaData(novo.data) != 1);

	novo.ativo = 1;
	fseek(arq, 0, SEEK_END);
	fwrite(&novo, sizeof(Gasto), 1, arq);
	fclose(arq);
	printf("\nGasto cadastrado com sucesso! (código %d)\n", novo.codigo);
	system("pause");
}

// ===== PACOTES DE MODALIDADES (ideia do Kauan, adaptada para a struct Pacote) =====
// Um pacote agrupa até 5 atividades (as modalidades cadastradas em Atividades) e tem um
// valor de acréscimo sobre o plano. A Matrícula usa o pacote para calcular o valor total.

// Mostra as atividades ativas (código e nome) para o usuário escolher. Devolve quantas existem.
int mostrarAtividadesAtivas(void) {
	FILE *arq;
	Atividade a;
	int total = 0;

	arq = fopen("atividades.bin", "rb");
	if(arq != NULL) {
		printf("\nATIVIDADES DISPONÍVEIS\n");
		while(fread(&a, sizeof(Atividade), 1, arq) == 1) {
			if(a.ativo == 1) {
				printf("  [%d] %s\n", a.codigo, a.nome);
				total++;
			}
		}
		fclose(arq);
	}
	return total;
}

// Pede as atividades do pacote (de 1 a 5, sem repetir, todas existentes e ativas) e
// preenche qtdAtivPacote e codAtividades. Devolve 1 se deu certo e 0 se não há atividades.
int escolherAtividadesPacote(Pacote *p) {
	int cod, i, repetido, qtd = 0;

	if(mostrarAtividadesAtivas() == 0) {
		printf("Não existem atividades ativas cadastradas!\n");
		return 0;
	}

	do {
		printf("\nInforme o código da atividade %d de 5 (0 para terminar): ", qtd + 1);
		scanf("%d", &cod);
		if(cod != 0) {
			repetido = 0;
			for(i = 0; i < qtd; i++) {
				if(p->codAtividades[i] == cod) repetido = 1;
			}
			if(repetido == 1) printf("Esta atividade já está no pacote!\n");
			else if(!existeAtividade(cod)) printf("Atividade Não Encontrada ou Inativa!\n");
			else {
				p->codAtividades[qtd] = cod;
				qtd++;
			}
		} else if(qtd == 0) {
			printf("O pacote precisa de pelo menos uma atividade!\n");
		}
	} while(qtd < 5 && (cod != 0 || qtd == 0));

	for(i = qtd; i < 5; i++) p->codAtividades[i] = 0;
	p->qtdAtivPacote = qtd;
	return 1;
}

// Mostra um pacote com o nome das atividades que o compõem.
void mostrarPacote(Pacote p) {
	int i;
	char nomeAt[61];

	printf("Código: %d\n", p.codigo);
	printf("Nome: %s\n", p.nome);
	printf("Atividades:\n");
	for(i = 0; i < p.qtdAtivPacote; i++) {
		if(!nomeDaAtividade(p.codAtividades[i], nomeAt)) strcpy(nomeAt, "(não encontrada)");
		printf("  %d) %s\n", i + 1, nomeAt);
	}
	printf("Acréscimo sobre o plano: R$ %.2f\n", p.valor);
	printf("Ativo: ");
	if(p.ativo == 1) printf("Sim\n");
	else printf("Não\n");
	printf("==============================================\n");
}

// Devolve 1 se a atividade faz parte de algum pacote ativo (usada para bloquear a exclusão da atividade).
int atividadeEmPacoteAtivo(int codAtividade) {
	FILE *arq;
	Pacote temp;
	int i, achou = 0;

	arq = fopen("pacotes.bin", "rb");
	if(arq != NULL) {
		while(achou == 0 && fread(&temp, sizeof(Pacote), 1, arq) == 1) {
			if(temp.ativo == 1) {
				for(i = 0; i < temp.qtdAtivPacote; i++) {
					if(temp.codAtividades[i] == codAtividade) achou = 1;
				}
			}
		}
		fclose(arq);
	}
	return achou;
}

// Devolve 1 se existe matrícula ativa com esse pacote (usada para bloquear a exclusão do pacote).
int pacoteTemMatriculaAtiva(int codPacote) {
	FILE *arq;
	Matricula temp;
	int achou = 0;

	arq = fopen("matriculas.bin", "rb");
	if(arq != NULL) {
		while(achou == 0 && fread(&temp, sizeof(Matricula), 1, arq) == 1) {
			if(temp.codPacote == codPacote && temp.situacao == 1) achou = 1;
		}
		fclose(arq);
	}
	return achou;
}

// CADASTRAR PACOTE: código (1 a 1000, sem repetir), nome, acréscimo sobre o plano e atividades.
void cadastrarPacote(void) {
	FILE *arq;
	int pos, ok;

	system("cls");
	printf("==============================================\n");
	printf("  CADASTRAR - Pacotes de Modalidades\n");
	printf("==============================================\n\n");

	arq = fopen("pacotes.bin", "ab+");
	if(arq == NULL) {
		printf("Erro na Abertura do Arquivo!\n\n");
		system("pause");
		return;
	}

	do {
		do {
			printf("Informe o Código do Pacote (1 a 1000): ");
			scanf("%d", &pacotes.codigo);
			if(pacotes.codigo < 1 || pacotes.codigo > 1000) printf("O Código Deve Estar Entre 1 e 1000!\n\n");
		} while(pacotes.codigo < 1 || pacotes.codigo > 1000);

		pos = busca(arq, pacotes.codigo, ENT_PACOTE);
		if(pos != -1) printf("Código Já Cadastrado!\n\n");
	} while(pos != -1);

	fflush(stdin);
	printf("Informe o Nome do Pacote: ");
	fgets(pacotes.nome, 61, stdin);
	if(validaExcedeu(pacotes.nome) == 1) printf("Somente os Primeiros 60 Caracteres Serão Cadastrados!\n\n");

	do {
		printf("Informe o Acréscimo sobre o Plano (R$, 0 se não houver): ");
		scanf("%f", &pacotes.valor);
		if(pacotes.valor < 0) printf("O Valor Não Pode Ser Negativo!\n\n");
	} while(pacotes.valor < 0);

	ok = escolherAtividadesPacote(&pacotes);
	if(ok == 0) {
		fclose(arq);
		system("pause");
		return;
	}

	pacotes.ativo = 1;
	fseek(arq, 0, SEEK_END);
	fwrite(&pacotes, sizeof(Pacote), 1, arq);
	fclose(arq);
	printf("\nPacote cadastrado com sucesso!\n");
	system("pause");
}

// LISTAR PACOTES: lê pacotes.bin do início ao fim.
void listarPacotes(void) {
	FILE *arq;
	int total = 0;

	system("cls");
	printf("==============================================\n");
	printf("  LISTAGEM - Pacotes de Modalidades\n");
	printf("==============================================\n");

	arq = fopen("pacotes.bin", "rb");
	if(arq == NULL) {
		printf("Nenhum pacote cadastrado!\n\n");
	} else {
		while(fread(&pacotes, sizeof(Pacote), 1, arq) == 1) {
			mostrarPacote(pacotes);
			total++;
		}
		fclose(arq);
		if(total == 0) printf("Nenhum pacote cadastrado!\n\n");
		else printf("Total de pacotes: %d\n", total);
	}
	system("pause");
}

// ALTERAR PACOTE: nome, acréscimo, atividades ou situação. Regrava na mesma posição.
void alterarPacote(void) {
	FILE *arq;
	int opcao, codigo, pos, gravar, ok;

	do {
		system("cls");
		printf("==============================================\n");
		printf("  ALTERAR - Pacotes de Modalidades\n");
		printf("==============================================\n\n");
		printf("  [1]  Nome\n");
		printf("  [2]  Acréscimo sobre o plano\n");
		printf("  [3]  Atividades do pacote\n");
		printf("  [4]  Ativo\n");
		printf("  [0]  Voltar\n\n");
		printf("----------------------------------------------\n");
		printf("  Opção: ");
		opcao = -1;
		scanf("%d", &opcao);

		if(opcao != 0) {
			if(opcao < 1 || opcao > 4) {
				printf("\nOpção Inválida!\n\n");
			} else {
				arq = fopen("pacotes.bin", "rb+");
				if(arq == NULL) {
					printf("\nNenhum pacote cadastrado!\n\n");
				} else {
					printf("\nInforme o código do pacote: ");
					scanf("%d", &codigo);
					pos = busca(arq, codigo, ENT_PACOTE);
					if(pos == -1) {
						printf("\nPacote Não Encontrado!\n\n");
					} else {
						fseek(arq, pos, SEEK_SET);
						fread(&pacotes, sizeof(Pacote), 1, arq);
						gravar = 0;

						switch(opcao) {
							case 1:
								fflush(stdin);
								printf("Informe o Novo Nome: ");
								fgets(pacotes.nome, 61, stdin);
								if(validaExcedeu(pacotes.nome) == 1) printf("Somente os Primeiros 60 Caracteres Serão Cadastrados!\n\n");
								gravar = 1;
								break;
							case 2:
								do {
									printf("Informe o Novo Acréscimo (R$): ");
									scanf("%f", &pacotes.valor);
									if(pacotes.valor < 0) printf("O Valor Não Pode Ser Negativo!\n\n");
								} while(pacotes.valor < 0);
								gravar = 1;
								break;
							case 3:
								ok = escolherAtividadesPacote(&pacotes);
								if(ok == 1) gravar = 1;
								break;
							case 4:
								do {
									printf("Informe a Situação (1- Ativo / 0- Inativo): ");
									scanf("%d", &pacotes.ativo);
									if(pacotes.ativo != 1 && pacotes.ativo != 0) printf("Informe Uma Situação Válida!\n\n");
								} while(pacotes.ativo != 1 && pacotes.ativo != 0);
								gravar = 1;
								break;
						}

						if(gravar == 1) {
							fseek(arq, pos, SEEK_SET);
							fwrite(&pacotes, sizeof(Pacote), 1, arq);
							printf("\nAlteração Realizada com Sucesso!\n");
						}
					}
					fclose(arq);
				}
			}
			system("pause");
		}
	} while(opcao != 0);
}

// EXCLUIR PACOTE (exclusão física): bloqueia se existe matrícula ativa com o pacote.
void excluirPacote(void) {
	FILE *arq, *temp;
	Pacote tempPacote;
	int codigo, pos;

	system("cls");
	printf("==============================================\n");
	printf("  EXCLUIR - Pacotes de Modalidades\n");
	printf("==============================================\n\n");

	arq = fopen("pacotes.bin", "rb");
	if(arq == NULL) {
		printf("Nenhum pacote cadastrado!\n\n");
	} else {
		printf("Informe o código do pacote: ");
		scanf("%d", &codigo);
		pos = busca(arq, codigo, ENT_PACOTE);
		if(pos == -1) {
			printf("\nPacote Não Encontrado!\n");
			fclose(arq);
		} else {
			fseek(arq, pos, SEEK_SET);
			fread(&pacotes, sizeof(Pacote), 1, arq);
			printf("==============================================\n");
			mostrarPacote(pacotes);

			if(pacoteTemMatriculaAtiva(codigo) == 1) {
				printf("Não é possível excluir: existe matrícula ativa com este pacote!\n");
				fclose(arq);
			} else {
				printf("Deseja excluir? S/N: ");
				if(toupper(getche()) == 'S') {
					temp = fopen("auxiliar.bin", "wb");
					if(temp == NULL) {
						printf("\nErro ao Criar Arquivo Temporário!\n");
						fclose(arq);
					} else {
						rewind(arq);
						while(fread(&tempPacote, sizeof(Pacote), 1, arq) == 1) {
							if(tempPacote.codigo != codigo) fwrite(&tempPacote, sizeof(Pacote), 1, temp);
						}
						fclose(temp);
						fclose(arq);
						remove("pacotes.bin");
						rename("auxiliar.bin", "pacotes.bin");
						printf("\nO Pacote Foi Excluído com Sucesso!\n");
					}
				} else {
					printf("\nExclusão Cancelada!\n");
					fclose(arq);
				}
			}
		}
	}
	system("pause");
}

// ===== AGENDA DE PROFISSIONAIS =====
// Cada registro é um horário fixo na semana: um profissional conduz uma atividade em um dia, de uma hora a outra.
// As horas são guardadas como número HHMM (ex.: 1830 = 18:30).

// Devolve 1 se o número é uma hora válida no formato HHMM (de 0000 até 2359).
int validaHora(int hhmm) {
	if(hhmm < 0 || hhmm > 2359) return 0;
	if(hhmm % 100 > 59) return 0;
	return 1;
}

// Nome do dia da semana (1 = segunda ... 7 = domingo).
char *nomeDiaSemana(int dia) {
	switch(dia) {
		case 1: return "Segunda-feira";
		case 2: return "Terça-feira";
		case 3: return "Quarta-feira";
		case 4: return "Quinta-feira";
		case 5: return "Sexta-feira";
		case 6: return "Sábado";
		case 7: return "Domingo";
	}
	return "";
}

// Devolve 1 se o profissional já tem outro horário ativo que bate com este (mesmo dia e horários que se cruzam).
// codIgnorar é o código do próprio horário que está sendo alterado (no cadastro, passar 0).
int agendaConflita(int codProfissional, int dia, int inicio, int fim, int codIgnorar) {
	FILE *arq;
	Agenda temp;
	int achou = 0;

	arq = fopen("agenda.bin", "rb");
	if(arq != NULL) {
		while(achou == 0 && fread(&temp, sizeof(Agenda), 1, arq) == 1) {
			if(temp.ativo == 1 && temp.codigo != codIgnorar && temp.codProfissional == codProfissional && temp.diaSemana == dia) {
				if(inicio < temp.horaFim && temp.horaInicio < fim) achou = 1;
			}
		}
		fclose(arq);
	}
	return achou;
}

// Mostra um horário da agenda, um campo por linha.
void mostrarAgenda(Agenda a) {
	char nomeP[61], nomeAt[61];

	if(!nomeDoProfissional(a.codProfissional, nomeP)) strcpy(nomeP, "(não encontrado)");
	if(!nomeDaAtividade(a.codAtividade, nomeAt)) strcpy(nomeAt, "(não encontrada)");
	printf("Código: %d\n", a.codigo);
	printf("Profissional: %d - %s\n", a.codProfissional, nomeP);
	printf("Atividade: %d - %s\n", a.codAtividade, nomeAt);
	printf("Dia: %s\n", nomeDiaSemana(a.diaSemana));
	printf("Horário: %02d:%02d às %02d:%02d\n", a.horaInicio / 100, a.horaInicio % 100, a.horaFim / 100, a.horaFim % 100);
	printf("Ativo: ");
	if(a.ativo == 1) printf("Sim\n");
	else printf("Não\n");
	printf("==============================================\n");
}

// Pede dia da semana, hora inicial e hora final e repete até estar tudo válido e sem conflito.
// codProfissional e codIgnorar servem para conferir o conflito de horário.
void lerHorarioAgenda(Agenda *a, int codIgnorar) {
	int ok;

	do {
		ok = 1;
		do {
			printf("\nDia da Semana (1-Seg 2-Ter 3-Qua 4-Qui 5-Sex 6-Sáb 7-Dom): ");
			scanf("%d", &a->diaSemana);
			if(a->diaSemana < 1 || a->diaSemana > 7) printf("Dia Inválido!\n");
		} while(a->diaSemana < 1 || a->diaSemana > 7);

		do {
			printf("Hora Inicial (HHMM, ex.: 0800): ");
			scanf("%d", &a->horaInicio);
			if(validaHora(a->horaInicio) == 0) printf("Hora Inválida!\n");
		} while(validaHora(a->horaInicio) == 0);

		do {
			printf("Hora Final (HHMM, ex.: 0900): ");
			scanf("%d", &a->horaFim);
			if(validaHora(a->horaFim) == 0) printf("Hora Inválida!\n");
			else if(a->horaFim <= a->horaInicio) {
				printf("A Hora Final Deve Ser Depois da Inicial!\n");
				a->horaFim = -1;
			}
		} while(validaHora(a->horaFim) == 0);

		if(agendaConflita(a->codProfissional, a->diaSemana, a->horaInicio, a->horaFim, codIgnorar) == 1) {
			printf("Este Profissional Já Tem Um Horário Que Bate Com Este!\n");
			ok = 0;
		}
	} while(ok == 0);
}

// CADASTRAR AGENDA: profissional ativo, atividade ativa, dia e horário (sem conflito para o profissional).
void cadastrarAgenda(void) {
	FILE *arq;
	Agenda nova;
	int pos;

	system("cls");
	printf("==============================================\n");
	printf("  CADASTRAR - Agenda de Profissionais\n");
	printf("==============================================\n\n");

	if(!temProfissionalAtivo()) {
		printf("Não Existem Profissionais Ativos Cadastrados!\n\n");
		system("pause");
		return;
	}

	arq = fopen("agenda.bin", "ab+");
	if(arq == NULL) {
		printf("Erro na Abertura do Arquivo!\n\n");
		system("pause");
		return;
	}

	do {
		do {
			printf("Informe o Código do Horário (1 a 1000): ");
			scanf("%d", &nova.codigo);
			if(nova.codigo < 1 || nova.codigo > 1000) printf("O Código Deve Estar Entre 1 e 1000!\n\n");
		} while(nova.codigo < 1 || nova.codigo > 1000);

		pos = busca(arq, nova.codigo, ENT_AGENDA);
		if(pos != -1) printf("Código Já Cadastrado!\n\n");
	} while(pos != -1);

	do {
		printf("Informe o Código do Profissional (0 para cancelar): ");
		scanf("%d", &nova.codProfissional);
		if(nova.codProfissional == 0) {
			fclose(arq);
			return;
		}
		if(!existeProfissionalAvaliacao(nova.codProfissional)) printf("Profissional Não Encontrado ou Inativo!\n\n");
	} while(!existeProfissionalAvaliacao(nova.codProfissional));

	do {
		printf("Informe o Código da Atividade (0 para cancelar): ");
		scanf("%d", &nova.codAtividade);
		if(nova.codAtividade == 0) {
			fclose(arq);
			return;
		}
		if(!existeAtividade(nova.codAtividade)) printf("Atividade Não Encontrada ou Inativa!\n\n");
	} while(!existeAtividade(nova.codAtividade));

	lerHorarioAgenda(&nova, 0);

	nova.ativo = 1;
	fseek(arq, 0, SEEK_END);
	fwrite(&nova, sizeof(Agenda), 1, arq);
	fclose(arq);
	printf("\nHorário cadastrado com sucesso!\n");
	system("pause");
}

// LISTAR AGENDA: código de profissional 0 mostra a agenda de todos.
void listarAgenda(void) {
	FILE *arq;
	Agenda a;
	int codProfissional, total = 0;

	system("cls");
	printf("==============================================\n");
	printf("  LISTAGEM - Agenda de Profissionais\n");
	printf("==============================================\n\n");

	printf("Informe o Código do Profissional (0 para todos): ");
	scanf("%d", &codProfissional);

	printf("\n==============================================\n");
	arq = fopen("agenda.bin", "rb");
	if(arq == NULL) {
		printf("Nenhum horário cadastrado!\n\n");
	} else {
		while(fread(&a, sizeof(Agenda), 1, arq) == 1) {
			if(codProfissional == 0 || a.codProfissional == codProfissional) {
				mostrarAgenda(a);
				total++;
			}
		}
		fclose(arq);
		if(total == 0) printf("Nenhum horário encontrado!\n\n");
		else printf("Total de horários: %d\n", total);
	}
	system("pause");
}

// ALTERAR AGENDA: profissional, atividade, dia e horário ou situação. Regrava na mesma posição.
void alterarAgenda(void) {
	FILE *arq;
	Agenda a;
	int opcao, codigo, pos, gravar, ok;

	do {
		system("cls");
		printf("==============================================\n");
		printf("  ALTERAR - Agenda de Profissionais\n");
		printf("==============================================\n\n");
		printf("  [1]  Profissional\n");
		printf("  [2]  Atividade\n");
		printf("  [3]  Dia e horário\n");
		printf("  [4]  Ativo\n");
		printf("  [0]  Voltar\n\n");
		printf("----------------------------------------------\n");
		printf("  Opção: ");
		opcao = -1;
		scanf("%d", &opcao);

		if(opcao != 0) {
			if(opcao < 1 || opcao > 4) {
				printf("\nOpção Inválida!\n\n");
			} else {
				arq = fopen("agenda.bin", "rb+");
				if(arq == NULL) {
					printf("\nNenhum horário cadastrado!\n\n");
				} else {
					printf("\nInforme o código do horário: ");
					scanf("%d", &codigo);
					pos = busca(arq, codigo, ENT_AGENDA);
					if(pos == -1) {
						printf("\nHorário Não Encontrado!\n\n");
					} else {
						fseek(arq, pos, SEEK_SET);
						fread(&a, sizeof(Agenda), 1, arq);
						gravar = 0;

						switch(opcao) {
							case 1:
								do {
									printf("Informe o Novo Código do Profissional: ");
									scanf("%d", &a.codProfissional);
									ok = existeProfissionalAvaliacao(a.codProfissional);
									if(!ok) printf("Profissional Não Encontrado ou Inativo!\n\n");
								} while(!ok);
								if(agendaConflita(a.codProfissional, a.diaSemana, a.horaInicio, a.horaFim, a.codigo) == 1) {
									printf("Este Profissional Já Tem Um Horário Que Bate Com Este!\n");
								} else {
									gravar = 1;
								}
								break;
							case 2:
								do {
									printf("Informe o Novo Código da Atividade: ");
									scanf("%d", &a.codAtividade);
									ok = existeAtividade(a.codAtividade);
									if(!ok) printf("Atividade Não Encontrada ou Inativa!\n\n");
								} while(!ok);
								gravar = 1;
								break;
							case 3:
								lerHorarioAgenda(&a, a.codigo);
								gravar = 1;
								break;
							case 4:
								do {
									printf("Informe a Situação (1- Ativo / 0- Inativo): ");
									scanf("%d", &a.ativo);
									if(a.ativo != 1 && a.ativo != 0) printf("Informe Uma Situação Válida!\n\n");
								} while(a.ativo != 1 && a.ativo != 0);
								gravar = 1;
								break;
						}

						if(gravar == 1) {
							fseek(arq, pos, SEEK_SET);
							fwrite(&a, sizeof(Agenda), 1, arq);
							printf("\nAlteração Realizada com Sucesso!\n");
						}
					}
					fclose(arq);
				}
			}
			system("pause");
		}
	} while(opcao != 0);
}

// EXCLUIR AGENDA (exclusão física): mostra o horário, pede S/N e copia o resto para auxiliar.bin.
void excluirAgenda(void) {
	FILE *arq, *temp;
	Agenda a;
	int codigo, pos;

	system("cls");
	printf("==============================================\n");
	printf("  EXCLUIR - Agenda de Profissionais\n");
	printf("==============================================\n\n");

	arq = fopen("agenda.bin", "rb");
	if(arq == NULL) {
		printf("Nenhum horário cadastrado!\n\n");
	} else {
		printf("Informe o código do horário: ");
		scanf("%d", &codigo);
		pos = busca(arq, codigo, ENT_AGENDA);
		if(pos == -1) {
			printf("\nHorário Não Encontrado!\n");
			fclose(arq);
		} else {
			fseek(arq, pos, SEEK_SET);
			fread(&a, sizeof(Agenda), 1, arq);
			printf("==============================================\n");
			mostrarAgenda(a);
			printf("Deseja excluir? S/N: ");
			if(toupper(getche()) == 'S') {
				temp = fopen("auxiliar.bin", "wb");
				if(temp == NULL) {
					printf("\nErro ao Criar Arquivo Temporário!\n");
					fclose(arq);
				} else {
					rewind(arq);
					while(fread(&a, sizeof(Agenda), 1, arq) == 1) {
						if(a.codigo != codigo) fwrite(&a, sizeof(Agenda), 1, temp);
					}
					fclose(temp);
					fclose(arq);
					remove("agenda.bin");
					rename("auxiliar.bin", "agenda.bin");
					printf("\nO Horário Foi Excluído com Sucesso!\n");
				}
			} else {
				printf("\nExclusão Cancelada!\n");
				fclose(arq);
			}
		}
	}
	system("pause");
}

// ===== RELATÓRIOS GERENCIAIS (filtros do ERS) =====

// Pede duas datas válidas (a final não pode ser antes da inicial).
void lerPeriodo(Data *inicio, Data *fim) {
	do {
		printf("Informe a Data Inicial (DD MM AAAA): ");
		scanf("%d%d%d", &inicio->dia, &inicio->mes, &inicio->ano);
		if(validaData(*inicio) == 0) printf("Insira Uma Data Valida!\n\n");
	} while(validaData(*inicio) != 1);

	do {
		printf("Informe a Data Final (DD MM AAAA): ");
		scanf("%d%d%d", &fim->dia, &fim->mes, &fim->ano);
		if(validaData(*fim) == 0) printf("Insira Uma Data Valida!\n\n");
		else if(dataNumero(*fim) < dataNumero(*inicio)) {
			printf("A Data Final Não Pode Ser Antes da Inicial!\n\n");
			fim->ano = 0;
		}
	} while(validaData(*fim) != 1);
}

// Devolve o código do aluno da matrícula (0 se a matrícula não existe).
int codAlunoDaMatricula(int codMatricula) {
	FILE *arq;
	Matricula m;
	int pos, codAluno = 0;

	arq = fopen("matriculas.bin", "rb");
	if(arq != NULL) {
		pos = busca(arq, codMatricula, ENT_MATRICULA);
		if(pos != -1) {
			fseek(arq, pos, SEEK_SET);
			fread(&m, sizeof(Matricula), 1, arq);
			codAluno = m.codAluno;
		}
		fclose(arq);
	}
	return codAluno;
}

// RELATÓRIO 1 - EXTRATO DE PAGAMENTOS: cobranças por período, aluno e situação.
// O período vale para a data do pagamento (cobrança paga) ou para o vencimento (nas demais).
void extratoPagamentos(void) {
	FILE *arq;
	Cobranca c;
	Data inicio, fim, ref;
	int codAluno, situacao, codAlunoCob, total = 0;
	float soma = 0;
	char nomeA[61];

	system("cls");
	printf("==============================================\n");
	printf("  RELATÓRIO - Extrato de Pagamentos\n");
	printf("==============================================\n\n");

	printf("Informe o Código do Aluno (0 para todos): ");
	scanf("%d", &codAluno);
	do {
		printf("Situação (0- Todas / 1- Pendente / 2- Paga / 3- Cancelada): ");
		scanf("%d", &situacao);
		if(situacao < 0 || situacao > 3) printf("Situação Inválida!\n\n");
	} while(situacao < 0 || situacao > 3);
	lerPeriodo(&inicio, &fim);

	printf("\n==============================================\n");
	arq = fopen("cobrancas.bin", "rb");
	if(arq == NULL) {
		printf("Nenhuma cobrança cadastrada!\n\n");
	} else {
		while(fread(&c, sizeof(Cobranca), 1, arq) == 1) {
			if(c.situacao == 2) ref = c.dataPagamento;
			else ref = c.vencimento;
			codAlunoCob = codAlunoDaMatricula(c.codMatricula);

			if((codAluno == 0 || codAlunoCob == codAluno) && (situacao == 0 || c.situacao == situacao)
			   && dataNumero(ref) >= dataNumero(inicio) && dataNumero(ref) <= dataNumero(fim)) {
				if(!nomeDoAluno(codAlunoCob, nomeA)) strcpy(nomeA, "(não encontrado)");
				printf("Cobrança: %d | Matrícula: %d\n", c.codigo, c.codMatricula);
				printf("Aluno: %d - %s\n", codAlunoCob, nomeA);
				printf("Referência: %02d/%d | Vencimento: %02d/%02d/%d\n", c.mesRef, c.anoRef, c.vencimento.dia, c.vencimento.mes, c.vencimento.ano);
				printf("Valor: R$ %.2f | ", c.valor);
				if(c.situacao == 2) printf("Paga em %02d/%02d/%d\n", c.dataPagamento.dia, c.dataPagamento.mes, c.dataPagamento.ano);
				else if(c.situacao == 1) printf("Pendente\n");
				else printf("Cancelada\n");
				printf("==============================================\n");
				total++;
				soma = soma + c.valor;
			}
		}
		fclose(arq);
		if(total == 0) printf("Nenhum registro encontrado!\n\n");
		else printf("Total de cobranças: %d | Soma: R$ %.2f\n\n", total, soma);
	}
	system("pause");
}

// RELATÓRIO 2 - ALUNOS POR PLANO: matrículas por plano e situação.
void alunosPorPlano(void) {
	FILE *arq;
	Matricula m;
	int codPlano, situacao, total = 0;
	char nomeA[61];

	system("cls");
	printf("==============================================\n");
	printf("  RELATÓRIO - Alunos por Plano\n");
	printf("==============================================\n\n");

	printf("Informe o Código do Plano (0 para todos): ");
	scanf("%d", &codPlano);
	do {
		printf("Situação da Matrícula (0- Todas / 1- Ativa / 2- Cancelada): ");
		scanf("%d", &situacao);
		if(situacao < 0 || situacao > 2) printf("Situação Inválida!\n\n");
	} while(situacao < 0 || situacao > 2);

	printf("\n==============================================\n");
	arq = fopen("matriculas.bin", "rb");
	if(arq == NULL) {
		printf("Nenhuma matrícula cadastrada!\n\n");
	} else {
		while(fread(&m, sizeof(Matricula), 1, arq) == 1) {
			if((codPlano == 0 || m.codPlano == codPlano) && (situacao == 0 || m.situacao == situacao)) {
				if(!nomeDoAluno(m.codAluno, nomeA)) strcpy(nomeA, "(não encontrado)");
				if(buscarPlano(m.codPlano) == -1) strcpy(planos.nome, "(não encontrado)");
				printf("Plano: %d - %s\n", m.codPlano, planos.nome);
				printf("Aluno: %d - %s\n", m.codAluno, nomeA);
				printf("Matrícula: %d | ", m.codigo);
				if(m.situacao == 1) printf("Ativa\n");
				else printf("Cancelada\n");
				printf("==============================================\n");
				total++;
			}
		}
		fclose(arq);
		if(total == 0) printf("Nenhum registro encontrado!\n\n");
		else printf("Total de alunos: %d\n\n", total);
	}
	system("pause");
}

// RELATÓRIO 3 - ALUNOS INADIMPLENTES: cobranças pendentes de um mês de referência, já vencidas
// em relação à data informada, com filtro opcional de plano.
void alunosInadimplentes(void) {
	FILE *arq;
	Cobranca c;
	Data hoje;
	int mes, ano, codPlano, codAlunoCob, total = 0;
	float soma = 0;
	char nomeA[61];

	system("cls");
	printf("==============================================\n");
	printf("  RELATÓRIO - Alunos Inadimplentes\n");
	printf("==============================================\n\n");

	do {
		printf("Informe o Mês de Referência (1 a 12): ");
		scanf("%d", &mes);
		if(mes < 1 || mes > 12) printf("Mês Inválido!\n\n");
	} while(mes < 1 || mes > 12);
	do {
		printf("Informe o Ano de Referência (1900 a 2100): ");
		scanf("%d", &ano);
		if(ano < 1900 || ano > 2100) printf("Ano Inválido!\n\n");
	} while(ano < 1900 || ano > 2100);
	printf("Informe o Código do Plano (0 para todos): ");
	scanf("%d", &codPlano);
	do {
		printf("Informe a Data de Hoje (DD MM AAAA): ");
		scanf("%d%d%d", &hoje.dia, &hoje.mes, &hoje.ano);
		if(validaData(hoje) == 0) printf("Insira Uma Data Valida!\n\n");
	} while(validaData(hoje) != 1);

	printf("\n==============================================\n");
	arq = fopen("cobrancas.bin", "rb");
	if(arq == NULL) {
		printf("Nenhuma cobrança cadastrada!\n\n");
	} else {
		while(fread(&c, sizeof(Cobranca), 1, arq) == 1) {
			if(c.situacao == 1 && c.mesRef == mes && c.anoRef == ano && dataNumero(c.vencimento) < dataNumero(hoje)
			   && (codPlano == 0 || codPlanoDaMatricula(c.codMatricula) == codPlano)) {
				codAlunoCob = codAlunoDaMatricula(c.codMatricula);
				if(!nomeDoAluno(codAlunoCob, nomeA)) strcpy(nomeA, "(não encontrado)");
				printf("Aluno: %d - %s\n", codAlunoCob, nomeA);
				printf("Matrícula: %d | Cobrança: %d\n", c.codMatricula, c.codigo);
				printf("Vencimento: %02d/%02d/%d | Valor: R$ %.2f\n", c.vencimento.dia, c.vencimento.mes, c.vencimento.ano, c.valor);
				printf("==============================================\n");
				total++;
				soma = soma + c.valor;
			}
		}
		fclose(arq);
		if(total == 0) printf("Nenhum inadimplente encontrado!\n\n");
		else printf("Total de inadimplentes: %d | Em aberto: R$ %.2f\n\n", total, soma);
	}
	system("pause");
}

// RELATÓRIO 4 - MATRÍCULAS POR SITUAÇÃO: por situação, período de início e plano.
void matriculasPorSituacao(void) {
	FILE *arq;
	Matricula m;
	Data inicio, fim;
	int situacao, codPlano, total = 0;
	float soma = 0;
	char nomeA[61];

	system("cls");
	printf("==============================================\n");
	printf("  RELATÓRIO - Matrículas por Situação\n");
	printf("==============================================\n\n");

	do {
		printf("Situação (0- Todas / 1- Ativa / 2- Cancelada): ");
		scanf("%d", &situacao);
		if(situacao < 0 || situacao > 2) printf("Situação Inválida!\n\n");
	} while(situacao < 0 || situacao > 2);
	printf("Informe o Código do Plano (0 para todos): ");
	scanf("%d", &codPlano);
	printf("Período da Data de Início da Matrícula:\n");
	lerPeriodo(&inicio, &fim);

	printf("\n==============================================\n");
	arq = fopen("matriculas.bin", "rb");
	if(arq == NULL) {
		printf("Nenhuma matrícula cadastrada!\n\n");
	} else {
		while(fread(&m, sizeof(Matricula), 1, arq) == 1) {
			if((situacao == 0 || m.situacao == situacao) && (codPlano == 0 || m.codPlano == codPlano)
			   && dataNumero(m.dataInicio) >= dataNumero(inicio) && dataNumero(m.dataInicio) <= dataNumero(fim)) {
				if(!nomeDoAluno(m.codAluno, nomeA)) strcpy(nomeA, "(não encontrado)");
				printf("Matrícula: %d | Aluno: %d - %s\n", m.codigo, m.codAluno, nomeA);
				printf("Plano: %d | Início: %02d/%02d/%d | Término: %02d/%02d/%d\n", m.codPlano, m.dataInicio.dia, m.dataInicio.mes, m.dataInicio.ano, m.dataTermino.dia, m.dataTermino.mes, m.dataTermino.ano);
				printf("Valor total: R$ %.2f | ", m.valorTotal);
				if(m.situacao == 1) printf("Ativa\n");
				else printf("Cancelada\n");
				printf("==============================================\n");
				total++;
				soma = soma + m.valorTotal;
			}
		}
		fclose(arq);
		if(total == 0) printf("Nenhum registro encontrado!\n\n");
		else printf("Total de matrículas: %d | Soma dos valores: R$ %.2f\n\n", total, soma);
	}
	system("pause");
}

// Mensalidades recebidas de um plano em um mês/ano (pela data do pagamento).
float somaRecebidoPlanoMes(int codPlano, int mes, int ano) {
	FILE *arq;
	Cobranca c;
	float total = 0;

	arq = fopen("cobrancas.bin", "rb");
	if(arq != NULL) {
		while(fread(&c, sizeof(Cobranca), 1, arq) == 1) {
			if(c.situacao == 2 && c.dataPagamento.mes == mes && c.dataPagamento.ano == ano && codPlanoDaMatricula(c.codMatricula) == codPlano) total = total + c.valor;
		}
		fclose(arq);
	}
	return total;
}

// Valores retidos em cancelamentos de um plano em um mês/ano.
float somaRetidoPlanoMes(int codPlano, int mes, int ano) {
	FILE *arq;
	Cancelamento c;
	float total = 0;

	arq = fopen("cancelamentos.bin", "rb");
	if(arq != NULL) {
		while(fread(&c, sizeof(Cancelamento), 1, arq) == 1) {
			if(c.data.mes == mes && c.data.ano == ano && codPlanoDaMatricula(c.codMatricula) == codPlano) total = total + c.valorRetido;
		}
		fclose(arq);
	}
	return total;
}

// RELATÓRIO 5 - FATURAMENTO POR PLANO E MÊS: tabela de 12 meses para cada plano do filtro.
void faturamentoPlanoMes(void) {
	FILE *arq;
	Plano p;
	int ano, codPlano, mes, total = 0;
	float recebido, retido, totalPlano;

	system("cls");
	printf("==============================================\n");
	printf("  RELATÓRIO - Faturamento por Plano e Mês\n");
	printf("==============================================\n\n");

	do {
		printf("Informe o Ano (1900 a 2100): ");
		scanf("%d", &ano);
		if(ano < 1900 || ano > 2100) printf("Ano Inválido!\n\n");
	} while(ano < 1900 || ano > 2100);
	printf("Informe o Código do Plano (0 para todos): ");
	scanf("%d", &codPlano);

	arq = fopen("planos.bin", "rb");
	if(arq == NULL) {
		printf("\nNenhum plano cadastrado!\n\n");
		system("pause");
		return;
	}

	while(fread(&p, sizeof(Plano), 1, arq) == 1) {
		if(codPlano == 0 || p.codigo == codPlano) {
			total++;
			totalPlano = 0;
			printf("\n==============================================\n");
			printf("Plano: %d - %s (%d)\n", p.codigo, p.nome, ano);
			printf(" Mês | Mensalidades |    Retenções |        Total\n");
			for(mes = 1; mes <= 12; mes++) {
				recebido = somaRecebidoPlanoMes(p.codigo, mes, ano);
				retido = somaRetidoPlanoMes(p.codigo, mes, ano);
				printf("  %02d | R$ %9.2f | R$ %9.2f | R$ %9.2f\n", mes, recebido, retido, recebido + retido);
				totalPlano = totalPlano + recebido + retido;
			}
			printf("Total do plano no ano: R$ %.2f\n", totalPlano);
		}
	}
	fclose(arq);
	if(total == 0) printf("\nNenhum plano encontrado!\n");
	printf("\n");
	system("pause");
}

// ===== CPF: não pode repetir (ERS) =====
// Devolve 1 (e avisa) se outro aluno/profissional já usa esse CPF. codigo é o do próprio registro.
int cpfDuplicado(char cpf[], int codEnt, int codigo) {
	FILE *arq;
	Aluno a;
	Profissional p;
	int achou = 0;

	if(codEnt == ENT_ALUNO) {
		arq = fopen("alunos.bin", "rb");
		if(arq != NULL) {
			while(achou == 0 && fread(&a, sizeof(Aluno), 1, arq) == 1) {
				if(a.codigo != codigo && strcmp(a.cpf, cpf) == 0) achou = 1;
			}
			fclose(arq);
		}
	} else {
		arq = fopen("profissionais.bin", "rb");
		if(arq != NULL) {
			while(achou == 0 && fread(&p, sizeof(Profissional), 1, arq) == 1) {
				if(p.codigo != codigo && strcmp(p.cpf, cpf) == 0) achou = 1;
			}
			fclose(arq);
		}
	}
	if(achou == 1) printf("CPF Já Cadastrado!\n\n");
	return achou;
}

//6 - FUNÇÃO DE RELATÓRIOS
// ===== RELATÓRIO EM ORDEM ALFABÉTICA =====
// Ordena o próprio arquivo pelo nome (troca registros vizinhos com fseek/fread/fwrite,
// igual ao ordenar() da agenda da professora) e depois lista na tela.
void relatorioAlfabetico(void) {
	FILE *arq;
	Aluno        al, ali;
	Profissional pr, pri;
	Treino       tr, tri;
	Plano        pl, pli;
	Atividade    at, ati;
	Equipamento  eq, eqi;
	char arquivo[31], titulo[31];
	int opcao, codEnt, qtde, total, i;

	system("cls");
	printf("==============================================\n");
	printf("  RELATÓRIO - Ordem Alfabética\n");
	printf("==============================================\n\n");
	printf("  [1]  Alunos\n");
	printf("  [2]  Profissionais\n");
	printf("  [3]  Treinos\n");
	printf("  [4]  Planos\n");
	printf("  [5]  Atividades\n");
	printf("  [6]  Equipamentos\n");
	printf("  [0]  Voltar\n\n");
	printf("----------------------------------------------\n");
	printf("  Opção: ");
	opcao = -1;
	scanf("%d", &opcao);
	fflush(stdin);

	switch(opcao) {
		case 1: codEnt = ENT_ALUNO;        strcpy(arquivo, "alunos.bin");        strcpy(titulo, "Alunos");        break;
		case 2: codEnt = ENT_PROFISSIONAL; strcpy(arquivo, "profissionais.bin"); strcpy(titulo, "Profissionais"); break;
		case 3: codEnt = ENT_TREINO;       strcpy(arquivo, "treinos.bin");       strcpy(titulo, "Treinos");       break;
		case 4: codEnt = ENT_PLANO;        strcpy(arquivo, "planos.bin");        strcpy(titulo, "Planos");        break;
		case 5: codEnt = ENT_ATIVIDADE;    strcpy(arquivo, "atividades.bin");    strcpy(titulo, "Atividades");    break;
		case 6: codEnt = ENT_EQUIPAMENTO;  strcpy(arquivo, "equipamentos.bin");  strcpy(titulo, "Equipamentos");  break;
		case 0: return;
		default:
			printf("\n Opção Inválida!\n");
			system("pause");
			return;
	}

	arq = fopen(arquivo, "rb+");
	if(arq == NULL) {
		printf("\nNenhum Registro Cadastrado!\n\n");
		system("pause");
		return;
	}

	// quantidade de registros = tamanho do arquivo / tamanho de cada registro
	fseek(arq, 0, SEEK_END);
	switch(codEnt) {
		case ENT_ALUNO:        qtde = ftell(arq) / sizeof(Aluno);        break;
		case ENT_PROFISSIONAL: qtde = ftell(arq) / sizeof(Profissional); break;
		case ENT_TREINO:       qtde = ftell(arq) / sizeof(Treino);       break;
		case ENT_PLANO:        qtde = ftell(arq) / sizeof(Plano);        break;
		case ENT_ATIVIDADE:    qtde = ftell(arq) / sizeof(Atividade);    break;
		case ENT_EQUIPAMENTO:  qtde = ftell(arq) / sizeof(Equipamento);  break;
	}
	total = qtde;

	// ordena pelo nome (bubble sort direto no arquivo)
	while(qtde > 1) {
		for(i = 0; i < qtde - 1; i++) {
			switch(codEnt) {
				case ENT_ALUNO:
					fseek(arq, i * sizeof(Aluno), SEEK_SET);
					fread(&al, sizeof(Aluno), 1, arq);
					fseek(arq, (i + 1) * sizeof(Aluno), SEEK_SET);
					fread(&ali, sizeof(Aluno), 1, arq);
					if(stricmp(al.nome, ali.nome) > 0) {
						fseek(arq, i * sizeof(Aluno), SEEK_SET);
						fwrite(&ali, sizeof(Aluno), 1, arq);
						fseek(arq, (i + 1) * sizeof(Aluno), SEEK_SET);
						fwrite(&al, sizeof(Aluno), 1, arq);
					}
					break;
				case ENT_PROFISSIONAL:
					fseek(arq, i * sizeof(Profissional), SEEK_SET);
					fread(&pr, sizeof(Profissional), 1, arq);
					fseek(arq, (i + 1) * sizeof(Profissional), SEEK_SET);
					fread(&pri, sizeof(Profissional), 1, arq);
					if(stricmp(pr.nome, pri.nome) > 0) {
						fseek(arq, i * sizeof(Profissional), SEEK_SET);
						fwrite(&pri, sizeof(Profissional), 1, arq);
						fseek(arq, (i + 1) * sizeof(Profissional), SEEK_SET);
						fwrite(&pr, sizeof(Profissional), 1, arq);
					}
					break;
				case ENT_TREINO:
					fseek(arq, i * sizeof(Treino), SEEK_SET);
					fread(&tr, sizeof(Treino), 1, arq);
					fseek(arq, (i + 1) * sizeof(Treino), SEEK_SET);
					fread(&tri, sizeof(Treino), 1, arq);
					if(stricmp(tr.nome, tri.nome) > 0) {
						fseek(arq, i * sizeof(Treino), SEEK_SET);
						fwrite(&tri, sizeof(Treino), 1, arq);
						fseek(arq, (i + 1) * sizeof(Treino), SEEK_SET);
						fwrite(&tr, sizeof(Treino), 1, arq);
					}
					break;
				case ENT_PLANO:
					fseek(arq, i * sizeof(Plano), SEEK_SET);
					fread(&pl, sizeof(Plano), 1, arq);
					fseek(arq, (i + 1) * sizeof(Plano), SEEK_SET);
					fread(&pli, sizeof(Plano), 1, arq);
					if(stricmp(pl.nome, pli.nome) > 0) {
						fseek(arq, i * sizeof(Plano), SEEK_SET);
						fwrite(&pli, sizeof(Plano), 1, arq);
						fseek(arq, (i + 1) * sizeof(Plano), SEEK_SET);
						fwrite(&pl, sizeof(Plano), 1, arq);
					}
					break;
				case ENT_ATIVIDADE:
					fseek(arq, i * sizeof(Atividade), SEEK_SET);
					fread(&at, sizeof(Atividade), 1, arq);
					fseek(arq, (i + 1) * sizeof(Atividade), SEEK_SET);
					fread(&ati, sizeof(Atividade), 1, arq);
					if(stricmp(at.nome, ati.nome) > 0) {
						fseek(arq, i * sizeof(Atividade), SEEK_SET);
						fwrite(&ati, sizeof(Atividade), 1, arq);
						fseek(arq, (i + 1) * sizeof(Atividade), SEEK_SET);
						fwrite(&at, sizeof(Atividade), 1, arq);
					}
					break;
				case ENT_EQUIPAMENTO:
					fseek(arq, i * sizeof(Equipamento), SEEK_SET);
					fread(&eq, sizeof(Equipamento), 1, arq);
					fseek(arq, (i + 1) * sizeof(Equipamento), SEEK_SET);
					fread(&eqi, sizeof(Equipamento), 1, arq);
					if(stricmp(eq.nome, eqi.nome) > 0) {
						fseek(arq, i * sizeof(Equipamento), SEEK_SET);
						fwrite(&eqi, sizeof(Equipamento), 1, arq);
						fseek(arq, (i + 1) * sizeof(Equipamento), SEEK_SET);
						fwrite(&eq, sizeof(Equipamento), 1, arq);
					}
					break;
			}
		}
		qtde--;
	}

	// exibe na tela
	system("cls");
	printf("==============================================\n");
	printf("  %s em Ordem Alfabética\n", titulo);
	printf("==============================================\n\n");
	rewind(arq);
	switch(codEnt) {
		case ENT_ALUNO:
			while(fread(&al, sizeof(Aluno), 1, arq) == 1) {
				printf("  Código: %-5d Nome: %-30s %s\n", al.codigo, al.nome, al.ativo == 1 ? "Ativo" : "Inativo");
			}
			break;
		case ENT_PROFISSIONAL:
			while(fread(&pr, sizeof(Profissional), 1, arq) == 1) {
				printf("  Código: %-5d Nome: %-30s %s\n", pr.codigo, pr.nome, pr.ativo == 1 ? "Ativo" : "Inativo");
			}
			break;
		case ENT_TREINO:
			while(fread(&tr, sizeof(Treino), 1, arq) == 1) {
				printf("  Código: %-5d Nome: %-30s %s\n", tr.codigo, tr.nome, tr.ativo == 1 ? "Ativo" : "Inativo");
			}
			break;
		case ENT_PLANO:
			while(fread(&pl, sizeof(Plano), 1, arq) == 1) {
				printf("  Código: %-5d Nome: %-30s %s\n", pl.codigo, pl.nome, pl.ativo == 1 ? "Ativo" : "Inativo");
			}
			break;
		case ENT_ATIVIDADE:
			while(fread(&at, sizeof(Atividade), 1, arq) == 1) {
				printf("  Código: %-5d Nome: %-30s %s\n", at.codigo, at.nome, at.ativo == 1 ? "Ativo" : "Inativo");
			}
			break;
		case ENT_EQUIPAMENTO:
			while(fread(&eq, sizeof(Equipamento), 1, arq) == 1) {
				printf("  Código: %-5d Nome: %-30s %s\n", eq.codigo, eq.nome, eq.ativo == 1 ? "Ativo" : "Inativo");
			}
			break;
	}
	printf("\n  Total: %d registro(s)\n\n", total);
	fclose(arq);
	system("pause");
}

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
		printf("  [7]  Cadastros em ordem alfabética\n");
		printf("       Alunos, profissionais, treinos, planos e mais\n");
		printf("  [0]  Voltar ao menu principal\n");
		printf("----------------------------------------------\n");
		printf("  Opção: ");
		Opcoes = 0;
		scanf("%d", &Opcoes);
		fflush(stdin);

		switch(Opcoes) {
			case 1:
				extratoPagamentos();
				break;
			case 2:
				alunosPorPlano();
				break;
			case 3:
				alunosInadimplentes();
				break;
			case 4:
				matriculasPorSituacao();
				break;
			case 5:
				faturamentoPlanoMes();
				break;
			case 6:
				listarFrequencia();
				break;
			case 7:
				relatorioAlfabetico();
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
