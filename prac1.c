#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define MAXLEN 1024

struct word {
    char part[MAXLEN];
    struct word *next;
};
struct node {
    int n;
    struct word *data;
    struct node *left;
    struct node *right;
};

struct word *reading_word(struct word **w, FILE *f);
void adding_word_to_tree(struct word *w, struct node **t);
int word_comparison(struct word *w1, struct word *w2);
void tree_to_dictionary(struct node *tree, struct node **dictionary);
void adding_word_to_dictionary(struct word *w,  int n, struct node **t);
void print_dictionary(struct node *dictionary, FILE *fout);
void print_word(struct word *w, FILE *fout);
void free_tree_memory(struct node *tree);
void free_word_memory(struct word *word);

int word_number = 0;

int main(int argc, char *argv[]) {

    FILE *fin = stdin, *fout = stdout;
    int param = 0;
    struct word *word = NULL, *punct = NULL;
    struct node *tree = NULL, *dictionary = NULL;

    while ((param = getopt(argc, argv, "i:o:")) != -1) {
        switch(param) {
            case 'i': fin = fopen(optarg, "r"); break;
            case 'o': fout = fopen(optarg, "w"); break;
            case '?': return 1;
        }
    }

    while (!feof(fin)) {
        word = NULL;
        punct = reading_word(&word, fin);
        if (punct) {
            adding_word_to_tree(punct, &tree);
        }
        if (word) {
            adding_word_to_tree(word, &tree);
        }
    }

    tree_to_dictionary(tree, &dictionary);
    printf("\n");
    print_dictionary(dictionary, fout);
    free_tree_memory(dictionary);
    free_tree_memory(tree);
    return 0;
}

struct word *reading_word(struct word **w, FILE *f) {
    char c, str[MAXLEN];
    int len = 0;
    struct word *punctuation = NULL;
    while ((len < MAXLEN - 1) && isalnum(c = fgetc(f))) {
        str[len] = c;
        len++;
    }
    str[len] = '\0';

    if (isalnum(c)) {
        *w = malloc(sizeof(struct word));
        strcpy((*w) -> part, str);
        punctuation = reading_word(&((*w) -> next), f);
    }
    else if (len) {
        *w = malloc(sizeof(struct word));
        strcpy((*w) -> part, str);
        (*w) -> next = NULL;
    }
    if (!isalnum(c) && !isspace(c) && (c != EOF)) {
        punctuation = malloc(sizeof(struct word));
        (punctuation -> part)[0] = c;
        (punctuation -> part)[1] = '\0';
        punctuation -> next = NULL;
    }
    return punctuation;
}

void adding_word_to_tree(struct word *w, struct node **t) {
    int comparison;
    if ((*t) == NULL) {
        (*t) = malloc(sizeof(struct node));
        (*t) -> n = 1;
        (*t) -> data = w;
        (*t) -> left = NULL;
        (*t) -> right = NULL;
        word_number += 1;
    }
    else if ((comparison = word_comparison(w, (*t) -> data)) == 0){
        (*t) -> n += 1;
    	word_number += 1;
	free_word_memory(w);
    }
    else if (comparison < 0)
        adding_word_to_tree(w, &((*t) -> left));
    else
        adding_word_to_tree(w, (&(*t) -> right));
}

int word_comparison(struct word *w1, struct word *w2) {
    int string_comparison;
    if ((string_comparison = strcmp(w1 -> part, w2 -> part)))
        return string_comparison;
    if (w1 -> next == NULL) {
        if (w2 -> next == NULL)
            return 0;
        else
            return -1;
    }
    if (w2 -> next == NULL)
        return 1;
    word_comparison(w1-> next, w2 -> next);
    return 2;
}

void tree_to_dictionary(struct node *tree, struct node **dictionary) {
    if (tree) {
        adding_word_to_dictionary(tree -> data, tree -> n, dictionary);
	tree -> data = NULL;
	tree_to_dictionary(tree -> left, dictionary);
        tree_to_dictionary(tree -> right, dictionary);

    }
}

void adding_word_to_dictionary(struct word *w,  int n, struct node **t) {
    if ((*t) == NULL) {
        (*t) = malloc(sizeof(struct node));
        (*t) -> data = w;
        (*t) -> n = n;
        (*t) -> left = NULL;
        (*t) -> right = NULL;
    }
    else if (n >= (*t) -> n)
        adding_word_to_dictionary(w, n, &((*t) -> left));
    else
        adding_word_to_dictionary(w, n, &((*t) -> right));
}

void print_dictionary(struct node *dictionary, FILE *fout) {
    if (dictionary) {
    	print_dictionary(dictionary -> left, fout);
        print_word(dictionary -> data, fout);
        fprintf(fout, " %d %.6f \n", dictionary -> n, ((double)dictionary -> n) / word_number);
	print_dictionary(dictionary -> right, fout);
    }
}

void print_word(struct word *w, FILE *fout) {
    if (w) {
        fprintf(fout, "%s", w -> part);
        print_word( w -> next, fout);
    }
}

void free_tree_memory(struct node *tree) {
    if (tree) {
        free_tree_memory(tree -> left);
        free_tree_memory(tree -> right);
        free_word_memory(tree -> data);
        free(tree);
    }

}

void free_word_memory(struct word *word){
    if (word) {
        free_word_memory(word -> next);
        free(word);
    }
}
