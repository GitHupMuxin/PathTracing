#include <assert.h>
#include <iostream>
#include "engine/core/memory/span.h"

namespace core
{
    namespace memory
    {
        Span::Span() : pageID_(0), n_(0), previousSpan_(nullptr), nextSpan_(nullptr), freeList_(nullptr), usedCount_(0), used_(false) { }

        void Span::Show() const noexcept
        {
            std::cout << "pageID :" << this->pageID_ << " usedCount :" << this->usedCount_ << " page count :" << this->n_
                    << " used in central cache :" << (this->used_ ? "true" : "false") << std::endl;
            std::cout << "memory :\n";
            int i = 1;
            void* temp = this->freeList_;
            while (temp)
            {
                std::cout << i++ << ". " << temp << "\n";
                temp = MemoryUtils::GetNextObject(temp);
            }
            std::cout << std::endl;
        }


        SpanList::SpanList()
        {
            this->head_ = new Span;
            this->head_->previousSpan_ = this->head_;
            this->head_->nextSpan_ = this->head_;
        }


        void SpanList::PushFront(Span* span)
        {
            Span* next = this->head_->nextSpan_;
            this->head_->nextSpan_ = span;
            span->previousSpan_ = this->head_;
            span->nextSpan_ = next;
            next->previousSpan_ = span;    
        }

        Span* SpanList::PopFront()
        {
            Span* result = this->head_->nextSpan_;
            this->Erase(result);
            
            return result;
        }

        void SpanList::Insert(Span* pos, Span* ptr)
        {
            assert(pos);
            assert(ptr);
            
            Span* node = pos->previousSpan_;

            node->nextSpan_ = ptr;
            ptr->previousSpan_  = node;

            ptr->nextSpan_ = pos;
            pos->previousSpan_ = ptr;
        }

        void SpanList::Erase(Span* pos)
        {
            assert(pos);
            assert(this->head_ != pos);

            Span* next = pos->nextSpan_;
            Span* previous = pos->previousSpan_;

            previous->nextSpan_ = next;
            next->previousSpan_ = previous;
        }

        Span* SpanList::Begin()
        {
            return this->head_->nextSpan_;
        }        

        Span* SpanList::End()
        {
            return this->head_;
        }

        bool SpanList::Empty()
        {
            return this->head_->nextSpan_ == this->head_;
        }

        void SpanList::Show() const noexcept
        {
            std::cout << "Show SpanList \n";
            Span* temp = this->head_->nextSpan_;
            int i = 1;
            while (temp)
            {
                if (this->head_ == temp) return;
                std::cout << "Span "<< i++ <<". :";      
                temp->Show();    
                temp = temp->nextSpan_;
            }
            std::cout << std::endl;
        }

        SpanList::~SpanList()
        {
            delete(this->head_);
        }
     
    }
}


