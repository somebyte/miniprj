/*
  Author: somebyte
*/

#include "queue.hpp"
#include <iostream>

using namespace gagarin;
using std::cout;
using std::endl;

void info (const queue& q, uint32_t index);

int main (int argc, const char** argv)
{
  // testing push(...)/pop()	
  queue q(100);
  info (q, 0);
  q.push (0);
  info (q, 1);
  q.push (1000);
  info (q, 2);
  q.push (2000);
  info (q, 3);

  printf ("a = %d\n", q.pop());
  printf ("b = %d\n", q.pop());
  printf ("c = %d\n", q.pop());
  info (q, 4);

  // testing a limit
  for (size_t i = 0; i <= 1000; ++i)
  {
    q.push(i);
  }
  info (q, 5);

  // testing size() with pop() 
  while (q.size()) q.pop();
  info (q, 6);

  // testing re-filling the queue 
  for (size_t i = 0; i <= 1000; ++i)
  {
    q.push(i);
  }
  info (q, 7);

  // testing clean()
  q.clean();
  info (q, 8);

  return 0;
}

void info (const queue& q, uint32_t index)
{
  cout.fill  ('0');
  cout.width (2);
  cout << "size"  << cout.width() << index << " = " << q.size() << endl;
  cout << "queue" << cout.width() << index << " = " << q << endl; 
}

