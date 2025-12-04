#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "analyzer.h" // Link to our header file

// --- GLOBAL VARIABLES ---
// Actual memory allocation happens here
char toxicList[MAX_TOXIC_WORDS][MAX_WORD_LEN];
int toxicListCount = 0;

WordData wordList[MAX_WORDS]; // Array of structs to store analysis data
int uniqueWordCount = 0;      // Counter for how many unique words we found

// --- HELPER FUNCTIONS (Internal logic) ---

// Helper: Remove punctuation and make everything lowercase
// Example: "Stupid!" becomes "stupid"
void cleanWord(char *source, char *dest) {
    int j = 0;
    for (int i = 0; source[i] != '\0'; i++) {
        // We only want letters (A-Z) and numbers, ignore symbols
        if (isalnum(source[i])) { 
            dest[j] = tolower(source[i]); // Force lowercase for consistency
            j++;
        }
    }
    dest[j] = '\0'; // Important: Terminate the string properly
}

// Helper: Linear search to check if a word is in our toxic dictionary
int checkIfToxic(char *targetWord) {
    for (int i = 0; i < toxicListCount; i++) {
        // strcmp returns 0 if strings are identical
        if (strcmp(targetWord, toxicList[i]) == 0) {
            return 1; // Found it! It's toxic.
        }
    }
    return 0; // Not found, safe word.
}

// --- MAIN FUNCTIONS ---

// Load the dictionary file into memory array
void loadToxicWords() {
    FILE *fptr = fopen("toxicwords.txt", "r");
    
    // Basic error handling if file is missing
    if (fptr == NULL) {
        printf("Error: Could not find 'toxicwords.txt'. Please create it first.\n");
        return;
    }
    
    toxicListCount = 0;
    char tempWord[MAX_WORD_LEN];

    // Read word by word. 
    // We check (fscanf == 1) to make sure we actually read data (avoids duplicate bugs)
    while (toxicListCount < MAX_TOXIC_WORDS && fscanf(fptr, "%s", tempWord) == 1) {
        // Clean the dictionary word just in case user typed uppercase
        cleanWord(tempWord, toxicList[toxicListCount]);
        toxicListCount++;
    }
    
    fclose(fptr); // Always close file to save resources
    
    // Feedback to user
    if (toxicListCount == 0) {
        printf("Warning: Dictionary file is empty!\n");
    } else {
        printf("System: Toxic dictionary loaded successfully (%d words).\n", toxicListCount);
    }
}

// The core logic: Read input file and populate our struct array
void processCommentFile(char *filename) {
    FILE *cfPtr = fopen(filename, "r");
    char rawWord[100];
    char cleanToken[100];

    if (cfPtr == NULL) {
        printf("Error: Could not open input file '%s'. check the name again.\n", filename);
        return;
    }

    printf("Processing file... please wait.\n");

    // Loop through the file until End of File (EOF)
    while (fscanf(cfPtr, "%s", rawWord) != EOF) {
        
        // 1. Clean the word first
        cleanWord(rawWord, cleanToken);

        // 2. Skip if the word became empty (e.g. it was just "!!!")
        if (strlen(cleanToken) == 0) continue; 

        // 3. Search: Have we seen this word before?
        int foundIndex = -1;
        for (int i = 0; i < uniqueWordCount; i++) {
            if (strcmp(wordList[i].word, cleanToken) == 0) {
                foundIndex = i; // Yes, remember its position
                break;
            }
        }

        // 4. Update or Add Logic
        if (foundIndex != -1) {
            // Case A: Word exists, just increase frequency
            wordList[foundIndex].frequency++;
        } else {
            // Case B: New word, add to list
            if (uniqueWordCount < MAX_WORDS) {
                strcpy(wordList[uniqueWordCount].word, cleanToken);
                wordList[uniqueWordCount].frequency = 1;
                // Check toxicity immediately upon adding
                wordList[uniqueWordCount].isToxic = checkIfToxic(cleanToken);
                uniqueWordCount++;
            } else {
                // Safety: Stop if array is full
                printf("Warning: Word limit reached! Some words ignored.\n");
                break;
            }
        }
    }
    fclose(cfPtr);
    printf("Done! %d unique words processed.\n", uniqueWordCount);
}

// Algorithm: Bubble Sort
// We sort descending so the most frequent words come first (Index 0)
void sortWordsByFrequency() {
    WordData temp; // Temporary struct for swapping
    
    for (int i = 0; i < uniqueWordCount - 1; i++) {
        for (int j = 0; j < uniqueWordCount - 1 - i; j++) {
            
            // Compare frequency of adjacent items
            if (wordList[j + 1].frequency > wordList[j].frequency) {
                // Swap the whole struct, not just the number
                temp = wordList[j];
                wordList[j] = wordList[j + 1];
                wordList[j + 1] = temp;
            }
        }
    }
}

// Calculate and print summary stats
void displayStatistics() {
    int totalWords = 0;
    int toxicCount = 0;

    // Loop through our struct array to sum up totals
    for (int i = 0; i < uniqueWordCount; i++) {
        totalWords += wordList[i].frequency;
        
        if (wordList[i].isToxic) {
            toxicCount += wordList[i].frequency;
        }
    }

    printf("\n--- GENERAL STATISTICS ---\n");
    printf("Total Unique Words: %d\n", uniqueWordCount);
    printf("Total Word Count: %d\n", totalWords);
    printf("Total Toxic Occurrences: %d\n", toxicCount);
    
    // Prevent division by zero error
    if (totalWords > 0) {
        float toxicityRate = (float)toxicCount / totalWords * 100;
        printf("Toxicity Rate: %.2f%%\n", toxicityRate);
    }
}

// Display top N words from the sorted list
void displayTopWords(int n) {
    printf("\n--- TOP %d FREQUENT WORDS ---\n", n);
    printf("%-20s | %-10s | %-10s\n", "Word", "Frequency", "Status");
    printf("------------------------------------------------\n");

    // Loop up to N, but make sure we don't go out of bounds
    for (int i = 0; i < n && i < uniqueWordCount; i++) {
        char *status = (wordList[i].isToxic) ? "TOXIC" : "CLEAN";
        
        // Formatting: %-20s aligns text to the left
        printf("%-20s | %-10d | %-10s\n", 
               wordList[i].word, 
               wordList[i].frequency, 
               status);
    }
}

// Display only toxic words from the list
void displayTopToxicWords(int n) {
    printf("\n--- TOP %d TOXIC WORDS ---\n", n);
    printf("%-20s | %-10s\n", "Word", "Frequency");
    printf("--------------------------------\n");

    int count = 0;
    for (int i = 0; i < uniqueWordCount; i++) {
        // Filter: Only print if it is flagged as toxic
        if (wordList[i].isToxic) {
            printf("%-20s | %-10d\n", wordList[i].word, wordList[i].frequency);
            count++;
            
            // Stop once we showed N words
            if (count == n) break;
        }
    }
    
    if (count == 0) printf("Good news! No toxic words found.\n");
}

// Export the results to a text file for reporting
void saveReportToFile(char *outputFilename) {
    FILE *fptr = fopen(outputFilename, "w"); // 'w' mode overwrites old file
    
    if (fptr == NULL) {
        printf("Error: Could not create output file.\n");
        return;
    }

    fprintf(fptr, "CYBERBULLYING ANALYSIS REPORT\n=============================\n\n");
    
    // Recalculate stats for the report
    int totalWords = 0, toxicCount = 0;
    for (int i = 0; i < uniqueWordCount; i++) {
        totalWords += wordList[i].frequency;
        if (wordList[i].isToxic) toxicCount += wordList[i].frequency;
    }
    
    // Write stats
    fprintf(fptr, "Total Unique Words: %d\n", uniqueWordCount);
    fprintf(fptr, "Total Word Count: %d\n", totalWords);
    fprintf(fptr, "Total Toxic Words: %d\n", toxicCount);
    
    if (totalWords > 0)
        fprintf(fptr, "Toxicity Level: %.2f%%\n\n", (float)toxicCount/totalWords * 100);

    // Write list
    fprintf(fptr, "--- TOP 20 WORDS LIST ---\n");
    for (int i = 0; i < 20 && i < uniqueWordCount; i++) {
        char *status = (wordList[i].isToxic) ? "TOXIC" : "CLEAN";
        fprintf(fptr, "%-20s : %d (%s)\n", wordList[i].word, wordList[i].frequency, status);
    }

    fclose(fptr);
    printf("Success! Report saved to '%s'.\n", outputFilename);
}