# Synology GitLab CE 컨테이너 업데이트 메모

date: 2022-03-22

> GitLab CE 14.8.3에서 14.8.4로 업데이트할 때 사용한 DSM Docker 화면과 설정 내보내기·가져오기 기록.


GitLab CE를 Docker Registry에서 받아 설치해 두었다. 설치한 이미지를 업데이트하는 방법을 찾아보다가 기록으로 남긴다.

2022년에 `14.8.3-ce.0`에서 `14.8.4-ce.0`으로 바꿀 때 사용한 DSM Docker 화면 기준이다.

## 새 이미지 받기

`DSM > Docker > Registry`에서 gitlab을 검색해 필요한 버전을 다운로드한다.

![Synology GitLab CE 컨테이너 업데이트 메모 — 화면 1](/images/server/blog-nas/synology-gitlab-docker-update/photo-01.png)

`latest`보다 버전을 지정해 두는 편이 다음 업데이트 때 관리하기 편하다. 당시에는 `14.8.4-ce.0`을 받았다.

![Synology GitLab CE 컨테이너 업데이트 메모 — 화면 2](/images/server/blog-nas/synology-gitlab-docker-update/photo-02.png)

## 설정 내보내기

`DSM > Docker > Container`에서 기존 GitLab CE 컨테이너를 선택하고, 우클릭 메뉴의 `설정 > 내보내기`를 선택한다.

![Synology GitLab CE 컨테이너 업데이트 메모 — 화면 3](/images/server/blog-nas/synology-gitlab-docker-update/photo-03.png)

적당한 폴더를 골라 내보낸다.

![Synology GitLab CE 컨테이너 업데이트 메모 — 화면 4](/images/server/blog-nas/synology-gitlab-docker-update/photo-04.png)

선택한 폴더에 JSON 파일이 생긴다.

![Synology GitLab CE 컨테이너 업데이트 메모 — 화면 5](/images/server/blog-nas/synology-gitlab-docker-update/photo-05.png)

텍스트 편집기로 연다.

![Synology GitLab CE 컨테이너 업데이트 메모 — 화면 6](/images/server/blog-nas/synology-gitlab-docker-update/photo-06.png)

이미지 이름의 버전을 새로 받은 버전으로 바꾸고 저장한다.

```json
"image": "gitlab/gitlab-ce:14.8.3-ce.0"
```

위 값을 다음과 같이 바꿨다.

```json
"image": "gitlab/gitlab-ce:14.8.4-ce.0"
```

![Synology GitLab CE 컨테이너 업데이트 메모 — 화면 7](/images/server/blog-nas/synology-gitlab-docker-update/photo-07.png)

## 컨테이너 교체

기존 컨테이너를 중지한다.

![Synology GitLab CE 컨테이너 업데이트 메모 — 화면 8](/images/server/blog-nas/synology-gitlab-docker-update/photo-08.png)

중지한 컨테이너를 삭제한다. 삭제하기 전에는 데이터를 백업하고, 버전 사이에 필요한 마이그레이션 경로도 확인해야 한다. 내보낸 설정 파일만으로 데이터 백업을 대신할 수는 없다.

![Synology GitLab CE 컨테이너 업데이트 메모 — 화면 9](/images/server/blog-nas/synology-gitlab-docker-update/photo-09.png)

`DSM > Docker > Container > 설정 > 가져오기`에서 수정한 파일을 선택한다.

![Synology GitLab CE 컨테이너 업데이트 메모 — 화면 10](/images/server/blog-nas/synology-gitlab-docker-update/photo-10.png)

새 버전으로 GitLab CE를 실행한다.

![Synology GitLab CE 컨테이너 업데이트 메모 — 화면 11](/images/server/blog-nas/synology-gitlab-docker-update/photo-11.png)

다른 Docker 이미지도 설정을 내보내고 이미지 버전을 바꾸는 방식으로 교체할 수 있다. 다만 애플리케이션마다 데이터와 버전 호환 조건은 따로 확인할 것.

