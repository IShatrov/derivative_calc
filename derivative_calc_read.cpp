#include "derivative_calc.h"

#define SKIP_SPACES(text) while(isspace(*text)) text++
#define SKIP_NUM(text) while(isdigit(*text) || (*text) == '.') text++

size_t get_file_size(FILE *stream)
{
    assert(stream != NULL);

    size_t ans = 0;

    assert(!fseek(stream, 0, SEEK_END));   //fseek returns zero upon success and non-zero otherwise
    ans = ftell(stream);
    assert(!ferror(stream));

    assert(!fseek(stream, 0, SEEK_SET));

    return ans;
}

char* read_text(FILE *stream)
{
    assert(stream != NULL);

    size_t len = get_file_size(stream);
    assert(len != 0);

    char* text = (char*)calloc(len + 1, sizeof(char)); //+1 for null terminator
    assert(text != NULL);

    fread(text, sizeof(char), len, stream);
    assert(!ferror(stream));

    return text;
}

my_tree parse_expression(const char *src_filename)
{
    assert(src_filename);

    FILE *src = fopen(src_filename, "r");
    assert(src);

    const char *text = read_text(src);
    const char *ptr = text;

    SKIP_SPACES(ptr);

    my_tree tree = get_grammar(&ptr);

    fclose(src);
    free((char*)text);

    return tree;
}

#undef SKIP_SPACES
