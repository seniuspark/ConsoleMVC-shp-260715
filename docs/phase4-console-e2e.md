# Phase 4 — 콘솔 View 구현 및 End-to-End 동작 확인

[← 전체 계획](Plan.md) | 이전: [Phase 3](phase3-controller.md) | 다음: [Phase 5](phase5-boundary-review.md)

## 목표

`ISampleView`의 실제 구현체 `ConsoleSampleView`를 작성해 `std::cin`/`std::cout`을
이 계층에만 두고, `Project1`의 `main()`에서 `SampleRepository` +
`SampleController` + `ConsoleSampleView`를 연결해 "시료 등록 → 목록 조회" 흐름이
실제 실행 파일에서 입력부터 출력까지 동작하는 것을 확인한다.

## 작성할 테스트 목록 (Red 단계)

콘솔 View 자체는 실제 `std::cin`/`std::cout`에 의존하므로 전통적인 단위 테스트
대상이 아니다. 대신 아래 방식으로 검증한다:

1. `ConsoleSampleViewTest.ShowSampleListFormatsHeaderAndRows` — `std::ostringstream`을
   출력 스트림으로 주입 가능하게 설계(`ConsoleSampleView(std::istream&, std::ostream&)`
   생성자)하고, `ShowSampleList`가 예상 텍스트(헤더 + 각 행)를 출력 스트림에
   쓰는지 검증. (이렇게 하면 콘솔 View도 스트림을 주입받아 완전 자동화 테스트
   가능 — `std::cin`/`std::cout`은 `main()`에서만 실제 스트림으로 바인딩.)
2. `ConsoleSampleViewTest.ReadMenuChoiceParsesIntegerFromInputStream` —
   `std::istringstream("2\n")`을 주입해 `ReadMenuChoice()`가 2를 반환하는지 확인.
3. `ConsoleSampleViewTest.ReadNewSampleInputParsesAllFields` — 개행으로 구분된
   입력 스트림을 주입해 `ReadNewSampleInput()`이 올바른 `Sample`을 구성하는지
   확인.
4. (수동/스모크) End-to-End 체크리스트 — 아래 "완료 기준"의 수동 확인 절차.

## 구현할 클래스/파일

- `Project1/Project1/View/ConsoleSampleView.h` (+ `.cpp`)
  - `ConsoleSampleView(std::istream& in = std::cin, std::ostream& out = std::cout)`
  - `ISampleView`의 모든 메서드 구현. 입력 파싱 실패(비정상 값)는 최소한의
    방어만(예: 잘못된 메뉴 번호는 그대로 반환하고 Controller가 처리).
- `Project1/Project1/main.cpp`
  - `SampleRepository repository;`
  - `ConsoleSampleView view;`
  - `SampleController controller(repository, view);`
  - `controller.Run();` — 종료 메뉴(0) 선택 시까지 반복.
- `Project1Tests`에 `ConsoleSampleViewTest` 3개 추가(스트림 주입 방식이므로
  Project1 실행 파일과 동일한 소스 공유, 테스트는 `std::cin`/`std::cout`을
  실제로 사용하지 않음).

## 완료 기준

- 위 3개 자동화 테스트가 통과한다.
- 수동 스모크: `Project1.exe`를 실행해 다음을 확인한다.
  1. 메뉴에서 "1. 시료 등록" 선택 → SampleId/Name/AvgProductionTime/Yield/Stock
     을 순서대로 입력 → 등록 성공 메시지 출력.
  2. 메뉴에서 "2. 목록 조회" 선택 → 방금 등록한 시료가 목록에 표시됨.
  3. 메뉴에서 "0. 종료" 선택 → 프로그램이 정상 종료됨.
- `ConsoleSampleView` 외 어떤 파일에도 `<iostream>`/`std::cin`/`std::cout`
  include·호출이 없다(grep으로 Controller/Model 대상 재확인).

## 다음 phase와의 연결점

이 phase까지 끝나면 CLAUDE.md의 "최소 1개 흐름이 실제로 동작한다"는 완료 기준을
충족한다. Phase 5에서는 이 구조가 우연이 아니라 규칙으로 지켜지는지(경계 위반이
없는지)를 검증 테스트/스크립트로 못 박고 문서를 정리한다.
