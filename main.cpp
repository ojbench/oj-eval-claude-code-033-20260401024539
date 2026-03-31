#include "list.hpp"
#include <iostream>
#include <string>
#include <cassert>

using namespace std;
using namespace sjtu;

int main() {
    // Basic test to ensure the list compiles and works
    list<int> mylist;

    // Test push operations
    mylist.push_back(1);
    mylist.push_back(2);
    mylist.push_back(3);
    mylist.push_front(0);

    // Test size
    assert(mylist.size() == 4);
    assert(!mylist.empty());

    // Test front/back
    assert(mylist.front() == 0);
    assert(mylist.back() == 3);

    // Test iterators
    int expected = 0;
    for (auto it = mylist.begin(); it != mylist.end(); ++it) {
        assert(*it == expected);
        expected++;
    }

    // Test pop operations
    mylist.pop_front();
    assert(mylist.front() == 1);
    mylist.pop_back();
    assert(mylist.back() == 2);

    // Test insert
    auto it = mylist.begin();
    ++it;
    mylist.insert(it, 5);
    assert(mylist.size() == 3);

    // Test erase
    it = mylist.begin();
    ++it;
    mylist.erase(it);
    assert(mylist.size() == 2);

    // Test clear
    mylist.clear();
    assert(mylist.empty());
    assert(mylist.size() == 0);

    // Test copy constructor and assignment
    list<int> list2;
    list2.push_back(10);
    list2.push_back(20);

    list<int> list3(list2);
    assert(list3.size() == 2);
    assert(list3.front() == 10);

    list<int> list4;
    list4 = list2;
    assert(list4.size() == 2);
    assert(list4.back() == 20);

    cout << "All basic tests passed!" << endl;

    return 0;
}
