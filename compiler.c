#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100000
#define MAX_BUFF 1000

// GLOBAL VARIABLES
int numidents = 0;
int numtokens = 0;
char identarr[MAX_BUFF][12];
char tokenarr[MAX_BUFF][12];

typedef enum {
  oddsym = 1,
  identsym = 2,
  numbersym = 3,
  plussym = 4,
  minussym = 5,
  multsym = 6,
  slashsym = 7,
  xorsym = 8,
  eqlsym = 9,
  neqsym = 10,
  lessym = 11,
  leqsym = 12,
  gtrsym = 13,
  geqsym = 14,
  lparentsym = 15,
  rparentsym = 16,
  commasym = 17,
  semicolonsym = 18,
  periodsym = 19,
  becomessym = 20,
  beginsym = 21,
  endsym = 22,
  ifsym = 23,
  thensym = 24,
  whilesym = 25,
  dosym = 26,
  callsym = 27,
  constsym = 28,
  varsym = 29,
  procsym = 30,
  writesym = 31,
  readsym = 32,
  elsesym = 33
} token_type;

typedef struct
{
	int kind; // const = 1, var = 2, proc = 3
	char name[12]; // name up to 11 chars
	int val; // number (ASCII value)
	int level; // L level
	int addr; // M address
	int mark; // to indicate unavailable or deleted
} symbol;

typedef struct
{
  int op;
  int L;
  int M;
} assem;

// GLOBAL VARIABLES
symbol table[500];
assem text[1000];
int token = 0;
// int numVars = 0;
int tableSize = 0;
int symIdx = 0;
int jpcIdx = 0;
int oprM = 0;
int loopIdx = 0;
int cx = 0;
int count = 0;
char name [12];
char opChange [4];
int constant = 0;
int lev = 0;
int tx = 0;
int dx = 3;
int levMax = 4;
int varArray [1000];
int varArrayLevels = -1;

// GLOBAL VARIABLES

// FUNCTION PROTOTYPES
int isReserved(char *buff);
int setIdentifier(char *buff);
int isWhiteSpace(char c);
int symbolTableCheck(char* str);
void program();
void block();
void const_dec();
void var_dec();
void statement();
void condition();
void expression();
void term();
void factor();
void getToken();
void printCrash(int code);
void emit(int op, int L, int M);
void symTableAdd(int kind, char* name, int val, int level, int mark, int addy);
void changeOP(int code, int i);
int errorHandler();
int symbTableDupCheck(char* str);

int main(int arc, char* argv[])
{
  char char_arr[MAX]; // stores entire program
  char buffer[MAX_BUFF];
  char lexeme_list[MAX_BUFF][12];
  int lexeme_count = 0;
  int c, i = 0, num_char = 0, buff_idx = 0;
  int res_flag = 0;

  FILE *ptr = fopen(argv[1], "r");

  if (ptr == NULL) 
  {
    printf("Error\n");
    return 0;
  }

  // FILE I/O
  while (1) 
  {
    c = fgetc(ptr); // reading the file
    if (c == EOF)
      break;
    num_char++;
    char_arr[i] = c;
    i++;
  }

  for (int o = 0; o < i; o++)
  {
    printf("%c", char_arr[o]);
  }

  printf("\n\n");
  fclose(ptr); // Closing the file

  // primary loop
   for (int j = 0; j < i;) 
  {
    // if (numtokens > 23)
    // {
    //   break;;
    // }
    if ((isspace(char_arr[j]) || iscntrl(char_arr[j])) || (isalpha(char_arr[j])) || (isdigit(char_arr[j]))) 
    {
      // ROUTE 1 (SPACE/CNTRL)
      while (isspace(char_arr[j]) || iscntrl(char_arr[j])) 
      {
        j++;
      }

      // ROUTE 2 (ALPHABETICAL)
      if (isalpha(char_arr[j])) 
      {
        while (isalpha(char_arr[j]) || isdigit(char_arr[j])) 
        {
          buffer[buff_idx] = char_arr[j];
          j++;
          buff_idx++;
        }

        switch (isReserved(buffer)) 
        {
          case 1:
            strcpy(tokenarr[numtokens], "28");
            numtokens++;
             break;
          case 2:
            strcpy(tokenarr[numtokens], "29");
            numtokens++;
             break;
          case 3:
            strcpy(tokenarr[numtokens], "30");
            numtokens++;
             break;
          case 4:
            strcpy(tokenarr[numtokens], "27");
            numtokens++;
             break;
          case 5:
            strcpy(tokenarr[numtokens], "21");
            numtokens++;
             break;
          case 6:
            strcpy(tokenarr[numtokens], "22");
            numtokens++;
             break;
          case 7:
            strcpy(tokenarr[numtokens], "23");
            numtokens++;
             break;
          case 8:
            strcpy(tokenarr[numtokens], "24");
            numtokens++;
             break;
          case 9:
            strcpy(tokenarr[numtokens], "8");
            numtokens++;
             break;
          case 10:
            strcpy(tokenarr[numtokens], "33");
            numtokens++;
             break;
          case 11:
            strcpy(tokenarr[numtokens], "25");
            numtokens++;
             break;
          case 12:
            strcpy(tokenarr[numtokens], "26");
            numtokens++;
             break;
          case 13:
            strcpy(tokenarr[numtokens], "32");
            numtokens++;
             break;
          case 14:
            strcpy(tokenarr[numtokens], "31");
            numtokens++;
             break;
					case 15:
            strcpy(tokenarr[numtokens], "1");
            numtokens++;
             break;
          default:
            if (setIdentifier(buffer) != -1)
            {
            strcpy(tokenarr[numtokens], "2");
            numtokens++;
            strcpy(tokenarr[numtokens], buffer);
            numtokens++;
            }

            else
            {
              printf("\nError : Identifer names cannot exceed 11 characters\n");
            }
        }
       
        //clear buffer
        for (int i = 0; i < buff_idx; i++)
        {
          buffer[i] = '\0';
        }
        
        buff_idx = 0;
      }

      // ROUTE 3 (DIGIT)
      if (isdigit(char_arr[j])) 
      {
        int erflag = 1;
        while (isdigit(char_arr[j])) 
        {
          buffer[buff_idx] = char_arr[j];
          j++;
          buff_idx++;
        if (buff_idx > 5)
        {
          while (isalpha(char_arr[j]) || isdigit(char_arr[j]))
          {
            j++;
          }
          
          for (int i = 0; i < buff_idx; i++)
          {
            buffer[i] = '\0';
          }
          buff_idx = 0;
          
          printf("Error: Numbers cannot exceed 5 digits\n");
          erflag = 0;
        }
          
          if (isalpha(char_arr[j]))
          {
            while (isalpha(char_arr[j]) || isdigit(char_arr[j]))
            {
              j++;
            }
            
            for (int i = 0; i < buff_idx; i++)
            {
              buffer[i] = '\0';
            }
            buff_idx = 0;
            printf("Error: Identifiers cannot begin with a digit\n");
            erflag = 0;
            break;
          }
          // load chars into buffer
        }

        if (erflag != 0)
        {

          strcpy(tokenarr[numtokens], "3");
          numtokens++;
          strcpy(tokenarr[numtokens], buffer);
          numtokens++;
        }
        
        for (int i = 0; i < buff_idx; i++)
        {
          buffer[i] = '\0';
        }

        buff_idx = 0;

      }
    }

      // ROUTE 4 (SYMBOL CODE)
    else 
    {
      int symflag = 1;
      char sym = char_arr[j];
      if (sym == '+' || sym == '-'|| sym == '*'|| sym == '('|| sym == ')'|| sym == ',' || sym == '.' || sym == ';' || sym == '=')
      {
        symflag = 0;
        switch(char_arr[j])
        {
          case '+':
            // fprintf(ptr2, "+\t\t4\n");
            strcpy(tokenarr[numtokens], "4");
            numtokens++;
            break;
          case '-':
            // fprintf(ptr2, "-\t\t5\n");
            strcpy(tokenarr[numtokens], "5");
            numtokens++;
            break;
          case '*':
            // fprintf(ptr2, "*\t\t6\n");
            strcpy(tokenarr[numtokens], "6");
            numtokens++;
             break;
          case '(':
            // fprintf(ptr2, "(\t\t15\n");
            strcpy(tokenarr[numtokens], "15");
            numtokens++;
             break;
          case ')':
            // fprintf(ptr2, ")\t\t16\n");
            strcpy(tokenarr[numtokens], "16");
            numtokens++;
             break;
          case ',':
            // fprintf(ptr2, ",\t\t17\n");
            strcpy(tokenarr[numtokens], "17");
            numtokens++;
             break;
          case '.':
            // fprintf(ptr2, ".\t\t19\n");
            strcpy(tokenarr[numtokens], "19");
            numtokens++;
             break;
          case ';':
            // fprintf(ptr2, ";\t\t18\n");
            strcpy(tokenarr[numtokens], "18");
            numtokens++;
            break;
          case '=':
            // fprintf(ptr2, "=\t\t9\n");
            strcpy(tokenarr[numtokens], "9");
            numtokens++;
            break;
        }
      }

      if (char_arr[j] == ':')
      {
        symflag = 0;
        j++;
        if (char_arr[j] == '=')
        {
          // j++;
          // fprintf(ptr2, ":=\t\t20\n");
          strcpy(tokenarr[numtokens], "20");
          numtokens++;
        }
      }

      if (char_arr[j] == '>')
      {
        symflag = 0;
        if (char_arr[j + 1] == '=')
        {
          // fprintf(ptr2, ">=\t\t14\n");
          strcpy(tokenarr[numtokens], "14");
          numtokens++;
          j++;
        }

        else  
        {
          // fprintf(ptr2, ">\t\t13\n");
          strcpy(tokenarr[numtokens], "13");
          numtokens++;
        }
      }
      
      if (char_arr[j] == '<')
      {
        symflag = 0;
        if (char_arr[j + 1] == '=') 
        {
          // fprintf(ptr2, "<=\t\t12\n");
          strcpy(tokenarr[numtokens], "12");
          numtokens++;
          j++;
        }

        else if (char_arr[j + 1] == '>')
        {
          // fprintf(ptr2, "<>\t\t10\n");
          strcpy(tokenarr[numtokens], "10");
          numtokens++;
          j++;
        }

        else
        {
          // fprintf(ptr2, "<\t\t11\n");
          strcpy(tokenarr[numtokens], "11");
          numtokens++;
        }
      }
      
      
      if (char_arr[j] == '/')
      {
        symflag = 0;
        if (char_arr[j + 1] == '*')
        {
          j++;
          int flag = 0;
          while (flag == 0)
          {     
            j++;
            if ((char_arr[j + 1] == '*') && (char_arr[j + 2] == '/'))
            {
              flag = 1;
            }
          }

          char_arr[j] = '\0';
          j++;
          j++;
        }

        else
        {
          // Exit
          // fprintf(ptr2, "/\t\t7\n");
          strcpy(tokenarr[numtokens], "7");
          numtokens++;
        }

      
      }

      if (symflag == 1)
      {
        // fprintf(ptr2, "Error: Invalid Symbol\n");
      }
      
      j++;
    }
    
  }


  // HW 3 START --------------------------------------------------------------------
  getToken();
  // emit(7, 0, 1);

  for (int i = 0; i < 1000; i++)
  {
    varArray[i] = 0;
  }

  program();

  printf("No errors, program is syntactically correct\n\n");
  printf("Assembly Code:\n");
  printf("Line  OP     L   M\n");
  
  // Open file to write into ouput file
  FILE *ptr2 = fopen("elf.txt", "w");

  // prints all assembly instructions
  
  for (int i = 0; i < cx; i++)
  {
    // Special cases for OPR due to multi instructions
    changeOP(text[i].op, i);
    if (text[i].op == 2)
        oprM = text[i].M;

    // Handles first JMP
    // Multiplies by 3 to get the right M value
    int enhancer  = 1;
    if (text[i].op == 7 || text[i].op == 8)
    {
      enhancer = 3;
    }

    int temp = enhancer * text[i].M;
    printf("%d     %s    %d   %d\n", i, opChange, text[i].L, temp);
    fprintf(ptr2 ,"%d %d %d\n", text[i].op, text[i].L ,(enhancer * text[i].M));
  } 
  
  fclose(ptr2);
  return 0;
}

int isReserved(char *buff) 
{
  if (!strcmp(buff, "const"))
    return 1;
  if (!strcmp(buff, "var"))
    return 2;
  if (!strcmp(buff, "procedure"))
    return 3;
  if (!strcmp(buff, "call"))
    return 4;
  if (!strcmp(buff, "begin"))
    return 5;
  if (!strcmp(buff, "end"))
    return 6;
  if (!strcmp(buff, "if"))
    return 7;
  if (!strcmp(buff, "then"))
    return 8;
  if (!strcmp(buff, "xor"))
    return 9;
  if (!strcmp(buff, "else"))
    return 10;
  if (!strcmp(buff, "while"))
    return 11;
  if (!strcmp(buff, "do"))
    return 12;
  if (!strcmp(buff, "read"))
    return 13;
  if (!strcmp(buff, "write"))
    return 14;
	if (!strcmp(buff, "odd"))
    return 15;
  return 0;
}

int setIdentifier(char *buff) 
{
  if (strlen(buff) > 11){
  return -1;
  }

  for (int i = 0; i < numidents; i++) 
  {
    if (!(strcmp(buff, identarr[i]))) 
    {
      return 1;
    }
  }
  
  strcpy(identarr[numidents], buff);
  numidents++;
  return 0;
}

int isWhiteSpace(char c) 
{
  if (c == ' ')
    return 1;
  if (c == '\n')
    return 1;
  if (c == '\t')
    return 1;
  if (c == '\v')
    return 1;
  if (c == '\f')
    return 1;
  if (c == '\r')
    return 1;
  return 0;
}

void program()
{
	block();
  
	if (token != 19)
  {
		printCrash(1);
  }

	emit(9,0,3); // halt code
}

void block()
{
  lev++;
  varArrayLevels++;

  int tx0;
  tx0 = tableSize;
  table[tableSize].addr = cx;
  emit (7, 0, 0);

  if (lev > levMax)
  {
    printCrash(21);
  }
 
  do
  {
    const_dec();
    
    var_dec();

    // procsym
    while (token == 30)
    {
      getToken();
			if (token == 2)
			{
        symTableAdd(       3,       name,       0,       varArrayLevels,        0,       0);
				getToken();
			}

			else
			{
        printCrash(17);
			}

			// token == semicolon
			if (token == 18)
			{
				getToken();
			}

			else
			{
				printCrash(20);
			}
			
			block();

      // semicolon
			if (token == 18)
			{
				getToken();
			}

			else
			{
				printCrash(20);
			}
    }
		
    
  } while ((token == 28)||(token == 29)||(token == 30));
  
  emit(6,0, 3 + varArray[varArrayLevels]);

	text[varArrayLevels].M = (cx - 1);
	table[tx0 - 1].addr = (3 * varArrayLevels);

	statement();
  if (varArrayLevels != 0)
    emit(2, 0, 0);
  varArrayLevels--;
  lev--;
} 

void const_dec()
{
	// constsym
	if (token == 28)
	{
		do{
			getToken();
			
			// identsym
			if (token != 2)
				printCrash(2);

	  		// duplicate identifier
			if (symbTableDupCheck(name) != -1) 
				printCrash(3);

			getToken();
			// eqlsym
			if(token != 9)
				printCrash(4);
	
			getToken();
			// numbersym
			if(token != 3)
				printCrash(5);
	
			// add to symbol table 
      symTableAdd(1, name, constant, varArrayLevels, 0, 0);

			getToken();
		} while(token == 17); // commasym

		// semicolonsym
		if(token != 18)
			printCrash(6);
	
		getToken();
	}
}

void var_dec()
{ 
    // varsym
    if (token == 29)
    {
      do
      {
        getToken();
        // token == identsym
				
        if (token != 2)
          printCrash(2);
          
        if (symbTableDupCheck(name) != -1)
          printCrash(3);


        symTableAdd(2, name, atoi(tokenarr[count]), varArrayLevels, 0, varArray[varArrayLevels] + 3);

        getToken();
        varArray[varArrayLevels]++;

      } while (token == 17); // commasym

      // semicolonsym
      if (token != 18)
         printCrash(6);

      getToken();
    }
}

void statement()
{
	if(token == 2)
	{ 
		int x = symbolTableCheck(name);

		if(x == -1)
    {
			printCrash(7);
    }
		
    if(table[x].kind != 2)
			printCrash(8);
    
		getToken();

    // becomessym
		if(token != 20)
			printCrash(9);
    
		getToken();

		expression();

		emit(4, varArrayLevels - table[x].level ,table[x].addr); 

		return;
	}
  
	// callsym
  if (token == 27)
	{
		getToken();

		if(token != 2)
		{
			printCrash(16);
		}
      
		else
		{
			int i = symbolTableCheck(name);
			
			if(i == -1)
			{
				printCrash(11);
			}

			else
			{
				if(table[i].kind == 3)
				{
    
				  emit(5, varArrayLevels - table[i].level, table[i].addr); // if kind = proc then gen (cal, level, adr)
				}
				else
				{
					printCrash(19);
				}

        getToken();
			}
		}
	}
  
  // beginsym
  if (token == 21)
  {
    do
    {
			getToken();
			statement();
      int skipflag = 0;
      
      if (token != 18)
      {
          skipflag = errorHandler();
      }
      
      if (skipflag == 1)
      {
          break;
      }
      
		} while(token == 18); // semicolonsym
		
    // endsym
		if (token != 22)
			printCrash(10);

		getToken();
		return;
  }

	// ifsym
  if (token == 23)
  {
    getToken();
    condition();

    int x = cx;
    emit(8,0,0);

    // thensym
    if (token != 24)
      printCrash(11);

    getToken();
    statement();
    text[x].M = cx;
  }

  // xor
  if (token == 8)
  {
    getToken();
    condition();
    int x = cx;
    emit (8,0,0);

    // thensym
    if (token != 24)
      printCrash(11);

    getToken();
    statement();

    // semicolonsym
    if (token != 18)
      printCrash(6);

    else
      getToken();
    
    // elsesym
    if (token != 33)
      printCrash(6);

    int y = cx;
    emit(7, 0, 0);
    getToken();

    text[x].M = cx;
    statement();
    text[y].M = cx;
  }
  
	// whilesym
  if (token == 25)
  {
    getToken();
    loopIdx = cx;
    condition();

    // dosym
    if (token != 26)
      printCrash(12);

    getToken();
    int x = cx;
    emit(8, 0, 0);
    
    statement();
    emit (7, 0, loopIdx);
    text[x].M = cx;
  }  

	// readsym
  if (token == 32)
  {
    getToken();
		if (token != 2) // identsym
			printCrash(2);

		symIdx = symbolTableCheck(name);
		if (symIdx == -1)
    {
			printCrash(7);
    }
		if(table[symIdx].kind != 2) // not a variable
			printCrash(8);

		getToken();
		emit(9,0,2); // emit read
		emit(4, varArrayLevels, table[symIdx].addr); // emit STO
  }
  
	// writesym
  if (token == 31)
  {
    getToken();
		expression();
		emit(9,0,1);
  }
}

void condition()
{
  // oddsym
	if(token == 1)
	{
		getToken();
		expression();
		emit(2,0,11);
	}

	else
	{
		expression();

    // eqlsym
		if (token == 9)
		{
			getToken();
      expression();
      emit(2,0,5);
		}

    // neqsym
		else if (token == 10)
		{
			getToken();
      expression();
      emit(2,0,6);
		}
    
    // lessym
		else if (token == 11)
		{
			getToken();
			expression();
			emit(2,0,7);
		}

    // leqsym
		else if (token == 12)
		{
			getToken();
			expression();
			emit(2,0,8);
		}

    // gtrsym
		else if (token == 13)
		{
			getToken();
			expression();
			emit(2,0,9);
		}

    // geqsym
		else if (token == 14)
		{
			getToken();
			expression();
			emit(2,0,10);
		}
		else
			printCrash(13);
	}
}

void expression()
{
  term();
  //          plussym       minussym 
  while (token == 4 || token == 5)
  {
    //        plussym
    if (token == 4)
    {
      getToken();
      term();
      emit (2,0,1);
    }

    //        minussym
    else
    {
      getToken();
      term();
      emit(2,0,2);
    }
  }
}

void term()
{
  factor();
  //           multsym      slashsym 
  while (token == 6 || token == 7)
    {
      //        multsym
      if (token == 6)
      {
        getToken();
        factor();
        emit(2,0,3);
      }
      
      //        slashsym
      else 
      {
        getToken();
        factor();
        emit(2,0,4);
      }
    }
}

void factor()
{

  if (!strcmp(tokenarr[count-2], "3"))
  {
    emit (1, 0, constant);
    getToken();
  }

  // identsym
  else if (token == 2)
  {
    symIdx = symbolTableCheck(name);


    if (symIdx  == -1)
    {
      printCrash(7);
    }

    if (table[symIdx].kind == 1)
    {
        emit(1, varArrayLevels, table[symIdx].val);
    }

    else if (table[symIdx].kind == 3)
    {
      printCrash(18);
    }

    else   
    {
        emit(3, varArrayLevels - table[symIdx].level, table[symIdx].addr); // LOD
    }
    getToken();
  }

  // lparentsym
  else if (token == 15)
  {
    getToken();
    expression();
    
    // rparentsym
    if (token != 16)
      printCrash(14);

    getToken();
  }

  else
  {
    printCrash(15);
  }
}

void getToken()
{
  if (!strcmp(tokenarr[count], "1"))
    token = 1;

  if (!strcmp(tokenarr[count], "2"))
  {
    token = 2;
    count++;
    strcpy(name, tokenarr[count]);
  }

  if (!strcmp(tokenarr[count], "3"))
  {
    token = 3;
    count++;
    constant = atoi(tokenarr[count]);
  }

  if (!strcmp(tokenarr[count], "4"))
    token = 4;
  if (!strcmp(tokenarr[count], "5"))
    token = 5;
  if (!strcmp(tokenarr[count], "6"))
    token = 6;
  if (!strcmp(tokenarr[count], "7"))
    token = 7;
  if (!strcmp(tokenarr[count], "8"))
    token = 8;
  if (!strcmp(tokenarr[count], "9"))
    token = 9;
  if (!strcmp(tokenarr[count], "10"))
    token = 10;
  if (!strcmp(tokenarr[count], "11"))
    token = 11;
  if (!strcmp(tokenarr[count], "12"))
    token = 12;
  if (!strcmp(tokenarr[count], "13"))
    token = 13;
  if (!strcmp(tokenarr[count], "14"))
    token = 14;
  if (!strcmp(tokenarr[count], "15"))
    token = 15;
  if (!strcmp(tokenarr[count], "16"))
    token = 16;
  if (!strcmp(tokenarr[count], "17"))
    token = 17;
  if (!strcmp(tokenarr[count], "18"))
    token = 18;
  if (!strcmp(tokenarr[count], "19"))
    token = 19;
  if (!strcmp(tokenarr[count], "20"))
    token = 20;
  if (!strcmp(tokenarr[count], "21"))
    token = 21;
  if (!strcmp(tokenarr[count], "22"))
    token = 22;
  if (!strcmp(tokenarr[count], "23"))
    token = 23;
  if (!strcmp(tokenarr[count], "24"))
    token = 24;
  if (!strcmp(tokenarr[count], "25"))
    token = 25;
  if (!strcmp(tokenarr[count], "26"))
    token = 26;
  if (!strcmp(tokenarr[count], "27"))
    token = 27;
  if (!strcmp(tokenarr[count], "28"))
    token = 28;
  if (!strcmp(tokenarr[count], "29"))
    token = 29;
  if (!strcmp(tokenarr[count], "30"))
    token = 30;
  if (!strcmp(tokenarr[count], "31"))
    token = 31;
  if (!strcmp(tokenarr[count], "32"))
    token = 32;
  if (!strcmp(tokenarr[count], "33"))
    token = 33;

    count++;
}

void emit(int op, int L, int M)
{
	text[cx].op = op; //opcode
	text[cx].L = L; // lexicographical level
	text[cx].M = M; // modifier
	cx++; //increment code index 
}

void printCrash(int code)
{
  // exit(0);
  FILE *ptr2 = fopen("elf.txt", "w");
  printf("Assembly Code:\n");
  printf("Line  OP    L   M\n");

  for (int i = 0; i < cx; i++)
  {
    changeOP(text[i].op, i);
    int enhancer  = 1;
    
    if (text[i].op == 7 || text[i].op == 8)
    {
      enhancer = 3;
    }
    
    printf("%d     %s    %d   %d\n", (i+1), opChange, text[i].L, (enhancer * text[i].M));
    fprintf(ptr2, "%d %d %d\n", text[i].op, text[i].L, (enhancer * text[i].M));
  } 

  switch(code)
  {
    case 1:
      printf("***** Error number 1 program must end with period\n");
      fclose(ptr2);
      exit(0);
      break;

    case 2:
      printf("***** Error number 2 const, var, and read keywords must be followed by identifier\n");
      fclose(ptr2);
      exit(0);
      break;

    case 3:
      printf("***** Error number 3 symbol name has already been declared\n");
      fclose(ptr2);
      exit(0);
      break;

    case 4:
      printf("***** Error number 4 constants must be assigned with =\n");
      fclose(ptr2);
      exit(0);
      break;

    case 5:
      printf("***** Error number 5 constants must be assigned an integer value\n");
      fclose(ptr2);
      exit(0);
      break;

    case 6:
      printf("***** Error number 6 constant and variable declarations must be followed by a semicolon\n");
      fclose(ptr2);
      exit(0);
      break;

    case 7:
      printf("***** Error number 7 undeclared identifier\n");
      fclose(ptr2);
      exit(0);
      break;

    case 8:
      printf("***** Error number 8 only variable values may be altered\n");
      fclose(ptr2);
      exit(0); 
      break;

    case 9:
      printf("***** Error number 9 assignment statements must use :=\n");
      fclose(ptr2);
      exit(0);
      break;

    case 10:
      printf("***** Error number 10 begin must be followed by end\n");
      fclose(ptr2);
      exit(0);
      break;

    case 11:
      printf("***** Error number 11 if must be followed by then\n");
      fclose(ptr2);
      exit(0);
      break;

    case 12:
      printf("***** Error number 12 while must be followed by do\n");
      fclose(ptr2);
      exit(0);
      break;

    case 13:
      printf("***** Error number 13 condition must contain comparison operator\n");
      fclose(ptr2);
      exit(0);
      break;

    case 14:
      printf("***** Error number 14 right parenthesis must follow left parenthesis\n");
      fclose(ptr2);
      exit(0);
      break;
    
    case 15:
      printf("***** Error number 15 arithmetic equations must contain operands, parentheses, numbers, or symbols\n");
      fclose(ptr2);
      exit(0);
      break;
    
    case 16:
      printf("***** Error number 16 call must be followed by an identifer\n");
      fclose(ptr2);
      exit(0);
      break;
    
    case 17:
      printf("***** Error number 17 const, var, procedure must be followed by identifier\n");
      fclose(ptr2);
      exit(0);
      break;

    case 18:
      printf("***** Error number 18 Expression must not contain a procedure identifier\n");
      fclose(ptr2);
      exit(0);
      break;
    case 19:
      printf("***** Error number 19 Call of a constant or variable is meaningless\n");
      fclose(ptr2);
      exit(0);
      break;
    case 20:
      printf("***** Error number 20 Semicolon expected\n");
      fclose(ptr2);
      exit(0);
      break;
    case 21:
      printf("***** Error number 21 Level exceeded maxLevel\n");
      fclose(ptr2);
      exit(0);
      break;

  }
}

int symbolTableCheck(char* str)
{
  for (int i = tableSize - 1; i >= 0; i--)
  {
    if (!(strcmp(name, table[i].name)) && varArrayLevels >= table[i].level)
      return i;
  }
  return -1;
}

// check for level
int symbTableDupCheck(char* str)
{
  for (int i = 0; i < tableSize; i++)
  {
    if (!(strcmp(name, table[i].name)) && varArrayLevels == table[i].level)
      return 0;
  }

  return -1;
}

void symTableAdd(int kind, char* name, int val, int level, int mark, int addy)
{
  table[tableSize].kind = kind;
  table[tableSize].val = val;
  strcpy(table[tableSize].name, name);
  table[tableSize].level = varArrayLevels;
  table[tableSize].mark = 0;
  table[tableSize].addr = addy;

  tableSize++;
}	

void changeOP(int code, int i)
{
  switch(code)
    {
      case 1:
        strcpy(opChange, "LIT");
        break;
      case 2:
        switch(text[i].M)
          {
            case 0:
              strcpy(opChange, "RTN");
              break;
            case 1:
              strcpy(opChange, "ADD");
              break;
            case 2:
              strcpy(opChange, "SUB");
              break;
            case 3:
              strcpy(opChange, "MUL");
              break;
            case 4:
              strcpy(opChange, "DIV");
              break;
            case 5:
              strcpy(opChange, "EQL");
              break;
            case 6:
              strcpy(opChange, "NEQ");
              break;
            case 7:
              strcpy(opChange, "LSS");
              break;
            case 8:
              strcpy(opChange, "LEQ");
              break;
            case 9:
              strcpy(opChange, "GTR");
              break;
            case 10:
              strcpy(opChange, "GEQ");
              break;
            case 11:
              strcpy(opChange, "ODD");
              break;
          }

        break;
      case 3:
        strcpy(opChange, "LOD");
        break;
      case 4:
        strcpy(opChange, "STO");
        break;
      case 5:
        strcpy(opChange, "CAL");
        break;
      case 6:
        strcpy(opChange, "INC");
        break;
      case 7:
        strcpy(opChange, "JMP");
        break;
      case 8:
        strcpy(opChange, "JPC");
        break;
      case 9:
        strcpy(opChange, "SYS");
        break;
    }
}

int errorHandler()
{
  while ((strcmp(tokenarr[count], "18")) == 1 && (strcmp(tokenarr[count], "22")) == 1)
  {
    count++;
  }
  
  if (strcmp(tokenarr[count], "18"))
    return 1;


  return 0;
}  
