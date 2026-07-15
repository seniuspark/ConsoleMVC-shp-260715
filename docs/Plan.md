# MVC PoC 전체 계획

이 문서는 `ConsoleMVC-shp-260715` (MVC 스켈레톤 PoC)의 phase별 진행 계획이다.
목표와 계층 책임 분리는 [`../CLAUDE.md`](../CLAUDE.md)를 따른다. 핵심 평가 기준은
도메인 로직의 완성도가 아니라 **Model / Controller / View 계층 구조와 그 경계가
테스트로 증명되는가**이다.

## Phase 개요

| Phase | 이름 | 목표 | 세부 계획 |
|---|---|---|---|
| 0 | 프로젝트/테스트 하네스 구성 | GoogleTest NuGet 설치, 테스트 프로젝트가 Project1 소스를 공유하도록 vcxproj 구성 | [phase0-harness.md](phase0-harness.md) |
| 1 | Model 엔티티 설계 | Sample, Order 등 순수 도메인 엔티티와 저장소(메모리 컬렉션) 단위 테스트/구현 | [phase1-model.md](phase1-model.md) |
| 2 | View 추상 인터페이스 + 테스트 스텁 | IView 인터페이스 정의, Controller 테스트에서 쓸 FakeView(테스트 더블) 작성 | [phase2-view-interface.md](phase2-view-interface.md) |
| 3 | Controller TDD (콘솔 I/O 없이) | FakeView를 주입해 Controller의 메뉴 흐름(시료 등록/목록 조회)을 gtest로 검증 후 최소 구현 | [phase3-controller.md](phase3-controller.md) |
| 4 | 콘솔 View 구현 + End-to-End 동작 | 실제 ConsoleView 구현, main()에서 메뉴 선택 → 결과 출력까지 동작 확인 | [phase4-console-e2e.md](phase4-console-e2e.md) |
| 5 | 계층 경계 검증 및 리팩터링 | 정적 검사(스크립트/리뷰)로 Controller에 cin/cout 없음을 증명, 중복 제거, 문서 정리 | [phase5-boundary-review.md](phase5-boundary-review.md) |

## Phase 간 의존관계

```
Phase 0 (하네스)
   └─▶ Phase 1 (Model)
          └─▶ Phase 2 (View 인터페이스 + Fake)
                 └─▶ Phase 3 (Controller TDD)
                        └─▶ Phase 4 (Console View + E2E)
                               └─▶ Phase 5 (경계 검증/리팩터링)
```

- Phase 0은 이후 모든 phase가 의존하는 테스트 실행 환경이므로 가장 먼저 끝나야 한다.
- Phase 1(Model)은 Controller/View 어느 쪽에도 의존하지 않는 순수 로직이라 Phase 2와
  병렬 진행도 가능하지만, Controller 테스트(Phase 3)가 Model 타입을 사용하므로 순서상
  먼저 배치한다.
- Phase 2(View 인터페이스)는 Phase 3의 선행 조건이다 — Controller가 참조할 추상
  인터페이스와 FakeView가 있어야 Controller를 콘솔 없이 테스트할 수 있다.
- Phase 3까지 끝나면 "메뉴 로직이 Model을 올바르게 조작한다"는 것이 콘솔 없이 증명된다.
- Phase 4는 Phase 3에서 검증된 Controller에 실제 ConsoleView를 연결만 하는 단계이므로
  비즈니스 로직 변경이 없어야 한다(연결 전용).
- Phase 5는 전 phase의 산출물을 대상으로 하는 마무리 단계.

## 이번 PoC의 최소 동작 흐름(Definition of Done 공통 기준)

- 시료 등록(메뉴 1) → 시료 목록 조회(메뉴 2)가 실제 콘솔 실행 파일에서 입력부터
  출력까지 동작한다.
- Controller 단위 테스트와 Model 단위 테스트가 모두 존재하고 통과한다.
- Controller 소스에 `std::cin`/`std::cout`/`std::cerr` 직접 호출이 없다.
- View 소스에 재고 비교/수율 계산 등 비즈니스 판단 로직이 없다.

## 범위 밖

- JSON 파일 영속성, 실시간 모니터링, 더미 데이터 생성, 주문 승인/생산 큐 계산의
  완전한 구현은 이 PoC의 범위가 아니다 (각각 `Json/`, `Monitor/`, `Dummy/`,
  `Main/`의 책임). 필요하면 Order 관련 메뉴는 스텁(미구현 안내 메시지)으로 남긴다.
