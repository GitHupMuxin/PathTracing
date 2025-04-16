namespace core
{
    template<typename T>
    ThreadSafeDequeue<T>::ThreadSafeDequeue() = default;

    template<typename T>
    ThreadSafeDequeue<T>::~ThreadSafeDequeue() = default;

    template<typename T>
    bool ThreadSafeDequeue<T>::Empty() const noexcept
    {
        std::lock_guard gl(this->lock_);
        return this->safeDequeue_.empty();
    }

    template<typename T>
    size_t ThreadSafeDequeue<T>::Size() const noexcept
    {
        std::lock_guard gl(this->lock_);
        return this->safeDequeue_.size();   
    }

    template<typename T>
    void ThreadSafeDequeue<T>::PushFront(T& t)
    {
        std::lock_guard gl(this->lock_);
        this->safeDequeue_.push_front(t);
    }

    template<typename T>
    void ThreadSafeDequeue<T>::PushFront(T&& t)
    {
        std::lock_guard gl(this->lock_);
        this->safeDequeue_.push_front(std::move(t));
    }

    template<typename T>
    void ThreadSafeDequeue<T>::PushBack(T& t)
    {
        std::lock_guard gl(this->lock_);
        this->safeDequeue_.push_back(t);
    }

    template<typename T>
    void ThreadSafeDequeue<T>::PushBack(T&& t)
    {
        std::lock_guard gl(this->lock_);
        this->safeDequeue_.push_back(std::move(t));
    }

    template<typename T>
    bool ThreadSafeDequeue<T>::PopFront(T& t)
    {
        std::lock_guard gl(this->lock_);
        if (this->safeDequeue_.empty())        
            return false;   
        t = std::move(this->safeDequeue_.front());
        this->safeDequeue_.pop_front();
        return true;
    }

    template<typename T>
    bool ThreadSafeDequeue<T>::PopBack(T& t)
    {
        std::lock_guard gl(this->lock_);
        if (this->safeDequeue_.empty())
            return false;
        t = std::move(this->safeDequeue_.back());
        this->safeDequeue_.pop_back();
        return true;
    }


    template<typename T>
    ThreadSafeQueue<T>::ThreadSafeQueue() = default;

    template<typename T>
    ThreadSafeQueue<T>::~ThreadSafeQueue() = default;

    template<typename T>
    bool ThreadSafeQueue<T>::Empty() const noexcept
    {
        return this->queue.Empty();
    }

    template<typename T>
    size_t ThreadSafeQueue<T>::Size() const noexcept
    {
        return this->queue.Size();
    }

    template<typename T>
    void ThreadSafeQueue<T>::Push(T& t) 
    {
        this->queue.PushBack(t);
    }

    template<typename T>
    void ThreadSafeQueue<T>::Push(T&& t)
    {
        this->queue.PushBack(std::move(t));
    }
     
    template<typename T>    
    bool ThreadSafeQueue<T>::Pop(T& t) 
    {
        return this->queue.PopFront(t);
    }
    

}

