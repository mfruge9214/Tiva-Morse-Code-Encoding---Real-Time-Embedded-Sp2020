/*
 * File: Morse.h
 * Author: Mike Fruge
 */


#define UPPER_CASE_SHIFT    32
#define MAX_MORSE_UNITS     6
#define NUM_MORSE_CHARS     37
#define MAX_INPUT_LENGTH    50

typedef enum {
    DOT = 1,
    DASH = 2,
    END = 3,
    SPACE = 4
} MorseUnit_e;

typedef struct {
    MorseUnit_e MorseWord[MAX_MORSE_UNITS];
} MorseTranslation_t;

static MorseTranslation_t MorseLookup[NUM_MORSE_CHARS] = {
                                                       {DOT, DASH, END},                // A
                                                       {DASH, DOT, DOT, DOT, END},      // B
                                                       {DASH, DOT, DASH, DOT, END},     // C
                                                       {DASH, DOT, DOT, END},           // D
                                                       {DOT, END},                      // E
                                                       {DOT, DOT, DASH, DOT, END},      // F
                                                       {DASH, DASH, DOT, END},          // G
                                                       {DOT, DOT, DOT, DOT, END},       // H
                                                       {DOT, DOT, END},                 // I
                                                       {DOT, DASH, DASH, DASH, END},    // J
                                                       {DASH, DOT, DASH, END},          // K
                                                       {DOT, DASH, DOT, DOT, END},      // L
                                                       {DASH, DASH, END},               // M
                                                       {DASH, DOT, END},                // N
                                                       {DASH, DASH, DASH, DASH, END},   // O
                                                       {DOT, DASH, DASH, DOT, END},     // P
                                                       {DASH, DASH, DOT, DASH, END},    // Q
                                                       {DOT, DASH, DOT, END},           // R
                                                       {DOT, DOT, DOT, END},            // S
                                                       {DASH, END},                     // T
                                                       {DOT, DOT, DASH, END},           // U
                                                       {DOT, DOT, DOT, DASH, DASH, END}, // V
                                                       {DOT, DASH, DASH, END},           // W
                                                       {DASH, DOT, DOT, DASH, END},          // X
                                                       {DASH, DOT, DASH, DASH, END},         // Y
                                                       {DASH, DASH, DOT, DOT, END},          // Z
                                                       {DASH, DASH, DASH, DASH, DASH, END},  // 0
                                                       {DOT, DASH, DASH, DASH, DASH, END},   // 1
                                                       {DOT, DOT, DASH, DASH, DASH, END},    // 2
                                                       {DOT, DOT, DOT, DASH, DASH, END},     // 3
                                                       {DOT, DOT, DOT, DOT, DASH, END},      // 4
                                                       {DOT, DOT, DOT, DOT, DOT, END},       // 5
                                                       {DASH, DOT, DOT, DOT, DOT, END},      // 6
                                                       {DASH, DASH, DOT, DOT, DOT, END},     // 7
                                                       {DASH, DASH, DASH, DOT, DOT, END},    // 8
                                                       {DASH, DASH, DASH, DASH, DOT, END},    // 9
                                                       {SPACE, END}                                // Space character

};




int DecodeString(char* str, MorseTranslation_t* ret);





