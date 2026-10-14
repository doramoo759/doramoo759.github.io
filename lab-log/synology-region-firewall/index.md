# Synology NAS에서 특정 지역 접속을 차단했다

date: 2019-10-27

> 반복되는 admin 로그인 시도를 보고 DSM 방화벽에서 지역별 규칙을 만들었다. 이후 특정 포트만 차단하도록 바꿨다.


NAS에 admin 계정으로 로그인하려는 흔적이 계속 보여서 IP 위치를 찾아봤다.

[IP 주소를 지도에서 확인하는 도구](https://www.ipvoid.com/ip-to-google-maps/)

2단계 인증을 켜 두고 비밀번호도 간단하게 쓰지 않았지만, 계속 시도가 보이니 기분이 좋지는 않았다.

## DSM 방화벽 설정

하는 김에 방화벽에서 특정 지역을 차단했다.

`DSM > 제어판 > 보안 > 방화벽 > 방화벽 프로파일 규칙 편집`

![Synology NAS에서 특정 지역 접속을 차단했다 — 화면 1](/images/server/blog-nas/synology-region-firewall/photo-01.png)

![Synology NAS에서 특정 지역 접속을 차단했다 — 화면 2](/images/server/blog-nas/synology-region-firewall/photo-02.png)

지역이나 IP 주소로 규칙을 만들 수 있었다.

처음에는 모든 포트를 막았는데, 다시 생각해 보니 그럴 필요는 없을 것 같았다. 글을 쓴 뒤 특정 포트만 차단하는 것으로 바꿨다.

## 내 네트워크의 IP 확인

지금 사용하는 인터넷 연결의 IP와 추정 위치는 [내 IP 주소 확인하기 — 공인 IP·통신사·위치 지도](/lab-log/check-my-ip/)에서 볼 수 있습니다. 현재 방문자의 연결을 조회하는 도구이며, 위에서 사용한 임의 IP 위치 조회 도구와는 용도가 다릅니다.

