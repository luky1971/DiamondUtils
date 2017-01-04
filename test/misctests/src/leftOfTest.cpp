/*
    Copyright 2016 Ahnaf Siddiqui

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "duMath.h"

using namespace Diamond;

int main() {
    // A
    Vector2<float> x(2, 3);
    Vector2<float> a(1, 3);
    Vector2<float> b(2, 1);

    std::cout << "A: " << Math::leftOf(x, a, b)
        << ", expected 1" << std::endl;

    // B
    x.set(1, 1);
    a.set(1, 3);
    b.set(2, 1);

    std::cout << "A: " << Math::leftOf(x, a, b)
        << ", expected 0" << std::endl;
}
