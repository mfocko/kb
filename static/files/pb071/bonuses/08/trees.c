#include <stdio.h>
#include <stdlib.h>

#define UNUSED(x) ((void) (x))

struct node
{
    int key;
    int rank;

    struct node *left;
    struct node *right;
};

struct tree
{
    struct node *root;
};

/**
 * @brief Creates new dynamically allocated node.
 * @param key Key that node holds.
 * @param rank Rank that node has.
 * @returns Pointer to dynamically allocated node, NULL if allocation fails.
 */
struct node *
create_node(int key, int rank)
{
    struct node *new_node = calloc(1, sizeof(struct node));
    if (new_node == NULL) {
        fprintf(stderr, "create_node: Cannot allocate memory for node.\n");
        return NULL;
    }

    new_node->key = key;
    new_node->rank = rank;
    return new_node;
}

/**
 * @brief Recursively frees node and its subtrees.
 * @param node Root of the subtree to be freed.
 */
void free_node(struct node *node)
{
    if (node == NULL) {
        return;
    }

    free_node(node->left);
    free_node(node->right);
    free(node);
}

/**
 * @brief Prettyprints given tree into the file.
 * @param file File for output.
 * @param tree Tree to be prettyprinted.
 */
void pretty_print_tree(FILE *file, const struct tree *tree)
{
    UNUSED(file);
    UNUSED(tree);
}

/**
 * @brief Loads tree from a file and returns root of it.
 * @param file File where the tree is saved.
 * @returns Pointer to the loaded tree, NULL if error happens or no tree is in file.
 */
struct node *load_tree(FILE *file)
{
    UNUSED(file);
}

int main(int argc, char **argv)
{
    if (argc < 2 || argc > 3) {
        printf("Usage: %s <input-file> [output-file]\n", argv[0]);
        return 1;
    }

    /* TODO */
    return 0;
}
