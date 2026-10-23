# C++11 push_back vs emplace_back 차이: 복사·이동과 vector 재할당

date: 2020-04-17

> C++11 vector의 push_back과 emplace_back 차이를 실행 가능한 예제로 비교한다. 생성자 인자 전달, 임시 객체와 복사·이동, reserve와 재할당, noexcept 및 C++17 복사 생략의 범위를 설명한다.


`push_back`은 넣을 **객체**를 받고, `emplace_back`은 그 객체를 만들 **생성자 인자**를 받을 수 있다. C++11에서 추가된 `emplace_back`의 핵심은 이 차이다. 생성자 인자를 바로 넘기면 별도의 `AClass` 임시 객체를 만든 뒤 옮기는 과정을 줄일 수 있다.

그렇다고 `push_back`을 전부 `emplace_back`으로 바꿀 이유는 없다. 이미 만들어진 객체를 넣는지, 새 객체를 그 자리에서 만드는지부터 봐야 한다. 여기에 `vector` 재할당까지 섞어 놓으면, 생성자 출력은 많아지는데 정작 어느 비용을 줄였는지 설명하기 어려워진다.

이 글에서는 **새 원소를 만드는 비용**과 **기존 원소를 새 저장 공간으로 옮기는 비용**을 나눠 살펴본다. 예제는 C++11 문법을 기준으로 하며, 실행 시간의 우열을 측정한 벤치마크는 아니다.

## push_back과 emplace_back의 차이

원소 형식이 `AClass`라면 `push_back`에는 `const AClass&`와 `AClass&&`를 받는 오버로드가 있다. 보통 이름 있는 객체를 전달하면 복사 생성하고, 이동 가능한 임시 객체나 `std::move`로 전달한 객체는 이동 생성한다. 반면 `emplace_back`은 전달받은 인자를 전달 참조와 완벽 전달(perfect forwarding)을 통해 원소 생성에 사용한다. [Microsoft의 vector 문서](https://learn.microsoft.com/en-us/cpp/standard-library/vector-class?view=msvc-170#emplace_back)

아래 표는 **추가할 여유 공간이 있고, 복사·이동 생성자가 모두 있는 형식**을 전제로 한다. 재할당으로 기존 원소가 옮겨지는 비용은 포함하지 않았다.

| 호출 | 전달하는 것 | 새 AClass 원소가 만들어지는 방식 |
| --- | --- | --- |
| `values.push_back(value)` | 이름 있는 기존 객체 | `value`를 복사 생성 |
| `values.push_back(std::move(value))` | 이동 대상으로 표시한 기존 객체 | 이동 생성 |
| `values.push_back(AClass(1, "a"))` | 먼저 생성한 임시 객체 | 임시 객체에서 이동 생성 |
| `values.emplace_back(1, "a")` | 생성자 인자 | vector의 원소 자리에서 직접 생성 |
| `values.emplace_back(value)` | 기존 객체 | 복사 생성. emplace라는 이름이 복사를 없애 주지는 않음 |
| `values.emplace_back(AClass(1, "a"))` | 먼저 생성한 임시 객체 | 임시 객체에서 이동 생성 |

`std::move`는 이동을 직접 수행하는 함수가 아니다. 인자의 값 범주를 바꿔 이동 생성자가 선택될 수 있게 한다. 이동 생성자가 없거나 객체가 `const`이면 복사 생성자가 선택될 수도 있다. 따라서 “rvalue를 넘기면 무조건 이동”이라는 설명도 형식의 생성자 구성을 빼놓으면 부정확하다. [C++ 작업 초안의 forward/move 도우미](https://eel.is/c++draft/forward)

## 그대로 컴파일해서 확인하는 C++11 예제

`AClass`의 일반 생성자, 복사 생성자, 이동 생성자에 서로 다른 출력을 넣었다. 각 비교는 독립된 `vector`에서 시작한다. 1~4번은 빈 컨테이너에 `reserve(1)`을 호출해 첫 삽입에서 재할당이 일어나지 않도록 했다.

로그는 `AClass`의 생성 경로를 보여 준다. 멤버인 `std::string` 내부의 할당 횟수나 프로그램 전체의 메모리 비용까지 세는 예제는 아니다. 소멸자 출력도 생략했다.

```cpp
#include <iostream>
#include <string>
#include <utility>
#include <vector>

class AClass {
public:
    AClass(int id, std::string name)
        : id_(id), name_(std::move(name)) {
        std::cout << "ctor " << id_ << '\n';
    }

    AClass(const AClass& other)
        : id_(other.id_), name_(other.name_) {
        std::cout << "copy " << id_ << '\n';
    }

    AClass(AClass&& other) noexcept
        : id_(other.id_), name_(std::move(other.name_)) {
        std::cout << "move " << id_ << '\n';
    }

private:
    int id_;
    std::string name_;
};

int main() {
    {
        std::cout << "[1] push_back temporary\n";
        std::vector<AClass> values;
        values.reserve(1);
        values.push_back(AClass(1, "a"));
    }
    {
        std::cout << "[2] emplace_back arguments\n";
        std::vector<AClass> values;
        values.reserve(1);
        values.emplace_back(1, "a");
    }
    {
        std::cout << "[3] push_back lvalue\n";
        std::vector<AClass> values;
        values.reserve(1);
        AClass value(1, "a");
        values.push_back(value);
    }
    {
        std::cout << "[4] emplace_back temporary\n";
        std::vector<AClass> values;
        values.reserve(1);
        values.emplace_back(AClass(1, "a"));
    }
    {
        std::cout << "[5] fill capacity\n";
        std::vector<AClass> values;
        values.reserve(1);
        while (values.size() < values.capacity()) {
            values.emplace_back(1, "a");
        }
        std::cout << "[5] append beyond capacity\n";
        values.emplace_back(2, "b");
    }
    return 0;
}
```

[예제 전체 내려받기](/examples/cpp11-emplace.cpp). 파일 이름을 `cpp11-emplace.cpp`로 저장한 폴더에서 다음과 같이 실행하면 된다. C++11을 지원하는 컴파일러와 표준 라이브러리가 필요하며 관리자 권한은 필요 없다.

```sh
g++ -std=c++11 -O0 -Wall -Wextra -Wpedantic cpp11-emplace.cpp -o cpp11-emplace
./cpp11-emplace
```

### 직접 실행한 환경과 출력

2026년 10월 23일 Linux x86_64 환경에서 GCC 13.3.0과 Clang 20.1.2로 확인했다. 두 컴파일러 모두 이 환경의 libstdc++를 사용했다. 각각 C++11의 `-O0`·`-O2`, C++17의 `-O2`로 빌드한 여섯 조합에서 경고 없이 컴파일됐고 출력도 같았다. Clang과 libc++ 조합이나 MSVC에서 실행한 결과까지 포함하는 것은 아니다.

아래는 GCC 13.3.0, `-std=c++11 -O0` 실행 결과다.

```text
[1] push_back temporary
ctor 1
move 1
[2] emplace_back arguments
ctor 1
[3] push_back lvalue
ctor 1
copy 1
[4] emplace_back temporary
ctor 1
move 1
[5] fill capacity
ctor 1
[5] append beyond capacity
ctor 2
move 1
```

1번의 `ctor 1`은 임시 객체 생성, `move 1`은 그 임시 객체에서 vector 원소를 만드는 과정이다. 2번은 `AClass`를 원소 자리에서 바로 생성하므로 `ctor 1`만 나온다. 3번에서는 기존 객체를 먼저 만든 뒤 복사한다. 4번처럼 `emplace_back`에 이미 만든 임시 객체를 넘기면 1번과 같은 이동이 남는다.

여기서 줄어든 것은 **이 예제의 AClass 이동 생성 한 번과 별도 임시 객체의 수명 관리**다. 이 로그만으로 실제 프로그램이 몇 퍼센트 빨라진다고 말할 수는 없다. `int`처럼 복사가 가벼운 형식과 자원 관리 비용이 큰 사용자 정의 형식을 같은 기준으로 평가해서도 안 된다.

## reserve와 vector 재할당은 별개의 문제다

`vector`는 연속된 저장 공간을 사용한다. `size()`는 살아 있는 원소의 수이고, `capacity()`는 재할당 없이 담을 수 있는 원소 수다. `reserve(n)`은 최소 `n`개를 담을 공간을 확보하도록 요청하며, 원소 수 자체를 늘리지는 않는다. `resize(n)`과 혼동하면 안 된다. [C++ 작업 초안의 vector.capacity](https://eel.is/c++draft/vector.capacity)

5번에서는 현재 `capacity()`만큼 원소를 채운 뒤 하나를 더 넣었다. `reserve(1)`이 정확히 1칸만 할당한다고 가정하지 않기 위해 반복문으로 용량을 채웠다. 이 환경에서는 첫 용량이 1이었고, 다음 `emplace_back(2, "b")`에서 새 원소 생성과 기존 원소 이동이 함께 관찰됐다.

**emplace_back을 써도 재할당된 저장 공간으로 기존 원소를 옮기는 작업은 남는다.** 어떤 구현은 새 원소부터 만들고 다른 구현은 세부 처리 순서가 다를 수 있으므로, 로그의 전체 순서를 이식 가능한 규칙으로 외우지 않는 편이 좋다. 용량 증가 비율 역시 이 예제로 일반화할 수 없다.

재할당이 발생하면 기존 원소를 가리키던 포인터·참조·반복자가 무효화된다. 속도 비교보다 이 문제를 먼저 확인해야 하는 코드도 많다. 삽입 전 얻은 참조를 다음 삽입 후에도 사용한다면, `push_back`을 `emplace_back`으로 바꾸는 것만으로 안전해지지 않는다. [C++ 작업 초안의 vector.modifiers](https://eel.is/c++draft/vector.modifiers)

예상 원소 수를 알고 있다면 적절한 시점에 `reserve`를 한 번 적용하는 것이 도움이 될 수 있다. 다만 이미 원소가 있는 vector의 용량을 늘리는 `reserve` 자체도 재배치를 일으킬 수 있다. 매 삽입마다 `reserve(size() + 1)`을 호출하는 방식은 자연스러운 용량 증가를 방해할 수 있어 피하는 편이 낫다.

### noexcept가 기존 원소의 이동·복사에 영향을 준다

예제의 이동 생성자에는 `noexcept`를 붙였다. 재할당 중 이동이 실패하면 이미 옮긴 원소의 상태를 복원하기 어려울 수 있기 때문이다. 복사할 수 있는 형식에서 이동 생성자가 예외를 던질 가능성이 있으면, libstdc++는 기존 원소를 복사하는 경로를 사용할 수 있다. 설치된 라이브러리의 `vector.tcc`에서도 `__uninitialized_move_if_noexcept_a` 경로를 확인할 수 있다. [GCC 13.3.0의 vector 구현](https://github.com/gcc-mirror/gcc/blob/releases/gcc-13.3.0/libstdc%2B%2B-v3/include/bits/vector.tcc)

그래서 생성자 로그를 읽을 때는 “내가 새로 넣는 객체의 이동인가, 기존 원소의 재배치인가”를 구분해야 한다. 위 프로그램에서 `noexcept`를 빼면 복사 경로가 나타날 수 있지만, 그 결과는 형식과 라이브러리에 따라 달라진다. 이동 생성자에 `noexcept`를 붙이는 판단은 실제로 예외를 던지지 않는 구현인지 확인한 뒤 해야 한다.

## C++17 복사 생략이면 push_back의 이동도 없어질까

C++17에서 같은 형식의 prvalue로 객체를 초기화할 때는 결과 객체를 직접 만드는 규칙이 적용되는 경우가 있다. 예를 들어 `AClass value = AClass(1, "a");` 같은 초기화다. 이름 있는 지역 변수를 반환하는 NRVO처럼 조건에 따라 허용되는 복사 생략과도 구분할 필요가 있다. [C++ 작업 초안의 객체 초기화](https://eel.is/c++draft/dcl.init), [복사·이동 생략 규칙](https://eel.is/c++draft/class.copy.elision)

하지만 `push_back(AClass(1, "a"))`에서는 임시 객체가 함수의 참조 매개변수에 연결되고, vector 안의 별도 원소를 그 객체에서 생성한다. 이 이동이 C++17의 보장된 복사 생략 대상이 되는 것은 아니다. 실제로 위 프로그램도 C++17 `-O2`에서 1번의 `move 1`을 출력했다.

생성자에 출력처럼 관찰 가능한 부수 효과가 없으면 컴파일러가 실제 기계어를 더 단순하게 만들 수 있다. 반대로 복사 생략이 허용되는 다른 문맥에서는 출력이 있는 생성자 호출도 생략될 수 있다. 이 때문에 생성자 로그는 **선택된 생성 경로를 설명하는 도구**로 보고, 실행 시간 비교와 분리해야 한다. 최적화 옵션을 올리면 모든 예제의 이동 출력이 사라진다고 기대해서는 안 된다.

참고로 `emplace_back`의 반환형은 C++11·14에서는 `void`, C++17부터는 추가된 원소의 참조다. 이 글의 예제는 반환값을 사용하지 않아 두 표준에서 그대로 실행할 수 있다. 이 차이는 GCC 헤더의 표준 버전별 선언에서도 확인할 수 있다. [GCC 13.3.0의 stl_vector.h](https://github.com/gcc-mirror/gcc/blob/releases/gcc-13.3.0/libstdc%2B%2B-v3/include/bits/stl_vector.h)

## 실무에서 두 함수를 고르는 기준

나는 호출을 읽었을 때 의도가 드러나는 쪽을 먼저 고른다. 완성된 객체를 추가한다면 `push_back(value)`, 더 이상 필요 없는 객체의 자원을 넘긴다면 `push_back(std::move(value))`, 생성자 인자로 새 원소를 만든다면 `emplace_back(args...)`가 자연스럽다. 이동한 객체의 이후 상태와 사용 가능 범위는 해당 형식의 계약을 확인한다.

또 하나의 점검 대상은 생성자 선택이다. 예를 들어 `std::vector<std::vector<int>>`에서 `emplace_back(10)`은 원소가 열 개인 내부 vector를 만든다. 반면 `push_back(std::vector<int>{10})`은 값이 10인 원소 하나를 가진 내부 vector를 추가한다. 함수 이름과 중괄호·소괄호를 함께 바꾸면 의미까지 달라질 수 있다는 예다. [Microsoft의 vector 생성자 설명](https://learn.microsoft.com/en-us/cpp/standard-library/vector-class?view=msvc-170#vector)

성능을 확인해야 한다면 같은 데이터와 초기 용량, 같은 컴파일러·표준 라이브러리·최적화 옵션에서 비교한다. 생성자 출력은 시간 측정에서 제거하고, 할당과 재할당을 비교에 포함할지도 먼저 정한다. 기존 코드의 가독성과 의미를 유지한 상태에서 병목을 측정해야 함수 이름만 바꾼 최적화에 머물지 않는다.

## 최근 수정 — 2026년 10월 23일

처음에는 C++ 개발 글을 시리즈로 이어 갈 생각이었다. 지금은 그 계획을 이어 가는 데 의미를 두지 않아, 개발 관련 새 글은 당분간 작성하지 않기로 했다. 이 글은 `push_back`과 `emplace_back`의 차이를 찾아오는 분들이 참고할 수 있는 독립적인 정보 글로 남겨 둔다.

