# Phase 3 — Controller TDD (콘솔 I/O 없이)

[← 전체 계획](Plan.md) | 이전: [Phase 2](phase2-view-interface.md) | 다음: [Phase 4](phase4-console-e2e.md)

## 목표

`FakeSampleView`를 주입한 상태로 `SampleController`의 메뉴 흐름을 gtest로
검증한다. 이 phase가 끝나면 "메뉴 선택 → Model 조작 → View에 결과 전달"이라는
핵심 흐름이 콘솔 실행 없이 100% 증명된다.

## 작성할 테스트 목록 (Red 단계)

`ControllerTests` 필터:

1. `SampleControllerTest.RegisterSampleAddsToRepository` — 메뉴 선택 1(등록)을
   큐에 넣고, FakeView가 미리 준비한 Sample 입력을 반환하도록 설정 →
   `Run()`(또는 `HandleOnce()`) 호출 후 Repository에 해당 시료가 존재하는지
   확인.
2. `SampleControllerTest.RegisterSampleShowsSuccessMessage` — 등록 성공 시
   `view.LastMessage()`에 성공 메시지가 기록되는지 확인.
3. `SampleControllerTest.ListSamplesCallsShowSampleListWithAllSamples` —
   시료 2개를 미리 Repository에 넣고 메뉴 선택 2(목록 조회) 실행 →
   `view.LastShownSamples()`가 Repository의 전체 목록과 일치.
4. `SampleControllerTest.ListSamplesWhenEmptyShowsEmptyList` — 빈 Repository
   상태에서 목록 조회 시 빈 벡터를 `ShowSampleList`에 전달(또는 안내 메시지).
5. `SampleControllerTest.UnknownMenuChoiceShowsErrorMessage` — 정의되지 않은
   메뉴 번호 입력 시 오류 메시지를 View에 표시하고 종료 없이 유지.
6. `SampleControllerTest.ExitChoiceStopsLoop` — 종료 코드(0) 선택 시 실행
   루프가 종료된다(예: `Run()`이 반환하거나 `IsRunning()`이 false).

## 구현할 클래스/파일

- `Project1/Project1/Controller/SampleController.h` (+ `.cpp`)
  - 생성자: `SampleController(SampleRepository& repository, ISampleView& view)`
  - `void RunOnce()` — View에서 메뉴 선택 1회를 읽어 분기 처리(등록/목록조회/
    종료/알수없음).
  - `void Run()` — `RunOnce()`를 종료 선택 전까지 반복 호출(선택적, E2E용).
  - 내부적으로 `std::cin`/`std::cout`을 절대 사용하지 않고 오직 `view` 참조만
    사용.

## 완료 기준

- 위 6개 테스트가 모두 작성되고 통과한다.
- `SampleController.cpp/h`에 `<iostream>` include가 없다(grep 확인).
- Controller는 재고 비교, 문자열 포맷팅 등 출력 형태를 View에 위임하고,
  자신은 "무엇을 보여줄지"(어떤 데이터)만 결정한다.

## 다음 phase와의 연결점

Phase 4에서는 이 Controller를 그대로 재사용하고, `ISampleView` 구현체만
`ConsoleSampleView`로 교체해 실제 콘솔 실행 파일에 연결한다. Controller/Model
코드는 Phase 4에서 수정하지 않는 것이 원칙이다(연결만 수행).
