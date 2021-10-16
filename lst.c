//
// Created by Luci Atkins on 10/16/21.
//
#include "trade.h"
typedef struct		s_list
{
    void			*content;
    struct s_list	*next;
}					t_list;

t_list	*ft_lstnew(void *content)
{
    t_list *ju;

    if (!(ju = malloc(sizeof(t_list))))
        return (NULL);
    ju->content = content;
    ju->next = NULL;
    return (ju);
}

t_list		*ft_lstlast(t_list *lst)
{
    if (lst)
    {
        while (lst->next != NULL)
        {
            lst = lst->next;
        }
    }
    return (lst);
}

void	ft_lstadd_back(t_list **lst, t_list *new)
{
    t_list *cur;

    if (!*lst)
    {
        *lst = new;
        return ;
    }
    cur = ft_lstlast(*lst);
    cur->next = new;
}

t_list *deletelem(t_list *lst, t_list *root)
{
    t_list *temp;
    temp = root;
    while (temp->next != lst) // просматриваем список начиная с корня
        { // пока не найдем узел, предшествующий lst
        temp = temp->next;
        }
    temp->next = lst->next; // переставляем указатель
    free(lst); // освобождаем память удаляемого узла
    return(temp);
}