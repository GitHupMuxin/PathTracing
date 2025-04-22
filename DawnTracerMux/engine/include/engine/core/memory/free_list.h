#pragma once

namespace core
{
    namespace memory
    {
        class ThreadCache;

        class FreeList
        {
            private:
                void*           freeList_;
                size_t          upperBound_;
                size_t          freeSize_;

            public:
                FreeList();    
                size_t          Size() noexcept;
                void            Push(void* object) noexcept;
                void            PushRange(void* star, void* end, size_t count) noexcept;
                void*           Pop() noexcept;
                void            PopRange(void* & star, void* & end, size_t count) noexcept;
                size_t          GetUpperBound_() const noexcept;
                void            SetUpperBound_(const size_t& number) noexcept;
                bool            IsEmpty() noexcept;
                void            Show() const noexcept;

                friend class ThreadCache;
        };
    }   
}
