/*
   Absolute Loader Program in C
   Reads an object program (H, T, E records)
   and prints loaded memory locations.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    FILE *fp;
    int i, j, staddr1;
    char line[100], name1[50], staddr[20];

    // --- Open object program file ---
    fp = fopen("ab.txt", "r");
    if (fp == NULL) {
        printf("Error: cannot open file\n");
        return 1;
    }

    // --- Read Header Record ---
    if (fscanf(fp, "%s", line) != 1) {
        printf("Error: empty file or invalid format.\n");
        fclose(fp);
        return 1;
    }

    // --- Extract program name ---
    for (i = 2, j = 0; line[i] != '^' && line[i] != '\0'; i++, j++)
        name1[j] = line[i];
    name1[j] = '\0';
    printf("Program name: %s\n\n", name1);
    printf("Address\tContents\n");
    printf("-------\t---------\n");

    // --- Process each record ---
    while (fscanf(fp, "%s", line) == 1) {
        if (line[0] == 'T') {
            // Extract starting address
            for (i = 2, j = 0; line[i] != '^' && line[i] != '\0'; i++, j++)
                staddr[j] = line[i];
            staddr[j] = '\0';
            staddr1 = atoi(staddr);

            // Skip length field
            while (line[i] != '^' && line[i] != '\0') i++;
            i++;

            // Print object codes
            while (line[i] != '\0') {
                if (line[i] != '^' && line[i + 1] != '\0') {
                    printf("%04d\t%c%c\n", staddr1, line[i], line[i + 1]);
                    staddr1++;
                    i += 2;
                } else {
                    i++;
                }
            }
        } 
        else if (line[0] == 'E') {
            printf("\nEnd Record Found.\n");
            break;
        }
    }

    fclose(fp);
    return 0;
}




/*ab.txt

H^PROG1^001000^00107A
T^001000^1E^141033^281030^300015^481039^3C1003^001036
T^00101E^15^4C0000^0C1039^0F2000
E^001000
