#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <vector>

using namespace std;

static inline string &ltrim(string &s)
{
  s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(std::isspace))));
  return s;
}

static inline string &rtrim(string &s)
{
  s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(std::isspace))).base(), s.end());
  return s;
}

static inline string &trim(string &s) { return ltrim(rtrim(s)); }

static string vifyInclude(const string &line)
{
  string ret = line.substr(2, line.size() - 2);
  
  size_t op1 = ret.find('\"');
  if(op1 == string::npos) return line;
  size_t op2 = ret.find('\"', op1 + 1);
  if(op2 == string::npos) return line;
  
  size_t slash = ret.rfind('/', op2 - 1);
  if(slash != string::npos) ret.erase(op1 + 1, slash - op1);
  
  return ret;
}

static bool processFile(const char *const file, ostream &out)
{
  char lineBuf[512];
  lineBuf[511] = 0;
  
  ifstream in(file);
  if(!in.is_open()) return false;
  
  vector<string> vis;
  
  unsigned lineNum = 0;
  while(!in.eof()) {
    ++lineNum;
    stringstream lineStream;
    do {
      in.getline(lineBuf, sizeof lineBuf - 1);
      lineStream << lineBuf;
    } while(in.fail() && !in.eof());
    
    string line = lineStream.str();
    line = trim(line);
    
    if(line.size() < 2) {
      out << line << endl;
      continue;
    }
    
    if(line.substr(0, 2) == "VH") {
      out << vifyInclude(line) << endl;
      continue;
    } else if(line.substr(0, 2) != "VI") {
      out << line << endl;
      continue;
    }
    
    const size_t open_paren  = line.find_first_of('(');
    if(open_paren == string::npos) {
      cerr << "Unable to find opening function paren. Skipping line " << lineNum << "." << endl;
      continue;
    }
    size_t space_delim = line.rfind(' ', open_paren);
    if(space_delim == string::npos) {
      cerr << "Unable to isolate function name. Skipping line " << lineNum << "." << endl;
      continue;
    }
    
    if(line[space_delim + 1] == '*') ++space_delim;

    const string funcName = line.substr(space_delim + 1, open_paren - space_delim - 1);
    vis.push_back("VTABLE_SET_DEFAULT(" + funcName + ", vi_" + funcName + ");");
    
    line.replace(space_delim + 1, open_paren - space_delim - 1, "vi_" + funcName);
    out << "static " << line << endl;
  }
  
  out << endl << endl;
  for(vector<string>::const_iterator it = vis.begin(); it != vis.end(); ++it) {
    out << *it << endl;
  }
  
  
  in.close();
  return true;
}

int main(int argc, char *argv[])
{
  const char *const outputFile = argv[1];
  ofstream out(outputFile);
  if(!out.is_open()) return 1;
  
  out << "// This file is auto-generated. Please do not edit." << endl << endl;
  
  for(int i = 2; i < argc; ++i) {
    if(!processFile(argv[i], out)) return 1;
  }
  
  out.close();
  
  return 0;
}