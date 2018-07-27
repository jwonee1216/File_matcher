#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define OFFSET 30

#define MARK_O ' '
#define MARK_X 'X'

#define FILE_A_PATH argv[1]
#define FILE_B_PATH argv[2]
#define ASSAY_PATH  ( (argc > 3) ? argv[3] : "result.txt" )

#define MAX_NUM(num1, num2) ( (num1 > num2) ? num1 : num2 )
#define MIN_NUM(num1, num2) ( (num1 < num2) ? num1 : num2 )


ssize_t get_file_size(FILE *fp);


int main(int argc, char **argv)
{
    FILE *fileA = NULL;
    FILE *fileB = NULL;
    FILE *assay = NULL;
    
    char fileA_str[OFFSET] = "";
    char fileB_str[OFFSET] = "";
    char assay_str[OFFSET] = "";

    ssize_t fileA_size = -1;
    ssize_t fileB_size = -1;
    ssize_t match_size = -1;

    ssize_t cur_size = -1;


    if(argc < 3) {
        fprintf(stderr, "Useage : %s [file A] [file B] {assay name}\n", argv[0]);
        exit(-1);
    }

    fileA = fopen(FILE_A_PATH, "rb");
    fileB = fopen(FILE_B_PATH, "rb");
    assay = fopen(ASSAY_PATH, "wb");

    fileA_size = get_file_size(fileA);
    fileB_size = get_file_size(fileB);
    match_size = MAX_NUM(fileA_size, fileB_size); 

    for(cur_size = 1; cur_size <= match_size; cur_size += OFFSET) {
        int i = -1;
        int read_sizeA = -1;
        int read_sizeB = -1;

        
        fprintf(assay, "[%ld ~ %ld]\n", cur_size, MIN_NUM(cur_size + OFFSET, match_size) );

        read_sizeA = fread(fileA_str, 1, OFFSET, fileA);
        read_sizeB = fread(fileB_str, 1, OFFSET, fileB);

        for(i = 0; i < read_sizeA; i++) {
            fprintf(assay, "%02X ", fileA_str[i]);
        }
        fputc('\n', assay);

        for(i = 0; i < read_sizeB; i++) {
            fprintf(assay, "%02X ", fileB_str[i]);
        }
        fputc('\n', assay);

        for(i = 0; i < OFFSET; i++) {
            if(read_sizeA <= i && read_sizeB <= i)
                break;
            
            if(read_sizeA > i && read_sizeB > i) {
                (fileA_str[i] == fileB_str[i]) ?
                    fprintf(assay, " %c ", MARK_O):
                    fprintf(assay, " %c ", MARK_X);
            }
            else {
                fprintf(assay, " %c ", MARK_X);
            }
        }
        fputc('\n', assay);
        
    }

    fclose(assay);
    fclose(fileB);
    fclose(fileA);

    return 0;
}


ssize_t get_file_size(FILE *fp)
{
    ssize_t now = -1;
    ssize_t fsize = -1;

    now = ftell(fp);
    if(now != -1) {
        fseek(fp, 0, SEEK_END);
        fsize = ftell(fp);

        fseek(fp, now, SEEK_SET);
    }

    return fsize;
}
