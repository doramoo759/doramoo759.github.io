# SSL For Free에서 와일드카드 인증서를 받았던 기록

date: 2019-12-11

> 2019년에 SSL For Free에서 와일드카드 인증서를 발급했던 기록. 이후에는 Mac의 Certbot으로 바꿨다.


블로그를 처음 만들 때 인증서를 살 생각으로 가격을 알아봤다. 여러 도메인을 쓰려니 비용이 올라가 최소 30만 원 이상이 필요해서 무료 발급 방법을 찾았다.

당시에는 SSL For Free에서 와일드카드 인증서를 받을 수 있었다. 대신 3개월마다 갱신해야 했다. 아래는 2019년에 이용했던 화면과 절차다.

## 도메인 입력

[SSL For Free](https://www.sslforfree.com/)에 접속해 원하는 도메인을 입력했다. 와일드카드는 `*.도메인이름` 형식으로 넣는다.

![SSL For Free에서 와일드카드 인증서를 받았던 기록 — 화면 1](/images/server/blog-nas/synology-wildcard-sslforfree/photo-01.png)

## TXT 레코드로 도메인 확인

수동 도메인 확인을 선택해서 TXT 레코드를 받는다.

![SSL For Free에서 와일드카드 인증서를 받았던 기록 — 화면 2](/images/server/blog-nas/synology-wildcard-sslforfree/photo-02.png)

도메인을 관리하는 DNS에 안내된 TXT 레코드를 추가한다. 당시 화면에서는 TTL을 1로 설정하라고 했지만 설정 항목이 없어 반영될 때까지 기다렸다.

입력한 도메인의 `_acme-challenge` 확인 버튼으로 레코드가 조회되는지 확인한 뒤 인증서를 다운로드했다.

메일 주소를 입력하면 만료 전에 알림을 받을 수 있다고 했다.

이후에는 Mac에서 직접 발급하는 방식으로 바꿨다. 그 기록은 [Certbot으로 발급해 NAS에 적용한 메모](/work/synology-wildcard-certbot-mac/)에 남겼다.

