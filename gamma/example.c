#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "gamma.h"

/* Пример использования библиотеки gamma.          
 * Приглашает ввести исходную последовательность и
 * и выводит на экран 10 последовательностей, где
 * каждый следующий увеличен на 1 относительно
 * предыдущего.
 */

int main(int argc, char** argv) {
	fprintf(stdout, "Use 'quit' to exit.\n");
	while(1) {
        char *sequence = gamma_init_sequence();
		fprintf(stdout, "\n# ");
        
        char *out = fgets(sequence, max_sequence_length, stdin);
        
        size_t last = strlen(sequence) - 1;
        if (sequence[last] == '\n')
            sequence[last] = 0;
        
        if (strcmp(sequence, "quit") == 0)
            break;
            
        if (gamma_parse_sequence() != 0) {
            fprintf(stderr, "The sequence '%s' is wrong!\n", sequence);
            continue;
        }
        
        fprintf(stdout, "Origin:\n   %s\n", sequence);
        for (size_t i = 0; i < 10; ++i)
            fprintf(stdout, "%lu: %s\n", i, gamma_inc_sequence());
    }

    return 0;
}
