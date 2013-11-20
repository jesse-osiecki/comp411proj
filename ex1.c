////This code is reproduced partly from 'The C Programming Language'
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
char *strdp(char *); //headers
#define HASHSIZE 100001

typedef struct nlist { /* table entry: */
    struct nlist *next; /* next entry in chain */
    char *name; /* defined name */
    char *defn; /* replacement text */
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
        if (np == NULL || (np->name = strdup(name)) == NULL)
          return NULL;
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } else /* already there */
        free((void *) np->defn); /*free previous defn */
    if ((np->defn = strdup(defn)) == NULL)
       return NULL;
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

#define WORDSLENGTH 100001
#define MAX_LINE 1000
main(){
	char **words;
	char ***words_ref = &words;
	printf("Enter a corpus of text from which the random text should be generated:\n\n");
	char delim[11] =  " ,.!?;:\"\n";
	words = malloc(WORDSLENGTH * sizeof(char*));//allocate space for huge array of words

	while(1){//if the input is \0 exit
		//get corpus from stdin
		char *inpLine = (char *) malloc(MAX_LINE); //inpline has room for 1000 chars including \0
		if (fgets(inpLine, MAX_LINE, stdin) == NULL){ break; }
		char *pch = (char *) malloc(MAX_LINE);
		pch = strtok(inpLine, delim); //returns begining of tokens
		while(pch != NULL){ //put all new tokenized values into the words array
			*words = (char *) malloc((strlen(pch)+1));
			*words = pch;
			words++;
		       	pch = strtok(NULL, delim);//I find this weird, however it causes pch to return the next token succesively
		}
	}
	*words = '\0';
	words = *words_ref;

	int i=0;
	while(i<3){
		printf("%s", *words);
		words++;i++;
	}
	return 0;
}
