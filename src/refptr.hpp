#include <utility>
#include <cassert>
#include "tagrefcnt.hpp"

template <typename T> concept ReferenceCounted = requires(T t) {
    t.AddRef();
    t.Release();
};

template <ReferenceCounted T>
class RefPtr;
template <ReferenceCounted T>
class RefCntAccountedFor;


template <ReferenceCounted T>
struct RefCntAccountedFor {
    RefCntAccountedFor() : mRawPtr(nullptr) {}
    RefCntAccountedFor(RefCntAccountedFor<T>& aOther) : mRawPtr(aOther.take()) {}

    [[nodiscard]]
    T* take() {
        auto result = mRawPtr;
        mRawPtr = nullptr;
        return result;
    }
    T* mRawPtr;
};

template <ReferenceCounted T>
class RefPtr {
public:
    RefPtr(T* aRawPtr) : mRawPtr(aRawPtr) {
        if(mRawPtr) {
            mRawPtr->AddRef();
        }
    }

    RefPtr<T>& operator=(RefCntAccountedFor<T>& aRhs) {
        T* oldPtr = mRawPtr;
        this->mRawPtr = aRhs.take();
        if(oldPtr) {
            oldPtr->Release();
        }
        return *this;
    }

    RefPtr<T>& operator=(RefCntAccountedFor<T>&& aRhs) {
        T* oldPtr = mRawPtr;
        this->mRawPtr = aRhs.take();
        if(oldPtr) {
            oldPtr->Release();
        }
        return *this;
    }

    ~RefPtr() {
        if(mRawPtr) {
            mRawPtr->Release();
        }
    }

    RefCntAccountedFor<T> forget() {
        RefCntAccountedFor<T> res{};
        auto tmp = mRawPtr->mRefCnt;
        std::swap(res.mRawPtr, mRawPtr);
        assert(!mRawPtr);
        return res;
    }

    T* operator->() {
        return mRawPtr;
    }

    private:
    T* mRawPtr;
};