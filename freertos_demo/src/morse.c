/*
 * File: Morse.c
 * Author: Mike Fruge
 *
 * Implements Morse Code API
 */
#include <stdint.h>


#include "morse.h"


/* Lookup table for encoding characters to Morse Code */
//static MorseCharStruct_t MorseLookup[36] = {
// {'A', 2, [DOT, DASH]}, {'B', 4, [DASH, DOT, DOT, DOT]}
//};


static int LookupIdx(char c);
/*
 * Decode String
 *
 * Function will parse the input string into character structures to
 */
int DecodeString(char* str, MorseTranslation_t* ret)
{
    char current;
    int start_idx = 0;
    int curr_idx = 0;
    int i = 0;
    MorseTranslation_t morsechar;
    MorseTranslation_t* translated_string = ret;
//    MorseUnit_e (*MorseWord)[MAX_MORSE_UNITS];
    MorseUnit_e* Morseword;

    /* Add the Start character to the beginning of the translation */
    morsechar = MorseLookup[NUM_MORSE_CHARS - 1];
    translated_string[start_idx] = morsechar;
    start_idx++;

    /* Check inputs and use lookup table */
    while(str[i] != 0 && i < MAX_INPUT_LENGTH)
    {
        current = str[i];

        // Shift lowercase letters to uppercase letters
        if(current > 'Z')
        {
            current = current - UPPER_CASE_SHIFT;
        }

        // If any values are outside of the bounds now, throw an error
        // Bounds are 0-9 and A-Z, these statements should encapsulate that
        if((current < 'A' && current > '9') || (current < '0' && current != ' ') || (current > 'Z'))
        {
            return -1;
        }

        /// Use lookup table to populate a structure and add it to the array
        int letter_idx = LookupIdx(current);

        if(letter_idx == -1) return -1;

        /* Save the character as its morse translation */
        morsechar = MorseLookup[letter_idx];

        /* Add to the return string */
        translated_string[i + start_idx] = morsechar;

        i++;

    }

    /* Return the length of the translated string */
    return i + start_idx;
}


/* Used to get encoding from lookup table using current character
 *  'A' - 'Z'  ->   0 - 25
 *  '1' - '9'  ->   26 - 34
 *  '0 ' -> 35
 */

static int LookupIdx(char c)
{
    int index = -1;
    if(c >= 'A' && c <= 'Z')        // Handles Alphabet
    {
        index = c - 'A';
    }

    else if(c >= '0' && c <= '9')   // Handles numbers
    {
        index = c - 22;
    }
    else if( c == ' ')      // Handles Space
    {
        index = NUM_MORSE_CHARS - 2;    // Second to last entry in the lookup table
    }

    return index;
}
