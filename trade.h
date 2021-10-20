#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "avltree.h"
#include <fcntl.h>
#include <string.h>
# include <unistd.h>

//typedef struct		s_list
//{
//    void			*content;
//    struct s_list	*next;
//}					t_list;

struct w_link{
    char alp;
    struct avl_node *avl;
};

typedef struct or_buy{
     int id;
     int num;
     double price;
     char* prici;
     struct avl_node *avl;
 } order_buy;

extern int id;

typedef struct or_sell{
     char buy;
     int id;
     char cont;
     int id_early;
     int id_present;
     int num;
     char* price;
 } order_sell;

typedef struct cancel{
    int id;
    char buy;
} canc;

struct keys{
    double price;
    int id;
};

struct kv_node{
    struct avl_node avl;
    struct keys *key;
    order_buy* value;
};

struct kv_node1{
    struct avl_node avl;
    int key;
    struct w_link* value;
};

// t_list	*ft_lstnew(void *content);
// void	ft_lstadd_back(t_list **lst, t_list *new);
// t_list *deletelem(t_list *lst, t_list **root);
void	ft_freetab(char **str, size_t s);
char	**ft_split(char const *s, char c);
int cmp_func1(struct avl_node *a, struct avl_node *b, void *aux);
int cmp_func(struct avl_node *a, struct avl_node *b, void *aux);
void free_tree(struct avl_tree *tree);
void free_tree1(struct avl_tree *tree);
char	*ft_strjoin(char const *s1, char const *s2);
int	get_next_line(int fd, char **line);