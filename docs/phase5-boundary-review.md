# Phase 5 — 계층 경계 검증 및 리팩터링

[← 전체 계획](Plan.md) | 이전: [Phase 4](phase4-console-e2e.md)

## 목표

앞선 phase들에서 만들어진 Model/Controller/View 구조가 실제로 CLAUDE.md의
계층 책임 분리 표를 지키고 있는지 자동/반자동 검사로 못박고, 발견된 위반이나
중복을 리팩터링한다. 이 phase는 새 기능을 추가하지 않는다.

## 작성할 테스트 목록 (Red 단계)

1. `LayerBoundaryTest.ControllerSourcesContainNoIostreamInclude` — 테스트
   코드에서 `Controller/*.cpp`, `Controller/*.h` 파일들을 읽어(파일 경로는
   빌드 시점에 알 수 있는 상대경로 상수 목록으로 하드코딩) `"<iostream>"`,
   `"std::cin"`, `"std::cout"` 문자열이 없는지 검사. (파일 텍스트를 읽는
   간단한 gtest — 컴파일러가 아니라 문자열 검사 수준이면 충분하다.)
2. `LayerBoundaryTest.ModelSourcesContainNoIostreamInclude` — 같은 방식으로
   `Model/*.h`, `Model/*.cpp` 검사.
3. `LayerBoundaryTest.ViewSourcesContainNoModelBusinessLogic` — (있다면)
   `View/*.cpp`에 재고 비교 연산자(`Stock <`, `Stock >=` 등 도메인 판단으로
   의심되는 패턴)가 없는지 휴리스틱 검사. 완벽한 검증은 어렵기 때문에 코드
   리뷰로 보완함을 주석에 명시.
4. (회귀) Phase 1~4에서 작성한 모든 테스트가 계속 통과하는지 전체 재실행.

## 점검/리팩터링 대상

- 중복 로직(예: 메뉴 번호 상수가 Controller와 View 양쪽에 매직 넘버로 흩어져
  있는지) → 공용 enum/상수로 정리.
- Controller가 View 인터페이스가 아닌 구체 클래스(`ConsoleSampleView`)를 직접
  참조하는 곳이 없는지 확인(의존성 역전 준수).
- `Project1Tests`와 `Project1` 간 소스 공유 방식이 파일 복사가 아니라 실제
  참조(같은 파일)인지 vcxproj 재확인.
- 테스트 이름/네임스페이스 일관성 정리(ModelTests/ControllerTests/
  LayerBoundaryTest 등).

## 완료 기준

- 신설한 `LayerBoundaryTest` 3종 + 기존 Phase 1~4 테스트 전체가 통과한다.
- Controller/Model 소스에 대한 `grep -n "std::cin\|std::cout\|<iostream>"`
  결과가 View 폴더 파일 제외 0건이다.
- `MVC/docs/`의 모든 phase 문서가 실제 구현 상태와 어긋나지 않도록 필요한
  경우 각 phase 문서의 "완료 기준" 항목에 체크 표시나 갱신 코멘트를 남긴다.

## 다음 단계 (이 PoC 밖)

이 phase가 끝나면 MVC PoC는 완료 상태다. 검증된 계층 구조(Model/Controller/
View 폴더 분리, `ISampleView` 인터페이스를 통한 의존성 역전, FakeView 기반
Controller 테스트 패턴)는 사람이 읽고 `Main/`에 재구현하거나 파일을 복사해
옮기는 방식으로 통합한다. `Json/`(영속성), `Monitor/`(모니터링), `Dummy/`(더미
데이터)는 이 PoC와 무관하게 각자 진행한다.
