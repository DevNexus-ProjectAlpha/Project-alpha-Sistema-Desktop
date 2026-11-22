# 💻 Project Alpha - Sistema Desktop

[![C](https://img.shields.io/badge/Linguagem-C-A8B9CC?style=flat-square&logo=c&logoColor=white)](https://en.cppreference.com/w/c)
[![Windows](https://img.shields.io/badge/Plataforma-Windows-0078D6?style=flat-square&logo=windows&logoColor=white)](#-como-compilar-e-executar)
[![Versão](https://img.shields.io/badge/vers%C3%A3o-3.5-C1121F?style=flat-square)](#-histórico-de-versões)

> Sistema de gestão interna da academia **Project Alpha** (Presidente Prudente – SP), feito em **linguagem C**, com os dados gravados em **arquivos binários**: cadastros, matrículas, cobranças, frequência, financeiro e relatórios.

**Website da academia:** [https://www.projectalpha.com.br/](https://www.projectalpha.com.br/) · [Repositório do website](https://github.com/DevNexus-ProjectAlpha/Project-alpha-Bootstrap)

---

## 📋 Sobre o projeto

Este sistema faz parte do conjunto de soluções que a consultoria **DevNexus** desenvolveu para a Project Alpha, junto com o website institucional. Ele é usado pela equipe da academia (recepção, instrutores, avaliador físico e gerência) para o dia a dia: cadastrar alunos e profissionais, fazer matrículas, receber mensalidades, registrar presenças e acompanhar o financeiro.

Desenvolvido na disciplina **Projeto Integrador de Disciplinas II**, do curso de **Análise e Desenvolvimento de Sistemas** da **Unoeste** (2026). As regras de negócio seguem a Especificação de Requisitos de Software (ERS) do projeto.

## ✨ Funcionalidades

O menu principal tem seis áreas:

| Menu | O que faz |
|---|---|
| **1 · Cadastros** | Alunos, profissionais, treinos, planos, avaliações físicas (com IMC calculado e classificado), pacotes de modalidades, agenda de profissionais (com checagem de conflito), equipamentos e atividades |
| **2 · Gerenciar** | Alterar, excluir, listar e consultar registros das nove entidades. Bloqueia exclusões de registros em uso, como um plano com matrícula |
| **3 · Operações** | Efetuar matrícula, efetuar recebimento e efetuar cancelamento |
| **4 · Controle de Frequência** | Chamada do dia, histórico e correção de presenças |
| **5 · Controle Financeiro** | Faturamento mensal, faturamento por plano, gastos mensais, pagamentos por aluno e cadastro de gastos |
| **6 · Relatórios** | Sete relatórios com filtros (veja abaixo) |

### 📊 Relatórios

| Relatório | Filtros |
|---|---|
| Extrato de pagamentos | Período, aluno e situação |
| Alunos por plano | Plano e situação da matrícula |
| Alunos inadimplentes | Mês de referência e plano |
| Matrículas por situação | Situação, período e plano |
| Faturamento por plano e mês | Ano e plano |
| Frequência | Aluno, período e atividade |
| Cadastros em ordem alfabética | Alunos, profissionais, treinos, planos, atividades e equipamentos |

## 💰 Regras de negócio

**Valor da matrícula:** (valor mensal do plano + acréscimo do pacote) × meses − desconto do plano.

| Plano | Meses | Desconto | Valor total com o pacote Start |
|---|---|---|---|
| Mensal | 1 | 0% | R$ 129,90 |
| Trimestral | 3 | 5% | R$ 370,22 |
| Semestral | 6 | 10% | R$ 701,46 |
| Anual | 12 | 15% | R$ 1.324,98 |

**Recebimento:** ao dar baixa em uma cobrança, o sistema gera automaticamente a do mês seguinte até o término do plano (dezembro gera janeiro do ano seguinte).

**Cancelamento:**
- **Plano mensal:** não há devolução, e as cobranças pendentes são canceladas.
- **Trimestral, semestral ou anual:** retém 30% do valor já pago e devolve 70%.

## 🗄️ Como os dados são guardados

Cada entidade tem o seu arquivo `.bin`. Os registros são gravados inteiros com `fwrite` e lidos com `fread`. Como todos os registros de um arquivo têm o mesmo tamanho, o sistema pula direto para qualquer um com `fseek`.

| Padrão | Como funciona |
|---|---|
| **Busca** | Uma única função `busca(arquivo, código, entidade)` para todas as entidades, que devolve a posição em bytes ou `-1` |
| **Alteração** | Abre em `rb+`, posiciona com `fseek` e regrava o registro na mesma posição |
| **Exclusão** | Física: copia todos os registros, menos o excluído, para `auxiliar.bin`, apaga o original e renomeia |
| **Leitura de texto** | `fgets` com o tamanho do campo, conferido por `validaExcedeu` |
| **Validações** | CPF, telefone, data e e-mail |

> ⚠️ Alterar um `struct` muda o tamanho dos registros, e os arquivos `.bin` existentes deixam de ser lidos corretamente. Quem mexer em um `struct` precisa refazer os arquivos de dados.

## 🧪 Base de dados de teste

Os arquivos `.bin` já vêm com dados fictícios de janeiro a dezembro de 2026 (data de referência: 15/11/2026), montados para todos os relatórios terem o que mostrar:

| Arquivo | Registros | | Arquivo | Registros |
|---|---|---|---|---|
| `alunos.bin` | 30 | | `cobrancas.bin` | 201 |
| `profissionais.bin` | 11 | | `cancelamentos.bin` | 2 |
| `atividades.bin` | 7 | | `treinos.bin` | 12 |
| `planos.bin` | 4 | | `avaliacoes.bin` | 20 |
| `pacotes.bin` | 4 | | `agenda.bin` | 16 |
| `matriculas.bin` | 26 | | `equipamentos.bin` | 12 |
| `presencas.bin` | 336 | | `gastos.bin` | 60 |

Situações preparadas de propósito: 9 alunos inadimplentes, 2 cancelamentos (um mensal e um anual), 4 alunos sem matrícula, cobranças a vencer em 2027, 2 equipamentos em manutenção, avaliações físicas em dois ciclos para acompanhar a evolução, 14% de faltas e resultado financeiro positivo no ano.

> **Faça uma cópia da pasta antes de testar.** A exclusão é física: o registro apagado não volta.

## ▶️ Como compilar e executar

O sistema usa `conio.h` e comandos do console do Windows, então roda no **Windows**.

**Opção 1: executável pronto**
1. Baixe ou clone o repositório
2. Execute `SistemaProjectAlpha.exe` na mesma pasta dos arquivos `.bin`

**Opção 2: compilar**
- **Dev-C++** ou **Code::Blocks**: abra `SistemaProjectAlpha.c` e compile/execute
- **GCC (MinGW)**:
  ```bash
  gcc SistemaProjectAlpha.c -o SistemaProjectAlpha.exe
  ```

O executável precisa estar na mesma pasta dos arquivos `.bin`.

## 📁 Estrutura

```
Project-alpha-Sistema-Desktop/
├── SistemaProjectAlpha.c             # Código-fonte (V3.5)
├── SistemaProjectAlpha.exe           # Executável (Windows)
├── Instrucoes_Menus_Dev_Nexus_v4.pdf # Relatório de instruções do sistema
├── alunos.bin, profissionais.bin ... # Base de dados de teste (14 arquivos)
├── docs/                             # Documentos do desenvolvimento
│   ├── Cronograma_Dev_Nexus_v1..v3.pdf
│   ├── Instrucoes_Menus_Dev_Nexus_v1..v3.pdf
│   ├── Relatorio_Pendencias_Dev_Nexus.pdf
│   └── Roteiro_Demonstracao_Apoio.pdf
└── README.md
```

O **Relatório de Instruções** (`Instrucoes_Menus_Dev_Nexus_v4.pdf`) explica cada parte do sistema em detalhes, com os padrões de código, a base de teste e o checklist de entrega.

## 📝 Histórico de Versões

Cada versão é um commit, com a data em que foi feita e quem trabalhou nela. Use `git log` ou a aba **Commits** para ver as mudanças, e as **tags** (`v1.0` a `v3.5`) para baixar uma versão específica.

| Versão | Data | O que mudou |
|---|---|---|
| **V3.5** | 19/11 | Classificação do IMC na avaliação física (William) |
| — | 18/11 | ✅ Checagem com a Profa. Carla Plantier Message: **nota 10** |
| V3.4 | 15/11 | Versão da checagem: correção do CPF, base de dados de teste e executável |
| V3.3 | 15/11 | Consulta de aluno e profissional por matrícula |
| V3.2 | 15/11 | Correções em alterar código, telefone e e-mail |
| V3.1 | 15/11 | `lerValorPlano` de volta e `arquivoExiste` na matrícula |
| V3.0 | 15/11 | Pacotes, Agenda, Relatórios 1 a 5, regras do ERS e CPF duplicado |
| V2.2 | 15/11 | Matrícula, Recebimento, Cancelamento, Frequência e Financeiro |
| V2.1 | 15/11 | `validaExcedeu` padronizado e `union` removido |
| V2.0 | 15/11 | Planos simplificado |
| V1.9 | 15/11 | `cadastrarTreino` completo e relatório em ordem alfabética |
| V1.8 | 15/11 | Treinos e Equipamentos |
| V1.7 | 14/11 | Segunda integração |
| V1.6 | 13/11 | Primeira integração das partes do grupo |
| V1.54 / V1.59 | 12/11* | Validações de CPF, telefone, data e e-mail |
| V1.53 | 11/11* | Primeira versão com arquivos binários e a função `busca()` |
| V1.52 | 10/11* | Alterar, excluir e listar alunos e profissionais |
| V1.5 / V1.51 | 08–09/11* | Cadastro de alunos e profissionais |
| V1.3 / V1.4 | 30/10 – 05/11* | Sistema inicial com vetores de registros |
| V1.0 a V1.2 | 12–15/10* | Rascunhos: menus e structs |

\* Datas aproximadas, reconstruídas a partir dos documentos do projeto (cronogramas e relatórios de instruções).

---

## 👥 Equipe DevNexus

| Integrante | Função |
|---|---|
| **Gustavo Santos Ribeiro de Goes** | Líder do Projeto |
| Ana Clara Lima Amorim | Desenvolvedora |
| Igor Domingos Marques | Desenvolvedor |
| Igor dos Santos Pereira | Desenvolvedor |
| Ismael Renan Pirondi Gola | Desenvolvedor |
| Kauan Felipe Oliveira Gois | Desenvolvedor |
| Luiggi de Aguiar Bezerra | Desenvolvedor |
| William Eduardo di Martini Minorelli | Desenvolvedor |

### 🧩 Contribuições

| Integrante | Partes do sistema |
|---|---|
| **Gustavo Santos** | Liderança, estrutura base e integração das partes; Recebimento, Cancelamento, Chamada (Frequência) e Financeiro; Agenda de profissionais; Relatórios 1 a 5 e relatório em ordem alfabética; consulta de aluno e profissional; regras do ERS (CPF duplicado e bloqueio de exclusão); padronização do `validaExcedeu`; adaptação de Treinos, Equipamentos e Pacotes ao padrão do sistema; testes, base de dados de teste e documentos |
| **William** | Alunos e Profissionais (cadastro, alterar, excluir e listar); validações de CPF, telefone, data e e-mail; classificação do IMC (V3.5) |
| **Ismael** | Avaliações físicas |
| **Luiggi** | Atividades |
| **Ana** | Planos, incluindo `lerValorPlano` |
| **Igor Domingos** | Treinos e Equipamentos |
| **Igor Santos** | Funções de apoio da matrícula e de desconto (`somaMeses`, `arquivoExiste`, `validaAluno`, `validaPlano`, `validaPacote`, `geraCobranca`) e início do `efetuarMatricula` |
| **Kauan** | Pacotes de modalidades |
| **Toda a equipe** | Checagem final em 18/11 com a Profa. Carla Plantier Message, com nota 10 |

Acadêmicos de **Análise e Desenvolvimento de Sistemas (ADS)** da **Unoeste**.

## 📄 Licença

Este projeto é fornecido como material educacional. Todos os direitos reservados aos autores.

---

<div align="center">

**Desenvolvido para fins acadêmicos · Presidente Prudente – SP · 2026**

</div>
