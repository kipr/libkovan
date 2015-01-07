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

static bool isLower(const char c)
{
  return (c >= 'a' && c <= 'z');
}

static bool isUpper(const char c)
{
  return (c >= 'A' && c <= 'Z');
}

static bool isAlpha(const char c)
{
  return isLower(c) || isUpper(c);
}

static bool isNumber(const char c)
{
  return c >= '0' && c <= '9';
}

static bool isId(const char c)
{
  return isAlpha(c) || isNumber(c) || c == '_';
}

static string extractArgs(const string &signature)
{
  if(signature.size() <= 2) return "()";
  
  stringstream ret;
  ret << "(";
  for(size_t i = 1; i < signature.size(); ++i)
  {
    if(signature[i] != ',' && signature[i] != ')') continue;
    size_t j = i - 1;
    for(; j > 0; --j)
    {
      if(!isId(signature[j])) break;
    }
    // Error?
    if(!j) return "()";
    
    ret << signature.substr(j + 1, i - j);
  }
  
  return ret.str();
}

static inline bool isDefine(const string token)
{
  bool success = true;
  for(size_t i = 0; i < token.size(); ++i) success &= isUpper(token[i]) || token[i] == '_';
  return success;
}

static inline vector<string> extractTokens(const string &area)
{
  vector<string> ret;
  size_t begin = 0;
  size_t i = 0;
  for(; i < area.size(); ++i)
  {
    if(area[i] != ' ') continue;
    size_t old = begin;
    begin = i + 1;
    if(i - old < 1) continue;
    ret.push_back(area.substr(old, i - old));
    
  }
  if(i - begin > 0) ret.push_back(area.substr(begin, i - begin));
  return ret;
}

static inline string extractReturnType(const string &area)
{
  stringstream str;
  vector<string> toks = extractTokens(area);
  for(vector<string>::const_iterator it = toks.begin(); it != toks.end(); ++it) {
    if(isDefine(*it)) continue;
    str << *it << " ";
  }
  const string ret = str.str();
  return ret.substr(0, ret.size() - 1);
}

static bool processFile(const char *const file, ostream &out)
{
  char lineBuf[512];
  lineBuf[511] = 0;
  
  ifstream in(file);
  if(!in.is_open()) return false;
  
  vector<string> vfs;
  
  bool listingEmitted = false;
  bool err = false;
  
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
    
    
    
    if(line.substr(0, 5) == "VFL")
    {
      if(listingEmitted)
      {
        cerr << "Listing can't be emitted twice. Error on " << lineNum << endl;
        err = true;
        goto end;
      }
      
      listingEmitted = true;
      for(vector<string>::const_iterator it = vfs.begin(); it != vfs.end(); ++it)
      {
        out << *it << endl;
      }
      continue;
    }
    
    if(line.substr(0, 2) != "VF")
    {
      out << line << endl;
      continue;
    }
    
    if(listingEmitted)
    {
      cerr << "Listing can't be before a VF. Error on " << lineNum << endl;
      err = true;
      goto end;
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
      
    const string returnType = extractReturnType(line.substr(0, space_delim + 1));
    
    if(returnType.empty()) {
      cerr << "Unable to isolate return type. Skipping line " << lineNum << "." << endl;
      continue;
    }
    
    const size_t close_paren = line.find(')', open_paren + 1);
    if(close_paren == string::npos) {
      cerr << "Unable to isolate function signature. Skipping line " << lineNum << "." << endl;
      continue;
    }
    const string funcName   = line.substr(space_delim + 1, open_paren - space_delim - 1);
    const string signature  = line.substr(open_paren, close_paren - open_paren + 1);
    
    if(returnType == "void") {
      vfs.push_back("VTABLE_FUNC_VOID(" + funcName + ", " + signature
        + ", " + extractArgs(signature) + ");");
    } else {
      vfs.push_back("VTABLE_FUNC(" + funcName + ", " + returnType + ", "
        + signature + ", " + extractArgs(signature) + ");");
    }
    
    // out << "static " << line.substr(2, line.size() - 2) << endl;
  }
  
  
end:
  in.close();
  
  if(!listingEmitted && !vfs.empty())
  {
    cerr << "Never emitted the VF Listing." << endl;
    return false;
  }
  
  return !err;
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