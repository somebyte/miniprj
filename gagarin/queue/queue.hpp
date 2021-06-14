/*
  Author: somebyte
*/

#ifndef __GAGARIN_QUEUE_HPP__
#define __GAGARIN_QUEUE_HPP__

#include <cstdint>
#include <climits>
#include <iostream>

namespace gagarin
{
  class queue;
  class queue_member;

  typedef queue        * queue_ptr;
  typedef queue_member * queue_member_ptr;

  class queue
  {
      queue (const queue&);
      queue& operator= (const queue&);

    public:
      explicit queue (uint32_t limit = UINT_MAX)
        :m_limit(limit), m_size (0), m_head (nullptr), m_last (nullptr)
        {}
     ~queue ();

      void     push  (int32_t i);
      int32_t  pop   ();
      uint32_t size  () const { return m_size; };
      void     print (std::ostream& stream = std::cout) const;
      void     clean ();

    private:
      uint32_t         m_limit;
      uint32_t         m_size;
      queue_member_ptr m_head;
      queue_member_ptr m_last;
  };

  class queue_member
  {
    friend void queue::push (int32_t);

    queue_member (const queue_member&);
    queue_member& operator= (const queue_member&);

    explicit queue_member (int value) : m_value (value), m_next (nullptr) {}

    public:
            ~queue_member () {} 

    int32_t value () const
      { return m_value; }

    queue_member_ptr next ()
      { return m_next; }

    void next (queue_member_ptr ptr)
      { m_next = ptr; }

    private:
      int32_t          m_value;
      queue_member_ptr m_next;
  };
}

#endif

