#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct		s_list
{
    void			*content;
    struct s_list	*next;
}					t_list;

typedef struct or_buy{
     int id;
     int num;
     double price;
    //  struct or *next;
    //  struct or *prev;
 } order_buy;

typedef struct or_sell{
     int id;
     char buy;
     int id_early;
     int id_present;
     int num;
     double price;
 } order_sell;

 t_list	*ft_lstnew(void *content);
 void	ft_lstadd_back(t_list **lst, t_list *new);