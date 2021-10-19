#include "trade.h"
id = 0;
//засунуть в списки
//push_front если цена больше для байера и наоборот
//https://github.com/greensky00/avltree/blob/master/avltree/avltree.c
//

int cmp_func1(struct avl_node *a, struct avl_node *b, void *aux)
{
    struct kv_node1 *aa, *bb;
    aa = _get_entry(a, struct kv_node1, avl);
    bb = _get_entry(b, struct kv_node1, avl);

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

int market(struct avl_tree *tree_sell, struct avl_tree *tree, struct avl_tree *base, FILE *fd)
{
	order_sell* ord;
	
	ord->id = 0;
	int price = 0;
	struct avl_node *cur_buy, *cur_sell, *cur_base;
	struct kv_node *node_buy, *node_sell;
	struct kv_node1 *find, query;

	cur_buy = avl_last(tree);
	node_buy = _get_entry(cur_buy, struct kv_node, avl);
	cur_sell = avl_first(tree_sell);
	node_sell = _get_entry(cur_sell, struct kv_node, avl);

	while (node_sell && node_buy && node_buy->value->price >= node_sell->value->price)
	{
		ord = malloc(sizeof(order_sell));
		id++;
		ord->id = id;
		ord->buy = 'T';
		if (node_buy->value->id < node_sell->value->id)
		{
			ord->cont = 'B';
			ord->id_early = node_buy->value->id;
			ord->id_present = node_sell->value->id;
			ord->price = node_buy->value->price;
		}
		else{
			ord->cont = 'S';
			ord->id_present = node_buy->value->id;
			ord->id_early = node_sell->value->id;
			ord->price = node_sell->value->price;
		}
		ord->num = node_sell->value->num > node_buy->value->num ? node_buy->value->num : node_sell->value->num;
		price = node_sell->value->num;
		node_sell->value->num = node_sell->value->num - node_buy->value->num;
		node_buy->value->num = node_buy->value->num - price;
		fprintf(fd, "%c,%d,%c,%d,%d,%d,%.2f\n", ord->buy, ord->id, ord->cont, ord->id_early, ord->id_present, ord->num, ord->price);
		free(ord);
		if (node_buy->value->num <= 0){
			find = _get_entry(node_buy->value->avl, struct kv_node1, avl);
			avl_remove(base, node_buy->value->avl);
			free(find->value);
			free(node_buy->value->avl);
			avl_remove(tree, cur_buy);
			free(node_buy->value);
			free(node_buy->key);
			free(cur_buy);
			cur_buy = avl_last(tree);
			node_buy = _get_entry(cur_buy, struct kv_node, avl);
		}
		if (node_sell->value->num <= 0){
			find = _get_entry(node_sell->value->avl, struct kv_node1, avl);
			avl_remove(base, node_sell->value->avl);
			free(find->value);
			free(node_sell->value->avl);
			avl_remove(tree_sell, cur_sell);
			free(node_sell->value);
			free(node_sell->key);
			free(cur_sell);
			cur_sell = avl_first(tree_sell);
			node_sell = _get_entry(cur_sell, struct kv_node, avl);
		}
	}
	return 0;
}

int parser(char **str, void* tree, struct avl_tree *base)
{
	struct kv_node *node;
	struct kv_node1 *gen;

	gen = (struct kv_node1 *)malloc(sizeof(struct kv_node1));
	node = (struct kv_node *)malloc(sizeof(struct kv_node));
    node->key = malloc(sizeof(struct keys));
	node->value = malloc(sizeof(order_buy));
	node->key->id = atoi(str[1]);
	gen->value = malloc(sizeof(struct w_link));
	gen->key = node->key->id;
	gen->value->alp = str[2][0];
	gen->value->avl = &node->avl;
	node->value->avl = &gen->avl;
	avl_insert(base, &gen->avl, cmp_func1);
	node->key->price = atof(str[4]);
	node->value->id = atoi(str[1]);
	node->value->num = atoi(str[3]);
	node->value->price = atof(str[4]);
	avl_insert((struct avl_tree*)tree, &node->avl, cmp_func);
	return 0;
}

int cansel_operation(void* tree, struct avl_tree *base, FILE *fd, struct kv_node1* find, struct avl_node* cur)
{
	canc *gg;
	struct kv_node *second;

	gg = malloc(sizeof(struct cancel));
	gg->buy = 'X';
	gg->id = find->key;
	fprintf(fd, "%c,%d\n", gg->buy, gg->id);
	free(gg);
	second = _get_entry(find->value->avl, struct kv_node, avl);
	avl_remove((struct avl_tree*)tree, find->value->avl);
	free(second->value);
	free(second->key);
	free(find->value->avl);
	avl_remove(base, cur);
	free(find->value);
	free(cur);
}

char *pattern_open()
{
    canc *gg;

	struct avl_tree tree;
	struct avl_tree base;
	struct avl_tree tree_sell;

    struct avl_node *cur;
    struct kv_node *node, query1;
	struct kv_node1 *gen, query, *find;

	char **str;

    FILE * fp;
    FILE* fd;
    char * line = NULL;
    size_t len = 0;
    int read;

	avl_init(&tree, NULL);
	avl_init(&tree_sell, NULL);
	avl_init(&base, NULL);

    fp = fopen("../trade/input.txt", "r");
    fd = fopen("output.txt", "w+");
    if (fp == NULL || fd == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
		str = ft_split(line, ',');
		if (str[0][0] == 'C') {
		    int x = atoi(str[1]);
		    query.key = x;
		    cur = avl_search(&base, &query.avl, cmp_func1);
		    find = _get_entry(cur, struct kv_node1, avl);
		    if (find == NULL)
			{
				ft_freetab(str, 2);
                continue;
			}
		    else if (find->value->alp == 'B')
		    {
				cansel_operation(&tree, &base, fd, find, cur);
		    }
		    else{
				cansel_operation(&tree_sell, &base, fd, find, cur);
		    }
			ft_freetab(str, 2);
		}
		else if (str[2][0] == 'B') {
			parser(str, &tree, &base);
			ft_freetab(str, 5);
		}
		else if (str[2][0] == 'S') {
			parser(str, &tree_sell, &base);
			ft_freetab(str, 5);
		}
		if (tree.root && tree_sell.root)
		{
		    market(&tree_sell, &tree, &base, fd);
		}
    }
    fclose(fd);
	fclose(fp);
    return ("fgg");
}

int main(void)
{
	pattern_open();
	return 0;
}