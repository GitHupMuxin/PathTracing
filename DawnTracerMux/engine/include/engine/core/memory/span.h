#pragma once
#include <stddef.h>
#include <mutex>
#include "memory_utils.h"

namespace core
{
    namespace memory
    {
        class Span
        {
            public:
                PageID  pageID_;
                size_t  n_;
                size_t  usedCount_;
                Span*   previousSpan_;
                Span*   nextSpan_;
                void*   freeList_;
                bool    used_;
                Span();
                void    Show() const noexcept;
        };

        class SpanList
        {
            private:
                Span*               head_;
            public: 
                mutable std::mutex  lock_;
                
                SpanList();
                ~SpanList();
                void    PushFront(Span* span);
                Span*   PopFront();
                void    Insert(Span* pos, Span* ptr);
                void    Erase(Span* pos);
                Span*   Begin();
                Span*   End();
                bool    Empty();
                void    Show() const noexcept;

        };

    }
}

