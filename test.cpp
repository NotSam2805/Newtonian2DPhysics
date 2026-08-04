#include "Maths/Maths.h"
#include <iostream>

using namespace std;

int main(){
    // Test Vector2D class

    Vector2D zero = Vector2D::Zero();
    Vector2D up(0,1);

    Vector2D double_up = 2.0 * up;

    cout << double_up;

    return 0;
}