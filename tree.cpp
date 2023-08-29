#include "tree.h"

Tree* create_empty_tree(errors_t* error)
{
	Tree* new_tree = (Tree*)calloc(1, sizeof(Tree));
	if(!new_tree)
	{
		*error = NO_MEMORY;
		return NULL;
	}
	new_tree->root = NULL;

	new_tree->size = 0;

	new_tree->graph = fopen("graph.txt", "w");
	if(!new_tree->graph)
	{
		free(new_tree);
		return NULL;
	}

	new_tree->logs = fopen("logs.txt", "w");
	if(!new_tree->logs)
	{
		fclose(new_tree->graph);
		free(new_tree);
		return NULL;
	}

	new_tree->file_with_code = fopen("code.txt", "r");
	if(!new_tree->file_with_code)
	{
		fclose(new_tree->graph);
		fclose(new_tree->logs);
		free(new_tree);
		return NULL;
	}

	return new_tree;
}

/*Tree* create_default_tree(errors_t* error)
{
	Tree* new_tree = create_empty_tree(error);

	FILE* base = fopen("default_create_base.txt", "r");

	read_base(new_tree, base, 1, NULL, error);

	return new_tree;
}*/

errors_t recursion_find(Node* root, char* value, Node** ans)
{
	if(!root || !root->data || !value || !ans)
	{
		return BAD_PTR;
	}

	Node* node = root;
	if(!strcmp(node->data, value))
	{
		*ans = node;
		return ALL_OK;
	}

	if(node->left)
		recursion_find(node->left, value, ans);
	if(node->right)
		recursion_find(node->right, value, ans);

	return ALL_OK;
}

Node* find(Node* root, char* value, errors_t* error)
{
	Node** node = (Node**)malloc(sizeof(Node*));
	if(!node)
	{
		*error = NO_MEMORY;
		return NULL;
	}
	
	*node = NULL;

	if(*error = recursion_find(root, value, node))
	{
		free(node);
		return NULL;
	}
	
	Node* find_node =  *node;

	free(node);

	return find_node;
}


//Node* add_elem_after(Tree* info, Node* after, char* value, errors_t* error, int level)  //if after == NULL then it is a top elem
Node* add_elem_after(Tree* info, Node* after, char* value, errors_t* error, int level)  
{
	if(!error || !info)
		return NULL;

	if(!after)
	{
		*error = AFTER_NULL_ELEM;
		return NULL;
	}

	if(!value)
	{
		*error = NULL_ELEM;
		return NULL;
	}

	Node* new_node = (Node*)malloc(sizeof(Node));
	if(!new_node)
	{
		*error = NO_MEMORY;
		return NULL;
	}

	new_node->data = (char*)calloc(sizeof(char), strlen(value) + 1);
	if(!new_node->data)
	{
		free(new_node);
		*error = NO_MEMORY;
		return NULL;
	}
	strcpy(new_node->data, value);

	new_node->level = level;
	new_node->left = NULL;
	new_node->right = NULL;

	new_node->parent = after;

	++info->size;

	return new_node;
}

void skip_spaces(int* position, char* buffer)
{
	while(buffer[*position] == ' ' || buffer[*position] == '\n')
		++*position;
}

void read_str(char* buffer, int* position, char* str)
{
	int index = 0;
	while(buffer[*position] != '}' && buffer[*position] != '{' && buffer[*position] != '\n' && index < MAX_STR_LEN)
	{
		str[index] = buffer[*position];
		++*position;
		++index;
	}

	str[index] = '\0';

	// symbols in buffer which more than MAX_STR_LEN will be skipped
	while(buffer[*position] != '}' && buffer[*position] != '{')
	{
		++*position;
	}
}

Node* make_tree_from_base(Tree* info, int level, Node* after, errors_t* error, char* buffer, int* position)
{
	if(*position >= strlen(buffer))
		return NULL;

	if(!error)
		return NULL;

	if(!info || !after || !buffer)
	{
		*error = BAD_PTR;
		return NULL;
	}

	skip_spaces(position, buffer);

	char c = buffer[*position];
	if(c == '{')
	{
		++*position;

		char* str = (char*)malloc(sizeof(char) * MAX_STR_LEN);
		if(!str)
		{
			*error = NO_MEMORY;
			return NULL;
		}

		read_str(buffer, position, str);

		Node* new_node = add_elem_after(info, after, str, error, level);
		free(str);

		skip_spaces(position, buffer);

		if(buffer[*position] != '}')
		{
			new_node->left = make_tree_from_base(info, level + 1, new_node, error, buffer, position);
			new_node->right = make_tree_from_base(info, level + 1, new_node, error, buffer, position);
			skip_spaces(position, buffer);

			if(buffer[*position] == '}')
				++*position;
		}
		else
			++*position;

		return new_node;
	}/**/
	else
		++*position;

	return NULL;
}

//firstly after = NULL, save_tree != NULL, level == 1
void read_base(Tree* save_tree, FILE* base, int level, Node* after, errors_t* error)
{
	if(!save_tree || !save_tree->file_with_base)
	{
		*error = BAD_PTR;
		return;
	}

	fseek(base, 0, SEEK_END);

	size_t size = ftell(base);

	fseek(base, 0, SEEK_SET);

	char* buffer = (char*)calloc(size + 1, sizeof(char));

	fread(buffer, sizeof(char), size, base);

	/*fseek(save_tree->file_with_base, 0, SEEK_END);

	size_t size = ftell(save_tree->file_with_base);

	fseek(save_tree->file_with_base, 0, SEEK_SET);

	char* buffer = (char*)calloc(size + 1, sizeof(char));

	fread(buffer, sizeof(char), size, save_tree->file_with_base);*/

	if(!after)
	{
		Node* empty_elem = (Node*)calloc(1, sizeof(Node));
		if(!empty_elem)
		{
			*error = NO_MEMORY;
			return;
		}

		int position = 0;
		save_tree->root = make_tree_from_base(save_tree, level, empty_elem, error, buffer, &position);

		save_tree->root->parent = NULL;

		free(empty_elem);

		//printf(" root = %p\n", save_tree->root);
	}
	else
	{
		int position = 0;
		after->left = make_tree_from_base(save_tree, level, after, error, buffer, &position);
	}

	free(buffer);
}

// (n - 1 ) - amount of ' '
errors_t print_tree(Node* node, int n)
{
	if(!node)
	{
		printf("{}\n");
		return BAD_PTR;
	}

	printf("\n%*s", n, "{");

	printf("%s", node->data);
	if(node->left)
		print_tree(node->left, n + 3);
	if(node->right)
		print_tree(node->right, n + 3);

	if(node->right || node->left)
		printf("%*s\n", n, "}");
	else
		printf("%s\n", "}");

	return ALL_OK;
}

errors_t recursion_print_for_graph(Tree* tree, Node* node, int* index)
{
	if(!tree || !node || !index)
		return BAD_PTR;

    fprintf(tree->graph, " box%d[style=\"filled\", fillcolor=\"white\", label = \"%s\"];\n", *index, node->data);
    
    if(node->left)
    {
    	++*index;
    	recursion_print_for_graph(tree, node->left, index);
    }
    if(node->right)
    {
    	++*index;
    	recursion_print_for_graph(tree, node->right, index);
    }

    return ALL_OK;
}

errors_t connections(Tree* tree, Node* node, int* index)
{
	if(!tree || !node || !index)
		return BAD_PTR;

	int cur_index = *index;
    int left_index = *index + 1;

    if(node->left)
    {
    	++*index;
    	connections(tree, node->left, index);
    }

    int right_index = *index + 1;

    if(node->right)
    {
    	++*index;
    	connections(tree, node->right, index);
    }

    if(node->left)
    	fprintf(tree->graph, " box%d->box%d[label = \" Yes \" color = green fontcolor = green];\n", cur_index, left_index);

    if(node->right)
    	fprintf(tree->graph, " box%d->box%d[label = \" No \" color = red fontcolor = red];\n", cur_index, right_index);

    return ALL_OK;
}

int print_graph(Tree* tree)
{
    if(tree->graph == NULL)
        return NULL_FILE;

    fprintf(tree->graph, "digraph G{\n node[fontsize=11, margin=\"0.01\"];\n");

    fprintf(tree->graph, " edge [color=\"blue\"];\n");

    int index = 0;
    recursion_print_for_graph(tree, tree->root, &index);

    index = 0;
    connections(tree, tree->root, &index);

    fprintf(tree->graph, "}");

    return ALL_OK;

}

// (n - 1) - amount of ' '
errors_t file_print_tree(FILE* fp, Node* node, int n)
{
	if(!node)
	{
		fprintf(fp, "{}\n");
		return BAD_PTR;
	}

	fprintf(fp, "\n%*s", n, "{");

	fprintf(fp, "%s", node->data);
	if(node->left)
		file_print_tree(fp, node->left, n + 3);
	if(node->right)
		file_print_tree(fp, node->right, n + 3);

	if(node->right || node->left)
		fprintf(fp, "%*s\n", n, "}");
	else
		fprintf(fp, "%s\n", "}");

	return ALL_OK;
}

int tree_dump(Tree* tree, errors_t reason)
{
	if(!tree)
		return NO_TREE;

    if(tree->logs == NULL)
        return NULL_FILE;

    if(reason != ALL_OK)
        fprintf(tree->logs, "Dump was called because %s(%d)\n", error_names[abs(reason)], reason);

    fprintf(tree->logs, "Tree\n{\n  size = %ld\n\n  Our tree:\n", tree->size);

    file_print_tree(tree->logs, tree->root, 3);

    fprintf(tree->logs, "\n  file_with_base = %p\n", tree->file_with_base);
    fprintf(tree->logs, "  logs = %p\n", tree->logs);
    fprintf(tree->logs, "  graph = %p\n", tree->graph);


    fprintf(tree->logs,"}\n\n");

    return ALL_OK;
}

void delete_node(Node* node)
{
	if(!node)
		return;

	if(node->left)
		delete_node(node->left);
	
	if(node->right)
		delete_node(node->right);

	if(node->data)
	{
		free(node->data);
		node->data = NULL;
	}

	free(node);
	node = NULL;
}

errors_t delete_tree(Tree** tree)
{
	if(!tree || !*tree)
		return BAD_PTR;

	delete_node((*tree)->root);

	fclose((*tree)->file_with_base);
	fclose((*tree)->graph);
	fclose((*tree)->logs);

	free(*tree);
	*tree = NULL;

	return ALL_OK;
}

