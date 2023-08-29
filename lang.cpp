#include "lang.h"

int main()
{
	char* str = (char*)calloc(100, sizeof(char));
	if(!str)
		return 1;

	scanf("%99s", str);

//
	errors_t error;
	hash_table* tab = create_tab(&error);
	char* tmp = "x";
	add_elem(tab, tmp, -90);
	printf("val = %d\n\n", get_value(tab, tmp, &error));
//

	int pos = 0;
	printf("%.3f\n", get_general(str, &pos, tab));

//
	delete_hash_table(&tab);
//

	free(str);

	return 0;
}

void syntax_error(char* str, int* pos)
{
	printf("You have syntax error! Your last string: %s, current symbol  = \"%c\"\n\n", str, str[*pos]);
}

double get_general(char* str, int* pos, hash_table* tab)
{
	double val = get_expression(str, pos, tab);

	clear_tabs(str, pos);

	if(str[*pos] == '$')
		++(*pos);
	else
	{
		SYNTAX_ERROR();
	}

	return val;
}

double get_num(char* str, int* pos)
{
	//printf("aaa pos = %d str[pos] = %c\n", *pos, str[*pos]);

	//clear_tabs(str, pos);

	double val = 0;
	int k = 0;

	while(str[*pos] >= '0' && str[*pos] <= '9')
	{
		val = val*10 + (str[*pos] - '0');
		++(*pos);
		++k;
	}

	if(!k)
	{
		SYNTAX_ERROR();
	}
		 

	//printf("now val = %d\n", val);

	clear_tabs(str, pos);

	return val;
}

double get_expression(char* str, int* pos, hash_table* tab)
{
	//clear_tabs(str, pos);

	double val = get_mul_div(str, pos, tab);

	while(str[*pos] == '+' || str[*pos] == '-')
	{
		char op = str[*pos];
		++(*pos);

		double val2 = get_mul_div(str, pos, tab);

		if(op == '+')
			val += val2;
		else
			val -= val2;
	}

	return val;
}

double get_mul_div(char* str, int* pos, hash_table* tab)
{
	double val = get_parenthesis(str, pos, tab);

	while(str[*pos] == '*' || str[*pos] == '/')
	{
		char op = str[*pos];
		++(*pos);

		int val2 = get_parenthesis(str, pos, tab);

		if(op == '*')
			val *= val2;
		else if(val2 != 0)
			val /= val2;
		else
			printf("You have division by 0!\n");
	}

	return val;
}

double get_parenthesis(char* str, int* pos, hash_table* tab)
{
	clear_tabs(str, pos);

	if(str[*pos] == '(')
	{
		++(*pos);

		double val = get_expression(str, pos, tab);

		if(str[*pos] == ')')
			++(*pos);
		else
		{
		SYNTAX_ERROR();
		}

		return val;
	}
	else if(str[*pos] <= '9' && str[*pos] >= '0')
		return get_num(str, pos);
	else
		return get_var(str, pos, tab);
}

double get_var(char* str, int* pos, hash_table* tab)
{
	int var_pos = 0;

	//clear_tabs(str, pos);

	char* var = (char*)calloc(30, sizeof(char));
	if(!var)
	{
		return POISON;
	}

	while((str[*pos] >= 'a' && str[*pos] <= 'z') || (str[*pos] >= 'A' && str[*pos] <= 'Z'))
	{
		var[var_pos] = str[*pos];

		++(*pos);
		++var_pos;
	}

	if(!var_pos)
	{
		SYNTAX_ERROR();
	}

	//printf("now val = %d\n", val);

	clear_tabs(str, pos);


	errors_t error;

	int value = get_value(tab, var, &error);

	free(var);

	return value;

	/*char* tmp = "x";

	if(!strcmp(tmp, var))
		return -1;
	else
		return -2;*/

	//return val;
}

void clear_tabs(char* str, int* pos)
{
	while(str[*pos] == '%')
	{
		//printf("----\n");
		++(*pos);
	}

	//printf("sym : \"%c\"\n", str[*pos]);
}
