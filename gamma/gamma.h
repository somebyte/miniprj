#ifndef __GAMMA_H___
#define __GAMMA_H___

extern const unsigned int max_sequence_length;

/* Функция возвращается указатель на пустую строку для заполнения. */
char*       gamma_init_sequence  ();

/* Функция проверяет, что строка заполнена корректно.
 * Возвращает:
 *  0 если проверка успешно завершена;
 * -1 если исходная строка пустая;
 * >0 число синтаксических ошибок.
 */
int         gamma_parse_sequence ();

/* Функция увеличивает исходную строку на 1 и возвращает её указатель */
const char* gamma_inc_sequence   ();

#endif
