# C++ std::tuple 활용법: 함수에서 여러 값 반환하고 std::tie로 받는 방법

date: 2021-06-26

> 함수 결과를 tuple로 묶고 get과 tie로 꺼내는 예제. 마지막 구조적 바인딩은 C++17 문법이다.


함수에서 결과 두 개를 반환해야 할 때가 있다. 결과를 받을 변수를 참조로 넘기는 방법도 있지만, `std::tuple`로 묶으면 간결하게 쓸 수 있다.

짝수 여부와 짝수일 때 1을 더한 값을 함께 반환하는 예제를 적어 두었다.

```cpp
#include <iostream>
#include <tuple>

std::tuple<bool,int> isEvenAdd(int val) {
    bool bis_even = (val % 2 == 0);
    int ret = val;
    if (bis_even) {
        ret = val+1;
    }
    return std::make_tuple(bis_even, ret);
}

int main()
{
    auto ret = isEvenAdd(10);
    std::cout << "ret contains: ";
    std::cout << std::get<0>(ret) << ' ';
    std::cout << std::get<1>(ret) << '\n';
    
    std::tie (myResult, myValue) = ret;
    std::cout << "tie contains: ";
    std::cout << myResult << ' ';
    std::cout << myValue << '\n';
    
    auto [bis_even, val] = isEvenAdd(11);
    std::cout << "auto tie contains: ";
    std::cout << bis_even << ' ';
    std::cout << val << '\n';
}
```

`std::get<index>(instance)`로 각 원소에 접근한다. `std::tie`로 여러 값을 한 번에 풀어 받을 수도 있다.

마지막 `auto [bis_even, val]` 형태는 C++17의 구조적 바인딩이다. `tuple` 자체는 C++11부터 사용할 수 있지만 이 부분은 버전이 다르다.

원문 예제의 `myResult`, `myValue`는 사용하는 곳에서 따로 선언해야 한다.

