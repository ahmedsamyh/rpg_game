#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define STRING_VIEW_IMPLEMENTATION
#include <string_view.h>

typedef struct {
  unsigned char* data;
  size_t size;
} Data;

#define defer(ret_val) \
  result = ret_val;\
  goto defer


bool is_char_valid_var_name(char ch){
  return ('a' <= ch && ch <= 'z' ||
	  'A' <= ch && ch <= 'Z' ||
	  ch == '_'
	  );
}

Data* slurp_file(const char* filename){
  FILE* f = fopen(filename, "rb");
  Data* result = (Data*)malloc(sizeof(Data));

  if (f == NULL){
    fprintf(stderr, "ERROR: slurp_file::fopen(\"%s\", \"rb\") -> %s\n", filename, strerror(errno));
    defer(NULL);
  }

  if (fseek(f, 0, SEEK_END) < 0) {
    fprintf(stderr, "ERROR: slurp_file::fseek(f, 0, SEEK_END) -> %s\n", filename, strerror(errno));
    defer(NULL);
  }

  int fsize = ftell(f);

  if (fsize == -1){
    fprintf(stderr, "ERROR: slurp_file::ftell(f) -> %s\n", filename, strerror(errno));
    defer(NULL);
  }

  result->size = (size_t)fsize;

  result->data = (char*)malloc(sizeof(char)*fsize);

  if (result == NULL){
    fprintf(stderr, "ERROR: slurp_file::malloc(%zu) -> %s\n", sizeof(char)*fsize, strerror(errno));
    defer(NULL);
  }

  if (fseek(f, 0, SEEK_SET) < 0) {
    fprintf(stderr, "ERROR: slurp_file::fseek(f, 0, SEEK_SET) -> %s\n", filename, strerror(errno));
    defer(NULL);
  }

  if (fread(result->data, sizeof(char), result->size, f) != result->size){
    fprintf(stderr, "ERROR: slurp_file::fread(result, %d, 1, f) -> %s\n", result->size, strerror(errno));
    defer(NULL);
  }

 defer:
  fclose(f);
  return result;
}

bool write_data_to_cfile(Data* data, const char* _out){
  char buff[1024] = {0};
  snprintf(buff, 1024, "%s.c", _out);
  const char* filename = buff;
  FILE* f = fopen(filename, "wb");
  bool result = true;
  char* out = (char*)malloc(sizeof(char)*strlen(_out));

  memcpy(out, _out, strlen(_out));

  if (f == NULL){
    fprintf(stderr, "ERROR: write_data_to_cfile::fopen(\"%s\", \"wb\") -> %s\n", filename, strerror(errno));
    /* fclose(f); */
    return false;
    /* defer(false); */
  }

  // todo: normalize out (transform to a valid c variable name)
  for (size_t i = 0; i < strlen(out); ++i){
    if (!is_char_valid_var_name(out[i])) {
      printf("Invalid char `%c` replaced with `_`\n", out[i]);
      out[i] = '_';
    }
  }

  fprintf(f, "// file created from `embed` program.\n");
  fprintf(f, "\n");
  fprintf(f, "#define %s_size (%zu)\n", out, data->size);
  fprintf(f, "static unsigned char %s_bytes[%s_size] = {\n", out, out);

  int per_line = 20;
  int n = 0;
  for (int i = 0; i < data->size; ++i){
    fprintf(f, "0x%X, ", (uint8_t)data->data[i]);
    if (++n >= per_line){
      n = 0;
      fprintf(f, "\n");
    }
  }

  fprintf(f, "};\n");

  printf("INFO: Created file `%s`\n", filename);

  free(out);
  return result;
}

const char* shift_args(int* argc, char*** argv){
  const char* result = NULL;
  if (*argc <= 0) return NULL;

  result = *(*argv);

  *argc = (*argc) - 1;
  *argv = (*argv) + 1;

  return result;
}

int main(int argc, char** argv){
  const char* program = shift_args(&argc, &argv);

  if (argc <= 0){
    fprintf(stderr, "Usage: <file>\n");
    fprintf(stderr, "Error: No input file is provided!\n");
    exit(1);
  }

  const char* filename = shift_args(&argc, &argv);

  Data* data = slurp_file(filename);

  if (data == NULL) return 1;

  String_view sv = sv_from_cstr(filename);

  sv_rremove_until_char_after(&sv, '.');

  if (!write_data_to_cfile(data, sv_to_cstr(sv))) return 1;

  fflush(stdout);

  return 0;
}
