/*
 *  Main file
 *  Written by Curtis Li
 *  Written by Jonata Pastro
 */

#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <argp.h>

#include "hexdiff.h"


/**************************************************
 *                                                *
 *    Functions                                   *
 *                                                *
 **************************************************/

using namespace std;


static struct arguments arguments;

const char *argp_program_version = "Hexdiff v0.2";
const char *argp_program_bug_address = "jonata.pastro@aluno.unb.br";

void parseCli(struct arguments *arguments, int argc, char **argv) {
  argp_parse(&argp, argc, argv, 0, 0, arguments);
}

error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *arguments =  (struct arguments *) state->input;
  switch (key) { 
    case ARGP_KEY_ARG:
      if (!arguments->arg1) {
        arguments->arg1 = arg;
      } else if (!arguments->arg2) {
        arguments->arg2 = arg;
      }
      break;
    case ARGP_KEY_END:
      if (state->arg_num < 2) {
	argp_usage(state);
      }
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}	


/*
 *  Main function
 */
int main(int argc, char** argv) {
  ifstream f1, f2;

  // Parse the CLI
  parseCli(&arguments, argc, argv);

  // Open the files
  f1.open(arguments.arg1, ios::in | ios::binary);
  f2.open(arguments.arg2, ios::in | ios::binary);

  // Solve for the Greatest Common Subsequence (GCS)
  vector<vector<int> > lookup = vector<vector<int> >(getSize(&f1), vector<int>(getSize(&f2)));;
  gcs(&lookup, &f1, &f2);
  
  // Reset the streams and get the GCS result
  f1.clear();
  f1.seekg(0, f1.beg);
  f2.clear();
  f2.seekg(0, f2.beg);
  gcsSol(&lookup, &f1, &f2);
  

  // Clsoe the files
  f1.close();
  f2.close();
}


/*
 *  Get the size of a file opened by the stream
 *  Will reset the stream position to the original state
 */
size_t getSize(ifstream *str) {
  int pos = str->tellg();
  str->seekg(0, str->end);
  size_t size = str->tellg();
  str->seekg(pos);
  return size;
}


/*
 *  Solve for the greatest common subsequence given two streams
 *  Stores the result in a lookup/memoization table
 */
int gcs(vector< vector<int> > *lookup, ifstream *f1, ifstream *f2) {
  // Get iteration case
  int row = f1->tellg();
  int col = f2->tellg();

  // Base case
  char byte1, byte2;
  if (!f1->get(byte1) || !f2->get(byte2)) {
    return 0;
  }

  // Fetch memoized results
  assert(row < lookup->size() && col < (*lookup)[row].size());
  if ((*lookup)[row][col] != 0) {
    return (*lookup)[row][col];
  }

  // Match
  if (byte1 == byte2) {
    (*lookup)[row][col] = 1 + gcs(lookup, f1, f2);
    return (*lookup)[row][col];
  }

  // Branch and return max
  f1->clear();
  f1->seekg(row, f1->beg);
  f2->clear();
  f2->seekg(col+1, f2->beg);
  int a = gcs(lookup, f1, f2);

  f1->clear();
  f1->seekg(row+1, f1->beg);
  f2->clear();
  f2->seekg(col, f2->beg);
  int b = gcs(lookup, f1, f2);

  // Memoize result
  (*lookup)[row][col] = a > b ? a : b;
  return a > b ? a : b;
}


/*
 *  Retrieve the GCS solution from a lookup/memoization table
 */
void gcsSol(vector< vector<int> > *lookup, ifstream *f1, ifstream *f2) {
  // Get iteration case
  int row = f1->tellg();
  int col = f2->tellg();
  char byte1, byte2;

  if (!f1->get(byte1) || !f2->get(byte2)) {
    cout << "ERROR" << endl;
    return;
  }

  // Match
  if (byte1 == byte2) { 
    fmtPrint(WHITE, byte2, "== ");
    return gcsSol(lookup, f1, f2);
  }
  
  // Bounds checking if we hit the base case
  bool rowAtBase = row + 1 >= lookup->size();
  bool colAtBase = col + 1 >= (*lookup)[row].size();
  if (rowAtBase || colAtBase) {
    if (rowAtBase && colAtBase) {
      cout << lookup->size() << " " << (*lookup)[row].size() << ": ";
      cout << "BASE" << endl;
      return;
    } else if (rowAtBase) {
      fmtPrint(RED, byte2, "BASE RIGHT");
      f1->clear();
      f1->seekg(row);
    } else if (byte2) {
      fmtPrint(GREEN, byte1, "BASE DOWN");
      f2->clear();
      f2->seekg(col);
    }
    return gcsSol(lookup, f1, f2);
  }
  
  // Step case
  if ((*lookup)[row+1][col] > (*lookup)[row][col+1]) {
    // Go down
    fmtPrint(RED, byte1, "-- ");
    f2->seekg(col);
  } else {
    // Go right
    fmtPrint(GREEN, byte2, "++ ");
    f1->seekg(row);
  }
  return gcsSol(lookup, f1, f2);
}

void fmtPrint(const char *colour, char c, string operation) {
  cout << colour << operation << hex(c) << " " << c << END << endl;
}
