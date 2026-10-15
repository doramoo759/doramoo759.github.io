# 맥(Mac)에서 Let's Encrypt 와일드카드 SSL 인증서 발급 후 시놀로지 NAS 적용 가이드

date: 2021-01-09

> Mac에서 Certbot을 실행하고 NAS DNS에 TXT 레코드를 추가한 뒤 인증서를 가져온 기록. 2021년 화면 기준.


예전에는 인증서를 발급해 주는 사이트를 이용했다. 유료로 바뀐 뒤에는 Mac에서 직접 발급해 NAS에 적용하고 있다.

3개월마다 갱신할 때 방법을 다시 찾곤 해서 적어 둔다. 아래는 2021년에 사용한 Certbot과 DSM 화면 기준이다.

## Mac에서 Certbot 설치

```bash
brew install certbot
```

설치가 끝나면 수동 발급 명령을 실행한다.

```bash
sudo certbot certonly --manual
```

인증받을 도메인을 입력한다. 와일드카드는 `*.도메인이름` 형식이다.

```text
Saving debug log to /var/log/letsencrypt/letsencrypt.log
Plugins selected: Authenticator manual, Installer None
Please enter in your domain name(s) (comma and/or space separated)  (Enter 'c'
to cancel): *.ramoo.net
```

## DNS TXT 레코드 추가

Certbot이 안내한 이름과 값으로 TXT 레코드를 등록한다. 아래 값은 당시 실행 결과다. 다음 발급 때는 새로 출력된 값을 써야 한다.

```text
Cert is due for renewal, auto-renewing...
Renewing an existing certificate for *.ramoo.net
Performing the following challenges:
dns-01 challenge for ramoo.net

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Please deploy a DNS TXT record under the name
_acme-challenge.ramoo.net with the following value:

IfxVNiyITgNqAAyfb6hVlRZ0PIKqIHxZD_Li-isWfbs

Before continuing, verify the record is deployed.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Press Enter to Continue
```

NAS에서 DNS 서버를 운영하고 있어서 여기에 추가했다.

![Mac에서 와일드카드 인증서를 발급해 NAS에 적용했다 — 화면 1](/images/server/blog-nas/synology-wildcard-certbot-mac/photo-01.png)

Certbot에 출력된 값으로 레코드를 등록한다.

![Mac에서 와일드카드 인증서를 발급해 NAS에 적용했다 — 화면 2](/images/server/blog-nas/synology-wildcard-certbot-mac/photo-02.png)

등록한 뒤 콘솔에서 Enter를 눌러 진행한다.

```text
Waiting for verification...
Cleaning up challenges

IMPORTANT NOTES:
 - Congratulations! Your certificate and chain have been saved at:
   /etc/letsencrypt/live/ramoo.net/fullchain.pem
   Your key file has been saved at:
   /etc/letsencrypt/live/ramoo.net/privkey.pem
   Your cert will expire on 2021-04-08. To obtain a new or tweaked
   version of this certificate in the future, simply run certbot
   again. To non-interactively renew *all* of your certificates, run
   "certbot renew"
 - If you like Certbot, please consider supporting our work by:

   Donating to ISRG / Let's Encrypt:   https://letsencrypt.org/donate
   Donating to EFF:                    https://eff.org/donate-le
```

검증이 끝나면 `/etc/letsencrypt/live/ramoo.net/`에 인증서 파일이 생성된다. 필요한 파일 3개를 따로 복사해 둔다.

## NAS에 인증서 적용

발급한 인증서를 NAS에 가져온다.

![Mac에서 와일드카드 인증서를 발급해 NAS에 적용했다 — 화면 3](/images/server/blog-nas/synology-wildcard-certbot-mac/photo-03.png)

![Mac에서 와일드카드 인증서를 발급해 NAS에 적용했다 — 화면 4](/images/server/blog-nas/synology-wildcard-certbot-mac/photo-04.png)

![Mac에서 와일드카드 인증서를 발급해 NAS에 적용했다 — 화면 5](/images/server/blog-nas/synology-wildcard-certbot-mac/photo-05.png)

![Mac에서 와일드카드 인증서를 발급해 NAS에 적용했다 — 화면 6](/images/server/blog-nas/synology-wildcard-certbot-mac/photo-06.png)

확인을 누르고 적용된 인증서를 확인한다.

![Mac에서 와일드카드 인증서를 발급해 NAS에 적용했다 — 화면 7](/images/server/blog-nas/synology-wildcard-certbot-mac/photo-07.png)

자동 갱신도 알아봤지만, 당시에는 수동으로 3개월마다 하기로 했다. 핑계 삼아 관리도 할 겸.

관련 메모: [Synology DNS Server 설정](/work/synology-dns-server/)

