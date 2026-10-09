# Mac에 Jenkins를 설치했던 기록

date: 2019-10-29

> Homebrew로 Jenkins를 설치하고 Java 8 오류를 해결했던 2019년 기록.


2019년에 Mac에 Jenkins를 설치했던 기록이다. 당시 Homebrew와 Java 8을 기준으로 적어 둔 명령이다.

## Homebrew 설치

이미 설치되어 있다면 넘어간다. 당시 사용한 설치 명령은 아래와 같다.

```bash
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```

## Jenkins 설치

```bash
brew install jenkins
```

설치 중 다음 오류를 만났다.

```text
jenkins: Java 1.8 is required to install this formula.
Install AdoptOpenJDK 8 with Homebrew Cask:
brew cask install homebrew/cask-versions/adoptopenjdk8
Error: An unsatisfied requirement failed this build.
```

Java 8을 설치한 뒤 다시 진행했다.

```bash
brew cask install homebrew/cask-versions/adoptopenjdk8
```

설치가 끝나면 `http://localhost:8080`에서 확인한다. 초기 비밀번호는 사용자 폴더의 `.jenkins/secrets/initialAdminPassword`에서 찾는다.

## 접속 주소와 포트

아래 경로로 이동했다.

```bash
cd /usr/local/opt/jenkins
```

Xcode로 `homebrew.mxcl.jenkins.plist`를 열어 주소와 포트를 바꿨다.

![Mac에 Jenkins를 설치했던 기록 — 화면 1](/images/off-duty/mac-jenkins-install/photo-01.png)

```text
--httpListenAddress=127.0.0.1 → --httpListenAddress=0.0.0.0
--httpPort=8080 → --httpPort=9090
```

그 뒤 포트 포워딩이나 nginx 리버스 프록시를 통해 접속하는 구성을 생각했다.

