# C++11 push_back vs emplace_back 차이점과 객체 생성 최적화 원리

date: 2020-04-17

> 생성자 인자를 바로 넘겨 원소를 만드는 emplace_back과 push_back을 비교한 코드 조각.


`vector`에 원소를 넣을 때 보통 `push_back`을 쓴다. C++11에서는 생성자 인자를 바로 전달할 수 있는 `emplace_back`도 있다.

당시 적어 둔 비교 예제다. `AClass` 정의는 생략된 코드 조각이다.

```cpp
void main(){
    std::vector<AClass> vA;
    va.push_back(AClass(1,"a"));
}
```

```cpp
void main(){
    std::vector<AClass>	va;
    va.emplace_back(1,"a");
}
```

`emplace_back`은 컨테이너 안에서 원소를 생성한다. 임시 객체를 만든 뒤 옮기는 일을 줄일 수 있다는 점을 기억해 둔다.

다만 `vector`가 커지면서 재할당되면 기존 원소의 이동이나 복사가 생길 수 있으니, 호출 전체에서 이동이 전혀 없다고 생각하면 안 된다.

