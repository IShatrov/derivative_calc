#ifndef DERIVATIVE_CALC_H
#define DERIVATIVE_CALC_H

#include "tree.h"
#include "dsl.h"

#define NUM_TO_STR(num) #num

//derivative_calc_read.cpp

//! @param[in] stream File to examine.
//! @return Returns amount of characters in stream.
//! @brief Gets amount of characters in stream.
size_t get_file_size(FILE *stream);

//! @param[in] stream File to read from.
//! @return Returns pointer to char array.
//! @brief Reads text from stream and stores it as char array.
char* read_text(FILE *stream);

my_tree parse_expression(const char *src_filename);

void parse_node(char **text, my_tree *tree, tree_node *node);

//derivative_calc_diff.cpp

my_tree get_derivative(const my_tree *init_tree);

tree_node* diff(my_tree *tree, tree_node *node, const tree_node *target);

tree_node* subtree_cpy(my_tree *tree, tree_node *dest, const tree_node *src);

char tree_find(tree_node *node, const char target);

//derivative_calc_write.cpp

void out_latex(const my_tree *tree);

void tex_print_math(FILE *tex, const tree_node *node);

#endif
