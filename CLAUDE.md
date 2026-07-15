# MVC 스켈레톤 코드 PoC — CLAUDE.md

이 디렉토리는 반도체 시료 생산주문관리 시스템의 **MVC 구조 자체를 검증하는 PoC**다.
독립된 Git Repository로 제출한다 (`ConsoleMVC-영문이름-사번`).
전체 배경/도메인 모델은 상위 `../CLAUDE.md`를 참고하되, 이 PoC는 도메인 로직의
완성도보다 **Model / Controller / View 패키지 구조와 역할 분리**가 핵심 목표다.

## 목표

- Model, Controller, View가 명확히 분리된 폴더/네임스페이스 구조를 완성한다.
- 실제 비즈니스 로직(주문 승인, 생산 큐 계산 등)은 최소한만 넣거나 스텁으로 두어도
  되지만, 최소 1개 이상의 흐름(예: 시료 등록 → 목록 조회)이 메뉴 선택부터
  결과 출력까지 실제로 동작해야 한다.
- 각 계층의 책임 경계를 테스트로 증명한다 (예: Controller는 콘솔 입출력을 하지
  않는다, View는 비즈니스 로직을 갖지 않는다).

## 계층 책임 분리

| 계층 | 책임 | 하지 않는 것 |
|---|---|---|
| Model | 도메인 엔티티(Sample, Order 등), 상태/데이터 보관 | 콘솔 입출력, 메뉴 흐름 제어 |
| Controller | 메뉴 선택에 따라 Model을 조작하고 결과를 View에 전달 | 직접 `std::cin`/`std::cout` 호출 |
| View | 메뉴 출력, 사용자 입력 수집, 결과 렌더링 | 도메인 로직 판단(예: 재고 충분 여부 계산) |

Controller가 View의 추상 인터페이스(입력받기/출력하기)를 호출하는 형태로 두면
View를 콘솔 대신 테스트용 스텁으로 교체해 gtest에서 검증하기 쉽다.

## 기술 스택

- C++20, Visual Studio(.vcxproj), x64/x86, Debug/Release
- GoogleTest는 NuGet으로 설치 (`Microsoft.googletest.v140.windesktop.msvcstdlib.static`)
- 테스트 프로젝트를 별도 vcxproj로 추가하거나, 기존 Project1에 gtest 실행 대상을
  분리해 구성한다 — 어느 쪽이든 실제 실행 파일과 테스트가 같은 소스를 공유해야 한다.

## TDD 진행 방식

1. Controller 단위 테스트를 먼저 작성한다: 가짜 View(테스트 더블)를 주입하고,
   특정 입력 시퀀스에 대해 Model이 기대한 상태로 바뀌는지 검증한다.
2. Model 단위 테스트를 작성한다: 엔티티 생성/조회 등 순수 로직.
3. 테스트를 통과시키는 최소 구현을 추가한다.
4. 리팩터링 시에도 계층 경계(콘솔 I/O는 View에만)를 유지한다.

## 범위 밖 (다른 PoC의 책임)

- 데이터 영속성(JSON 저장/로딩)은 `Json/`에서 검증한다. 이 PoC의 Model은
  메모리 상의 컬렉션(`std::vector`, `std::map` 등)만 사용해도 충분하다.
- 실시간 모니터링 도구는 `Monitor/`, 더미 데이터 생성은 `Dummy/`의 책임이다.
- 이 PoC에서 검증한 계층 구조를 그대로 `Main/`에 옮겨 적용한다.
