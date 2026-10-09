# Mac Time Machine 백업 속도 메모

date: 2019-09-26

> 업데이트 문제를 Time Machine으로 복원한 뒤 백업 속도를 조정해 본 기록.


Mac을 업데이트하다 한 번 날려 먹은 적이 있다. 그때 Time Machine으로 복원하고 안도의 한숨을 쉬었다.

이후로 꾸준히 백업하고 있지만 속도가 느려서, 당시 찾아 쓴 명령을 적어 둔다.

```bash
sudo sysctl debug.lowpri_throttle_enabled=0
```

낮은 우선순위 작업의 제한을 조정하는 설정이다. 내 환경에서는 엄청나게 빨라지는 정도는 아니었지만 어느 정도 효과가 있었다.

