#include <assert.h>
#include <stddef.h>
#include <iostream>
#include "engine/core/memory/free_list.h"
#include "engine/core/memory/memory_utils.h"

namespace core
{
    namespace memory
    {
        FreeList::FreeList() : freeList_(nullptr), upperBound_(1), freeSize_(0) { }

        size_t FreeList::Size() noexcept
        {
            return this->freeSize_;
        }

        void FreeList::Push(void* object) noexcept
        {
            assert(object);
            void* temp = this->freeList_;
            MemoryUtils::GetNextObject(object) = this->freeList_;
            this->freeList_ = object;
            this->freeSize_++;
        }

        void FreeList::PushRange(void* star, void* end, size_t count) noexcept
        {
            MemoryUtils::GetNextObject(end) = this->freeList_;
            this->freeList_ = star;
            this->freeSize_ += count;
        }

        void* FreeList::Pop() noexcept
        {
            void* result = this->freeList_;
            this->freeList_ = MemoryUtils::GetNextObject(this->freeList_);
            this->freeSize_--;
            return result;
        }

        void FreeList::PopRange(void* & star, void* & end, size_t count) noexcept
        {
            assert(count <= this->Size());
            star = end = this->freeList_;
            for (int i = 0; i < count - 1; i++)
                end = MemoryUtils::GetNextObject(end);
            this->freeList_ = MemoryUtils::GetNextObject(end);
            MemoryUtils::GetNextObject(end) = nullptr;
            this->freeSize_ -= count;
        }

        size_t FreeList::GetUpperBound_() const noexcept
        {
            return this->upperBound_; 
        }

        void FreeList::SetUpperBound_(const size_t& number) noexcept
        {
            this->upperBound_ = number;
        }

        bool FreeList::IsEmpty() noexcept
        {
            return this->freeList_ == nullptr;
        }

        void FreeList::Show() const noexcept
        {            
            std::cout << "upperBund_ : " << this->upperBound_ << "freeSize : " << this->freeSize_ << std::endl; 
            std::cout << "memory lists: \n";
            void* temp = this->freeList_;
            int i = 1;
            while (temp)
            {
                std::cout << i++ <<  ". " << temp << "\n";
                temp = MemoryUtils::GetNextObject(temp);
            }
            std::cout << std::endl;
        }

    }

}


