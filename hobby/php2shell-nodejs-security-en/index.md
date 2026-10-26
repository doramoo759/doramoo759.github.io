# PHP2Shell: AI-Found PHP Vulnerabilities and Node.js Security Updates

date: 2026-10-23

> PHP2Shell CVEs, fixed PHP and Node.js versions, Japan's shutdown advice, practical checking tools and the reported cybersecurity-stock reaction.


![A fictional scene of Japanese Prime Minister Sanae Takaichi asking an AI robot for help as it repairs a PHP and JS server shield, with an AI illustration disclosure at upper left and ramoo.net at lower right.](/images/tech/php2shell-nodejs-security/cover-en-v2.webp)

*An AI-generated fictional scene imagining Japan’s prime minister asking an AI robot for help. It does not depict an actual meeting or statement.*

A PHP vulnerability became serious enough for Japan's prime minister to mention taking websites temporarily offline. If PHP sounds like something only an old website uses, PHP2Shell is a reminder to look beneath the application: updated CMS plugins do not patch a vulnerable PHP runtime.

**As checked on October 23, 2026, the first fixed releases to verify are PHP 8.2.35, 8.3.36, 8.4.27 and 8.5.12, plus Node.js 22.23.4, 24.22.1 and 26.12.1.** Use those or later fixed releases in the respective supported branches, accounting for vendor backports. This is an operator-oriented comparison of public advisories, not a claim that I performed penetration tests. [PHP advisory](https://www.php.net/archive/2026.php#2026-10-21-5), [Node.js advisory](https://nodejs.org/en/blog/vulnerability/october-2026-security-releases)

## Who found PHP2Shell? More than one security company

**PHP2Shell** is the name attached to this PHP issue. Wiz Research describes using AI-assisted analysis to demonstrate unauthenticated remote code execution against real applications. Remote code execution, or RCE, means an attacker can cause a server to run attacker-chosen code. [Wiz's research](https://www.wiz.io/blog/php2shell-php-rce)

The discovery is not accurately described as one firm's isolated achievement. The PHP Foundation traces early signs of the OOM problem to work connected with a 2024 Quarkslab audit. Calif reported RCE with the same root cause on August 11, 2026; OpenAI reported independently, and Wiz supplied additional findings on August 24. Fixed releases arrived on October 21. [PHP Foundation's account](https://thephp.foundation/blog/2026/10/21/todays-php-security-release-and-how-were-strengthening-php-security/)

To me, the important development is AI helping researchers establish practical exploitation of a difficult, longstanding problem. That does not leave administrators more time to patch. It makes a faster defensive response more valuable.

## The two PHP vulnerabilities explained

| Vulnerability | Underlying problem | Possible result |
| --- | --- | --- |
| [CVE-2026-109618](https://github.com/php/php-src/security/advisories/GHSA-w8hw-w9fx-j79w) | Out-of-memory handling leaves partially initialized internal data that cleanup treats as valid | Memory corruption and RCE; a mod_php path was demonstrated before application code runs |
| [CVE-2026-109619](https://github.com/php/php-src/security/advisories/GHSA-6wmf-4rv5-p3rp) | A worker retains state referring to a previous request's freed memory | Use-after-free, worker crashes and conditional RCE |

PHP's advance notice rated both **Critical, CVSS v4.0 9.2**. Its advisory covers web-serving installations including PHP-FPM, mod_php and php-cgi, with mod_php considered particularly high risk. Neither using FPM nor having a secure login is sufficient reason to exclude a deployment. [PHP security guidance](https://www.php.net/archive/2026.php#2026-10-21-5)

An affected installation is not proof of a completed compromise. Results depend on reachable paths, configuration and process privileges. KISA warns of web shells, data theft, defacement and access to internal resources. Secrets readable by the web process and systems it can reach are important limits on potential damage. [KISA response guidance](https://www.krcert.or.kr/kr/bbs/view.do?bbsId=B0000133&menuNo=205020&nttId=72230&pageIndex=4)

## Fixed PHP versions by branch

This table lists the first fixed release in each supported branch for both CVEs. Compare within a branch; a numerically larger-looking version from another branch is not a substitute.

| PHP branch | Unpatched upstream releases | Fixed release |
| --- | --- | --- |
| 8.2 | Before 8.2.35 | **8.2.35 or later** |
| 8.3 | Before 8.3.36 | **8.3.36 or later** |
| 8.4 | Before 8.4.27 | **8.4.27 or later** |
| 8.5 | Before 8.5.12 | **8.5.12 or later** |
| 8.1 and older | Unsupported upstream branches | Migrate; verify any distribution or LTS-vendor backport separately |

Source: [KISA's PHP update advisory](https://krcert.or.kr/kr/bbs/view.do?bbsId=B0000133&menuNo=205020&nttId=72227&pageIndex=1). This is not a complete historical affected-version matrix. PHP 8.2 itself reaches security end of life on December 31, 2026, making a migration plan worthwhile alongside the emergency patch. [PHP 8.2.35 announcement](https://news-web.php.net/php.internals/132889)

Distributions can backport fixes without adopting the upstream version number. Conversely, installing a new command-line PHP binary does not prove the web worker changed. For hosting, NAS packages and appliances with embedded PHP, check the supplier's CVE-specific notice.

## Did Japan's prime minister advise shutting websites down?

**The substance of that report is supported.** J-CAST reported that Prime Minister Sanae Takaichi posted on X on October 21 urging operators to check PHP use and prepare to patch quickly. Where immediate patching was difficult, she described temporarily suspending websites as a way to reduce risk. [J-CAST's October 22 report](https://www.j-cast.com/2026/10/22518636.html)

[![Japanese Prime Minister Sanae Takaichi in the photograph and original caption displayed in J-CAST's report; the caption identifies September 2026 as the photo date.](/images/tech/php2shell-nodejs-security/jcast-takaichi-article-capture-v2.webp)](https://www.j-cast.com/2026/10/22518636.html)

*Source: photo-and-caption area captured on October 23 from [J-CAST News's October 22, 2026 report](https://www.j-cast.com/2026/10/22518636.html). The original caption dates this file photograph to September 2026; it does not show the PHP security appeal taking place.*

That is conditional risk-reduction advice, not evidence that every Japanese server was compromised or that all websites received a blanket shutdown order. What interests me is availability becoming an explicit management decision: sometimes a planned interruption protects more than keeping a vulnerable service online.

## Node.js security releases: a separate set of vulnerabilities

Node.js also published security releases on October 21. The timing does not mean the PHP flaw spread into Node.js. A particularly important item is **CVE-2026-111497, an out-of-bounds write in the Brotli decoder permitting RCE**. The announcement credits OpenAI Codex Security researchers. Check whether attacker-controlled compressed data can reach the affected processing path; applications do not all have identical exposure.

The following summarizes the ten entries in the [Node.js security advisory](https://nodejs.org/en/blog/vulnerability/october-2026-security-releases). Severity follows that advisory: CVE-2026-106072 is listed as Medium there, although individual release notes group two TLS fixes under High.

| CVE | Severity | Function to review | Branches |
| --- | --- | --- | --- |
| CVE-2026-105482 | High | DNS names containing NUL characters | 22, 24, 26 |
| CVE-2026-105483 | High | TLS hostname validation for fullwidth-digit IPs | 24, 26 |
| CVE-2026-106069 | High | TLS validation of hostnames loosely resolving to IPs | 22, 24, 26 |
| CVE-2026-111497 | High | Brotli decoder out-of-bounds write and RCE | 22, 24, 26 |
| CVE-2026-105481 | Medium | Keep-alive response ordering | 22, 24, 26 |
| CVE-2026-106072 | Medium | Certificate validation for noncanonical IPv4 hostnames | 22, 24, 26 |
| CVE-2026-106074 | Medium | Repeated random-cache values across snapshots and single-executable applications | 22, 24, 26 |
| CVE-2026-106070 | Low | Returned bytes after invalid ZIP-header validation | 26 |
| CVE-2026-106071 | Low | Permission Model wildcard grants | 26 |
| CVE-2026-106073 | Low | Permission Model bypass through `fs.realpathSync` | 22, 24, 26 |

Review the features your service actually uses: decompression of external data, outbound TLS connections, and code relying on restricted filesystem permissions. A CVE name alone does not establish an identical application-level outcome everywhere.

| Node.js branch | Security update |
| --- | --- |
| 22.x | **[22.23.4 or later](https://nodejs.org/en/blog/release/v22.23.4/)** |
| 24.x | **[24.22.1 or later](https://nodejs.org/en/blog/release/v24.22.1/)** |
| 26.x | **[26.12.1 or later](https://nodejs.org/en/blog/release/v26.12.1/)** |

Unsupported branches need a migration decision. These are patch baselines for this release, not a promise that future advisories will never require a newer version.

## What I would check first as an operator

I would start with **the runtime serving real requests and the person responsible for it**, before running an external scanner. Even a personal blog can span managed hosting and independently maintained containers.

1. Inventory public sites, admin interfaces and forgotten test services. Identify PHP and Node.js, including APIs behind static pages.
2. Match running binaries, packages and containers against official fixes and supplier notices. Compare shell versions with the actual service.
3. Prepare recovery and compatibility checks, deploy patched packages, then restart workers or redeploy containers. Consider limiting access if patching must wait.
4. Recheck versions and important functions. A WAF rule does not replace a runtime patch.
5. Review exposure-period errors, process activity, outbound connections and file changes. For suspected compromise, preserve evidence while isolating, then handle secrets and recovery.

This sequence draws on [PHP's response guidance](https://www.php.net/archive/2026.php#2026-10-21-5) and [KISA's operational guidance](https://www.krcert.or.kr/kr/bbs/view.do?bbsId=B0000133&menuNo=205020&nttId=72230&pageIndex=4). Suspected compromise changes the sequence: preserve volatile evidence before restarting. Routine patch maintenance and incident investigation are different situations.

These are **example read-only commands**, to run on the relevant host or inside the service container. They are not results from testing your server.

```sh
php -v
node --version
node -p 'process.versions.brotli'
```

The first may show CLI PHP, while a service manager or container can select a different Node binary too. I would use internal administration and deployment records rather than leave a diagnostic `phpinfo()` page publicly accessible.

## Checking sites and tools: understand the test's scope

| Resource or tool | How to use it | Scope and limits |
| --- | --- | --- |
| [PHP2Shell information hub](https://www.php2shell.com/) | Compare CVE and upgrade guidance with your inventory | A research and response hub, not a URL-based safety verdict |
| [NCSC-NL product inventory](https://github.com/NCSC-NL/php2shell) | Find your supplier or CMS and follow its notices | Helps locate embedded PHP; investigation status is not confirmed vulnerability |
| [MDN HTTP Observatory](https://developer.mozilla.org/en-US/observatory) | Submit a public domain you administer | Tests headers and related controls, not PHP2Shell or Node runtime patch status |
| [Trivy](https://trivy.dev/docs/latest/guide/scanner/vulnerability/) | Compare container components against a vulnerability database | Coverage and database freshness matter; third-party and source-installed software can create gaps |
| [npm audit](https://docs.npmjs.com/cli/commands/npm-audit/) | Audit project dependencies from the project directory | Checks npm dependencies; it does not replace upgrading the Node executable |

A high Observatory score is not certification that software is current; its [FAQ](https://developer.mozilla.org/en-US/observatory/docs/faq) explicitly describes that limit. I would trust verified package and running-binary status over an absent version header or a scanner finding nothing.

Record the image or package deployed, who restarted the service, and where the running version was verified. That gives the next administrator something more useful than a message saying the update is done.

## Did cybersecurity stocks rise?

Some did. **U.S. figures quoted in reporting and Korean closing results use different observation bases.** This is not a synchronized return comparison.

| Market and basis | Stock or fund | Reported change from the preceding session |
| --- | --- | --- |
| U.S. October 21 trading, quoted in an October 22 report; closing figures not independently verified | Palo Alto Networks | +7.0% |
| Same basis | CrowdStrike | +5.2% |
| Same basis | Zscaler | +7.8% |
| Same basis | HACK / CIBR cybersecurity ETFs | +3.2% / +2.7% |
| Korea, October 22 close | Genians / Monitorapp | +1.47% / +3.33% |
| Korea, October 22 close | RaonSecure / Xgate | −3.18% / −5.00% |

U.S. figures come from [PrimeXBT quoting CNBC](https://primexbt.com/news/cybersecurity-stocks-jump-after-nsa-and-fbi-warn-of-php-vulnerability/); Korean figures come from [Newsis' closing-market report](https://www.newsis.com/view/NISX20261022_0003837318). I did not recompute them from exchange price data. Newsis also discusses financial-sector hacking incidents and a falling broader market.

My interpretation is that security-spending expectations helped some names. These observations do not isolate PHP as the cause, and the public stocks are not a list of the firms that discovered the bugs. Growing defensive demand and a particular company's ability to convert it into revenue and profit are separate questions.

The first return worth protecting here is safe service uptime. As AI helps accelerate vulnerability research, administrators need shorter paths from inventory to patching and verification. My priority would be connecting the official fixed versions to the actual deployment, while leaving specialist incident analysis to people equipped to do it.

