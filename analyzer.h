#ifndef ANALYZER_H
#define ANALYZER_H

#include <stdio.h>

// --- SETTINGS / CONSTANTS ---
// We set limits here so it's easy to change later if needed
#define MAX_WORDS 1500       // Max unique words we can track
#define MAX_WORD_LEN 50      // Max characters per word
#define MAX_TOXIC_WORDS 300  // Dictionary limit

// --- DATA STRUCTURE ---
// We use a struct to keep everything related to a word in one place.
// This makes sorting much easier compared to using parallel arrays.
typedef struct {
    char word[MAX_WORD_LEN]; // The actual word string
    int frequency;           // How many times it appears
    int isToxic;             // Flag: 1 = Bad word, 0 = Clean word
} WordData;

// --- GLOBAL VARIABLES ---
// Using 'extern' so these variables can be shared with analyzer.c
extern char toxicList[MAX_TOXIC_WORDS][MAX_WORD_LEN];
extern int toxicListCount;
extern WordData wordList[MAX_WORDS];
extern int uniqueWordCount;

// --- FUNCTION PROTOTYPES ---
// List of available functions for main.c to use
void loadToxicWords();
void processCommentFile(char *filename);
void sortWordsByFrequency();
void displayStatistics();
void displayTopWords(int n);
void displayTopToxicWords(int n);
void saveReportToFile(char *outputFilename);

#endif