# FlatBuffers 기본 개념과 Ubuntu 22.04 빌드 메모

date: 2026-04-23

> FlatBuffers가 버퍼를 읽는 방식과 스키마·생성 코드, Ubuntu 22.04 소스 빌드 명령.


![FlatBuffers 개념과 설치 가이드 이미지](images/tech/flatbuffer-cover.png)

FlatBuffers에서 눈여겨볼 부분은 데이터를 읽는 방식이다. 직렬화된 버퍼를 객체로 전부 풀어 놓지 않고, 필요한 필드에 직접 접근한다.

기본 개념과 Ubuntu 22.04에서 소스로 빌드하는 방법을 적어 둔다. 성능 측정과 실제 적용 후기는 아직 없다.

## 버퍼를 읽기까지

먼저 `.fbs` 파일에 데이터 구조를 정의한다. 테이블, 구조체, 열거형, 벡터 같은 타입을 쓸 수 있다. `flatc`로 이 스키마에서 언어별 코드를 생성하고, 애플리케이션은 생성된 API로 버퍼를 만들거나 읽는다.

읽을 때 전체 데이터를 별도 객체로 변환하는 과정이 필요 없다는 것이 특징이다. 흔히 말하는 zero-copy 읽기가 이 부분이다. 필요한 데이터에 접근하므로 읽기 비용과 추가 메모리 할당을 줄이는 데 도움이 된다. 자세한 설명은 [FlatBuffers 공식 문서](https://flatbuffers.dev/)에 있다.

스키마를 여러 언어에서 공유할 수 있다는 점도 쓸모가 있다. C++ 프로그램과 다른 언어로 만든 프로그램이 같은 데이터 형식을 써야 할 때, `.fbs`를 기준으로 코드를 생성할 수 있다.

## 쓰기 전에 생각할 점

스키마 파일과 생성 코드를 함께 관리해야 한다. 데이터 구조가 바뀌면 이미 배포한 프로그램과의 호환성도 확인해야 한다.

직렬화된 데이터를 수정할 때는 새 버퍼를 만들어야 하는 경우가 있다. 사람이 파일을 열어 값을 고치는 용도라면 JSON 같은 텍스트 형식이 더 편할 수 있다.

게임 리소스, 센서 데이터, 네트워크 메시지처럼 읽기 비용을 줄이고 싶은 곳에서 검토할 수 있다. 다만 이 글에는 다른 형식과 비교한 벤치마크가 없으므로, 어느 정도 빨라지는지는 실제 데이터로 따로 확인해야 한다.

## Ubuntu 22.04에서 소스 빌드

빌드 도구를 설치한다.

```bash
sudo apt update
sudo apt install -y cmake g++ zlib1g-dev
```

소스를 받은 뒤 빌드 디렉토리를 만든다.

```bash
git clone https://github.com/google/flatbuffers.git
cd flatbuffers
mkdir build
cd build
```

`flatc` 빌드와 설치 옵션을 켜고 Release로 빌드한다.

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release -DFLATBUFFERS_BUILD_FLATC=ON -DFLATBUFFERS_INSTALL=ON
make -j$(nproc)
```

설치 후 라이브러리 캐시를 갱신한다.

```bash
sudo make install
sudo ldconfig
```

위 명령은 저장소의 기본 브랜치를 받는다. 특정 버전을 계속 사용할 프로젝트라면 사용할 태그를 정해 두는 편이 재현하기 쉽다.

