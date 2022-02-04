#include "matplotlibcpp.h"
#include <vector>

using namespace std;
namespace plt = matplotlibcpp;

int main() {
    vector<int> x;
    vector<int> y;
    for (int i = 0; i < 5; i++) {
        x.push_back(i);
        y.push_back(i*i);
    }
    plt::plot(x, y);
    plt::save("./basic.png");
}