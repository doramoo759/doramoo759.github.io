# C++ 람다를 쓰며 적어 둔 메모

date: 2021-01-30

> 캡처 방식과 std::function 콜백, 정렬에 람다를 쓰는 형태를 적어 둔 메모.


람다는 이름 없는 함수처럼 쓸 수 있다. 사용 빈도가 늘어나서 필요한 내용을 적어 둔다.

[람다 설명](https://en.cppreference.com/w/cpp/language/lambda)

## 형태와 캡처

```cpp
[capture] (params) -> return_type { body }
[capture] (params) { body }
```

- `[&]`: 필요한 주변 자동 변수를 참조로 캡처한다.
- `[=]`: 필요한 주변 자동 변수를 복사로 캡처한다.
- `[&, a]`: 기본은 참조 캡처, `a`만 복사한다.
- `[=, &a]`: 기본은 복사 캡처, `a`만 참조로 잡는다.
- `[&, &a]`: 기본 참조 캡처와 같은 방식을 중복 지정해서 오류가 난다.

복사로 캡처한 값을 람다 안에서 바꾸려면 `mutable` 같은 조건도 확인해야 한다.

## 저장과 콜백

`std::function`에 담아 둘 수 있다.

```cpp
#include <vector>
#include <functional>
std::function<int(int)> func = [](int i)->int{ return i+1; };
```

콜백을 받는 형태로도 쓸 수 있다. 아래는 당시 적어 둔 코드 조각이다.

```cpp
class Foo {
public:
	void SetCallback(std::function<void(const char*)> cb){
		m_fcb = std::move(cb);
	}
	void ReqPrint(const char* pszText){
		m_fcb(pszText);
	}
	
private:
	std::function<void(const char*)>	m_fcb;	
}

int main(){
	Foo	_f;
	_f.SetCallback([](const char* pszText){
		std::cout << "callback: " << pszText << '\n';
	});
	
	_f.ReqPrint("myFoo");
	
	return 0;
}
```

## 정렬에 사용

비교용 함수 객체를 따로 만드는 대신 람다를 넘기면 짧게 쓸 수 있다.

```cpp
#include <algorithm>
#include <functional>
#include <array>
 
int main()
{
    std::array<int, 10> s = {5, 7, 4, 2, 8, 6, 1, 9, 0, 3};
 
	// case 1 
    struct {
        bool operator()(int a, int b) const { return a < b; }
    } customLess;
    std::sort(s.begin(), s.end(), customLess);

 	// case 2
    std::sort(s.begin(), s.end(), [](int a, int b) {
        return a > b;
    });
}
```

