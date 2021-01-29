#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
Function to differentiate between tokens that are different types of numbers(decimal integers, octal integers, hexadecimal integers, decimal floating-point).
Takes char* input as input, determines what the next token is and which type of number it is, prints it, and then returns the substring of input after the number token
*/
char* number (char* input) {
	int i=0;
   	char* token;
   	char* next_token;

	if(input[0] == '0' && (input[1] == 'x' || input[1] == 'X')){	//hex
		i = 2; // move i to be after 0x
		char delims[] = "abcdefABCDEF0123456789"; // delim which we will compare chars against to see when the hex ends
		while(strchr(delims, input[i]) != NULL){
			i++;
		}
		if (i == 2) { // if i == 2, this means our string was something like 0xy, which is not a hex int, it is a decimal int 0 followed by the word xy
			printf("integer \"0\"\n");
			next_token = &input[1]; // set next token to be address of char following 0
			return next_token;
		} else {
			// if we enter here, token must be a hex int
			// malloc space for token, copy it into token from input, print it w/formatting, and set next token to the following char
			token = (char*)malloc(sizeof(char) * (i + 1));
			token[i] = '\0';
			strncpy(token, input, i);
			printf("hexadecimal integer \"%s\"\n", token);
			free(token);
			next_token = &input[i];
			return next_token;
		}	
	} else {
		int floating_point = 0;
		int octal = 0;
		if (input[0] == '0' && isdigit(input[1])) octal = 1;
		
		while (isdigit(input[i])) {									//integers
			if (input[i] == '8' || input[i] == '9') octal = 0;
	       	i++;
		}
		if (input[i] == '.' && isdigit(input[i + 1])) {										//floating-points
			i++;
			floating_point = 1;
			octal = 0;
			while (isdigit(input[i])) {
	       		i++;
			}
			if (input[i] == 'e' && (isdigit(input[i + 1]) || (input[i + 1] == '-' && isdigit(input[i + 2])))) {	//exponents
				i++;
				if (input[i] == '-') i++;
				while (isdigit(input[i])) {
	       			i++;
				}
			}
		}
		token = (char*)malloc(sizeof(char)*(i+1));
		token[i] = '\0';
		strncpy(token, input, i);
		if (floating_point == 1) {
			printf("floating-point \"%s\"\n", token);

		} else if (octal == 1) {
			printf("octal integer \"%s\"\n", token);

		} else {
			printf("decimal integer \"%s\"\n", token);
		}
		free(token);
		next_token = &input[i];
		return next_token;
    } 


}

/*
alphabetic character followed by any number of alphanumeric characters
*/
char* word (char* input) {
    int i = 0;
    char* token;
    char* next_token;	
    while(isalpha(input[i]) || isdigit(input[i])) {
	    i++;
    }	
    token = (char*)malloc(sizeof(char)*(i + 1));
	token[i] = '\0';
    strncpy(token, input, i);
	if (strcmp(token, "sizeof") == 0) {				// sizeof operator
		printf("sizeof operator \"%s\"\n", token);
	} else if (strcmp(token, "if") == 0 || strcmp(token, "else") == 0 || strcmp(token, "while") == 0 || strcmp(token, "for") == 0 || strcmp(token, "return") == 0 || strcmp(token, "auto") == 0 || strcmp(token, "const") == 0 || strcmp(token, "double") == 0 || strcmp(token, "float") == 0 || strcmp(token, "int") == 0||strcmp(token, "short") == 0 || strcmp(token, "struct") == 0 || strcmp(token, "unsigned") == 0 || strcmp(token, "break") == 0 || strcmp(token, "continue") == 0 || strcmp(token, "long") == 0 || strcmp(token, "signed") == 0 || strcmp(token, "switch") == 0 || strcmp(token, "void") == 0 || strcmp(token, "case") == 0||strcmp(token, "default") == 0 || strcmp(token, "enum") == 0 || strcmp(token, "goto") == 0 || strcmp(token, "register") == 0 || strcmp(token, "typedef") == 0 || strcmp(token, "volatile") == 0 || strcmp(token, "char") == 0 || strcmp(token, "do") == 0 || strcmp(token, "extern") == 0 || strcmp(token, "static") == 0||strcmp(token, "union")==0) {
		printf("keyword \"%s\"\n", token);			// C keywords
	} else {
    	printf("word \"%s\"\n", token);				// regular word tokens
	}
	free(token);
    next_token = &input[i];
    return next_token;
}


char* comment (char* input) {
	int i;
	char* next_token = &input[strlen(input)];

	if (input[1] == '*') {								/* block comments */
		for (i = 3; i < strlen(input); i++) {
			if (input[i] == '/' && input[i - 1] == '*') {
				next_token = &input[i + 1];
				break;
			}
		}
	} else {											// line comments terminated by newline
		for (i = 3; i < strlen(input); i++) {
			if (input[i] == '\n') {
				next_token = &input[i + 1];
				break;
			}
		}
	}
	
	return next_token;

}

char* quotes (char* input) {
	char type = input[0]; 
	char* next_token = &input[1];
	
	int i;
	for (i = 1; i < strlen(input); i++) {
		if (input[i] == type) {
			char* output = (char*)malloc(sizeof(char) * (i + 2));
			output[i + 1] = '\0';
			strncpy(output, input, i + 1);
			printf("string %s\n", output);			//matched quote, output string
			free(output);
			next_token = &input[i + 1];
			return next_token;
		}
	}
	if (type == '\"') {							//unmatched quotes
		printf("double quote \"%c\"\n", type);
	} else {
		printf("single quote \"%c\"\n", type);
	}
	return next_token;

}

/*
recursive function
takes each substring passed from main
checks first character to determine if first token of substring is number, word, or operator
*/
void split(char* input) {               

    //base case, called when string is null
	if (input[0] == '\0') {
		return;
	}     

	if (input[0] == '\"' || input[0] == '\'') {			// single or double quotes
		char* next_token = quotes(input);
		split(next_token);
    }
     if(input[0] == ' ' || input[0] == '\t' || input[0] == '\v' || input[0] == '\f' ||input[0] == '\n' ||input[0] == '\r'){ //In designing our function, if the character is one of the delimiters then it will end the previous token without needing to know which one as they aren't alphanumeric or operators. This means if input[0] is any of them it can just move on to the next character. That being said, they also aren't an error, which is why this check is necessary
            split(&input[1]);
    } else if (isdigit(input[0])) {             	    //if next token is number
    	char* next_token = number(input);
		split(next_token);
    } else if (isalpha(input[0])) {             //if next token is letter
		char* next_token = word(input);
		split(next_token);
    } else {                                    //if next token is operator
    	if (input[0] == ' ') {
	    	split(&input[1]);
       	} else if (input[0] == '.') {
			printf("structure member \".\"\n");
			split(&input[1]);
		} else if (input[0] == '+') {
			if (input[1] == '+') {
				printf("increment \"++\"\n");
				split(&input[2]);
			} else if (input[1] == '=') {
				printf("plus equals \"+=\"\n");
				split(&input[2]);
			} else { 
				printf("addition \"+\"\n");
				split(&input[1]);
			}
		} else if (input[0] == '-') {
			if (input[1] == '-') {
				printf("decrement \"--\"\n");
				split(&input[2]);
			} else if(input[1] == '=') {
				printf("minus equals \"-=\"\n");
				split(&input[2]);
			} else if(input[1] == '>') {
				printf("structure pointer \"->\"\n");
				split(&input[2]);
			} else {
				printf("minus/subtract operator \"-\"\n");
				split(&input[1]);
			}
		} else if (input[0] == '(') {
			printf("left parentheses \"(\"\n");
			split(&input[1]);
		} else if (input[0] == ')') {
			printf("right parentheses \")\"\n");
			split(&input[1]);
		} else if (input[0] == '[') {
			printf("left bracket \"[\"\n");
			split(&input[1]);
		} else if (input[0] == ']') {
			printf("right bracket \"]\"\n");
			split(&input[1]);
		} else if(input[0] == '=') {
			if (input[1] == '=') {
				printf("equality test \"==\"\n");
				split(&input[2]);
			} else {
				printf("assignment \"=\"\n");
				split(&input[1]);
			}
		} else if (input[0] == '!') {
			if (input[1] == '=') {
				printf("inequality test \"!=\"\n");
				split(&input[2]);
			} else {
				printf("negate \"!\"\n");
				split(&input[1]);
			}
		} else if (input[0] == '<'){
			if (input[1] == '<') {
				if (input[2] == '=') {
					printf("shift left equals \"<<=\"\n");
					split(&input[3]);
				} else {
					printf("shift left \"<<\"\n");
					split(&input[2]);
				}
			} else if (input[1] == '='){
				printf("less than or equal test \"<=\"\n");
				split(&input[2]);
			} else {
				printf("less than test \"<\"\n");
				split(&input[1]);
			}
		} else if (input[0] == '>') {
			if (input[1] == '>') {
				if (input[2] == '=') {
					printf("shift right equals \">>=\"\n");
					split(&input[3]);
				} else {
					printf("shift right \">>\"\n");
					split(&input[2]);
				}
			} else if (input[1] == '=') {
				printf("greater than or equal test \">=\"\n");
				split(&input[2]);
			} else {
				printf("greater than test \">\"\n");
				split(&input[1]);
			}
		}  else if (input[0] == '&') {
			if (input[1] == '&') {
				printf("logical AND \"&&\"\n");
				split(&input[2]);
			} else if(input[1] == '=') {
				printf("bitwise AND EQUALS \"&=\"\n");
				split(&input[2]);
			} else {
				printf("AND/address operator \"&\"\n");
				split(&input[1]);
			}
		}else if (input[0] == '|') {
			if (input[1] == '|') {
				printf("logical OR \"||\"\n");
				split(&input[2]);
			} else if (input[1] == '=') {
				printf("bitwise OR equals \"|=\"\n");
				split(&input[2]);
			} else {
				printf("bitwise OR \"|\"\n");
				split(&input[1]);
			}
		} else if (input[0] == '*') {
			if(input[1] == '=') {
				printf("times equals \"*=\"\n");
				split(&input[2]);
			} else {
				printf("multiply/dereference operator \"*\"\n");
				split(&input[1]);
			}
		} else if (input[0] == '/') {
			if (input[1] == '/' || input[1] == '*') {			//handles comments
				char* next_token = comment(input);
	   			split(next_token);
			} else if (input[1] == '=') {
				printf("divide equals \"/=\"\n");
				split(&input[2]);
			} else {
				printf("division \"/\"\n");
				split(&input[1]);
			}
		}  else if (input[0] == '^') {
			if (input[1] == '=') {
				printf("bitwise XOR equals \"^=\"\n");
				split(&input[2]);
			} else {
				printf("bitwise XOR \"^\"\n");
				split(&input[1]);
			}
		} else if (input[0] == ','){
			printf("comma \",\"\n");
			split(&input[1]);
		} else if (input[0] == '~'){
			printf("1s complement \"~\"\n");
			split(&input[1]);
		} else if (input[0] == '?'){
			printf("conditional true \"?\"\n");
			split(&input[1]);
		} else if (input[0] == ':'){
			printf("conditional false \":\"\n");
			split(&input[1]);
		} else if (input[0] == '%' && input[1] == '=') {
			char temp = '%';
			printf("mod equals \"%c=\"\n", temp);
			split(&input[2]);
		}
	}

	return;

}

int main(int argc, char **argv) {
	
	if (argc < 2) {
		printf("Error: no input\n");
	} else if (argc > 2) {
		printf("Error: too many inputs (try putting input string in double quotes)\n");
	} else {
		char* input = (char*)malloc(sizeof(char) * (strlen(argv[1]) + 1));
		input[strlen(argv[1])] = '\0';
		strcpy(input, argv[1]);
		//printf("input: %s\n", input);
		split(input);
		free(input);
	} 

    return 0;
}
