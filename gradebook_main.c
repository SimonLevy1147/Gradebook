#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "gradebook.h"

#define MAX_CMD_LEN 128

// This is in general similar to the list_main file seen in lab
// One big difference is the notion of switching between gradebooks in one
// run of the program.
// You have to create or load a gradebook from a file before you can do things
// like add, lookup, or write.
// Also, the user must explicitly clear the current gradebook before
// they can create or load in a new one.
int main(int argc, char **argv) {
    gradebook_t *book = NULL;
    bool echo = false;
    int len = strlen(argv[1]);
    char file_name[len];
    strcpy(file_name, &argv[1][len-4]);
    if (argc > 1 && strcmp("-echo", argv[1]) == 0) {
        echo = true;
    } else if (argc > 1 && strcmp(file_name, ".txt") == 0) {
            read_gradebook_from_text(argv[1]);
            if (strcmp("-echo", argv[2]) == 0) {
                echo = true;
            }
        }
        else if (argc > 1 && strcmp(file_name, ".bin") == 0) {
            read_gradebook_from_binary(argv[1]);
            if (strcmp("-echo", argv[2]) == 0) {
                echo = true;
            }
        }
        else {
            printf("Error: Unknown gradebook file extension\n");
            if (strcmp("-echo", argv[2]) == 0) {
                echo = true;
            }
        }


    printf("CSCI 2021 Gradebook System\n");
    printf("Commands:\n");
    printf("  create <name>:          creates a new class with specified name\n");
    printf("  class:                  shows the name of the class\n");
    printf("  add <name> <score>:     adds a new score\n");
    printf("  lookup <name>:          searches for a score by student name\n");
    printf("  clear:                  resets current gradebook\n");
    printf("  print:                  shows all scores, sorted by student name\n");
    printf("  write_text:             saves all scores to text file\n");
    printf("  read_text <file_name>:  loads scores from text file\n");
    printf("  write_bin:              saves all scores to binary file\n");
    printf("  read_bin <file_name>:   loads scores from binary file\n");
    printf("  exit:                   exits the program\n");

    char cmd[MAX_CMD_LEN];
    while (true) {
        printf("gradebook> ");
        if (scanf("%s", cmd) == EOF) {
            printf("\n");
            break;
        }

        if (strcmp("exit", cmd) == 0) {
            if (echo) {
                printf("exit\n");
            }
            break;
        }

        else if (strcmp("create", cmd) == 0) {
            scanf("%s", cmd);
            if (echo) {
                printf("create %s\n", cmd);
            }

            if (book != NULL) {
                printf("Error: You already have a gradebook.\n");
                printf("You can remove it with the \'clear\' command\n");
            } else {
                book = create_gradebook(cmd);
                if (book == NULL) {
                    printf("Gradebook creation failed\n");
                }
            }
        }

        else if (strcmp("add", cmd) == 0) {
            int score;
            scanf("%s %d",cmd, &score);
            strcat(cmd, "\0");
            if (echo) {
                printf("add %s %d\n", cmd, score);
            }
            if (book == NULL) {
                printf("Error: You must create or load a gradebook first\n");
            } else {
                if (score < 0) {
                    printf("Error: You must enter a score in the valid range (0 <= score)\n");
                } else {
                    add_score(book, cmd, score);
                }
            }
        }
        else if (strcmp("lookup", cmd) == 0) {
            char name[MAX_NAME_LEN];
            scanf("%s", name);
            if (echo) {
                printf("%s %s\n", cmd, name);
            }
            if (book == NULL) {
                printf("Error: You must create or load a gradebook first\n");
            } else {
                find_score(book, name); 
            }
        }
        else if (strcmp("clear", cmd) == 0) {
            if (echo) {
                printf("%s\n", cmd);
            }
            if (book == NULL) {
                printf("Error: No gradebook to clear\n");
            }
            free_gradebook(book);
            book = NULL;
        }
        else if (strcmp("print",cmd) == 0) {
            if (echo) {
                printf("print\n");
            }
            if (book == NULL) {
                printf("Error: You must create or load a gradebook first\n");
            } else {
            print_gradebook(book);
            }
        }
        else if (strcmp("class", cmd) == 0) {
            if (echo) {
                printf("class\n");
            }
            if (book == NULL) {
                printf("Error: You must create or load a gradebook first\n");
            } else {
            printf("%s %s",book->class_name, "\n");
            }
        }
        else if (strcmp("write_text", cmd) == 0) {
            if (echo) {
                printf("%s\n", cmd);
            }
            if (book == NULL) {
                printf("Error: You must create or load a gradebook first\n");
            } else {
                    write_gradebook_to_text(book);
                }
            }
            
        else if (strcmp("read_text", cmd) == 0) {
         char file_name[MAX_NAME_LEN];
            scanf("%s", file_name);
            if (echo) {
                printf("%s %s\n", cmd, file_name);
            }
            if (book != NULL) {
                printf("Error: You must clear current gradebook first\n");
            } else {
            book = read_gradebook_from_text(file_name);
            
            }
        }

        else if (strcmp("write_bin", cmd) == 0) {
            if (echo) {
                printf("%s\n", cmd);
            }
            if (book == NULL) {
                printf("Error: You must create or load a gradebook first\n");
            } else {
                    write_gradebook_to_binary(book);
                }
        }

        else if (strcmp("read_bin", cmd) == 0) {
            char file_name[MAX_NAME_LEN];
            scanf("%s", file_name);
            if (echo) {
                printf("%s %s\n", cmd, file_name);
            }
            if (book != NULL) {
                printf("Error: You must clear current gradebook first\n");
            } else {
            book = read_gradebook_from_binary(file_name);
            
            }
        }


        else {
            if (echo) {
                printf("%s\n", cmd);
            }
            printf("Unknown command %s\n", cmd);
        }
    }

    free_gradebook(book);
    return 0;
}
