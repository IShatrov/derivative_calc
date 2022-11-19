#ifndef DERIVATIVE_CALC_H
#define DERIVATIVE_CALC_H

#include "tree.h"
#include "dsl.h"
#include "err_msgs.h"

#define NUM_TO_STR(num) #num

const double EPSILON = 1e-6;

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

my_tree get_derivative(const my_tree *init_tree, FILE *degenerator);

tree_node* diff(my_tree *tree, tree_node *node, const tree_node *target, FILE *degenerator);

tree_node* subtree_cpy(my_tree *tree, tree_node *dest, const tree_node *src);

char tree_find(tree_node *node, const char target);

int are_doubles_equal(double n1, double n2);

char simplify(my_tree *tree, tree_node *node);

char fold_consts(tree_node *node);

char fold_neutral(my_tree *tree, tree_node *node);

//derivative_calc_write.cpp

void out_latex(const my_tree *tree);

void tex_print_math(FILE *tex, const tree_node *node);

void tex_print_math_brackets(FILE *tex, const tree_node *node);

FILE* degenerator_ctor();

void degenerator_dtor(FILE *tex);

void tex_print_derivative(FILE *tex, const tree_node *node);

void degenerator_print(FILE *tex, const tree_node *node);

#endif
