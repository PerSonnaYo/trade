//
// Created by Luci Atkins on 10/16/21.
//
#include "trade.h"


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
    t_list *tmp = *lst;

    if (!*lst)
    {
        *lst = new;
        return ;
    }
    cur = ft_lstlast(tmp);
    cur->next = new;
}

t_list *deletelem(t_list *lst, t_list **root)
{
    t_list *temp;
    temp = *root;
    if (temp == lst)
    {
        temp = lst->next;
    }
    else{
        while (temp->next != lst) // просматриваем список начиная с корня
            { // пока не найдем узел, предшествующий lst
            temp = temp->next;
            }
        temp->next = lst->next; // переставляем указатель
    }
    free(lst); // освобождаем память удаляемого узла
    return(temp);
}

size_t		dln(int n)
{
    size_t len;

    len = 0;
    if (n < 0)
    {
        len++;
        n = n * -1;
    }
    while (n && (n /= 10))
    {
        len++;
    }
    return (len + 1);
}

char		*ft_itoa(int n)
{
    char	*ju;
    long	g;
    size_t	len;

    len = dln(n);
    if (!(ju = malloc(sizeof(char) * (len + 1))))
        return (NULL);
    ju[len] = '\0';
    g = n;
    if (g == 0)
        ju[0] = 48;
    if (g < 0)
    {
        ju[0] = '-';
        g = g * -1;
    }
    while (g)
    {
        ju[--len] = g % 10 + '0';
        g = g / 10;
    }
    return (ju);
}