#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

#define MAX_PETS 100
#define TAM_NOME 50
#define TAM_CPF 15

typedef struct {
    char nomeTutor[TAM_NOME];
    char cpfTutor[TAM_CPF];
    char nomePet[TAM_NOME];
    char raca[TAM_NOME];
    int idade;
    float peso;
} Pet;

/* ---------- Funções auxiliares ---------- */

void limparBuffer() {
    int c;

    while ((c = getchar()) != '\n' && c != EOF) {
        /* Limpa os caracteres restantes */
    }
}

void lerTexto(char texto[], int tamanho) {
    if (fgets(texto, tamanho, stdin) != NULL) {
        texto[strcspn(texto, "\n")] = '\0';
    }
}

int validarCPF(const char cpf[]) {
    int i;

    if (strlen(cpf) != 11) {
        return 0;
    }

    for (i = 0; i < 11; i++) {
        if (!isdigit((unsigned char) cpf[i])) {
            return 0;
        }
    }

    return 1;
}

void lerCPF(char cpf[]) {
    char entrada[100];

    lerTexto(entrada, sizeof(entrada));

    if (validarCPF(entrada)) {
        strcpy(cpf, entrada);
    } else {
        cpf[0] = '\0';
    }
}

void linhaDupla() {
    printf("+------------------------------------------------+\n");
}

void linhaSimples() {
    printf("|------------------------------------------------|\n");
}

void tituloSistema() {
    linhaDupla();
    printf("|               PETSHOP MANAGER                  |\n");
    printf("|          Sistema de Cadastro de Pets           |\n");
    linhaDupla();
}

void tituloTela(const char titulo[]) {
    printf("\n");
    linhaDupla();
    printf("| %-46s |\n", titulo);
    linhaDupla();
}

void pausar() {
    printf("\nPressione ENTER para voltar ao menu...");
    getchar();
}

int buscarPetPorNome(Pet pets[], int quantidade, const char nome[]) {
    int i;

    for (i = 0; i < quantidade; i++) {
        if (strcmp(pets[i].nomePet, nome) == 0) {
            return i;
        }
    }

    return -1;
}

int buscarPetPorNomeECpf(
    Pet pets[],
    int quantidade,
    const char nome[],
    const char cpf[]
) {
    int i;

    for (i = 0; i < quantidade; i++) {
        if (
            strcmp(pets[i].nomePet, nome) == 0 &&
            strcmp(pets[i].cpfTutor, cpf) == 0
        ) {
            return i;
        }
    }

    return -1;
}

void mostrarPet(Pet pet, int numeroRegistro) {
    linhaDupla();
    printf("| PET #%02d                                        |\n", numeroRegistro);
    linhaDupla();
    printf("| Tutor        : %-31s |\n", pet.nomeTutor);
    printf("| CPF do tutor : %-31s |\n", pet.cpfTutor);
    linhaSimples();
    printf("| Nome do pet  : %-31s |\n", pet.nomePet);
    printf("| Raca         : %-31s |\n", pet.raca);
    printf("| Idade        : %-2d ano(s)                      |\n", pet.idade);
    printf("| Peso         : %-6.2f kg                        |\n", pet.peso);
    linhaDupla();
}

/* ---------- Funções principais do CRUD ---------- */

void cadastrarPet(Pet pets[], int *quantidade) {
    tituloTela("CADASTRAR NOVO PET");

    if (*quantidade >= MAX_PETS) {
        printf("Limite maximo de cadastros atingido.\n");
        return;
    }

    printf("Nome do tutor......: ");
    lerTexto(pets[*quantidade].nomeTutor, TAM_NOME);

    do {
        printf("CPF do tutor.......: ");
        lerCPF(pets[*quantidade].cpfTutor);

        if (!validarCPF(pets[*quantidade].cpfTutor)) {
            printf("CPF invalido. Digite exatamente 11 numeros.\n");
        }
    } while (!validarCPF(pets[*quantidade].cpfTutor));

    printf("Nome do pet........: ");
    lerTexto(pets[*quantidade].nomePet, TAM_NOME);

    printf("Raca...............: ");
    lerTexto(pets[*quantidade].raca, TAM_NOME);

    do {
        printf("Idade..............: ");

        if (scanf("%d", &pets[*quantidade].idade) != 1) {
            limparBuffer();
            pets[*quantidade].idade = -1;
            printf("Idade invalida. Digite apenas numeros.\n");
            continue;
        }

        limparBuffer();

        if (pets[*quantidade].idade < 0) {
            printf("Idade invalida. Digite um valor maior ou igual a zero.\n");
        }
    } while (pets[*quantidade].idade < 0);

    do {
        printf("Peso em kg.........: ");

        if (scanf("%f", &pets[*quantidade].peso) != 1) {
            limparBuffer();
            pets[*quantidade].peso = 0;
            printf("Peso invalido. Digite apenas numeros.\n");
            continue;
        }

        limparBuffer();

        if (pets[*quantidade].peso <= 0) {
            printf("Peso invalido. Digite um valor maior que zero.\n");
        }
    } while (pets[*quantidade].peso <= 0);

    (*quantidade)++;

    printf("\n[SUCESSO] Pet cadastrado com sucesso!\n");
}

void listarPets(Pet pets[], int quantidade) {
    int i;

    tituloTela("LISTAGEM DE PETS");

    if (quantidade == 0) {
        printf("Nenhum pet cadastrado no sistema.\n");
        return;
    }

    printf("Total de pets cadastrados: %d\n\n", quantidade);

    for (i = 0; i < quantidade; i++) {
        mostrarPet(pets[i], i + 1);
        printf("\n");
    }
}

void editarPet(Pet pets[], int quantidade) {
    char nomeBusca[TAM_NOME];
    int indice;

    tituloTela("EDITAR CADASTRO DE PET");

    if (quantidade == 0) {
        printf("Nenhum pet cadastrado no sistema.\n");
        return;
    }

    printf("Digite o nome do pet que deseja editar: ");
    lerTexto(nomeBusca, TAM_NOME);

    indice = buscarPetPorNome(pets, quantidade, nomeBusca);

    if (indice == -1) {
        printf("\n[ERRO] Pet nao encontrado.\n");
        return;
    }

    printf("\nRegistro localizado:\n\n");
    mostrarPet(pets[indice], indice + 1);

    printf("\nInforme os novos dados do cadastro:\n\n");

    printf("Novo nome do tutor.: ");
    lerTexto(pets[indice].nomeTutor, TAM_NOME);

    do {
        printf("Novo CPF do tutor..: ");
        lerCPF(pets[indice].cpfTutor);

        if (!validarCPF(pets[indice].cpfTutor)) {
            printf("CPF invalido. Digite exatamente 11 numeros.\n");
        }
    } while (!validarCPF(pets[indice].cpfTutor));

    printf("Novo nome do pet...: ");
    lerTexto(pets[indice].nomePet, TAM_NOME);

    printf("Nova raca..........: ");
    lerTexto(pets[indice].raca, TAM_NOME);

    do {
        printf("Nova idade.........: ");

        if (scanf("%d", &pets[indice].idade) != 1) {
            limparBuffer();
            pets[indice].idade = -1;
            printf("Idade invalida. Digite apenas numeros.\n");
            continue;
        }

        limparBuffer();

        if (pets[indice].idade < 0) {
            printf("Idade invalida. Digite um valor maior ou igual a zero.\n");
        }
    } while (pets[indice].idade < 0);

    do {
        printf("Novo peso em kg....: ");

        if (scanf("%f", &pets[indice].peso) != 1) {
            limparBuffer();
            pets[indice].peso = 0;
            printf("Peso invalido. Digite apenas numeros.\n");
            continue;
        }

        limparBuffer();

        if (pets[indice].peso <= 0) {
            printf("Peso invalido. Digite um valor maior que zero.\n");
        }
    } while (pets[indice].peso <= 0);

    printf("\n[SUCESSO] Cadastro atualizado com sucesso!\n");
}

void excluirPet(Pet pets[], int *quantidade) {
    char nomeBusca[TAM_NOME];
    char cpfBusca[TAM_CPF];
    char confirmar;
    int indice;
    int i;

    tituloTela("EXCLUIR CADASTRO DE PET");

    if (*quantidade == 0) {
        printf("Nenhum pet cadastrado no sistema.\n");
        return;
    }

    printf("Digite o nome do pet que deseja excluir: ");
    lerTexto(nomeBusca, TAM_NOME);

    do {
        printf("Digite o CPF do tutor do pet: ");
        lerCPF(cpfBusca);

        if (!validarCPF(cpfBusca)) {
            printf("CPF invalido. Digite exatamente 11 numeros.\n");
        }
    } while (!validarCPF(cpfBusca));

    indice = buscarPetPorNomeECpf(
        pets,
        *quantidade,
        nomeBusca,
        cpfBusca
    );

    if (indice == -1) {
        printf("\n[ERRO] Pet nao encontrado para o nome e CPF informados.\n");
        return;
    }

    printf("\nRegistro localizado:\n\n");
    mostrarPet(pets[indice], indice + 1);

    printf("\nDeseja realmente excluir este cadastro? (S/N): ");
    scanf(" %c", &confirmar);
    limparBuffer();

    if (confirmar != 'S' && confirmar != 's') {
        printf("\n[AVISO] Exclusao cancelada pelo usuario.\n");
        return;
    }

    for (i = indice; i < *quantidade - 1; i++) {
        pets[i] = pets[i + 1];
    }

    (*quantidade)--;

    printf("\n[SUCESSO] Pet excluido com sucesso!\n");
}

/* ---------- Funcionalidade extra ---------- */

void buscarPetsPorCPF(Pet pets[], int quantidade) {
    char cpfBusca[TAM_CPF];
    int i;
    int encontrou = 0;

    tituloTela("BUSCAR PETS POR CPF DO TUTOR");

    if (quantidade == 0) {
        printf("Nenhum pet cadastrado no sistema.\n");
        return;
    }

    do {
        printf("Digite o CPF do tutor: ");
        lerCPF(cpfBusca);

        if (!validarCPF(cpfBusca)) {
            printf("CPF invalido. Digite exatamente 11 numeros.\n");
        }
    } while (!validarCPF(cpfBusca));

    printf("\nResultado da busca:\n\n");

    for (i = 0; i < quantidade; i++) {
        if (strcmp(pets[i].cpfTutor, cpfBusca) == 0) {
            mostrarPet(pets[i], i + 1);
            printf("\n");
            encontrou = 1;
        }
    }

    if (!encontrou) {
        printf("[AVISO] Nenhum pet foi encontrado para esse CPF.\n");
    } else {
        printf("[SUCESSO] Busca finalizada.\n");
    }
}

/* ---------- Menu ---------- */

void exibirMenu() {
    printf("\n");
    tituloSistema();
    printf("| [1] Cadastrar pet                               |\n");
    printf("| [2] Listar pets                                 |\n");
    printf("| [3] Editar pet                                  |\n");
    printf("| [4] Excluir pet                                 |\n");
    printf("| [5] Buscar pets por CPF do tutor                |\n");
    printf("| [0] Sair                                        |\n");
    linhaDupla();
    printf("Escolha uma opcao: ");
}

int main() {
    Pet pets[MAX_PETS];
    int quantidade = 0;
    int opcao = -1;

    setlocale(LC_ALL, "");

    do {
        exibirMenu();

        if (scanf("%d", &opcao) != 1) {
            limparBuffer();
            printf("\n[ERRO] Digite apenas uma opcao numerica.\n");
            pausar();
            continue;
        }

        limparBuffer();

        switch (opcao) {
            case 1:
                cadastrarPet(pets, &quantidade);
                pausar();
                break;

            case 2:
                listarPets(pets, quantidade);
                pausar();
                break;

            case 3:
                editarPet(pets, quantidade);
                pausar();
                break;

            case 4:
                excluirPet(pets, &quantidade);
                pausar();
                break;

            case 5:
                buscarPetsPorCPF(pets, quantidade);
                pausar();
                break;

            case 0:
                printf(
                    "\nSistema encerrado. "
                    "Obrigado por utilizar o PetShop Manager!\n"
                );
                break;

            default:
                printf("\n[ERRO] Opcao invalida. Tente novamente.\n");
                pausar();
        }

    } while (opcao != 0);

    return 0;
}
