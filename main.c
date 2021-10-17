
#include "trade.h"
#include "avltree.h"
            #include <fcntl.h>
//            #include <sys\types.h>
//            #include <sys\stat.h>

int		ft_strncmp(const char *s1, const char *s2, size_t len)
{
	size_t i;

	i = 0;
	if (len == 0)
		return (0);
	while (s1[i] && s2[i] && s1[i] == s2[i] && --len)
	{
		++i;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void	ft_freetab(char **str, size_t s)
{
	size_t i;

	i = 0;
	while (i < s){
	    free(str[i]);
	    i++;
	}
	free(str);
}


size_t		lw(char const *s, char c)
{
	int		i;
	size_t	r;

	i = 0;
	r = 0;
	while (s[i] && s[i] != c)
	{
		++i;
		++r;
	}
	return (r);
}

size_t		count_words(char const *str, char c)
{
	size_t i;

	i = 0;
	while (*str)
	{
		while (*str && *str == c)
			++str;
		if (*str)
			++i;
		while (*str && *str != c)
			++str;
		i++;
	}
	return (i);
}

char		*cp(char const *s, char c)
{
	char	*ju;
	size_t	i;

	if (!(ju = malloc(sizeof(char**) * (lw(s, c) + 1))))
		return (NULL);
	i = 0;
	while (*s && *s != c)
		ju[i++] = *s++;
	ju[i] = '\0';
	return (ju);
}

char		**ft_split(char const *s, char c)
{
	int		y;
	char	**ju;

	y = 0;
	if (!(s) || !(ju = malloc(sizeof(char*) * (count_words(s, c) + 1))))
		return (NULL);
	while (*s)
	{
		while (*s == c && *s)
			++s;
		if (*s != c && *s)
		{
			if (!(ju[y++] = cp(s, c)))
			{
				ft_freetab(ju, count_words(s, c));
				return (NULL);
			}
			s = s + lw(s, c);
		}
	}
	ju[y] = NULL;
	return (ju);
}
//засунуть в списки
//push_front если цена больше для байера и наоборот
//https://github.com/greensky00/avltree/blob/master/avltree/avltree.c
//
// order_buy	*create_cmd(order_buy *tmp, char **av, int n_elem, int type) {
// 	order_buy	*new_cmd;
// 	int		i = -1;

// 	if (!(new_cmd = malloc(sizeof(order_buy))))
// 		exit_fatal();
// 	new_cmd->type = type;
// 	new_cmd->next = NULL;
// 	new_cmd->prev = tmp;
// 	if (tmp != NULL)
// 		tmp->next = new_cmd;
// 	if (!(new_cmd->args = malloc(sizeof(char *) * (n_elem + 1))))
// 		exit_fatal();
// 	while (++i < n_elem)
// 		new_cmd->args[i] = ft_strdup(av[i]);
// 	new_cmd->args[i] = NULL;
// 	return (new_cmd);
// }
struct keys{
    double price;
    int id;
};

struct w_link{
    char alp;
    struct avl_node *avl;
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

int cmp_func1(struct avl_node *a, struct avl_node *b, void *aux)
{
    struct kv_node *aa, *bb;
    aa = _get_entry(a, struct kv_node, avl);
    bb = _get_entry(b, struct kv_node, avl);

    if (aa->key < bb->key) return -1;
    else if (aa->key > bb->key) return 1;
    else return 0;
}

int cmp_func(struct avl_node *a, struct avl_node *b, void *aux)
        {
    struct kv_node *aa, *bb;
    aa = _get_entry(a, struct kv_node, avl);
    bb = _get_entry(b, struct kv_node, avl);

    if ((aa->key->price <= bb->key->price) && (aa->key->id > bb->key->id)) return -1;
    else if ((aa->key->price > bb->key->price) && (aa->key->id > bb->key->id)) return 1;
    else if ((aa->key->price < bb->key->price) && (aa->key->id < bb->key->id)) return -1;
    else if ((aa->key->price >= bb->key->price) && (aa->key->id < bb->key->id)) return 1;
    else return 0;
        }

int market(t_list *sell, struct avl_tree *tree)
{
	order_sell* ord;
	order_buy* order_sell;
	ord = malloc(sizeof(order_sell));
	ord->id = 0;
	struct avl_node *cur;
	struct kv_node *node, query;

	cur = avl_last(tree);
	node = _get_entry(cur, struct kv_node, avl);
	order_sell = (order_buy*)sell->content;
	while(1)
	{
		if (node->value->price >= order_sell->price)
		{
			while (sell && node && node->value->price >= order_sell->price)
			{
				ord->id++;
				ord->buy = 'T';
				if (node->value->id < order_sell->id)
				{
					ord->id_early = node->value->id;
					ord->id_present = order_sell->id;
					ord->price = node->value->price;
				}
				else{
					ord->id_present = node->value->id;
					ord->id_early = order_sell->id;
					ord->price = order_sell->price;
				}
				ord->num = order_sell->num > node->value->num ? node->value->num : order_sell->num;
				order_sell->num = order_sell->num - node->value->num;
				node->value->num = node->value->num - order_sell->num;
				if (node->value->num <= 0){
					avl_remove(tree, cur);
					free(node->value);
					free(cur);
					cur = avl_last(tree);
					node = _get_entry(cur, struct kv_node, avl);
				}
				if (order_sell->num <= 0){
					sell = deletelem(sell, &sell);
					if (sell)
						order_sell = (order_buy*)sell->content;
				}
			}
		}
		else{
			if (sell->next)
				sell = sell->next;
			else
				return 0;
		}
	}
	return 0;
}

char *pattern_open()
{
	struct avl_tree tree;
	struct avl_tree base;
    struct avl_node *cur;
    struct kv_node *node, query1;
	struct kv_node1 *gen, query, *find;

	char **str;
	// char **base;

	order_buy or_buy;
	t_list *lst_sell;
	t_list *lst_gen;
	// lst_gen = malloc(sizeof(lst));
	avl_init(&tree, NULL);
	avl_init(&base, NULL);
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    int read;
    fp = fopen("../input.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    while ((read = getline(&line, &len, fp)) != -1) {
		gen = (struct kv_node1 *)malloc(sizeof(struct kv_node1));
		str = ft_split(line, ',');
		if (str[2][0] == 'B') {
			node = (struct kv_node *)malloc(sizeof(struct kv_node));
        	node->key = malloc(sizeof(struct keys));
        	node->value = malloc(sizeof(order_buy));
        	node->key->id = atoi(str[1]);
			gen->value = malloc(sizeof(struct kv_node1));
			gen->key = node->key->id;
			gen->value->alp = str[2][0];
			gen->value->avl = &node->avl;
			avl_insert(&base, &gen->avl, cmp_func1);
        	node->key->price = atof(str[5]);
			node->value->id = atoi(str[1]);
			node->value->num = atoi(str[4]);
			node->value->price = atof(str[5]);
        	avl_insert(&tree, &node->avl, cmp_func);
		}
		if (str[2][0] == 'S') {
			node = (struct kv_node *)malloc(sizeof(struct kv_node));
			or_buy.id = atoi(str[1]);
			gen->value = malloc(sizeof(struct kv_node1));
			gen->key = node->key->id;
			gen->value->alp = str[2][0];
			gen->value->avl = &node->avl;
			avl_insert(&base, &gen->avl, cmp_func1);
			or_buy.price = atof(str[5]);
			or_buy.num = atoi(str[4]);
			lst_sell = ft_lstnew(&or_buy);
			ft_lstadd_back(&lst_gen, lst_sell);
			market(lst_gen, &tree);
		}
		if (str[0][0] == 'C') {
			int x = atoi(str[1]);
			query.key = x;
    		cur = avl_search(&base, &query.avl, cmp_func1);
    		find = _get_entry(cur, struct kv_node1, avl);
			if (find->value->alp == 'B')
			{
				avl_remove(&tree, find->value->avl);
			}
			else{
				t_list *pont = lst_gen;
				while(pont)
				{
					order_buy* pp = (order_buy*)pont->content;
					if (pp->id == x){
						pont = deletelem(pont, &lst_gen);
						break;
					}
				}
			}
		}
		ft_freetab(str, 6);
        }
		return ("fgg");
}

int main(void)
{
	pattern_open();
	// order_buy* or_buy;
	// or_buy = malloc(sizeof(order_buy));
	// or_buy->id = 1;
	// or_buy->num = 10;
	// or_buy->price = 334.3;

    // avl_init(&tree, NULL);

    // // create and insert key-value pairs
    // for (i=0;i<n;++i){
    //     node = (struct kv_node *)malloc(sizeof(struct kv_node));
    //     node->key = i*10;
    //     node->value = (void*)or_buy;
    //     avl_insert(&tree, &node->avl, cmp_func);
    // }

    // // retrieve each key-value pair by key
    // printf("retrieve by key\n");
    // for (i=0;i<n;++i){
    //     query.key= i*10;
    //     cur = avl_search(&tree, &query.avl, cmp_func);
    //     node = _get_entry(cur, struct kv_node, avl);
    //     printf("key %f, value %f\n", node->key, node->value->price);
    // }
	// query.key = 35;
	// cur = avl_last(&tree);
	// node = _get_entry(cur, struct kv_node, avl);
	// printf("key %f, value %f\n", node->key, node->value->price);
}