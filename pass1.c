#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Function declarations
void passOne(char label[10], char opcode[10], char operand[10], char code[10], char mnemonic[3]);
void display();

int main() {
    // For reading input file
    char label[10], opcode[10], operand[10];
    // For reading optab file
    char code[10], mnemonic[3];

    // Call Pass One of the assembler
    passOne(label, opcode, operand, code, mnemonic);

    return 0;
}

/* 
   PASS ONE FUNCTION:
   - Reads input assembly program
   - Generates symbol table (SYMTAB)
   - Generates intermediate file with LOCCTR
   - Calculates program length
*/
void passOne(char label[10], char opcode[10], char operand[10], char code[10], char mnemonic[3]) {
    int locctr, start, length;

    // File pointers
    FILE *fp1, *fp2, *fp3, *fp4, *fp5;

    // Open necessary files
    fp1 = fopen("input.txt", "r");        // Input source code
    fp2 = fopen("optab.txt", "r");        // Opcode table
    fp3 = fopen("symtab.txt", "w");       // Symbol table
    fp4 = fopen("intermediate.txt", "w"); // Intermediate file
    fp5 = fopen("length.txt", "w");       // Program length

    // Read first line from input
    fscanf(fp1, "%s\t%s\t%s", label, opcode, operand);

    // If START directive present
    if (strcmp(opcode, "START") == 0) {
        start = atoi(operand);           // Convert operand to integer (starting address)
        locctr = start;                  // Initialize LOCCTR
        // Write first line to intermediate file
        fprintf(fp4, "\t%s\t%s\t%s\n", label, opcode, operand);
        // Read next line
        fscanf(fp1, "%s\t%s\t%s", label, opcode, operand);
    } else {
        locctr = 0; // Default starting address
    }

    // Process each line until END is found
    while (strcmp(opcode, "END") != 0) {
        // 1. Write line to intermediate file with LOCCTR
        fprintf(fp4, "%d\t%s\t%s\t%s\n", locctr, label, opcode, operand);

        // 2. If label is not '', add to symbol table
        if (strcmp(label, "") != 0) {
            fprintf(fp3, "%s\t%d\n", label, locctr);
        }

        // 3. Search opcode in OPTAB
        fscanf(fp2, "%s\t%s", code, mnemonic);
        while (strcmp(code, "END") != 0) {
            if (strcmp(opcode, code) == 0) {
                locctr += 3; // Each machine instruction = 3 bytes
                break;
            }
            fscanf(fp2, "%s\t%s", code, mnemonic); // Read next entry in OPTAB
        }

        // 4. Handle assembler directives
        if (strcmp(opcode, "WORD") == 0) {
            locctr += 3; // WORD = 3 bytes
        }
        else if (strcmp(opcode, "RESW") == 0) {
            locctr += (3 * atoi(operand)); // Reserve n words = 3*n bytes
        }
        else if (strcmp(opcode, "RESB") == 0) {
            locctr += atoi(operand); // Reserve n bytes
        }
        else if (strcmp(opcode, "BYTE") == 0) {
            // Handle BYTE constants
            int len = strlen(operand);
            if (operand[0] == 'C' || operand[0] == 'c') {
                // Format: C'EOF' → length = 3
                len = len - 3;
            } else {
                // Format: X'F1' → 2 hex digits = 1 byte
                len = (len - 3) / 2;
            }
            locctr += len;
        }

        // 5. Read next line from input
        fscanf(fp1, "%s\t%s\t%s", label, opcode, operand);
    }

    // Write last line (END) to intermediate file
    fprintf(fp4, "%d\t%s\t%s\t%s\n", locctr, label, opcode, operand);

    // Close files
    fclose(fp4);
    fclose(fp3);
    fclose(fp2);
    fclose(fp1);

    // Calculate program length
    length = locctr - start;
    fprintf(fp5, "%d", length);
    fclose(fp5);

    printf("\nThe length of the code : %d\n", length);

    // Display all tables
    display();
}

/* 
   DISPLAY FUNCTION:
   - Prints contents of input file
   - Prints contents of intermediate file
   - Prints contents of SYMTAB
*/
void display() {
    char str;
    FILE *fp1, *fp2, *fp3;

    // Input Table
    printf("\nThe contents of Input Table :\n\n");
    fp1 = fopen("input.txt", "r");
    str = fgetc(fp1);
    while (str != EOF) {
        printf("%c", str);
        str = fgetc(fp1);
    }
    fclose(fp1);

    // Intermediate Table
    printf("\n\nThe contents of Output (Intermediate) Table :\n\n");
    fp2 = fopen("intermediate.txt", "r");
    str = fgetc(fp2);
    while (str != EOF) {
        printf("%c", str);
        str = fgetc(fp2);
    }
    fclose(fp2);

    // Symbol Table
    printf("\n\nThe contents of Symbol Table :\n\n");
    fp3 = fopen("symtab.txt", "r");
    str = fgetc(fp3);
    while (str != EOF) {
        printf("%c", str);
        str = fgetc(fp3);
    }
    fclose(fp3);
}




/*input.txt


**	START	2000
**	LDA	FIVE
**	STA	ALPHA
** 	LDCH 	CHARZ
** 	STCH 	C1
ALPHA 	RESW 	2
FIVE 	WORD 	5
CHARZ 	BYTE 	C'Z'
C1 	RESB 	1
** 	END	**


optab.txt




LDA 03
STA	0f
LDCH	53
STCH	57
END	*

*/
