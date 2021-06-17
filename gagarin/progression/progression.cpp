/*
  Author: somebyte
*/

#include <thread>
#include <fstream>
#include <vector>
#include <mutex>
#include <iostream>
#include <sstream>

using std::ostream;
using std::ofstream;
using std::thread;
using std::mutex;
using std::lock_guard;
using std::endl;
using std::cout;
using std::istringstream;
using std::string;

ofstream fout;
mutex calc_mtx;
mutex print_mtx;
const size_t NMAX = 10000;
size_t N = NMAX;
uint32_t n;

void calc_progression (uint32_t id, ostream& out);
void print (ostream& out, uint32_t id, uint32_t forn, uint32_t value);
void reset();

uint32_t GetNext (uint32_t & i);

int main (int argc, const char** argv)
{
  if (argc > 1)
    {
      string s (argv[1]);
      
      if (s == "help")
        {
          cout << "\nprogression [filename] [N]" <<endl;
          cout << "\t filename - the result file\n";
          cout << "\t N        - number of iteration\n" << endl;
          return 0;
        }
      
      istringstream iss (s);
      if (!(iss >> N))
        {
          fout.open (s);
          N = NMAX;

          if (argc > 2)
            {
              iss.clear();
              iss.str (argv[2]);
              if (!(iss >> N))
                {
                  cout << "error: number of iterations is wrong" << endl;
                  cout << "N = " << NMAX << endl;
                  return 1;
                }
            }
        }
    }

  reset();
  thread t1 ([](){ calc_progression(0, fout.is_open()?fout:cout); });
  thread t2 ([](){ calc_progression(1, fout.is_open()?fout:cout); });
  t1.join();
  t2.join();

  if (fout)
    fout.close();
}

uint32_t
GetNext (uint32_t& i)
{
  calc_mtx.lock();
  i = n++;
  calc_mtx.unlock();
  return 2*i*i+3*i+5;
}

void
reset()
{
  lock_guard<mutex> lock (calc_mtx);
  n = 0;
}

void
calc_progression (uint32_t id, ostream& out)
{
  uint32_t forn = n, result;

  while (1)
    {
      result = GetNext (forn);
      if (forn <= N)
        print (out, id, forn, result);
      else
        break;
    }
}

void
print (ostream& out, uint32_t id, uint32_t forn, uint32_t value)
{
  lock_guard<mutex> lock (print_mtx);
  out << "thread" << id << ": for n=" << forn << " P=" << value << endl;
}

