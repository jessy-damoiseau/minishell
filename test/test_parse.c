#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>
#include "minishell.h"

typedef enum    e_token_type // ici je liste les constantes definissant les token
{
	pipeline,
	dollar,
	space,
	redir_left,
	redir_right,
	dble_redir_right,
	dble_redir_left,
	sgle_quote,
	dble_quote,
	literal,
}				t_token_type;

// typedef enum e_tokcmd
// {
// 	ls,
// 	cat,
// }			t_token_cmd;

void	ft_del(void *content)
{
	free(content);
	content = NULL;
}

typedef struct	s_token // ici struct qui va contenir les token
{
	void	*value;
	t_token_type type; // la je vais appeler la const enum correpondant au token recherche
}				t_token;

// declaration litterale d'un tableau 1d de type struct t_token de sorte que
// tab[i].value = le symbole cherche
// tab[i].type = le type defini par le enum t_token_type
t_token		tab_token[] = 
{
	{"|", pipeline},
	{"$", dollar},
	{" ", space},
	{">", redir_right},
	{"<", redir_left},
	{">>", dble_redir_left},
	{"<<", dble_redir_right},
	{0, 0}, // la valeur 0 en fin me permettra de marquer l'arret lors du parcours du tab 
	// => evite d'avoir a fixer arret du while avec la size precise du nombre de valeurs definies dans tab_token
};


t_token_type	ft_define_token(char charset) // envoyer ici en str char par char le prompt lu
{
	int i;
	t_token_type type;
	char *str;

	i = 0;
	type = literal; // par defaut on defini le type comme literal
	while(tab_token[i].value) // je parcoure le tab de type struct jusqu'a atteindre value = 0
	{
		str = tab_token[i].value;
		if (str[0] == charset)
		{
			type = tab_token[i].type;
			return (type);
		}
		i++;
	}
	return (type);
}

t_token		*ft_malloc_token(char *value, t_token_type type, int i, int j)
{
	t_token	*token;
	char *str;

	// str = malloc(sizeof(char) * 2);
	// str[0] = value;
	// str[1] = '\0';
	// str = NULL;
	token = malloc(sizeof(t_token)); // ajouter au gbcollector ici
	if (!token)
		token = NULL; // ajouter fonction exit de gbcollector ici
	token->type = type;
	if (!j)
	{
		str = malloc(sizeof(char) * 2);
		ft_strncpy(str, value, 1);
		// str[0] = value[0];
		// str[1] = '\0';
		// token->value = ft_strdup(str);
	}
	else
	{
		str = malloc(sizeof(char) * (j - i + 1));
		ft_strncpy(str, value, j - i + 1);
		// token->value = ft_strdup(str);
	}
	token->value = ft_strdup(str);
	free(str);
	// token->value = ft_strdup(value);
	// token->value = ft_strdup(str);
	// free(str);
	// printf("check token.value %p\n", token->value);
	// printf("check token.value %s\n", (char *)token->value);
	// printf("check token.value %s\n", str);
	// free(str);
	// str = NULL;
	return (token);
}

void	ft_clear_token(t_list **lst, void (*del)(void *))
{
	t_list	*tmp;
	t_token	*token;

	tmp = NULL;
	token = NULL;

	while (*lst)
	{
		tmp = *lst;
		token = tmp->content;
		*lst = (*lst)->next;
		del(token->value);
		del(tmp->content);
		free(tmp);
	}
	*lst = NULL;
}




void	ft_create_token(char *str)
{
	int i;
	// t_token_type	type;
	t_token	*token;
	t_list	*list;
	t_list	*new;
	// char *word;
	int j;

	i = 0;
	j = 0;
	list = NULL;
	// printf("len = %ld\n", ft_strlen(str));
	while(str[i])
	{
		// type = ft_define_token(str[i]);
		// printf("check type %d\n", type);
		if (literal != ft_define_token(str[i]))
			token = ft_malloc_token(str + i, ft_define_token(str[i]), i, 0);
		else
		{
			j = i;
			while(str[j] && literal == ft_define_token(str[j]))
			{
				j++;
			}
			// printf("check j = %d\n", j);
			token = ft_malloc_token(str + i, ft_define_token(str[i]), i, j - 1);
			i = j - 1;
			// printf("check 1 = %d\n", i);
		}
		// printf("check token %p\n", token);
		// printf("check token.value %s\n", (char *)token->value);
		// printf("check token.type %d\n", token->type);
		new = ft_lstnew(token);
		// free(token);
		//if (!list)
		//	exit(1); // rajouter ici fonction exit globale
		ft_lstadd_back(&list, new);
		// printf("%s\n", (char *)token->value);
		// printf("check 2 = %d\n", i);
		// if (str[i] != '\0')
		i++;
	}
	// ft_join_word(list);
	t_list *test;
	test = list;
	t_token *testtok;
	// char *s;
	while (test)
	{
		// printf("%s", list->content);
		testtok = test->content;
		// s = testtok->value;
		// printf("%s", (char *)s);
		printf("check token.value %s\n", (char *)testtok->value);
		// printf("check token.type %d\n", testtok->type);
		test = test->next;
	}
	// while(list)
	// {
	// 	testtok = list->content;
	// 	free(testtok->value);
	// 	testtok->value = NULL;
	// 	printf("check value 1 %p\n", testtok->value);
	// 	printf("check token 1 %p\n", testtok);
	// 	free(testtok);
	// 	testtok = NULL;
	// 	// printf("check value 2 %p", testtok->value);
	// 	printf("check token 2 %p\n", testtok);
	// 	list = list->next;
	// }
	// ft_lstclear(&list, &ft_del);
	ft_clear_token(&list, &ft_del);
}

// creer une fonction tokenizer qui:
// recupere le type de ft_define_token
// cree un chaine de type token dans list avec la valeur lu en char et le type associe
// prendre chaine dans fonction qui va grouper les valeur litterales en mots puis distinguer celles en cmd et les autres
// puis expand les valeurs dans la fonction

// parcourir la str, checker char par char son type
// fonction de check type = par defaut type literal sauf si check me dit que est un token autre
// une fois chaine tokeniser - copier dans une autre chaine en distinguant
// cmd, operateur, et en ayant expand les var + sans les espaces du coup car fonctionne sans
// sauf dans les quotes car espaces a conserver
int main(int argc, char **argv)
{
	// int i;
	// t_token_type type;

	// i = 0;
	// (void)argc;
	// (void)argv;
	// printf("%s\n", tab_token[1].value);
	// printf("%d\n", tab_token[1].type);
	// printf("%s\n", tab_token[0].value);
	// printf("%s\n", argv[1]);
	if (argc == 2)
	{
		ft_create_token(argv[1]);
		// while(argv[i])
		// {
		// 	type = ft_define_token(*argv[i]);
		// 	printf("token ref = %d\n", type);
		// 	i++;
		// }
	}
	return (0);
}