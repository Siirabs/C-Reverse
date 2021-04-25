#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


struct lines
{
	char * string;
	struct lines * prev;
	struct lines * next;
};

int read_lines(struct lines ** line, FILE * stream)
{
	struct lines * lp;
	
	char *buffer = NULL;
  	size_t len = 0;

	
	while (getline(&buffer, &len, stream) != -1) {
		
		//memory allocation
		if (*line == NULL) {
			if ((*line = lp = malloc(sizeof(struct lines))) == NULL) {
				fprintf(stderr, "Malloc failed\n");
				return -1;
			}
			lp->prev = lp->next = NULL;
		} else {
			if ( (lp->next = malloc(sizeof(struct lines))) == NULL) {
				fprintf(stderr, "Malloc failed\n");
				return -1;
			}
			//linking list correctly
			lp->next->prev = lp;
			lp = lp->next;
			
		}
		
		if ( (lp->string = malloc(strlen(buffer) + 1)) == NULL) {
			fprintf(stderr, "Malloc failed\n");
			return -1;
		}
		
		strcpy(lp->string, buffer);
	}
	free(buffer);

	return 0;
}

void write_to_file(struct lines * line, FILE * stream)
{

	struct lines * lp;
	lp = line;

	//moves to end of lp
	while (lp->next != NULL) {
		lp = lp->next;
	}

	//loops from back to start while writing current string to file
	while (lp != NULL) {
		fprintf(stream, "%s",lp->string);
		lp = lp->prev;
	}
	
}

void print_lines(struct lines * line, FILE * stream)
{
	struct lines * lp;
	lp = line;

	//moves to end of lp
	while (lp->next != NULL) {
		lp = lp->next;
	}

	//loops from back to start while printing current string
	while (lp != NULL) {
		printf("%s",lp->string);
		lp = lp->prev;
	}
	
			
}

void delete_lines(struct lines * line)
{
	struct lines * lp;
	lp = line;
	//loops through structure freeing data until lp is null
	while (lp != NULL) {
		line = lp->next;
		free(lp->string);
		free(lp);
		lp = line;
	}

}

int main(int argc, char **argv)
{
	
	struct lines * line = NULL;

	if (argc == 1) {
		//reading lines directly from stdin
		if (read_lines(&line, stdin) == -1)
			exit(1);
		printf("\n\n");
		print_lines(line, stdout);
		delete_lines(line);
	}
	if (argc == 2) {
		FILE *input = NULL;
		//checks for null input
		if((input = fopen(argv[1], "r")) == NULL) {
			fprintf(stderr,"Error: cannot open input file '%s'\n", argv[1]);
			exit(1);
		}
	
		read_lines(&line, input);
		print_lines(line, input);
		delete_lines(line);
		fclose(input);
	}	

	if (argc == 3) {
		FILE *input = NULL;
		FILE *output = NULL;
		//comparing first and second arguments
		if ( ! strcmp(argv[1], argv[2])) {
		fprintf(stderr, "Input and output file must differ.\n");
		exit(1);
		}
		//checks if file doesn't exist
		if((input = fopen(argv[1], "r")) == NULL) {
			fprintf(stderr,"Error: cannot open input file '%s'\n", argv[1]);
			exit(1);
		}
		//checks if argument is null
		if((output = fopen(argv[2], "w")) == NULL) {
			fprintf(stderr,"Error: cannot open output file '%s'\n", argv[2]);
			exit(1);
		}
		
		read_lines(&line, input);		
		write_to_file(line, output);
		delete_lines(line);
		
		fclose(output);
		
		
	}
	//If given too many arguments, give instructions for usage
	if (argc > 3) {
		fprintf(stderr,"Usage: reverse <input> <output>\n");
		exit(1);
	}
	
	
	
	return 0;
}
//https://c-for-dummies.com/blog/?p=1112
