#include "refptr.hpp"
#include <string>
#include <iostream>

struct Foo {
    std::string name;

    Foo(std::string name) : name(name), mRefCnt(0) {}
    ~Foo() = default;
    void AddRef() {
        mRefCnt++;
    }
    void Release() {
        mRefCnt--;
    }
    int mRefCnt;
};


RefPtr<Foo> make_foo(Foo* f) {
    return RefPtr{f};
}

RefCntAccountedFor<Foo> get(Foo* f) {
    auto ref = make_foo(f);
    return ref.forget();
}

void do_twice(Foo* f) {
    auto ref1 = make_foo(f);
    auto ref2 = get(f);
}

int main() {
    auto foo = new Foo{"Hello world"};
    do_twice(foo);
    std::cout << "ref count should be 0 again" << std::endl;
}