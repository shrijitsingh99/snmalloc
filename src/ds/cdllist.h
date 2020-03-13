#pragma once

#include <cstdint>
#include <type_traits>
#include "defines.h"

namespace snmalloc
{
  /**
   * Special class for cyclic doubly linked non-empty linked list
   *
   * This code assumes there is always one element in the list. The client
   * must ensure there is a sentinal element.
   **/
  class CDLLNode
  {
    CDLLNode* next;
    CDLLNode* prev;

  public:
    CDLLNode()
    {
      next = this;
      prev = this;
    }

    SNMALLOC_FAST_PATH bool is_empty()
    {
      return next == this;
    }

    SNMALLOC_FAST_PATH void remove()
    {
      SNMALLOC_ASSERT(!is_empty());
      debug_check();
      next->prev = prev;
      prev->next = next;
      next->debug_check();
      
#ifndef NDEBUG
      next = nullptr;
      prev = nullptr;
#endif
    }

    SNMALLOC_FAST_PATH CDLLNode* get_next()
    {
      return next;
    }

    SNMALLOC_FAST_PATH CDLLNode* get_prev()
    {
      return prev;
    }

    SNMALLOC_FAST_PATH void insert_next(CDLLNode* item)
    {
      debug_check();
      item->next = next;
      next->prev = item;
      item->prev = this;
      next = item;
      debug_check();
    }

    SNMALLOC_FAST_PATH void insert_prev(CDLLNode* item)
    {
      debug_check();
      item->prev = prev;
      prev->next = item;
      item->next = this;
      prev = item;
      debug_check();
    }

    void debug_check()
    {
#ifndef NDEBUG
      CDLLNode* item = this->next;
      CDLLNode* p = this;

      do
      {
        SNMALLOC_ASSERT(item->prev == p);
        p = item;
        item = item->next;
      } while (item != this);
#endif
    }
  };
} // namespace snmalloc