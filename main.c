#include <stdio.h>
#include <stdlib.h>
#include "trade.h"
//засунуть в списки
//push_front если цена больше для байера и наоборот
//https://github.com/greensky00/avltree/blob/master/avltree/avltree.c
//
order_buy	*create_cmd(order_buy *tmp, char **av, int n_elem, int type) {
	order_buy	*new_cmd;
	int		i = -1;

	if (!(new_cmd = malloc(sizeof(order_buy))))
		exit_fatal();
	new_cmd->type = type;
	new_cmd->next = NULL;
	new_cmd->prev = tmp;
	if (tmp != NULL)
		tmp->next = new_cmd;
	if (!(new_cmd->args = malloc(sizeof(char *) * (n_elem + 1))))
		exit_fatal();
	while (++i < n_elem)
		new_cmd->args[i] = ft_strdup(av[i]);
	new_cmd->args[i] = NULL;
	return (new_cmd);
}
char *pattern_open()
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    int read;
    fp = fopen("input.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    while ((read = getline(&line, &len, fp)) != -1) {
            return ("%s", line);
        }
}
int main(void)
{
	order_buy or_buy;
	order_sell or_sell;

}