#include "queue.hpp"
#include <stdint.h>
#include <memory>
#include <iostream>

namespace gagarin
{
  using std::ostream;

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

  void
  queue::print (ostream& stream) const
  {
     queue_member_ptr ptr = m_head;
     stream << "[";
     while (ptr)
       {
         stream << ptr->value();
         ptr = ptr->next();
         stream << (ptr ? "," : "");
       }
     stream << "]";
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

