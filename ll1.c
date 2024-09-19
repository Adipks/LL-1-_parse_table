#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PROD 20
#define MAX_SYMBOLS 100

#define MAX_TERMINALS 100
#define MAX_NON_TERMINALS 100

char terminals[MAX_TERMINALS];
int num_terminals = 0;

char* parsing_table[MAX_NON_TERMINALS][MAX_TERMINALS];

typedef struct {
    char symbol;
    char production[MAX_PROD][MAX_SYMBOLS];
    int num_productions;
    char first_set[MAX_SYMBOLS];
    int first_set_size;
    char follow_set[MAX_SYMBOLS];
    int follow_set_size;
} NonTerminal;

NonTerminal* non_terminals;
int num_nt = 0;

void add_to_set(char* set, int* set_size, char symbol) {
    for (int i = 0; i < *set_size; i++) {
        if (set[i] == symbol) return;
    }
    set[(*set_size)++] = symbol;
}

int find_non_terminal(char symbol) {
    for (int i = 0; i < num_nt; i++) {
        if (non_terminals[i].symbol == symbol) {
            return i;
        }
    }
    return -1;
}

void compute_first(int nt_index) {
    if (non_terminals[nt_index].first_set_size > 0) return;

    for (int i = 0; i < non_terminals[nt_index].num_productions; i++) {
        char* prod = non_terminals[nt_index].production[i];
        int all_nullable = 1;

        for (int j = 0; prod[j] != '\0'; j++) {
            if (islower(prod[j]) || !isalpha(prod[j])) {
                add_to_set(non_terminals[nt_index].first_set, &non_terminals[nt_index].first_set_size, prod[j]);
                all_nullable = 0;
                break;
            } else if (prod[j] == '#') {
                add_to_set(non_terminals[nt_index].first_set, &non_terminals[nt_index].first_set_size, '#');
                break;
            } else {
                int next_nt = find_non_terminal(prod[j]);
                if (next_nt != -1) {
                    compute_first(next_nt);
                    int nullable = 0;
                    for (int k = 0; k < non_terminals[next_nt].first_set_size; k++) {
                        if (non_terminals[next_nt].first_set[k] != '#') {
                            add_to_set(non_terminals[nt_index].first_set, &non_terminals[nt_index].first_set_size, non_terminals[next_nt].first_set[k]);
                        } else {
                            nullable = 1;
                        }
                    }
                    if (!nullable) {
                        all_nullable = 0;
                        break;
                    }
                }
            }
        }

        if (all_nullable) {
            add_to_set(non_terminals[nt_index].first_set, &non_terminals[nt_index].first_set_size, '#');
        }
    }
}

void compute_follow(int nt_index) {
    for (int i = 0; i < num_nt; i++) {
        for (int j = 0; j < non_terminals[i].num_productions; j++) {
            char* prod = non_terminals[i].production[j];
            int prod_len = strlen(prod);
            for (int k = 0; k < prod_len; k++) {
                if (prod[k] == non_terminals[nt_index].symbol) {
                    if (k + 1 < prod_len) {
                        if (islower(prod[k + 1]) || !isalpha(prod[k + 1])) {
                            add_to_set(non_terminals[nt_index].follow_set, &non_terminals[nt_index].follow_set_size, prod[k + 1]);
                        } else {
                            int next_nt = find_non_terminal(prod[k + 1]);
                            if (next_nt != -1) {
                                for (int l = 0; l < non_terminals[next_nt].first_set_size; l++) {
                                    if (non_terminals[next_nt].first_set[l] != '#') {
                                        add_to_set(non_terminals[nt_index].follow_set, &non_terminals[nt_index].follow_set_size, non_terminals[next_nt].first_set[l]);
                                    }
                                }
                                
                                if (strchr(non_terminals[next_nt].first_set, '#') != NULL) {
                                    int m = k + 2;
                                    while (m < prod_len) {
                                        if (islower(prod[m]) || !isalpha(prod[m])) {
                                            add_to_set(non_terminals[nt_index].follow_set, &non_terminals[nt_index].follow_set_size, prod[m]);
                                            break;
                                        } else {
                                            int next_next_nt = find_non_terminal(prod[m]);
                                            if (next_next_nt != -1) {
                                                for (int n = 0; n < non_terminals[next_next_nt].first_set_size; n++) {
                                                    if (non_terminals[next_next_nt].first_set[n] != '#') {
                                                        add_to_set(non_terminals[nt_index].follow_set, &non_terminals[nt_index].follow_set_size, non_terminals[next_next_nt].first_set[n]);
                                                    }
                                                }
                                                if (strchr(non_terminals[next_next_nt].first_set, '#') == NULL) {
                                                    break;
                                                }
                                            }
                                        }
                                        m++;
                                    }
                                    if (m == prod_len && i != nt_index) {
                                        for (int l = 0; l < non_terminals[i].follow_set_size; l++) {
                                            add_to_set(non_terminals[nt_index].follow_set, &non_terminals[nt_index].follow_set_size, non_terminals[i].follow_set[l]);
                                        }
                                    }
                                }
                            }
                        }
                    } else if (i != nt_index) {
                        for (int l = 0; l < non_terminals[i].follow_set_size; l++) {
                            add_to_set(non_terminals[nt_index].follow_set, &non_terminals[nt_index].follow_set_size, non_terminals[i].follow_set[l]);
                        }
                    }
                }
            }
        }
    }
}

void collect_terminals() {
    for (int i = 0; i < num_nt; i++) {
        for (int j = 0; j < non_terminals[i].first_set_size; j++) {
            if (non_terminals[i].first_set[j] != '#' && !isupper(non_terminals[i].first_set[j])) {
                add_to_set(terminals, &num_terminals, non_terminals[i].first_set[j]);
            }
        }
        for (int j = 0; j < non_terminals[i].follow_set_size; j++) {
            if (non_terminals[i].follow_set[j] != '#' && !isupper(non_terminals[i].follow_set[j])) {
                add_to_set(terminals, &num_terminals, non_terminals[i].follow_set[j]);
            }
        }
    }
    add_to_set(terminals, &num_terminals, '$');  // Add end-of-input symbol
}

int find_terminal(char symbol) {
    for (int i = 0; i < num_terminals; i++) {
        if (terminals[i] == symbol) {
            return i;
        }
    }
    return -1;
}

void compute_parsing_table() {
    // Initialize table with NULL entries
    for (int i = 0; i < num_nt; i++) {
        for (int j = 0; j < num_terminals; j++) {
            parsing_table[i][j] = NULL;
        }
    }

    for (int i = 0; i < num_nt; i++) {
        for (int j = 0; j < non_terminals[i].num_productions; j++) {
            char* prod = non_terminals[i].production[j];
            if (prod[0] == '#') {  // epsilon production
                for (int k = 0; k < non_terminals[i].follow_set_size; k++) {
                    char follow_symbol = non_terminals[i].follow_set[k];
                    int terminal_index = find_terminal(follow_symbol);
                    if (terminal_index != -1 && parsing_table[i][terminal_index] == NULL) {
                        parsing_table[i][terminal_index] = prod;
                    }
                }
            } else {
                int first_nt_index = find_non_terminal(prod[0]);
                if (first_nt_index != -1) {  // production starts with a non-terminal
                    for (int k = 0; k < non_terminals[first_nt_index].first_set_size; k++) {
                        char first_symbol = non_terminals[first_nt_index].first_set[k];
                        if (first_symbol != '#') {
                            int terminal_index = find_terminal(first_symbol);
                            if (terminal_index != -1 && parsing_table[i][terminal_index] == NULL) {
                                parsing_table[i][terminal_index] = prod;
                            }
                        }
                    }
                    if (strchr(non_terminals[first_nt_index].first_set, '#') != NULL) {
                        for (int k = 0; k < non_terminals[i].follow_set_size; k++) {
                            char follow_symbol = non_terminals[i].follow_set[k];
                            int terminal_index = find_terminal(follow_symbol);
                            if (terminal_index != -1 && parsing_table[i][terminal_index] == NULL) {
                                parsing_table[i][terminal_index] = prod;
                            }
                        }
                    }
                } else {  // production starts with a terminal
                    int terminal_index = find_terminal(prod[0]);
                    if (terminal_index != -1 && parsing_table[i][terminal_index] == NULL) {
                        parsing_table[i][terminal_index] = prod;
                    }
                }
            }
        }
    }
}

void print_parsing_table() {
    printf("\nLL(1) Parsing Table:\n");
    printf("%10s", ""); // Blank space for alignment
    for (int j = 0; j < num_terminals; j++) {
        printf("%10c", terminals[j]);
    }
    printf("\n");
    
    for (int i = 0; i < num_nt; i++) {
        printf("%10c", non_terminals[i].symbol);
        for (int j = 0; j < num_terminals; j++) {
            if (parsing_table[i][j] != NULL) {
                printf("%10s", parsing_table[i][j]);
            } else {
                printf("%10s", "");
            }
        }
        printf("\n");
    }
}


int main() 
{
    printf("Enter the number of non-terminals: ");
    scanf("%d", &num_nt);
    getchar(); // Consume newline

    non_terminals = (NonTerminal*)malloc(num_nt * sizeof(NonTerminal));
    if (non_terminals == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    for (int i = 0; i < num_nt; i++) {
        printf("Enter non-terminal %d: ", i + 1);
        scanf("%c", &non_terminals[i].symbol);
        getchar(); // Consume newline

        printf("Enter production for %c: ", non_terminals[i].symbol);
        fgets(non_terminals[i].production[0], MAX_SYMBOLS, stdin);
        non_terminals[i].production[0][strcspn(non_terminals[i].production[0], "\n")] = 0; // Remove newline

        non_terminals[i].num_productions = 1;
        non_terminals[i].first_set_size = 0;
        non_terminals[i].follow_set_size = 0;

        char* token = strtok(non_terminals[i].production[0], "|");
        int j = 0;
        while (token != NULL) {
            strcpy(non_terminals[i].production[j++], token);
            token = strtok(NULL, "|");
        }
        non_terminals[i].num_productions = j;
    }

    for (int i = 0; i < num_nt; i++) {
        compute_first(i);
    }

    // Assuming the first non-terminal is the start symbol
    add_to_set(non_terminals[0].follow_set, &non_terminals[0].follow_set_size, '$');
    for (int i = 0; i < num_nt; i++) {
        compute_follow(i);
    }

    printf("\nFirst sets:\n");
    for (int i = 0; i < num_nt; i++) {
        printf("First(%c) = { ", non_terminals[i].symbol);
        for (int j = 0; j < non_terminals[i].first_set_size; j++) {
            printf("%c", non_terminals[i].first_set[j]);
            if (j < non_terminals[i].first_set_size - 1) printf(", ");
        }
        printf(" }\n");
    }

    printf("\nFollow sets:\n");
    for (int i = 0; i < num_nt; i++) {
        printf("Follow(%c) = { ", non_terminals[i].symbol);
        for (int j = 0; j < non_terminals[i].follow_set_size; j++) {
            printf("%c", non_terminals[i].follow_set[j]);
            if (j < non_terminals[i].follow_set_size - 1) printf(", ");
        }
        printf(" }\n");
    }
    collect_terminals();
    compute_parsing_table();
    print_parsing_table();

    free(non_terminals);
    return 0;
}
