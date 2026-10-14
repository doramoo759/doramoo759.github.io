# Synology DNS Server 설치와 레코드 설정 메모

date: 2021-07-31

> DSM 7.0에서 DNS Server를 설치하고 정방향 영역, A·PTR 레코드, 네임서버 설정을 살펴본 기록.


![Synology DNS Server 메인 화면](images/server/synology-dns/cover-synology-dns-server.png)

Synology NAS의 DNS Server 패키지를 설치하고 영역과 레코드를 설정한 기록이다. DSM 7.0, `DNS Server 9.11.23-6069` 기준이라 다른 버전에서는 화면이 달라질 수 있다.

예시에는 `example.com`과 `12.23.34.56`을 사용했다. 설정할 때는 사용할 도메인과 서버 IP로 바꿔야 한다.

## 패키지 설치

패키지 센터에서 `DNS Server`를 설치한다.

![DNS Server 설치 화면](/images/server/synology-dns/step-01-install.webp)

{{< notice "info" >}}
다른 네트워크에서 조회하려면 공유기와 방화벽 설정도 필요하다. DNS에 사용하는 UDP·TCP 53번을 조회가 필요한 범위에 허용한다. 내부망에서만 쓸 때는 인터넷에 포트를 열 필요가 없다.
{{< /notice >}}

## 정방향 영역 만들기

도메인 이름으로 IP를 조회하는 영역이다. **생성 > Master 영역 설정**으로 들어간다.

![Master 영역 생성 시작 화면](/images/server/synology-dns/step-02-zone-start.webp)

당시 외부 네임서버 구성 예시에서는 아래처럼 입력했다.

- 도메인 유형: `정방향 영역`
- 도메인 이름: 사용할 도메인
- 마스터 DNS 서버: 공인 IP 주소

![정방향 영역 설정 화면](/images/server/synology-dns/step-02-forward-zone.webp)

내부망 DNS를 구성한다면 서버 주소와 레코드에는 그 환경에서 접근할 수 있는 내부 IP를 사용한다.

영역이 만들어지면 목록에 나타난다.

![정방향 영역 생성 완료 화면](/images/server/synology-dns/step-02-forward-complete.webp)

## A 레코드 추가

`example.com`을 선택하고 **편집 > 리소스 레코드 > 생성 > A 유형**으로 들어간다. 예를 들어 `www.example.com`에 연결할 IP를 등록한다.

![정방향 리소스 레코드 진입 화면](/images/server/synology-dns/step-04-records-menu.webp)

![A 레코드 생성 화면](/images/server/synology-dns/step-04-a-record.webp)

TTL은 조회한 레코드를 캐시에 보관하는 시간이다. 주소를 바꾼 뒤에도 이전 조회 결과가 남을 수 있으니, 변경할 때는 이 값도 함께 확인한다.

## 역방향 영역과 PTR 레코드

역방향은 IP로 이름을 조회할 때 사용한다. IPv4에서는 주소를 거꾸로 적고 `in-addr.arpa`를 붙인다. 예를 들어 `12.23.34.56`의 PTR 조회 이름은 `56.34.23.12.in-addr.arpa`이다. [Cloudflare의 PTR 설명](https://www.cloudflare.com/learning/dns/dns-records/dns-ptr-record/)에 예시가 있다.

`12.23.34.0/24` 범위의 역방향 영역이라면 이름은 `34.23.12.in-addr.arpa`이고, 그 안에 `56`에 해당하는 PTR을 추가하는 식이다. 이전 글의 `56.34.23` 예시는 주소 순서가 잘못되어 수정했다. 아래 이미지는 당시 화면으로, 입력값보다 메뉴 위치를 참고하면 된다.

![정방향과 역방향 영역 목록](/images/server/synology-dns/step-03-zones-list.webp)

역방향 영역을 선택한 뒤 **편집 > 리소스 레코드 > 생성 > PTR 유형**에서 IP에 대응하는 이름을 등록한다.

![역방향 리소스 레코드 진입 화면](/images/server/synology-dns/step-05-ptr-menu.webp)

![PTR 레코드 생성 화면](/images/server/synology-dns/step-05-ptr-record.webp)

내부 DNS에서 조회할 PTR을 등록하는 것과 공인 IP의 인터넷 역방향 조회를 설정하는 것은 별개다. 공인 IP의 PTR은 IP 관리 주체에 확인해야 한다.

## 외부 네임서버로 사용할 경우

내부 장비의 이름을 조회하는 용도라면 도메인 등록 업체의 네임서버를 변경할 필요가 없다. 클라이언트가 NAS의 DNS를 조회하도록 설정하면 된다.

소유한 도메인의 외부 네임서버로 NAS를 쓰려는 경우에는 등록 업체에서 네임서버 설정을 변경한다. 당시에는 whois의 설정 화면을 사용했다.

![도메인 등록 업체의 네임서버 설정 화면](/images/server/synology-dns/step-06-provider.webp)

![네임서버 설정 예시](/images/server/synology-dns/step-06-example.webp)

등록 업체가 요구하는 네임서버 수와 등록 조건도 확인해야 한다. DDNS 주소가 있다는 것만으로 외부 위임 설정까지 끝나는 것은 아니다.

## 조회 확인

먼저 NAS의 DNS를 직접 지정해서 등록한 레코드를 확인한다. 아래의 `NAS_DNS_IP`는 실제 DNS 서버 주소로 바꾼다.

```bash
nslookup www.example.com NAS_DNS_IP
dig @NAS_DNS_IP www.example.com A
dig @NAS_DNS_IP -x 12.23.34.56
```

외부 네임서버까지 변경했다면 외부 네트워크에서도 조회한다.

```bash
nslookup example.com
dig example.com
dig example.com NS
```

NAS에 직접 물었을 때 레코드가 나오는지, 일반 DNS 조회에서도 설정이 반영되는지를 나눠 확인하면 문제가 생긴 위치를 찾기 쉽다.

## 현재 외부 IP 확인

인터넷에서 보이는 현재 주소가 궁금하다면 [내 IP 주소 확인하기](/lab-log/check-my-ip/)에서 공인 IP와 네트워크 정보를 조회할 수 있습니다. 기기의 내부 주소와 외부에 표시되는 주소가 다를 수 있으므로 DNS 레코드를 설정할 때 사용할 주소를 구분해서 확인합니다.

