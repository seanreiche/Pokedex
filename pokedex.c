#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Pokemon {

	char *num;
	char *name;
	int type1;
	int type2;
};

char *Types[18] = {"Normal", "Fire", "Water", "Electric", "Grass", "Ice", "Fighting", "Poison", "Ground", "Flying", "Psychic", "Bug", "Rock", "Ghost", "Dragon", "Dark", "Steel", "Fairy"};

float TypeChart[18][18] = { /* [attack][defense] */
{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0.5, 0, 1, 1, 0.5, 1},		/* Normal	*/
{1, 0.5, 0.5, 1, 2, 2, 1, 1, 1, 1, 1, 2, 0.5, 1, 0.5, 1, 2, 1}, 	/* Fire 	*/
{1, 2, 0.5, 1, 0.5, 1, 1, 1, 2, 1, 1, 1, 2, 1, 0.5, 1, 1, 1}, 		/* Water 	*/
{1, 1, 2, 0.5, 0.5, 1, 1, 1, 0, 2, 1, 1, 1, 1, 0.5, 1, 1, 1}, 		/* Electric 	*/
{1, 0.5, 2, 1, 0.5, 1, 1, 0.5, 2, 0.5, 1, 0.5, 2, 1, 0.5, 1, 0.5, 1}, 	/* Grass 	*/
{1, 0.5, 0.5, 1, 2, 0.5, 1, 1, 2, 2, 1, 1, 1, 1, 2, 1, 0.5, 1}, 	/* Ice		*/
{2, 1, 1, 1, 1, 2, 1, 0.5, 1, 0.5, 0.5, 0.5, 2, 0, 1, 2, 2, 0.5},     	/* Fighting 	*/
{1, 1, 1, 1, 2, 1, 1, 0.5, 0.5, 1, 1, 1, 0.5, 0.5, 1, 1, 0, 2},	      	/* Poison 	*/
{1, 2, 1, 2, 0.5, 1, 1, 2, 1, 0, 1, 0.5, 2, 1, 1, 1, 2, 1}, 	      	/* Ground 	*/
{1, 1, 1, 0.5, 2, 1, 2, 1, 1, 1, 1, 2, 0.5, 1, 1, 1, 0.5, 1},	      	/* Flying 	*/
{1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 0.5, 1, 1, 1, 1, 0, 0.5, 1}, 	      	/* Psychic 	*/
{1, 0.5, 1, 1, 2, 1, 0.5, 0.5, 1, 0.5, 2, 1, 1, 0.5, 1, 2, 0.5, 0.5}, 	/* Bug 		*/
{1, 2, 1, 1, 1, 2, 0.5, 1, 0.5, 2, 1, 2, 1, 1, 1, 1, 0.5, 1}, 		/* Rock 	*/
{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 0.5, 1, 1}, 		/* Ghost 	*/
{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 0.5, 0}, 		/* Dragon 	*/
{1, 1, 1, 1, 1, 1, 0.5, 1, 1, 1, 2, 1, 1, 2, 1, 0.5, 1, 0.5}, 		/* Dark 	*/
{1, 0.5, 0.5, 0.5, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 0.5, 2}, 	/* Steel 	*/
{1, 0.5, 1, 1, 1, 1, 2, 0.5, 1, 1, 1, 1, 1, 1, 2, 2, 0.5, 1} 		/* Fairy 	*/
};

void create_pokedex(struct Pokemon **, FILE *);
void create_party(struct Pokemon **, struct Pokemon **, FILE *);
void destroy_pokedex(struct Pokemon **);
void print_pokedex(struct Pokemon **);
struct Pokemon *create_pokemon(char *, char *, char *, char *);
void display_pokemon(struct Pokemon **, struct Pokemon **, int);
void menu(char, struct Pokemon **, struct Pokemon**, int);
void save_party(struct Pokemon **, FILE *);
void calculate_effective(float *, int, int);
int search(struct Pokemon **, char *);
void change_party(struct Pokemon **, struct Pokemon **);
void print_party(struct Pokemon **, struct Pokemon **, int);

int main()
{
	struct Pokemon *Pokedex[1044];
	struct Pokemon *Party[6];
			
	FILE *pokedex_file = fopen("pokedex.txt", "r");
	FILE *party_pokemon_read = fopen("party.txt", "r");
	FILE *party_pokemon_write;

	create_pokedex(Pokedex, pokedex_file);
	
	create_party(Party, Pokedex, party_pokemon_read);
	
	fclose(pokedex_file);
	fclose(party_pokemon_read);
	
	/* The start of the porgram */
	
	display_pokemon(Pokedex, Party, 0);
	
	party_pokemon_write = freopen("party.txt", "w", stdout);
	save_party(Party, party_pokemon_write);
	fclose(party_pokemon_write);
	
	destroy_pokedex(Pokedex);
	
	/* Test party.txt
	
Bulbasaur
Charmander
Squirtle
Caterpie
Beedrill
Pidgeot (Mega Pidgeot)

	*/
	
	return 0;
}

void create_pokedex(struct Pokemon **pokedex, FILE *pokedex_file)
{	
	int national_i = 0;
	char current = getc(pokedex_file);
	
	struct Pokemon *current_pokemon;
	
	while (current != EOF)
	{		
		while (current != '\n')
		{
			int i = 0;
			char tmp[50];
			char type2[20];	
			char num[10];
			char name[40];
			char type1[20];
			
			while (current != '/')
			{
				tmp[i] = current;
				current = getc(pokedex_file);
				i++;
			}
			tmp[i] = '\0';
			
			/* char num[strlen(tmp)]; */
			strcpy(num, tmp);
			
			for (i = 0; i < 50; i++)
			{
				tmp[i] = 0;
			}
			i = 0;
			current = getc(pokedex_file);
			
			while (current != '/')
			{
				tmp[i] = current;
				current = getc(pokedex_file);
				i++;
			}
			tmp[i] = '\0';
			
			/* char name[strlen(tmp)]; */
			strcpy(name, tmp);	
			
			for (i = 0; i < 50; i++)
			{
				tmp[i] = 0;
			}
			i = 0;
			current = getc(pokedex_file);
			
			while (current != '/')
			{
				tmp[i] = current;
				current = getc(pokedex_file);
				i++;
			}
			tmp[i] = '\0';
			
			/* char type1[strlen(tmp)]; */
			strcpy(type1, tmp);
			
			for (i = 0; i < 50; i++)
			{
				tmp[i] = 0;
			}
			i = 0;
			current = getc(pokedex_file);
			
			if (current != EOF && current != '\n')
			{
				while (current != '/')
				{
					tmp[i] = current;
					current = getc(pokedex_file);
					i++;
				}
				tmp[i] = '\0';
				
				strcpy(type2, tmp);				
			
			}
			else
			{
				for (i = 0; i < 20; i++)
				{
					type2[i] = 0;
				}
								
			}
			
			current = getc(pokedex_file);
				
			/* printf("%s - %s - %s %s\n", num, name, type1, type2); */
			
			current_pokemon = create_pokemon(num, name, type1, type2);
				
			pokedex[national_i] = current_pokemon;
			
			national_i++;
		}
			
		current = getc(pokedex_file);
	}
}

void create_party(struct Pokemon **party, struct Pokemon **pokedex, FILE *party_pokemon)
{	
	char current = getc(party_pokemon);
	char tmp[100];
	int party_i = 0;
	int i = 0;
	char pname[40];
	
	while (current != EOF)
	{
		i = 0;
		
		while (current != '\n')
		{
			tmp[i] = current;
			current = getc(party_pokemon);
			i++;
		}
		tmp[i] = '\0';
		
		strcpy(pname, tmp);
		
		for (i = 0; i < 100; i++)
		{
			tmp[i] = 0;
		}
		
		for (i = 0; i < 1044; i++)
		{
			char *name = pokedex[i]->name;
			
			if (strcmp(pname, name) == 0)
			{				
				party[party_i] = pokedex[i];
				party_i++;
			}
		}	
				
		current = getc(party_pokemon);
	}
}

struct Pokemon *create_pokemon(char *num, char *name, char *type1, char *type2)
{
	struct Pokemon *pokemon;
	int i;
	int type1_num = 0;
	int type2_num = -1;
	char *tmp;
	
	pokemon = (struct Pokemon *) malloc(sizeof(struct Pokemon));
	
	for (i = 0; i < 18; i++)
	{
		if (strcmp(Types[i], type1) == 0)
		{
			type1_num = i;
		}
	}

	if (strlen(type2) != 0)
	{
		for (i = 0; i < 18; i++)
		{
			if (strcmp(Types[i], type2) == 0)
			{
				type2_num = i;
			}
		}
	}

	tmp = malloc(strlen(num) + 1);
	strcpy(tmp, num);
	
	pokemon->num = tmp;
	
	tmp = malloc(strlen(name) + 1);
	strcpy(tmp, name);
	
	pokemon->name = tmp;
	
	pokemon->type1 = type1_num;
	pokemon->type2 = type2_num;
	
	return pokemon;
}

void destroy_pokedex(struct Pokemon **pokedex)
{
	int i;
	
	for (i = 0; i < 1044; i++)
	{
		free(pokedex[i]->num);
		free(pokedex[i]->name);
		
		free(pokedex[i]);		
	}
}

void print_pokedex(struct Pokemon **pokedex)
{	
	int i;
	
	for (i = 0; i < 1044; i++)
	{	
		char *num = pokedex[i]->num;
		char *name = pokedex[i]->name;
		int type1 = pokedex[i]->type1;
		int type2 = pokedex[i]->type2;
		
		printf("%s - %s - %d %d\n", num, name, type1, type2);
	}
}

void print_party(struct Pokemon **pokedex, struct Pokemon **party, int index)
{
	int i;
	char ch;
	
	while ((ch = getchar()) != '\n' && ch != EOF);	
	
	system("clear");
	
	for (i = 0; i < 6; i++)
	{
		printf("%s\n", party[i]->name);
	}
	
	printf("\nPress enter to continue\n");
	
	ch = getchar();
		
	display_pokemon(pokedex, party, index);
}

void display_pokemon(struct Pokemon **pokedex, struct Pokemon **party, int i)
{
	char *num = pokedex[i]->num;
	char *name = pokedex[i]->name;
	int type1 = pokedex[i]->type1;
	int type2 = pokedex[i]->type2;
	char option[20];
	int index;
	float effective[18];
	
	char *type1_text = Types[type1];
	char *type2_text;
	
	system("clear");
	
	printf("%s -- %s\n", name, num);

	printf("%s ", type1_text);
	
	if (type2 != -1)
	{
		type2_text = Types[type2];
		printf("%s \n", type2_text);
	}
	else
	{
		printf("\n");
	}
	
	calculate_effective(effective, type1, type2);
		
	printf("\n\nULTRA-effective:\n\t");
	
	for (index = 0; index < 18; index++)
	{
		if (effective[index] == 4)
		{
			printf("%s ", Types[index]);
		}
	}
	
	printf("\n\nSuper-effective:\n\t");
	
	for (index = 0; index < 18; index++)
	{
		if (effective[index] == 2)
		{
			printf("%s ", Types[index]);
		}
	}
	
	printf("\n\nNeutral:\n\t");
	
	for (index = 0; index < 18; index++)
	{
		if (effective[index] == 1)
		{
			printf("%s ", Types[index]);
		}
	}
	
	printf("\n\nNot very effective:\n\t");
	
	for (index = 0; index < 18; index++)
	{
		if (effective[index] == 0.5)
		{
			printf("%s ", Types[index]);
		}
	}
	
	printf("\n\nNo effect:\n\t");
	
	for (index = 0; index < 18; index++)
	{
		if (effective[index] == 0)
		{
			printf("%s ", Types[index]);
		}
	}
	
	if (i != 0)
	{
		printf("\n\n<---- %s (a)", pokedex[i - 1]->name);
	}
	else
	{
		printf("\n\n            ");
	}	
	
	printf("\t");
	
	if (i != 1044)
	{
		printf("(d) %s ---->\n", pokedex[i + 1]->name);
	}
	else
	{
		printf("            \n");
	}
	
	printf("\nSearch (s)\nPrint party (p)\nChange party (c)\nQuit (q)\n\nOption: ");
	
	scanf("%s", option);
	
	menu(option[0], pokedex, party, i);
}

void calculate_effective(float *effective, int type1, int type2)
{
	int i;
	
	for (i = 0; i < 18; i++)
	{
		effective[i] = TypeChart[i][type1];
		
		if (type2 != -1)
		{
			effective[i] *= TypeChart[i][type2];
		}
	}	
}

void change_party(struct Pokemon **party, struct Pokemon **pokedex)
{
	int i;
	int new_i;
	char name[40];
	
	system("clear");
	
	while (1)
	{
		for (i = 0; i < 6; i++)
		{			
			printf("%d: %s\n", i, party[i]->name);
		}
		
		i = 0;
		
		printf("Enter the number of the pokemon you would like to change\n\n");
		
		scanf("%d", &i);
		
		printf("%d\n", i);
		
		printf("Enter the name of the pokemon you would like to change it with\n\n");
		
		scanf("%s", name);
		
		new_i = search(pokedex, name);
		
		if (new_i == -1)
		{
			printf("No pokemon found");
			continue;
		}
		
		party[i] = pokedex[new_i];
		display_pokemon(pokedex, party, new_i);
		break;
	}		
}

int search(struct Pokemon **pokedex, char *query)
{
	int result = 0;
	int i;
	int j;
	int k;

	for (i = 0; i < 1044; i++)
	{
		char *name = pokedex[i]->name;
				
		for (j = 0; j < (int) strlen(name); j++)
		{
			if (name[j] == query[0])
			{				
				for (k = 0; k < (int) strlen(query); k++)
				{
					if (k + j >= (int) strlen(name))
					{
						/* It can't be it */
						break;
					}
					
					if (query[k] != name[k + j])
					{
						/* Doesn't match */
						break;
					}
					else
					{
						if (k == (int) strlen(query) - 1)
						{
							/* It matches */					
							
							result = i;
							return result;
						}
					}					
				} 
			}
		}	
	}
	
	return 0;
}

void save_party(struct Pokemon **party, FILE *party_pokemon)
{
	int i;
	
	for (i = 0; i < 6; i++)
	{	
		fprintf(party_pokemon, "%s\n", party[i]->name);
	}
}

void menu(char option, struct Pokemon **pokedex, struct Pokemon **party, int i)
{	
	char new_option[20];
	char query[40];
	
	switch(option) 
	{
		case 'a' :
			if (i != 0)
			{
				i--;
			}
		
			display_pokemon(pokedex, party, i);
			break;
			
		case 'd' :
			if (i != 1043)
			{
				i++;
			}

			display_pokemon(pokedex, party, i);
			break;
			
		case 's' :
			printf("\nSearch: ");
			scanf("%s", query);
			i = search(pokedex, query);		
			display_pokemon(pokedex, party, i);
			break;
		
		case 'p' :
			print_party(pokedex, party, i);
			break;
		case 'c' :
			change_party(party, pokedex);
			break;
			
		case 'q' :
			return;
			
		default :
			printf("Invalid Input\n" );
			printf("\nOption: ");
			scanf("%s", new_option);
			menu(new_option[0], pokedex, party, i);
	}
}


/*

 uuub	_____	duuu
  \\   @|O,O|@   //
   \\  (+++++)  //
    \\___| |___//
     \\--- ---//
      \_._|_._/ 

*/

