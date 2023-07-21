#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_FILENAME_LEN 256
#define MAX_SYMBOLS 256
#define CHUNK_SIZE 1024

typedef struct huffman_node {
    unsigned char symbol;
    int frequency;
    bool is_leaf;
    struct huffman_node *left;
    struct huffman_node *right;
    struct huffman_node *parent;
} huffman_node;

typedef struct {
    unsigned char *buffer;
    int length;
} bit_buffer;

typedef struct {
    huffman_node *root;
    int num_symbols;
    int *frequencies;
    huffman_node *leaves[MAX_SYMBOLS];
} huffman_tree;

void print_usage() {
    fprintf(stderr, "Usage: compress input_file output_file\n");
}

void print_error(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
    exit(EXIT_FAILURE);
}

void print_file_error(const char *filename) {
    fprintf(stderr, "\n   Error: could not open file '%s'\n", filename);
    fprintf(stderr,"\n-----Unable to compress the file. Please try again.-----\n");
    exit(EXIT_FAILURE);
}

void print_memory_error() {
    fprintf(stderr, "Error: out of memory\n");
    exit(EXIT_FAILURE);
}

huffman_node *create_huffman_node(unsigned char symbol, int frequency, bool is_leaf) {
    huffman_node *node = malloc(sizeof(huffman_node));
    if (node == NULL) {
        print_memory_error();
    }
    node->symbol = symbol;
    node->frequency = frequency;
    node->is_leaf = is_leaf;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

void destroy_huffman_node(huffman_node *node) {
    if (node == NULL) {
        return;
    }
    destroy_huffman_node(node->left);
    destroy_huffman_node(node->right);
    free(node);
}

int compare_huffman_nodes(const void *a, const void *b) {
    const huffman_node *node1 = *(const huffman_node **) a;
    const huffman_node *node2 = *(const huffman_node **) b;
    return node1->frequency - node2->frequency;
}

huffman_node *build_huffman_tree(int *frequencies) {
    huffman_node *nodes[MAX_SYMBOLS];
    int num_nodes = 0;

    for (int i = 0; i < MAX_SYMBOLS; i++) {
        if (frequencies[i] > 0) {
            nodes[num_nodes] = create_huffman_node((unsigned char) i, frequencies[i], true);
            num_nodes++;
        }
    }

    while (num_nodes > 1) {
        qsort(nodes, num_nodes, sizeof(huffman_node *), compare_huffman_nodes);
        huffman_node *node1 = nodes[0];
        huffman_node *node2 = nodes[1];
        huffman_node *new_node = create_huffman_node(0, node1->frequency + node2->frequency, false);
        new_node->left = node1;
        new_node->right = node2;
        node1->parent = new_node;
        node2->parent = new_node;
        nodes[0] = new_node;
        for (int i = 2; i < num_nodes; i++) {
            nodes[i - 1] = nodes[i];
        }
        num_nodes--;
    }

    return nodes[0];
}

void build_huffman_table(huffman_tree *tree) {
    for (int i = 0; i < MAX_SYMBOLS; i++) {
        tree->leaves[i] = NULL;
    }

    if (tree->root == NULL) {
        return;
    }

    if (tree->root->is_leaf) {
        tree->leaves[tree->root->symbol] = tree->root;
        return;
    }

    huffman_node *stack[MAX_SYMBOLS];
    int top = 0;
    stack[top] = tree->root;

    while (top >= 0) {
        huffman_node *node = stack[top];
        top--;
        if (node == NULL) {
            continue;
        }
        if (node->is_leaf) {
            tree->leaves[node->symbol] = node;
        } else {
            stack[top + 1] = node->left;
            stack[top + 2] = node->right;
            top += 2;
        }
    }
}

void encode_symbol(huffman_tree *tree, unsigned char symbol, bit_buffer *buffer) {
    huffman_node *leaf = tree->leaves[symbol];
    while (leaf != tree->root) {
        if (leaf == leaf->parent->left) {
            buffer->buffer[buffer->length / 8] &= ~(1 << (buffer->length % 8));
        } else {
            buffer->buffer[buffer->length / 8] |= 1 << (buffer->length % 8);
        }
        buffer->length++;
        leaf = leaf->parent;
    }
}

bool encode_file(huffman_tree *tree, const char *input_filename, const char *output_filename) {
    FILE *input_file = fopen(input_filename, "rb");
    if (input_file == NULL) {
        print_file_error(input_filename);
        return false; // Compression failed
    }

    FILE *output_file = fopen(output_filename, "wb");
    if (output_file == NULL) {
        fclose(input_file);
        print_file_error(output_filename);
        return false; // Compression failed
    }

    // Write header
    fwrite(&(tree->num_symbols), sizeof(int), 1, output_file);
    for (int i = 0; i < MAX_SYMBOLS; i++) {
        if (tree->frequencies[i] > 0) {
            fwrite(&i, sizeof(unsigned char), 1, output_file);
            fwrite(&(tree->frequencies[i]), sizeof(int), 1, output_file);
        }
    }

    // Encode data in chunks
    unsigned char buffer[CHUNK_SIZE];
    bit_buffer bit_buf;
    bit_buf.buffer = malloc(CHUNK_SIZE / 8); // Each element represents 8 bits
    if (bit_buf.buffer == NULL) {
        fclose(input_file);
        fclose(output_file);
        print_memory_error();
        return false; // Compression failed
    }
    bit_buf.length = 0;

    unsigned char byte;
    while (1) {
        int bytes_read = fread(buffer, sizeof(unsigned char), CHUNK_SIZE, input_file);
        if (bytes_read == 0) {
            break; // End of file
        }

        // Reset bit buffer to zeros for each chunk
        memset(bit_buf.buffer, 0, CHUNK_SIZE / 8);
        bit_buf.length = 0;

        for (int i = 0; i < bytes_read; i++) {
            encode_symbol(tree, buffer[i], &bit_buf);
            while (bit_buf.length >= 8) {
                fwrite(bit_buf.buffer, sizeof(unsigned char), 1, output_file);
                bit_buf.length -= 8;
                bit_buf.buffer[0] = bit_buf.buffer[1];
                for (int j = 1; j < bit_buf.length / 8 + 1; j++) {
                    bit_buf.buffer[j] = 0;
                }
            }
        }
    }

    // Write any remaining bits in the buffer
    if (bit_buf.length > 0) {
        fwrite(bit_buf.buffer, sizeof(unsigned char), 1, output_file);
    }

    free(bit_buf.buffer);
    fclose(input_file);
    fclose(output_file);

    return true; // Compression was successful
}

bool compress_file(const char *input_filename, const char *output_filename) {
    FILE *input_file = fopen(input_filename, "rb");
    if (input_file == NULL) {
        print_file_error(input_filename);
        return false; // Compression failed
    }

    // Calculate symbol frequencies
    int frequencies[MAX_SYMBOLS] = {0};
    unsigned char byte;
    while (fread(&byte, sizeof(unsigned char), 1, input_file) == 1) {
        frequencies[byte]++;
    }

    // Build Huffman tree
    huffman_tree tree;
    tree.root = build_huffman_tree(frequencies);
    tree.num_symbols = 0;
    tree.frequencies = frequencies;
    for (int i = 0; i < MAX_SYMBOLS; i++) {
        if (frequencies[i] > 0) {
            tree.num_symbols++;
        }
    }
    build_huffman_table(&tree);

    // Encode file
    bool compression_success = encode_file(&tree, input_filename, output_filename);

    // Cleanup
    destroy_huffman_node(tree.root);
    fclose(input_file);

    return compression_success;
}

int main() {
    char input_filename[MAX_FILENAME_LEN];
    char output_filename[MAX_FILENAME_LEN];

    printf("-------------File Compressing------------");

    printf("\n\n   Enter the input file name: ");
    if (scanf("%s", input_filename) != 1) {
        fprintf(stderr, "\n   Error: Unable to read input file name.\n");
        return -1;
    }

    printf("\n   Enter the output file name: ");
    if (scanf("%s", output_filename) != 1) {
        fprintf(stderr, "\n   Error: Unable to read output file name.\n");
        return -1;
    }

    bool compression_success = compress_file(input_filename, output_filename);

    if (compression_success) {
        printf("\n-------File compressed successfully.-------\n");
    } else {
        printf("\n-------Error: Unable to compress the file. Please try again.-------\n");
    }


    return 0;
}
