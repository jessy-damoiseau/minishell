#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>

typedef enum    e_tokop // ici je liste les constantes definissant les token
{
	pipe,
	dollar,
}				t_token_op;

typedef enum e_tokcmd
{
	ls,
	cat,
}			t_token_cmd;

typedef struct	s_token // ici struct qui va contenir les token
{
	void	*value;
	t_token_type type; // la je vais appeler la const enum correpondant au token recherche
}				t_token;

t_token		tab_token[] // declaration litterale d'un tableau 2d avec 
{
	{"|", pipe},
	{"$", dollar},
	{0, 0};
};

int main(int argc, char **argv)
{

}