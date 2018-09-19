// Bonus homework assignment 
// Name: patils2
// Section 3
// TA: Anand Sahu
/* 
	Assumptions: Based on homework handout I am assuming identifier declarations occur
	position 0. Otherwise there is whitespace padding other parts of the MIPS code. I
	am also assuming an identifier won't occur more than 80 times in any given file.
	Additionally, although it is entirely possible not all white space is ' ' 
	(hence me using isspace to check for blank lines and whatnot), when it comes to setting 
	delimiter for I assumed any one of the standard C whitespace characters could be present 
	and set all of them as possible delimiters. Last assumption is rather small but I assumed that
	if an identifier name occured in a comment in a line, that the identifier was also used as part
	of active code on the same line. I only had one input example to go off of so I made sure.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define LINE_LENGTH 80

struct identifier
{
	char var[11];
	int positions[80];
	int defined;
};
typedef struct identifier structure;

structure members[100]; /* array of identifiers */ 
int checkBlankLine(char inpLine[LINE_LENGTH]); /* checks if line is blank */
int isComment(char inpLine[LINE_LENGTH]); /* determines if lines is a comment line */
void printStruct(FILE *oustr, int numStructs); /* prints info for every identifier struct */
/* skims line to see if previously declared identifier was used on this line. Returns 1 only if a new identifier was added. Otherwise returns 0. */
int checkStruct(char inpLine[LINE_LENGTH], int lineNum, int structIndex); 

int main(int argc, char *argv[])
{
	/* opens the input file checks if it is valid */
	//char *input_file = argv[2];
	//char *output_file = argv[3];
	FILE *instr = fopen("test_input.s", "r");
	if(instr == NULL)
	{
		printf("Could not open file for reading\n");
		exit(1);
	}
	FILE *oustr = fopen("outputTest.txt", "w");
	if(oustr == NULL)
	{
		printf("Could not open file for writing\n");
		exit(1);
	}

	char inputLine[LINE_LENGTH];
	int lineNum = 1;
	int numStructs = 0;

	while(1)
	{
		if(fgets(inputLine, LINE_LENGTH, instr) == NULL) /* End of file, break out of loop */
		{
			break;
		}
		if(isComment(inputLine)) /*no need to check rest of line for anything, simply print it*/
		{
			fprintf(oustr, "%d  %s", lineNum, inputLine);
			lineNum++;
			continue;
		}
		else if(checkBlankLine(inputLine)) /* echo line without line number */
		{
			fprintf(oustr, "%s", inputLine);
			continue;
		}
		/* Line is neither blank nor a comment. Parse it. */
		else
		{
			fprintf(oustr, "%d  %s", lineNum, inputLine);
			numStructs += checkStruct(inputLine, lineNum, numStructs); /* if function returns 1 (new struct was created), then numstructs gets incremented */
			lineNum++;
		}
	}
	fclose(instr); /* No more input, close instr */

	fprintf(oustr, "\n");
	fprintf(oustr, "\n");
	fprintf(oustr, "Identifier Reference Table\n");
	fprintf(oustr, "\n");
	fprintf(oustr, "  Identifier Definition-Line Use Line(s)\n");
	printStruct(oustr, numStructs);
	fclose(oustr);
	return 0;
}

void printStruct(FILE *oustr, int numStructs) /* prints info for all structs line by line */
{
	int i;
	int endIndex = numStructs + 1;
	for(i = 0; i < endIndex; i++)
	{
		fprintf(oustr, "  %-10s %-15d ", members[i].var, members[i].defined);
		int j = 0;
		while(members[i].positions[j] != 0 && j < 79)
		{
			fprintf(oustr, "%d, ", members[i].positions[j]);
			j++;
		}
		fprintf(oustr, "\n"); /* end line and go to next struct or identifier */
	}
}

int checkStruct(char inpLine[LINE_LENGTH], int lineNum, int structIndex) /* parses each line, creates new structs if necessary, updates existing ones if necessary */
{
	int created = 0;
	char newStructName[11];
	int i;
	char *token;
	int endIndex = structIndex-1;

	if(isspace(inpLine[0]) == 0) /* new identifier declaration */
	{
		for(i = 0; i < LINE_LENGTH; i++)
		{
			if(inpLine[i] != ':')
			{
				newStructName[i] = inpLine[i];
			}
			else if(inpLine[i] == ':')
			{
				break;
			}
		}
		strcpy(members[structIndex].var, newStructName);
		members[structIndex].defined = lineNum;
		created = 1;
		endIndex += 1;
	}

	token = strtok(inpLine, " ,:\n\t\f\r\v"); /* tokenize by commas, colons, any of the standard whitespace characters */
	while(token != NULL)
	{
		/* check every token with names of known identifiers */
		for(i = 0; i < endIndex; i++)
		{
			int isSame = strcmp(token, members[i].var); /* compares both char arrays to see if they are the same */
			if(isSame == 0)
			{
				int j;
				for(j = 0; j < 80; j++)
				{
					if(members[i].positions[j] == lineNum) /* incase this line num was already added to this identfiers positions array */
					{
						break;
					}
					if(members[i].positions[j] == 0)
					{
						members[i].positions[j] = lineNum; /* if same then updates that identifiers position to include current line number */
						break;
					}
				}
			}
		}
		token = strtok(NULL, " ,:\n\t\f\r\v"); /* get next token */
	}
	return created;
}

int checkBlankLine(char inpLine[LINE_LENGTH]) /* bool function to test whether input line is blank */
{
	int i = 0;
	while(inpLine[i] && i < 80)
	{
		if(isspace(inpLine[i]))
		{
			i++;
			continue;
		}
		else
		{
			return 0;
		}
	}
	return 1;
}

int isComment(char inpLine[LINE_LENGTH]) /* bool function to test whether input line is comment */
{
	if(inpLine[0] == '#')
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
