#include <stdio.h>
#include <stdlib.h>
#include "analyzer.h" // Import our custom library

int main() {
    int choice;
    char filename[100];
    int n;

    // 1. Important: Load the dictionary before doing anything else
    loadToxicWords();

    // 2. Main Menu Loop (keeps running until user picks Exit)
    do {
        printf("\n==========================================\n");
        printf("       CYBERBULLYING TEXT ANALYZER \n");
        printf("==========================================\n");
        printf("1. Load Comment File (Input)\n");
        printf("2. Display General Statistics\n");
        printf("3. Display Top Frequent Words\n");
        printf("4. Display Top Toxic Words\n");
        printf("5. Save Report to File\n");
        printf("6. Exit\n");
        printf("------------------------------------------\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter filename (e.g., comments.txt): ");
                scanf("%s", filename);
                
                // Reset counter so we don't mix data if loading a new file
                uniqueWordCount = 0; 
                processCommentFile(filename);
                
                // Auto-sort data right after loading so it's ready to display
                sortWordsByFrequency(); 
                break;

            case 2:
                displayStatistics();
                break;

            case 3:
                printf("How many words to display? ");
                scanf("%d", &n);
                displayTopWords(n);
                break;

            case 4:
                printf("How many toxic words to display? ");
                scanf("%d", &n);
                displayTopToxicWords(n);
                break;

            case 5:
                saveReportToFile("analysis_report.txt");
                break;

            case 6:
                printf("Thank you. Exiting program...\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 6);

    return 0;
}