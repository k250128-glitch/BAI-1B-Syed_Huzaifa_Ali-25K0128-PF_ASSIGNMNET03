#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define INITIAL_CAPACITY 4

char *readLine(void) {
    size_t buffer_size = 128;
    size_t length = 0;
    char *buffer = malloc(buffer_size);
    if (!buffer) {
        fprintf(stderr, "Memory allocation failed in readLine()\n");
        exit(EXIT_FAILURE);
    }

    int ch;
    while ((ch = getchar()) != EOF) {
        if (ch == '\n') break;
        buffer[length++] = (char)ch;

        if (length + 1 >= buffer_size) {
            size_t new_size = buffer_size * 2;
            char *temp_ptr = realloc(buffer, new_size);
            if (!temp_ptr) {
                free(buffer);
                fprintf(stderr, "Memory allocation failed while reading line\n");
                exit(EXIT_FAILURE);
            }
            buffer = temp_ptr;
            buffer_size = new_size;
        }
    }

    if (length == 0 && ch == EOF) {
        free(buffer);
        return NULL;
    }

    buffer[length] = '\0';

    char *result = realloc(buffer, length + 1);
    return (result ? result : buffer);
}

void ensureCapacity(char ***text_lines_ptr, int *line_capacity, int needed_size) {
    if (*line_capacity >= needed_size) return;

    int new_capacity = (*line_capacity == 0) ? INITIAL_CAPACITY : (*line_capacity * 2);
    while (new_capacity < needed_size) new_capacity *= 2;

    char **temp_ptr = realloc(*text_lines_ptr, new_capacity * sizeof(char *));
    if (!temp_ptr) {
        fprintf(stderr, "Memory allocation (realloc) failed in ensureCapacity(): %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    *text_lines_ptr = temp_ptr;
    *line_capacity = new_capacity;
}

void insertLine(char ***text_lines_ptr, int *line_count, int *line_capacity, int index, const char *input_text) {
    if (index < 0) index = 0;
    if (index > *line_count) index = *line_count;

    ensureCapacity(text_lines_ptr, line_capacity, (*line_count) + 1);

    if (index < *line_count) {
        memmove(&(*text_lines_ptr)[index + 1],
                &(*text_lines_ptr)[index],
                ((*line_count) - index) * sizeof(char *));
    }

    size_t text_length = strlen(input_text);
    char *new_line = malloc(text_length + 1);
    if (!new_line) {
        fprintf(stderr, "Memory allocation failed in insertLine()\n");
        exit(EXIT_FAILURE);
    }
    memcpy(new_line, input_text, text_length + 1);

    (*text_lines_ptr)[index] = new_line;
    (*line_count)++;
}

void deleteLine(char ***text_lines_ptr, int *line_count, int *line_capacity, int index) {
    if (*line_count <= 0) {
        fprintf(stderr, "Buffer is empty. Nothing to delete.\n");
        return;
    }
    if (index < 0 || index >= *line_count) {
        fprintf(stderr, "Invalid index %d for deleteLine (0..%d)\n", index, *line_count - 1);
        return;
    }

    free((*text_lines_ptr)[index]);

    if (index < *line_count - 1) {
        memmove(&(*text_lines_ptr)[index],
                &(*text_lines_ptr)[index + 1],
                ((*line_count) - index - 1) * sizeof(char *));
    }

    (*line_count)--;
}

void printAllLines(char **text_lines, int line_count) {
    if (line_count == 0) {
        printf("(Buffer is empty)\n");
        return;
    }

    for (int i = 0; i < line_count; ++i) {
        printf("%4d: %s\n", i + 1, text_lines[i]);
    }
}

void freeAll(char **text_lines, int line_count) {
    if (!text_lines) return;

    for (int i = 0; i < line_count; ++i) {
        free(text_lines[i]);
    }
    free(text_lines);
}

void shrinkToFit(char ***text_lines_ptr, int *line_capacity, int line_count) {
    if (line_count == 0) {
        free(*text_lines_ptr);
        *text_lines_ptr = NULL;
        *line_capacity = 0;
        return;
    }

    char **temp_ptr = realloc(*text_lines_ptr, line_count * sizeof(char *));
    if (!temp_ptr) {
        fprintf(stderr, "realloc failed in shrinkToFit: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    *text_lines_ptr = temp_ptr;
    *line_capacity = line_count;
}

int saveToFile(char **text_lines, int line_count, const char *file_name) {
    FILE *file_ptr = fopen(file_name, "w");
    if (!file_ptr) {
        fprintf(stderr, "Failed to open file for writing: %s\n", file_name);
        return -1;
    }

    for (int i = 0; i < line_count; ++i) {
        if (fprintf(file_ptr, "%s\n", text_lines[i]) < 0) {
            fprintf(stderr, "Write error while saving file\n");
            fclose(file_ptr);
            return -1;
        }
    }

    fclose(file_ptr);
    return 0;
}

int loadFromFile(char ***text_lines_ptr, int *line_count, int *line_capacity, const char *file_name) {
    FILE *file_ptr = fopen(file_name, "r");
    if (!file_ptr) {
        fprintf(stderr, "Failed to open file for reading: %s\n", file_name);
        return -1;
    }

    if (*text_lines_ptr) {
        for (int i = 0; i < *line_count; ++i) free((*text_lines_ptr)[i]);
        free(*text_lines_ptr);
    }

    *text_lines_ptr = NULL;
    *line_count = 0;

    char *line_buffer = NULL;
    size_t buffer_capacity = 0;
    size_t len = 0;
    int ch;

    while ((ch = fgetc(file_ptr)) != EOF) {
        if (ch == '\r') continue;

        if (len + 1 >= buffer_capacity) {
            size_t new_capacity = (buffer_capacity == 0) ? 128 : buffer_capacity * 2;
            char *temp_ptr = realloc(line_buffer, new_capacity);
            if (!temp_ptr) {
                free(line_buffer);
                fclose(file_ptr);
                fprintf(stderr, "Memory alloc failed while loading file\n");
                return -1;
            }
            line_buffer = temp_ptr;
            buffer_capacity = new_capacity;
        }

        if (ch == '\n') {
            line_buffer[len] = '\0';

            ensureCapacity(text_lines_ptr, line_capacity, (*line_count) + 1);

            char *stored_line = malloc(len + 1);
            if (!stored_line) {
                fprintf(stderr, "Memory allocation failed while loading file\n");
                free(line_buffer);
                fclose(file_ptr);
                return -1;
            }

            memcpy(stored_line, line_buffer, len + 1);
            (*text_lines_ptr)[*line_count] = stored_line;
            (*line_count)++;

            len = 0;
        } else {
            line_buffer[len++] = (char)ch;
        }
    }

    if (len > 0) {
        line_buffer[len] = '\0';

        ensureCapacity(text_lines_ptr, line_capacity, (*line_count) + 1);

        char *stored_line = malloc(len + 1);
        if (!stored_line) {
            fprintf(stderr, "Memory allocation failed while loading file (last line)\n");
            free(line_buffer);
            fclose(file_ptr);
            return -1;
        }

        memcpy(stored_line, line_buffer, len + 1);
        (*text_lines_ptr)[*line_count] = stored_line;
        (*line_count)++;
    }

    free(line_buffer);
    fclose(file_ptr);
    return 0;
}

int main(void) {
    char **text_lines = NULL;
    int line_count = 0;
    int line_capacity = 0;

    printf("Minimal Line-based Text Editor (dynamic)\n");
    printf("Commands:\n");
    printf("  a     : append a line\n");
    printf("  i idx : insert a line at position idx (1-based)\n");
    printf("  d idx : delete the line at position idx (1-based)\n");
    printf("  p     : print all lines\n");
    printf("  s fn  : save to file 'fn'\n");
    printf("  l fn  : load from file 'fn' (replaces buffer)\n");
    printf("  r     : shrinkToFit (reduce pointer-array memory)\n");
    printf("  q     : quit (frees memory)\n");

    while (1) {
        printf("\n> ");
        fflush(stdout);

        int command = getchar();
        if (command == EOF) break;

        while (command == ' ' || command == '\t' || command == '\n') {
            command = getchar();
            if (command == EOF) break;
        }
        if (command == EOF) break;

        if (command == 'a') {
            int flush_ch = getchar();
            while (flush_ch != '\n' && flush_ch != EOF) flush_ch = getchar();

            printf("Enter line to append:\n");
            char *input_line = readLine();
            if (!input_line) {
                printf("(No input)\n");
                continue;
            }

            insertLine(&text_lines, &line_count, &line_capacity, line_count, input_line);
            free(input_line);

            printf("Appended. Total lines: %d\n", line_count);
        }

        else if (command == 'i') {
            int position;
            if (scanf("%d", &position) != 1) {
                printf("Usage: i idx\n");
                int flush_ch;
                while ((flush_ch = getchar()) != '\n' && flush_ch != EOF);
                continue;
            }

            int flush_ch = getchar();
            while (flush_ch != '\n' && flush_ch != EOF) flush_ch = getchar();

            if (position < 1) {
                printf("Index must be >= 1\n");
                continue;
            }

            printf("Enter line to insert at %d:\n", position);
            char *input_line = readLine();

            if (!input_line) {
                printf("(No input)\n");
                continue;
            }

            insertLine(&text_lines, &line_count, &line_capacity, position - 1, input_line);
            free(input_line);

            printf("Inserted. Total lines: %d\n", line_count);
        }

        else if (command == 'd') {
            int position;
            if (scanf("%d", &position) != 1) {
                printf("Usage: d idx\n");
                int flush_ch;
                while ((flush_ch = getchar()) != '\n' && flush_ch != EOF);
                continue;
            }

            int flush_ch = getchar();
            while (flush_ch != '\n' && flush_ch != EOF) flush_ch = getchar();

            if (position < 1 || position > line_count) {
                printf("Invalid index. Valid 1..%d\n", line_count);
                continue;
            }

            deleteLine(&text_lines, &line_count, &line_capacity, position - 1);
            printf("Deleted. Total lines: %d\n", line_count);
             printf("CODE BY HUZAIFA");
        }

        else if (command == 'p') {
            int flush_ch = getchar();
            while (flush_ch != '\n' && flush_ch != EOF) flush_ch = getchar();

            printAllLines(text_lines, line_count);
        }

        else if (command == 's') {
            char file_name[1024];

            if (scanf("%1023s", file_name) != 1) {
                printf("Usage: s filename\n");
                printf("CODE BY HUZAIFA");
                int flush_ch;
                while ((flush_ch = getchar()) != '\n' && flush_ch != EOF);
                continue;
            }

            int flush_ch = getchar();
            while (flush_ch != '\n' && flush_ch != EOF) flush_ch = getchar();

            if (saveToFile(text_lines, line_count, file_name) == 0)
                printf("Saved %d lines to '%s'\n", line_count, file_name);
            else
                printf("Failed to save to '%s'\n", file_name);
        }

        else if (command == 'l') {
            char file_name[1024];

            if (scanf("%1023s", file_name) != 1) {
                printf("Usage: l filename\n");
                int flush_ch;
                while ((flush_ch = getchar()) != '\n' && flush_ch != EOF);
                continue;
            }

            int flush_ch = getchar();
            while (flush_ch != '\n' && flush_ch != EOF) flush_ch = getchar();

            if (loadFromFile(&text_lines, &line_count, &line_capacity, file_name) == 0)
                printf("Loaded %d lines from '%s'\n", line_count, file_name);
            else
                printf("Failed to load from '%s'\n", file_name);
        }

        else if (command == 'r') {
            int flush_ch = getchar();
            while (flush_ch != '\n' && flush_ch != EOF) flush_ch = getchar();

            shrinkToFit(&text_lines, &line_capacity, line_count);
            printf("ShrinkToFit complete. Capacity is now %d\n", line_capacity);
        }

        else if (command == 'q') {
            int flush_ch = getchar();
            while (flush_ch != '\n' && flush_ch != EOF) flush_ch = getchar();
            break;
        }
// CODE BY HUZIFA
        else {
            int flush_ch = getchar();
            while (flush_ch != '\n' && flush_ch != EOF) flush_ch = getchar();

            printf("Unknown command: '%c'\n", (char)command);
            printf("Valid commands: a, i idx, d idx, p, s fn, l fn, r, q\n");
        }
        printf("CODE BY HUZAIFA");
    }

    freeAll(text_lines, line_count);
    printf("Editor exited. Memory freed.\n");
    return 0;
}
