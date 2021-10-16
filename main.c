
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
struct kv_node{
    struct avl_node avl;
    struct keys *key;
    order_buy* value;
};
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
        int cmp_func1(struct avl_node *a, struct avl_node *b, void *aux)
                {
    struct kv_node *aa, *bb;
    aa = _get_entry(a, struct kv_node, avl);
    bb = _get_entry(b, struct kv_node, avl);

    if ((aa->key->price <= bb->key->price) && (aa->key->id > bb->key->id)) return -1;
//    else if ((aa->key->price > bb->key->price) && (aa->key->id > bb->key->id)) return 1;
    else if ((aa->key->price <= bb->key->price) && (aa->key->id < bb->key->id)) return 1;
//    else if ((aa->key->price >= bb->key->price) && (aa->key->id < bb->key->id)) return 1;
    else return 0;
                }
//struct kv_node1{
//    struct avl_node avl;
//    int key;
//    order_buy* value;
//};

int market(struct avl_tree *tree_sell, struct avl_tree *tree)
{
	order_sell* ord;
	ord = malloc(sizeof(order_sell));
	ord->id = 0;
	struct avl_node *cur;
	struct avl_node *cur_sell;
	struct kv_node *node, query;
	struct kv_node *node1, query1;
	query1.key = malloc(sizeof(struct keys));

	cur = avl_last(tree);
	node = _get_entry(cur, struct kv_node, avl);
	query1.key->price = node->value->price;
	query1.key->id = 1;
	cur_sell = avl_search_smaller(tree_sell, &query1.avl, cmp_func1);
	cur_sell = avl_first(tree_sell);
	node1 = _get_entry(cur_sell, struct kv_node, avl);
	while (node1 && node && node->value->price >= node1->value->price && node1->value->num > 0)
	{
		ord->id++;
		ord->buy = 'T';
		if (node->value->id < node1->value->id)
		{
			ord->id_early = node->value->id;
			ord->id_present = node1->value->id;
			ord->price = node->value->price;
		}
		else{
			ord->id_present = node->value->id;
			ord->id_early = node1->value->id;
			ord->price = node1->value->price;
		}
		ord->num = node1->value->num > node->value->num ? node->value->num : node1->value->num;
		node1->value->num = node1->value->num - node->value->num;
		node->value->num = node->value->num - node1->value->num;
		if (node->value->num <= 0){
			avl_remove(tree, cur);
			free(node->value);
			free(cur);
			cur = avl_last(tree);
			node = _get_entry(cur, struct kv_node, avl);
		}
		if (node1->value->num <= 0){
		    avl_remove(tree_sell, cur_sell);
		    free(node1->value);
		    free(cur_sell);
		    if (node){
//		        query1.key = malloc(sizeof(struct keys));
//		        query1.key->price = node->value->price;
//		        query1.key->id = node->value->id;
//		        cur_sell = avl_search_smaller(tree_sell, &query1.avl, cmp_func);
		        cur_sell = avl_first(tree_sell);
		        node1 = _get_entry(cur_sell, struct kv_node, avl);
//		        cur_sell = avl_first(tree_sell);
//		        node1 = _get_entry(cur_sell, struct kv_node, avl);
		    } else
		        node1 = NULL;
		}
	}
//	if (node1->value->num > 0)
//	    return 1;
	return 0;
}


//int cmp_func_sell(struct avl_node *a, struct avl_node *b, void *aux)
//        {
//    struct kv_node *aa, *bb;
//    aa = _get_entry(a, struct kv_node, avl);
//    bb = _get_entry(b, struct kv_node, avl);
//
//    if (aa->key < bb->key) return -1;
//    else if (aa->key > bb->key) return 1;
//    else return 0;
//        }
char *pattern_open()
{
	struct avl_tree tree;
	struct avl_tree tree_sell;
    struct avl_node *cur;
    struct kv_node *node, query;
	struct avl_node *cur1;
    struct kv_node *node1, query1;
	char **str;
	order_buy or_buy;
//	or_buy = malloc(sizeof(order_buy));
	avl_init(&tree, NULL);
	avl_init(&tree_sell, NULL);
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    int read;
    fp = fopen("../input.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    while ((read = getline(&line, &len, fp)) != -1) {
		node = (struct kv_node *)malloc(sizeof(struct kv_node));
		node1 = (struct kv_node1 *)malloc(sizeof(struct kv_node));
		str = ft_split(line, ',');
		if (str[2][0] == 'B') {
        	node->key = malloc(sizeof(struct keys));
        	node->value = malloc(sizeof(order_buy));
        	node->key->id = atoi(str[1]);
        	node->key->price = atof(str[5]);
			node->value->id = atoi(str[1]);
			node->value->num = atoi(str[4]);
			node->value->price = atof(str[5]);
//        	node->value = &or_buy;
        	avl_insert(&tree, &node->avl, cmp_func);
		}
		if (str[2][0] == 'S') {
//			node1->key = atoi(str[1]);
			node1->key = malloc(sizeof(struct keys));
			node1->value = malloc(sizeof(order_buy));
			node1->key->id = atoi(str[1]);
			node1->key->price = atof(str[5]);
			node1->value->id = atoi(str[1]);
			node1->value->num = atoi(str[4]);
			node1->value->price = atof(str[5]);
			avl_insert(&tree_sell, &node1->avl, cmp_func);
			market(&tree_sell, &tree);
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