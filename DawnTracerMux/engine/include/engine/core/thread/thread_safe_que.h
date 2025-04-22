#pragma once
#include <deque>
#include <mutex>

namespace core
{
    template <typename T>
    class ThreadSafeDequeue
    {
        private:
            std::deque<T>           safeDequeue_;
            mutable std::mutex      lock_;

        public:

            ThreadSafeDequeue();
            ~ThreadSafeDequeue();

            ThreadSafeDequeue(const ThreadSafeDequeue<T>& safeQueue) = delete;
            ThreadSafeDequeue(const ThreadSafeDequeue<T>&& safeQueue) = delete;            
            ThreadSafeDequeue<T>& operator = (const ThreadSafeDequeue<T>& safeQueue) = delete;
            ThreadSafeDequeue<T>& operator = (const ThreadSafeDequeue<T>&& safeQueue) = delete;           

            bool                    Empty() const noexcept;
            size_t                  Size() const noexcept;
            void                    PushFront(T& t);
            void                    PushFront(T&& t);
            void                    PushBack(T& t);
            void                    PushBack(T&& t);
            bool                    PopFront(T& t);
            bool                    PopBack(T& t);
    };


    template<typename T>
    class ThreadSafeQueue
    {
        private:
            ThreadSafeDequeue<T> queue;

        public:
            ThreadSafeQueue();
            ~ThreadSafeQueue();

            ThreadSafeQueue(const ThreadSafeQueue<T>& safeQueue) = delete;
            ThreadSafeQueue(const ThreadSafeQueue<T>&& safeQueue) = delete;
            
            ThreadSafeQueue<T>&     operator = (const ThreadSafeQueue<T>& safeQueue) = delete;
            ThreadSafeQueue<T>&     operator = (const ThreadSafeQueue<T>&& safeQueue) = delete;

            bool                    Empty() const noexcept;
            size_t                  Size() const noexcept;
            void                    Push(T& t);
            void                    Push(T&& t);
            bool                    Pop(T& t);
    };

}

#include "thread_safe_que_inl.hpp"
