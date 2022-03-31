#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gradebook.h"

gradebook_t *create_gradebook(const char *class_name) {
    gradebook_t *new_book = malloc(sizeof(gradebook_t));
    if (new_book == NULL) {
        return NULL;
    }

    strcpy(new_book->class_name, class_name);
    new_book->root = NULL;
    return new_book;
}

const char *get_gradebook_name(const gradebook_t *book) {
    printf("%s", book->class_name);
    return NULL;
}

node_t* insertion(const char *name, int score) {
    node_t *new_node = malloc(sizeof(node_t));
    strcpy(new_node->name,name);
    new_node->score=score;
    new_node->left=NULL;
    new_node->right=NULL;
    return new_node;
}

int add_helper(gradebook_t *book, const char *name, int score, node_t *current) {
    if (score < 0) {
        printf("Error: You must create or load a gradebook first");
        return 0;
    }
    if (book->root == NULL) {
        book->root = insertion(name, score);
        return 0;
    }
    if (strcmp(name, current->name) == 0) {
        current->score = score;
        return 0;
    }
    if (strcmp(name, current->name) < 0) {
        if (current->left != NULL) {
            add_helper(book, name, score, current->left);
        } else {
            current->left = insertion(name, score);
            return 0;
        }
    } else if (strcmp(name, current->name) > 0) {
        if (current->right != NULL) {
            add_helper(book, name, score, current->right);
        } else {
            current->right = insertion(name, score);
            return 0;
        }
    }
    return 0;
}

int add_score(gradebook_t *book, const char *name, int score) {
    node_t *current = book->root;
    add_helper(book, name, score, current);
    return 0;
}

int find_helper(const gradebook_t *book, const char *name, node_t *current) {
    if (book == NULL) {
            printf("Error: You must create or load a gradebook first");
            return 0;
        }
        if (current == NULL) {
            printf("No score for '%s' found\n",name);
            return 0;
        }
        if (strcmp(current->name, name) == 0) {
            printf("%s: %d\n", current->name, current->score);
            return 1;
        }
        if (strcmp(name, current->name) < 0) {
            if (current->left != NULL) {
                find_helper(book, name, current->left);
            } else {
                printf("No score for '%s' found\n",name);
                return 0;
            }
        } else if (strcmp(name, current->name) > 0) {
            if (current->right != NULL) {
                find_helper(book, name, current->right);
            } else {
                printf("No score for '%s' found\n",name);
                return 0;
            }
        }
        
    return 0;
}

int find_score(const gradebook_t *book, const char *name) {
    node_t *current = book->root;
    find_helper(book, name, current);
    return 0;
}

void print_gradebook_aux(const node_t *current) {
    if (current == NULL) {
        return;
    }

    print_gradebook_aux(current->left);
    printf("%s: %d\n",current->name, current->score); 
    print_gradebook_aux(current->right);
}

void print_gradebook(const gradebook_t *book) {
    printf("Scores for all students in %s:\n", book->class_name);
    
    print_gradebook_aux(book->root);
}

void free_gradebook_aux(node_t * current) {
    if (current != NULL) {
        free_gradebook_aux(current->right);
        free_gradebook_aux(current->left);
        free(current);
    }
}
void free_gradebook(gradebook_t *book) {
    if (book != NULL) {
    free_gradebook_aux(book->root);
    }
    free(book);
}

// Helper function to allow recursion for writing out tree
int write_gradebook_to_text_aux(const node_t *current, FILE *f) {
    if (current == NULL) {
        return 0;
    }

    // Each line is formatted as "<name> <score>"
    fprintf(f, "%s %d\n", current->name, current->score);

    if (current->left != NULL) {
        if (write_gradebook_to_text_aux(current->left, f) != 0) {
            return -1;
        }
    }

    if (current->right != NULL) {
        if (write_gradebook_to_text_aux(current->right, f) != 0) {
            return -1;
        }
    }

    return 0;
}

int write_gradebook_to_text(const gradebook_t *book) {
    char file_name[MAX_NAME_LEN + strlen(".txt")];
    strcpy(file_name, book->class_name);
    strcat(file_name, ".txt");

    FILE *f = fopen(file_name, "w");
    if (f == NULL) {
        printf("Failed to write gradebook to text file\n");
        return -1;
    }

    int result = write_gradebook_to_text_aux(book->root, f);
    printf("Gradebook successfully written to %s\n", file_name);
    fclose(f);
    return result;
}

gradebook_t *read_gradebook_from_text(const char *file_name) {
            FILE *f = fopen(file_name, "r");
            if (f == NULL) {
                printf("Failed to read gradebook from text file\n");
            } else {
            char *dec = strchr(file_name, '.');
                if(dec != NULL) {
                    *dec = '\0';
                }
                gradebook_t *book = create_gradebook(file_name);
            char name[MAX_NAME_LEN];
            int score;
            while (true) {
                if (fscanf(f,"%s", name) == EOF) {
                    printf("Gradebook loaded from text file\n");
                    fclose(f);
                    return book;
                } else {
                    fscanf(f,"%d", &score);
                    add_score(book, name, score);
                }
            }
            return book;
            }
    return NULL;
}

int write_gradebook_to_binary_aux(const node_t *current, FILE *f) {
    if (current == NULL) {
        return 0;
    }
    int len = strlen(current->name);
    fwrite(&len, sizeof(len), 1, f);
    fwrite(current->name, len*sizeof(char), 1, f);
    fwrite(&current->score, sizeof(current->score), 1, f);

    if (current->left != NULL) {
        if (write_gradebook_to_binary_aux(current->left, f) != 0) {
            return -1;
        }
    }

    if (current->right != NULL) {
        if (write_gradebook_to_binary_aux(current->right, f) != 0) {
            return -1;
        }
    }

    return 0;
}

int write_gradebook_to_binary(const gradebook_t *book) {
    char file_name[MAX_NAME_LEN + strlen(".bin")];
    strcpy(file_name, book->class_name);
    strcat(file_name, ".bin");

    FILE *f = fopen(file_name, "wb");
    if (f == NULL) {
        printf("Failed to write gradebook to binary file\n");
        return -1;
    }

    write_gradebook_to_binary_aux(book->root, f);
    printf("Gradebook successfully written to %s\n", file_name);
    fclose(f);
    return -1;
}

gradebook_t *read_gradebook_from_binary(const char *file_name) { //main changes needed as well
            FILE *f = fopen(file_name, "rb");
            if (f == NULL) {
                printf("Failed to read gradebook from binary file\n");
            } else {
            char *dec = strchr(file_name, '.');
                if(dec != NULL) {
                    *dec = '\0';
                }
                gradebook_t *book = create_gradebook(file_name);
            while (!feof(f)) {
                int score;
                int len;
                fread(&len, sizeof(len), 1, f);
                char name[len];
                fread(name, len*sizeof(char), 1, f);
                name[len] = '\0';
                fread(&score, sizeof(int), 1, f);
                add_score(book, name, score);
                }
                printf("Gradebook loaded from binary file\n");
                fclose(f);
                return book;
            }
    return NULL;
}
