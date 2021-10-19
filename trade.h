#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "avltree.h"
#include <fcntl.h>

typedef struct		s_list
{
    void			*content;
    struct s_list	*next;
}					t_list;

struct w_link{
    char alp;
    struct avl_node *avl;
};

typedef struct or_buy{
     int id;
     int num;
     double price;
     struct avl_node *avl;
    //  struct or *next;
    //  struct or *prev;
 } order_buy;

int id;

typedef struct or_sell{
     char buy;
     int id;
     char cont;
     int id_early;
     int id_present;
     int num;
     double price;
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

 t_list	*ft_lstnew(void *content);
 void	ft_lstadd_back(t_list **lst, t_list *new);
 t_list *deletelem(t_list *lst, t_list **root);
 void	ft_freetab(char **str, size_t s);
 char		**ft_split(char const *s, char c);