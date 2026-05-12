#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the structure for a node in our linked list
struct line_node {
    char *text;
    struct line_node *next;
};

int main(int argc, char *argv[]) {
    FILE *in = stdin;   // Default input: Standard Input
    FILE *out = stdout; // Default output: Standard Output

    // 1. Argument Checking
    if (argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    // 2. Handle Files
    if (argc == 3) {
        // Check if input and output filenames are identical strings
        if (strcmp(argv[1], argv[2]) == 0) {
            fprintf(stderr, "Input and output file must differ\n");
            exit(1);
        }

        in = fopen(argv[1], "r");
        if (in == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
            exit(1);
        }

        out = fopen(argv[2], "w");
        if (out == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
            exit(1);
        }
    } else if (argc == 2) {
        in = fopen(argv[1], "r");
        if (in == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
    }
    // If argc is 1, we stick with stdin and stdout

    // 3. Read Input and Build Linked List
    struct line_node *head = NULL; // Start with an empty list
    char *buffer = NULL;
    size_t len = 0;
    ssize_t read;

    // getline reads arbitrarily long lines automatically
    while ((read = getline(&buffer, &len, in)) != -1) {
        // Create a new node
        struct line_node *node = malloc(sizeof(struct line_node));
        if (node == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }

        // Allocate memory for the string and copy it
        // We must copy because 'buffer' gets overwritten by getline next loop
        node->text = malloc(read + 1); 
        if (node->text == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        strcpy(node->text, buffer);

        // Push to the FRONT of the list (this is what creates the reversal)
        node->next = head;
        head = node;
    }

    // Cleanup the buffer allocated by getline
    if (buffer) free(buffer);
    if (in != stdin) fclose(in);

    // 4. Print the List (Traverse from Head to Tail)
    struct line_node *current = head;
    while (current != NULL) {
        fprintf(out, "%s", current->text);
        
        // Cleanup memory as we go
        struct line_node *temp = current;
        current = current->next;
        free(temp->text);
        free(temp);
    }

    if (out != stdout) fclose(out);

    return 0;
}
