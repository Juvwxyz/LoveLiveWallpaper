#pragma once
#include <type_traits>

namespace LLEngine
{
    namespace Details
    {
        template <typename T>
        class ComPtrRef // T should be the ComPtr<T> or a derived type of it, not just the interface
        {
            using InterfaceType = typename T::InterfaceType;
        public:
            ComPtrRef(_In_opt_ T* ptr) noexcept
            {
                this->ptr_ = ptr;
            }

            // Conversion operators
            operator void** () const noexcept
            {
                return reinterpret_cast<void**>(ptr_->ReleaseAndGetAddressOf());
            }

            // This is our operator ComPtr<U> (or the latest derived class from ComPtr (e.g. WeakRef))
            operator T* () noexcept
            {
                ptr_ = nullptr;
                return ptr_;
            }

            // We define operator InterfaceType**() here instead of on ComPtrRefBase<T>, since
            // if InterfaceType is IUnknown or IInspectable, having it on the base will collide.
            operator InterfaceType** () noexcept
            {
                return ptr_->ReleaseAndGetAddressOf();
            }

            // This is used for IID_PPV_ARGS in order to do __uuidof(**(ppType)).
            // It does not need to clear  ptr_ at this point, it is done at IID_PPV_ARGS_Helper(ComPtrRef&) later in this file.
            InterfaceType* operator *() noexcept
            {
                return ptr_->Get();
            }

            // Explicit functions
            InterfaceType* const* GetAddressOf() const noexcept
            {
                return ptr_->GetAddressOf();
            }

            InterfaceType** ReleaseAndGetAddressOf() noexcept
            {
                return ptr_->ReleaseAndGetAddressOf();
            }

        protected:
            T* ptr_;
        };
    }

    template <typename T>
    class ComPtr
    {
    public:
        typedef T InterfaceType;

    protected:
        InterfaceType* ptr_;
        template<class U> friend class ComPtr;

        void InternalAddRef() const noexcept
        {
            if (ptr_ != nullptr)
            {
                ptr_->AddRef();
            }
        }

        unsigned long InternalRelease() noexcept
        {
            unsigned long ref = 0;
            T* temp = ptr_;

            if (temp != nullptr)
            {
                ptr_ = nullptr;
                ref = temp->Release();
            }

            return ref;
        }

    public:
#pragma region constructors
        ComPtr() noexcept : ptr_(nullptr)
        {
        }

        ComPtr(nullptr_t) noexcept : ptr_(nullptr)
        {
        }

        template<class U>
        ComPtr(_In_opt_ U* other) noexcept : ptr_(other)
        {
            InternalAddRef();
        }

        ComPtr(const ComPtr& other) noexcept : ptr_(other.ptr_)
        {
            InternalAddRef();
        }

        // copy constructor that allows to instantiate class when U* is convertible to T*
        template<class U>
        ComPtr(const ComPtr<U>& other, typename ::std::enable_if<::std::is_convertible<U*, T*>::value, void*>::type* = 0) noexcept :
            ptr_(other.ptr_)
        {
            InternalAddRef();
        }

        ComPtr(_Inout_ ComPtr&& other) noexcept : ptr_(nullptr)
        {
            if (this != reinterpret_cast<ComPtr*>(&reinterpret_cast<unsigned char&>(other)))
            {
                Swap(other);
            }
        }

        // Move constructor that allows instantiation of a class when U* is convertible to T*
        template<class U>
        ComPtr(_Inout_ ComPtr<U>&& other, typename ::std::enable_if<::std::is_convertible<U*, T*>::value, void*>::type* = 0) noexcept :
            ptr_(other.ptr_)
        {
            other.ptr_ = nullptr;
        }
#pragma endregion

#pragma region destructor
        ~ComPtr() noexcept
        {
            InternalRelease();
        }
#pragma endregion

#pragma region assignment
        ComPtr& operator=(nullptr_t) noexcept
        {
            InternalRelease();
            return *this;
        }

        ComPtr& operator=(_In_opt_ T* other) noexcept
        {
            if (ptr_ != other)
            {
                ComPtr(other).Swap(*this);
            }
            return *this;
        }

        template <typename U>
        ComPtr& operator=(_In_opt_ U* other) noexcept
        {
            ComPtr(other).Swap(*this);
            return *this;
        }

        ComPtr& operator=(const ComPtr& other) noexcept
        {
            if (ptr_ != other.ptr_)
            {
                ComPtr(other).Swap(*this);
            }
            return *this;
        }

        template<class U>
        ComPtr& operator=(const ComPtr<U>& other) noexcept
        {
            ComPtr(other).Swap(*this);
            return *this;
        }

        ComPtr& operator=(_Inout_ ComPtr&& other) noexcept
        {
            ComPtr(static_cast<ComPtr&&>(other)).Swap(*this);
            return *this;
        }

        template<class U>
        ComPtr& operator=(_Inout_ ComPtr<U>&& other) noexcept
        {
            ComPtr(static_cast<ComPtr<U>&&>(other)).Swap(*this);
            return *this;
        }
#pragma endregion

#pragma region modifiers
        void Swap(_Inout_ ComPtr&& r) noexcept
        {
            T* tmp = ptr_;
            ptr_ = r.ptr_;
            r.ptr_ = tmp;
        }

        void Swap(_Inout_ ComPtr& r) noexcept
        {
            T* tmp = ptr_;
            ptr_ = r.ptr_;
            r.ptr_ = tmp;
        }
#pragma endregion

        operator bool() const noexcept
        {
            return Get() != nullptr;
        }

        T* Get() const noexcept
        {
            return ptr_;
        }

        InterfaceType* operator->() const noexcept
        {
            return ptr_;
        }

        Details::ComPtrRef<ComPtr<T>> operator&() noexcept
        {
            return Details::ComPtrRef<ComPtr<T>>(this);
        }

        const Details::ComPtrRef<const ComPtr<T>> operator&() const noexcept
        {
            return Details::ComPtrRef<const ComPtr<T>>(this);
        }

        T* const* GetAddressOf() const noexcept
        {
            return &ptr_;
        }

        T** GetAddressOf() noexcept
        {
            return &ptr_;
        }

        T** ReleaseAndGetAddressOf() noexcept
        {
            InternalRelease();
            return &ptr_;
        }

        T* Detach() noexcept
        {
            T* ptr = ptr_;
            ptr_ = nullptr;
            return ptr;
        }

        void Attach(_In_opt_ InterfaceType* other) noexcept
        {
            if (ptr_ != nullptr)
            {
                ptr_->Release();
            }

            ptr_ = other;
        }

        unsigned long Reset()
        {
            return InternalRelease();
        }

        long CopyTo(_Outptr_result_maybenull_ InterfaceType** ptr) const noexcept
        {
            InternalAddRef();
            *ptr = ptr_;
            return S_OK;
        }

        long CopyTo(REFIID riid, _Outptr_result_nullonfailure_ void** ptr) const noexcept
        {
            return ptr_->QueryInterface(riid, ptr);
        }

        template<typename U>
        long CopyTo(_Outptr_result_nullonfailure_ U** ptr) const noexcept
        {
            return ptr_->QueryInterface(__uuidof(U), reinterpret_cast<void**>(ptr));
        }

        // query for U interface
        template<typename U>
        long As(_Inout_ Details::ComPtrRef<ComPtr<U>> p) const noexcept
        {
            return ptr_->QueryInterface(__uuidof(U), p);
        }

        // query for U interface
        template<typename U>
        long As(_Out_ ComPtr<U>* p) const noexcept
        {
            return ptr_->QueryInterface(__uuidof(U), reinterpret_cast<void**>(p->ReleaseAndGetAddressOf()));
        }

    };    // ComPtr

}