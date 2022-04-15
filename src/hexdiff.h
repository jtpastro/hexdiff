
/*
 *  Header file for hexdiff.cpp
 */
#include <iostream>
#include <iomanip>
#include <vector>

struct HexCharStruct {
  unsigned char c;
  HexCharStruct(unsigned char _c) : c(_c) { }
};

inline std::ostream& operator<<(std::ostream& o, const HexCharStruct& hs) {
  return (o << std::hex << std::setw(2) << (int)hs.c << std::setfill('0'));
}

inline HexCharStruct hex(unsigned char _c) {
  return HexCharStruct(_c);
}

size_t getSize(std::ifstream *str);

int gcs(std::vector< std::vector<int> > *lookup, std::ifstream *f1, std::ifstream *f2);
void gcsSol(std::vector< std::vector<int> > *lookup, std::ifstream *f1, std::ifstream *f2);

void fmtPrint(const char *colour, char c, std::string operation);


/*
 *  Header file for colours
 */

#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define WHITE "\033[0m"
#define END "\033[0m"


extern const char *argp_program_version;
extern const char *argp_program_bug_address;
static char doc[] = "Get the diff of binary files.";
static char args_doc[] = "FILE1 FILE2";
static struct argp_option options[] = {
   { 0 }
};

struct arguments {
  char *arg1, *arg2;
  char **args;
};

void parseCli(struct arguments *arguments, int argc, char **argv);
error_t parse_opt(int key, char *arg, struct argp_state *state);
static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };

