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
const size_t NMAX = 10001;
size_t N = NMAX;
uint32_t n = 0;

void calc_progression (uint32_t id, ostream& out);
void print (ostream& out, uint32_t id, uint32_t i, uint32_t value);
void reset();

uint32_t GetNext();

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
                  cout << "N = 10001" << endl;
                  return 1;
                }
            }
        }
    }

  reset();
  thread t1 ([](){ calc_progression(0, fout?fout:cout); });
  thread t2 ([](){ calc_progression(1, fout?fout:cout); });
  t1.join();
  t2.join();

  if (fout)
    fout.close();
}

uint32_t
GetNext ()
{
  calc_mtx.lock();
  uint32_t r = n++;
  calc_mtx.unlock();
  return 2*r*r+3*r+5;
}

void
reset()
{
  lock_guard<mutex> lock (calc_mtx);
  n = 0;
}

void
calc_progression(uint32_t id, ostream& out)
{
  while (n < N)
    print (out, id, n, GetNext());
}

void
print (ostream& out, uint32_t id, uint32_t i, uint32_t value)
{
  lock_guard<mutex> lock (print_mtx);
  out << "thread" << id << ": for n=" << i-1 << " P=" << value << endl;
}

