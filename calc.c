#include <stdio.h>
#include <stdlib.h>


double get_num(char* str, int* pos);
void syntax_error(char* str, int* pos);
double get_expression(char* str, int* pos);
double get_general(char* str, int* pos);
double get_mul_div(char* str, int* pos);
double get_parenthesis(char* str, int* pos);
void clear_tabs(char* str, int* pos);

int main()
{
	char* str = calloc(100, sizeof(char));
	if(!str)
		return 1;

	scanf("%99s", str);

	int pos = 0;
	printf("%.3f\n", get_general(str, &pos));

	free(str);

	return 0;
}

void syntax_error(char* str, int* pos)
{
	printf("You have syntax error! Your last string: %s, current symbol  = \"%c\"\n", str, str[*pos]);
}

double get_general(char* str, int* pos)
{
	double val = get_expression(str, pos);

	if(str[*pos] == '$')
		++(*pos);
	else
		syntax_error(str, pos);

	return val;
}

double get_num(char* str, int* pos)
{
	//printf("aaa pos = %d str[pos] = %c\n", *pos, str[*pos]);

	double val = 0;
	int k = 0;

	while(str[*pos] >= '0' && str[*pos] <= '9')
	{
		val = val*10 + (str[*pos] - '0');
		++(*pos);
		++k;
	}

	if(!k)
		syntax_error(str, pos);

	//printf("now val = %d\n", val);

	clear_tabs(str, pos);

	return val;
}

double get_expression(char* str, int* pos)
{
	//clear_tabs(str, pos);

	double val = get_mul_div(str, pos);

	while(str[*pos] == '+' || str[*pos] == '-')
	{
		char op = str[*pos];
		++(*pos);

		double val2 = get_mul_div(str, pos);

		if(op == '+')
			val += val2;
		else
			val -= val2;
	}

	return val;
}

double get_mul_div(char* str, int* pos)
{
	double val = get_parenthesis(str, pos);

	while(str[*pos] == '*' || str[*pos] == '/')
	{
		char op = str[*pos];
		++(*pos);

		int val2 = get_parenthesis(str, pos);

		if(op == '*')
			val *= val2;
		else if(val2 != 0)
			val /= val2;
		else
			printf("You have division by 0!\n");
	}

	return val;
}

double get_parenthesis(char* str, int* pos)
{
	if(str[*pos] == '(')
	{
		++(*pos);

		double val = get_expression(str, pos);

		if(str[*pos] == ')')
			++(*pos);
		else
			syntax_error(str, pos);
		return val;
	}
	else
		return get_num(str, pos);
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

