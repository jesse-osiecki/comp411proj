////This code is reproduced partly from 'The C Programming Language'
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
char *strdp(char *); //headers
#define HASHSIZE 32767
#define CHAINSIZE 1000
typedef struct nlist { /* table entry: */
	struct nlist *next; /* next entry in chain */
	char *name; /* defined name */
	char **defn; /* replacement text THIS IS AN ARRAY IN MY IMPL */
	int defn_size;
} NList;

static NList *hashtab[HASHSIZE]; /* pointer table */

/* hash: form hash value for string s */
unsigned hash(char *s)
{
	unsigned hashval;
	for (hashval = 0; *s != '\0'; s++)
	  hashval = *s + 31 * hashval;
	return hashval % HASHSIZE;
}

/* lookup: look for s in hashtab */
NList *lookup(char *s)
{
	NList *np;
	for (np = hashtab[hash(s)]; np != NULL; np = np->next)
		if (strcmp(s, np->name) == 0)
		  return np; /* found */
	return NULL; /* not found */
}

/* install: put (name, defn) in hashtab */
NList *install(char *name, char *defn)
{
	NList *np;
	unsigned hashval;
	if ((np = lookup(name)) == NULL) { /* not found */
		np = (struct nlist *) malloc(sizeof(*np));
		if (np == NULL || (np->name = strdp(name)) == NULL)
		  return NULL;
		hashval = hash(name);
		np->next = hashtab[hashval];
		hashtab[hashval] = np;
		np->defn = malloc(CHAINSIZE * sizeof(char*));
		np->defn_size = 0;
	}//else /* already there /////*/
	 //   free((void *) np->defn); /*free previous defn */
	if((np->defn[np->defn_size] = strdp(defn)) == NULL)//Input was null
	   return NULL;
	else
		np->defn_size++; //adding string to list succeeded to add one to size
	return np;
}

char *strdp(char *s) /* make a duplicate of s */
{
	char *p;
	p = (char *) malloc(strlen(s)+1); /* +1 for ’\0’ */
	if (p != NULL)
	   strcpy(p, s);
	return p;
}

/////////////////////////Dictionary End


#define WORDSLENGTH 32767
#define MAX_LINE 1000
main(){
	char **words;
	int i = 0;
//	printf("Enter a corpus of text from which the random text should be generated:\n\n");
	char delim[11] =  " ,.!?;:\"\n";
	words = malloc(WORDSLENGTH * sizeof(char*));//allocate space for huge array of words

	while(1){//if the input is \0 exit
		//get corpus from stdin
		char *inpLine = (char *) malloc(MAX_LINE); //inpline has room for 1000 chars including \0
		if (fgets(inpLine, MAX_LINE, stdin) == NULL){ break; }
		char *pch = (char *) malloc(MAX_LINE);
		pch = strtok(inpLine, delim); //returns begining of tokens
		while(pch != NULL){ //put all new tokenized values into the words array
			words[i] = (char *) malloc((strlen(pch)+1));
			words[i] = pch;
			i++;
		    pch = strtok(NULL, delim);//I find this weird, however it causes pch to return the next token succesively
		}
	}
	int array_size = i;
	/*i=0;
	while(i<array_size){
		printf("%s\n", words[i]);
		i++;
	}
	printf("done1");*/
	for( i=0; i < array_size-1; i++){
		install(words[i], words[i+1]);
	}
	for( i=0; i < 10; i++){
		NList *np = lookup(words[i]);
		if(np == NULL){ break;}
		printf("%s%s\n", np->name, np->defn[0]);
	}
	return 0;
}
