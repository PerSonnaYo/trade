#include "trade.h"
int id = 0;

//https://github.com/greensky00/avltree/blob/master/avltree/avltree.c


struct kv_node *costil(struct avl_node **cur_buy, struct avl_tree *tree, struct avl_node* (*avl_s)(struct avl_tree*), struct avl_node* (*avl_l)(struct avl_node*) )
{
    struct avl_node *cur_base, *next;
    struct kv_node *node_buy, *n_node;
    cur_base = avl_s(tree);
    *cur_buy = cur_base;
    node_buy = _get_entry(cur_base, struct kv_node, avl);
    while(1)
    {
        next = avl_l(cur_base);
        n_node = _get_entry(next, struct kv_node, avl);
        if (n_node && node_buy && node_buy->key->price == n_node->key->price)
        {
            if (node_buy->key->id > n_node->key->id)
            {
                cur_base = next;
                *cur_buy = next;
                node_buy = n_node;
            } else{
                cur_base = next;
            }
        } else
            break;
    }
    return node_buy;
}


void market(struct avl_tree *tree_sell, struct avl_tree *tree, struct avl_tree *base, FILE *fd)
{
	order_sell* ord;

	int price = 0;
	struct avl_node *cur_buy, *cur_sell;
	struct kv_node *node_buy, *node_sell;
	struct kv_node1 *find;

	node_buy = costil(&cur_buy, tree, &avl_last, &avl_prev);// ищем максимальную цену покупки
	node_sell = costil(&cur_sell, tree_sell, &avl_first, &avl_next);//ищем минимальную цену продажи
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
			ord->price = node_buy->value->prici;
		}
		else{
			ord->cont = 'S';
			ord->id_present = node_buy->value->id;
			ord->id_early = node_sell->value->id;
			ord->price = node_sell->value->prici;
		}
		ord->num = node_sell->value->num > node_buy->value->num ? node_buy->value->num : node_sell->value->num;
		price = node_sell->value->num;
		node_sell->value->num = node_sell->value->num - node_buy->value->num;
		node_buy->value->num = node_buy->value->num - price; //корректируем количество товара
		fprintf(fd, "%c,%d,%c,%d,%d,%d,%s", ord->buy, ord->id, ord->cont, ord->id_early, ord->id_present, ord->num, ord->price);
		free(ord);
		if (node_buy->value->num <= 0){
			find = _get_entry(node_buy->value->avl, struct kv_node1, avl);
			avl_remove(base, node_buy->value->avl);
			free(find->value);
			free(node_buy->value->avl);
			avl_remove(tree, cur_buy); // удаляем ноду если количество товара ниже 0 (берем ссылку на базовую ноду и тоже удаляем)
			free(node_buy->value->prici);
			free(node_buy->value);
			free(node_buy->key);
			free(cur_buy);
			node_buy = costil(&cur_buy, tree, &avl_last, &avl_prev);// берем новую ноду(дешевле)
		}
		if (node_sell->value->num <= 0){
			find = _get_entry(node_sell->value->avl, struct kv_node1, avl);
			avl_remove(base, node_sell->value->avl);
			free(find->value);
			free(node_sell->value->avl);
			avl_remove(tree_sell, cur_sell); // аналогично с деревом продаж
			free(node_sell->value->prici);
			free(node_sell->value);
			free(node_sell->key);
			free(cur_sell);
			node_sell = costil(&cur_sell, tree_sell, &avl_first, &avl_next);//берем новую ноду(дороже)
		}
	}
}

void parser(char **str, void* tree, struct avl_tree *base)
{
	struct kv_node *node;
	struct kv_node1 *gen;
	size_t i = 0;

	gen = (struct kv_node1 *)malloc(sizeof(struct kv_node1)); // создаем базовое дерево с ключем id для того чтобы легче было искать и отменять операции
	node = (struct kv_node *)malloc(sizeof(struct kv_node));// создаем структуру для добавления в дерево продаж/покупок
    node->key = malloc(sizeof(struct keys));
	node->value = malloc(sizeof(order_buy));
	node->key->id = atoi(str[1]);
	gen->value = malloc(sizeof(struct w_link));
	gen->key = node->key->id;
	gen->value->alp = str[2][0];
	gen->value->avl = &node->avl; //добавляем обоюдные ссылки чтобы легче было удалять
	node->value->avl = &gen->avl;
	avl_insert(base, &gen->avl, cmp_func1);// заполняем дерево
	node->key->price = atof(str[4]);
	node->value->id = node->key->id;
	node->value->num = atoi(str[3]);
	node->value->price = atof(str[4]);
	while(str[4][i] != '.')
	    i++;
	while(str[4][i] != '\0')  // делаем так чтобы не мешал 0 в конце
	{
	    if (str[4][i] == '0' && str[4][i + 1] == '\n')
	        break;
	    i++;
	}
	if (i != strlen(str[4]))
	{
	    if (str[4][i - 1] == '.')
	        node->value->prici = strdup(str[4]);
	    else{
	        str[4][i] = '\n';
	        str[4][i + 1] = '\0';
	        node->value->prici = strdup(str[4]);
	    }
	}
	else
        node->value->prici = strdup(str[4]);
	avl_insert((struct avl_tree*)tree, &node->avl, cmp_func);
}

void cansel_operation(void* tree, struct avl_tree *base, FILE *fd, struct kv_node1* find, struct avl_node* cur)
{
	canc *gg;
	struct kv_node *second;

	gg = malloc(sizeof(struct cancel));
	gg->buy = 'X';
	gg->id = find->key;
	fprintf(fd, "%c,%d\n", gg->buy, gg->id);
	free(gg);
	second = _get_entry(find->value->avl, struct kv_node, avl); //берем ссылку на дерево продаж/покупок и удаляем
	avl_remove((struct avl_tree*)tree, find->value->avl);
	free(second->value->prici);
	free(second->value);
	free(second->key);
	free(find->value->avl); // удаляем ноду из базового дерева
	avl_remove(base, cur);
	free(find->value);
	free(cur);
}

void pattern_open()
{
	struct avl_tree tree;
	struct avl_tree base;
	struct avl_tree tree_sell;

    struct avl_node *cur;
	struct kv_node1 query, *find;

	char **str;

    FILE* fp;
    FILE* fd;
    char *line = NULL;
    size_t len = 0;

    avl_init(&tree, NULL);//инициализация деревьев
	avl_init(&tree_sell, NULL);
	avl_init(&base, NULL);

    fp = fopen("input.txt", "r");
    fd = fopen("output.txt", "w+");
    if (fp == NULL || fd == NULL)
        exit(EXIT_FAILURE);
    while ((getline(&line, &len, fp)) != -1) {
            str = ft_split(line, ',');//разбиваем строку на нужные нам данные
            free(line);
            line = NULL;
            if (str[0][0] == 'C') { //если оперция отмены
                int x = atoi(str[1]);
                query.key = x;
                cur = avl_search(&base, &query.avl, cmp_func1); // ищем в базовом дереве ключ
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
	free_tree1(&base);
	free_tree(&tree);
	free_tree(&tree_sell);
}

int main(void)
{
	pattern_open();
	return 0;
}