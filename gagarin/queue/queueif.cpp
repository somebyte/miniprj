#include <cstdlib>
#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <fstream>

#include "queue.hpp"

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::getline;
using std::string;
using std::map;
using std::istringstream;
using std::ofstream;
using std::ifstream;
using std::fstream;
using gagarin::queue;

typedef enum
{
  QIF_SUCCESS = 0,
  QIF_ARGERR  = 1,
  QIF_NUMERR  = 2,
  QIF_CMDERR  = 3,
  QIF_FILEERR = 4,
  QIF_CONTERR = 5,
  QIF_AWRONG  = 6
} qif_t;

typedef void (*func_t)(int argc, const char** argv);
typedef map <std::string, func_t> strfunc_t;

strfunc_t cmdif;
string filename ("/tmp/queue");
size_t number = 0;
queue q[2];

void cmdif_init (void);
void help       (void);
void handle_err (qif_t, const std::string& s = "");
void read_queue (void);
void save_queue (void);

void pop   (int argc, const char** argv);
void push  (int argc, const char** argv);
void print (int argc, const char** argv);
void info  (int argc, const char** argv);
void bit   (int argc, const char** argv);

int main (int argc, const char** argv)
{
  string str;

  if (argc <= 1)
    {
      handle_err (QIF_ARGERR);
    }

  if (string(argv[1]) == "help")
    {
      help();
      handle_err (QIF_SUCCESS);
    }
 
  if (argc <= 2)
    {
      handle_err (QIF_ARGERR);
    }

  cmdif_init();

  istringstream ss(argv[1]);

  if (!(ss >> number) || (number > 1))
    {
      handle_err (QIF_NUMERR);
    }

  filename += argv[1];

  read_queue();
  
  strfunc_t::const_iterator p = cmdif.find (argv[2]);
  if (p == cmdif.end())
    {
      handle_err (QIF_CMDERR, argv[2]);
    }

  (p->second) (argc, argv);

  return QIF_SUCCESS;
}

void
cmdif_init (void)
{
  cmdif["pop"]   = pop;
  cmdif["push"]  = push;
  cmdif["print"] = print;
  cmdif["info"]  = info;
  cmdif["bit"]   = bit;
}

void
help (void)
{
  cout << "\nqueueif number cmd [args]\n" << endl;
  cout << "\tnumber: 0|1" << endl;
  cout << "\tcmd [args]:" << endl;
  cout << "\t  pop    - remove first element" << endl;
  cout << "\t  push X - add a new element X to the end"  << endl;
  cout << "\t  print  - print the contents of the queue" << endl;
  cout << "\t  info   - print the size and contents of the queue" << endl;
  cout << "\t  bit N  - print the elements of the queue with set bit N (N = 0 - 31)\n" << endl;
  cout << "NOTES: print & info can create queue if it doesn't exist\n" << endl;
}

void
handle_err (qif_t err, const string& str)
{
  switch (err)
   {
   case QIF_ARGERR:
     cerr << "too few arguments" << endl;
   break;
   case QIF_NUMERR:
     cerr << "queue number error" << endl;
   break;
   case QIF_CMDERR:
     cerr << "command " << "\"" << str << "\" not found" << endl;
   break;
   case QIF_FILEERR:
     cerr << "file \"" << str << "\" error" << endl;
   break;
   case QIF_CONTERR:
     cerr << "file \"" << str << "\" content error" << endl;
   break;
   case QIF_AWRONG:
     cerr << "argument \"" << str << "\" is wrong" << endl;
   break;
   default:
   break;
   }

   exit (err);   
}

void
read_queue()
{
  ifstream fin (filename);

  if (!fin)
    {
      ofstream fout (filename);
      if (!fout)
        {
          handle_err (QIF_FILEERR, filename);
        }
      fout << "[]" << endl;
      fout.close();
      return;
    }

  string str;

  getline (fin, str);

  size_t n, i;
  char symbols[3] = { '[', ']', ',' };

  for (i = 0; i < 2; ++i)
    {
      n = str.find (symbols[i]);
      if (n != string::npos)
        str.erase (n,1);
    }

  do
    {
      n = str.find (symbols[i]);
      if (n != string::npos)
        str.replace (n, 1, " ");
    }
  while (n != string::npos);

  if (str.size())
    {
      istringstream ss (str);
      while (!ss.eof())
        {
          int a;
          if (ss >> a)
            q[number].push (a);
          else
            handle_err (QIF_CONTERR, filename);
        }
    }  

  fin.close();
}

void
save_queue ()
{     
  ofstream fout (filename);
  if (!fout)
    {
      handle_err (QIF_FILEERR, filename);
    }

  q[number].print (fout);  

  fout.close();
}

void
pop (int argc, const char** argv)
{
  cout << q[number].pop () << endl;
  save_queue();
}

void
push (int argc, const char** argv)
{
  if (argc <= 3)
    {
      handle_err (QIF_ARGERR);
    }
  
  int32_t k;
  istringstream ss(argv[3]);
  
  if (!(ss >> k))
    {
      handle_err (QIF_AWRONG);
    }
  
  q[number].push (k);
  save_queue();
}

void
print (int argc, const char** argv)
{
  cout << "q" << " = ";
  q[number].print();
  cout << endl;
}

void
info (int argc, const char** argv)
{
  cout << "size" << " = " << q[number].size() << endl;
  print (argc, argv);
}

void
bit (int argc, const char** argv)
{
  if (argc <= 3)
    {
      handle_err (QIF_ARGERR);
    }
  
  uint32_t k;
  istringstream ss(argv[3]);
  
  if (!(ss >> k) || (k >= 32))
    {
      handle_err (QIF_AWRONG, argv[3]);
    }

  queue& qq = q[number];
  queue bitq;
  while (qq.size())
    {
      uint32_t el = (uint32_t)qq.pop();
      if (el & (uint32_t)(1 << k))
        {
          bitq.push (el);
        } 
    }
  cout << "q = ";
  bitq.print ();
  cout << endl;
  read_queue ();
}

