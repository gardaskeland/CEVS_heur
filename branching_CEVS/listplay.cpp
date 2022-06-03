#include <list>
#include <iostream>

using namespace std;

int main() {
    list<int> li = {2, 3, 4, 5};
    list<int>::iterator it = li.begin();
    cout << "First: " << *it << "\n";
    ++it;
    cout << "Second: " << *it << "\n";
    ++it;
    cout << "Third: " << *it << "\n";
    --it;
    it = li.erase(it);
    //++it;
    it = li.erase(it);
    for (int i : li) cout << i << " ";
    cout  << "\n";
    cout << "Third: " << *it << "\n";
    ++it;
    cout << "Fourth: " << *it << "\n";
}