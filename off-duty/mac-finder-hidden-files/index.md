# Mac Finder 숨김 파일·폴더 보기 단축키(Cmd+Shift+.) 및 터미널 명령어 정리

date: 2019-10-29

> 한 번 설정하고 잊기 쉬운 Finder 숨김 파일 표시 명령과 재실행 방법.


Finder에서 숨김 파일을 표시하는 명령을 적어 둔다. 한 번 설정하면 다시 만질 일이 별로 없어서 잊기 쉽다.

터미널에서 실행한다.

```bash
defaults write com.apple.Finder AppleShowAllFiles TRUE
```

Finder를 다시 실행하면 숨김 파일이 보인다.

```bash
killall Finder
```

