/*
  Author: somebyte
*/

#include "queue.hpp"
#include <stdint.h>
#include <memory>
#include <string>
#include <iostream>
#include <sstream>

namespace gagarin
{
  using std::ostream;
  using std::string;
  using std::istringstream;

  void
  queue::push (int32_t i)
  {
    if (m_size >= m_limit)
      return;

    queue_member_ptr ptr = new (std::nothrow) queue_member(i);

    if (ptr)
      {
         if (m_head == nullptr && m_last == nullptr)
           {
             m_head = ptr;
             m_last = ptr;
           }
         else
           {
             m_last->next (ptr);
             m_last = ptr;
           }
         ++m_size;
      }
  }

  int32_t
  queue::pop ()
  {
    int32_t value = 0;
    
    if (m_head)
      {
        value  = m_head->value();
        queue_member_ptr ptr = m_head;
        m_head = ptr->next();
        delete ptr;
        --m_size;
      }
    else
      {
/* may be need someone exception */
      }

    if (m_head == nullptr)
      m_last = nullptr;

    return value;
  }

  ostream&
  operator<< (ostream& stream, const queue& q)
  {
     queue_member_ptr ptr = q.m_head;
     stream << "[";
     while (ptr)
       {
         stream << ptr->value();
         ptr = ptr->next();
         stream << (ptr ? "," : "");
       }
     stream << "]";

     return stream;
  }

  std::istream& operator>> (std::istream& stream, queue& q)
  {
    std::string str;

    getline (stream, str);

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
              q.push (a);
            else
             {
/* may be need someone exception */
               break;
             }
          }
      }

    return stream;
  }

  void
  queue::clean ()
  {
    if (m_head == nullptr)
      return;

    while (size())
      {
        pop();
      }
  }

  queue::~queue ()
  {
    clean();
  }
}

