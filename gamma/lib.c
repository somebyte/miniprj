#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define NGROUPS    10                         // всего 10 групп символов
#define SEQLENGTH  (NGROUPS*2 + NGROUPS - 1)  // в каждой группе 2 символа + разделитель групп символ '-'
#define SSEQLENGTH (SEQLENGTH + 1)            // размер строки вместе с '\0'

static char __s_sequence[SSEQLENGTH] = "\0"; 
char *sequence = __s_sequence;
const unsigned int max_sequence_length = SSEQLENGTH;

static const char  __separator_ch = '-';
static const char* __forbidden_chars = "DFGJMQV";
static const char* __error = "sequence error";
static char*  __sequence = NULL;
static size_t __n = 0;


char* gamma_init_sequence()
{
	sequence[0] = '\0';
	return sequence;
}

int gamma_parse_sequence ()
{
	__sequence = NULL;    
	__n = strlen(sequence);
	
	if (SEQLENGTH < __n) {
	    __n = SEQLENGTH;
            sequence[__n] = 0;
	}
	    
	if (!__n)
	    return -1;
	    
	size_t err = 0;
	for(size_t i = 3; i < __n + 3; i++) {
		
		size_t id = i%3;
		char *p = &sequence[i - 3];
		
		*p = toupper(*p);
		switch (id) {
			case 0:
				if (!isalpha(*p))
					++err;
				if (strchr(__forbidden_chars, *p))
					++err;
			break;
			
			case 1:
				if (!isdigit(*p))
				    ++err;
				if (*p == '0')
				    ++err;
			break;
			
			case 2:
				if(*p != __separator_ch)
					++err;
			break;
		}
	}
	
	if (!err)
		__sequence = sequence;
 
    return err;
}

const char* gamma_inc_sequence ()
{
	if (!__sequence)
	    return __error;
	    	    
	char *x = NULL, *y = NULL;
	size_t n = --__n;
	while (n)
	{
	    y = __sequence + n;
	    x = y - 1;
	
	    ++*y;
	    if (*y > '9') {
		    *y = '1';
		    ++*x;
		    while(strchr(__forbidden_chars, *x))
		        ++*x;
        }
    
        if ((*x > 'Z' && *x < 'a') || (*x > 'z'))  {
		    *x = 'A';
		    if ((n == 1) && (strlen(__sequence) <= SEQLENGTH-3)) {
				strcat(__sequence, "-A1");
				break;
			}
			n -= 3;
	    }
	    else
			break;
	}
    
	__n = strlen(sequence);
	return __sequence;
}
