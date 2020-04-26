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
    int i = 0;
    MorseTranslation_t morsechar;
    MorseTranslation_t* translated_string = ret;
//    MorseUnit_e (*MorseWord)[MAX_MORSE_UNITS];
    MorseUnit_e* Morseword;

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

        morsechar = MorseLookup[letter_idx];

        translated_string[i] = morsechar;

        i++;

    }

    return i;
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
        index = NUM_MORSE_CHARS - 1;    // Last entry in thee lookup table
    }

    return index;
}
