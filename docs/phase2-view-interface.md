# Phase 2 — View 추상 인터페이스 + 테스트 스텁

[← 전체 계획](Plan.md) | 이전: [Phase 1](phase1-model.md) | 다음: [Phase 3](phase3-controller.md)

## 목표

Controller가 콘솔에 직접 의존하지 않도록, 입력 수집과 출력 렌더링을 추상화한
`ISampleView` 인터페이스를 정의한다. 이와 함께 Controller 테스트에서 사용할
`FakeSampleView`(테스트 더블)를 작성해, 미리 정해진 입력 시퀀스를 재생하고
Controller가 호출한 출력 내용을 기록/검증할 수 있게 한다.

## 작성할 테스트 목록 (Red 단계)

`FakeSampleView` 자체는 테스트 더블이지만, 더블이 의도대로 동작하는지 최소한의
자기 검증 테스트를 둔다:

1. `FakeSampleViewTest.ReturnsQueuedMenuChoicesInOrder` — `EnqueueMenuChoice`로
   순서대로 넣은 값을 `ReadMenuChoice()` 호출 시 그 순서대로 반환한다.
2. `FakeSampleViewTest.RecordsShownSamples` — Controller 대신 테스트 코드가
   `ShowSampleList(samples)`를 호출하면, 이후 `LastShownSamples()`로 동일한
   벡터를 조회할 수 있다.
3. `FakeSampleViewTest.RecordsShownMessage` — `ShowMessage("...")` 호출 내용을
   `LastMessage()`로 조회 가능.

이 테스트들은 Phase 3의 Controller 테스트가 의존할 인프라이므로, Controller
테스트보다 먼저 통과시켜 둔다.

## 구현할 클래스/파일

- `Project1/Project1/View/ISampleView.h` — 순수 가상 인터페이스.
  최소 메서드(1개 흐름: 시료 등록 → 목록 조회 기준):
  - `virtual int ReadMenuChoice() = 0;`
  - `virtual Sample ReadNewSampleInput() = 0;` (등록에 필요한 필드 입력받기)
  - `virtual void ShowSampleList(const std::vector<Sample>& samples) = 0;`
  - `virtual void ShowMessage(const std::string& message) = 0;`
  - `virtual ~ISampleView() = default;`
- `Project1Tests` 전용 `View/FakeSampleView.h` — 위 인터페이스 구현체.
  - 입력 큐: `std::queue<int> menuChoices`, `std::queue<Sample> sampleInputs`
  - 기록: `std::vector<Sample> lastShownSamples`, `std::string lastMessage`
  - 헬퍼: `EnqueueMenuChoice(int)`, `EnqueueSampleInput(Sample)`,
    `LastShownSamples() const`, `LastMessage() const`

`FakeSampleView`는 실제 실행 파일(Project1)에는 포함하지 않고 테스트
프로젝트(Project1Tests)에만 둔다 — 테스트 더블이 프로덕션 바이너리에 섞이지
않도록 분리한다.

## 완료 기준

- 위 3개 자기 검증 테스트가 통과한다.
- `ISampleView.h`는 Model(`Sample`)만 참조하고 Controller를 참조하지 않는다.
- `ISampleView`에 재고 계산, 유효성 검증 등 비즈니스 로직이 없다(입출력 형태
  정의만 존재).

## 다음 phase와의 연결점

Phase 3의 Controller는 생성자/메서드에서 `ISampleView&`(또는 `shared_ptr`)를
받아 사용하도록 설계한다. 테스트에서는 `FakeSampleView`를, 실행 파일에서는
Phase 4의 `ConsoleSampleView`를 주입한다.

## 완료 확인 (Phase 5 재검증)

✅ 완료. `ISampleView.h`는 `Model/Sample.h`만 include하며 Controller를
참조하지 않는다. Phase 5에서 Controller/View가 공유하는 메뉴 번호를
`MenuOption` enum으로 이 인터페이스 헤더에 정의해, Controller의 switch 분기와
View의 프롬프트 출력이 매직 넘버 대신 같은 상수를 참조하도록 리팩터링했다.
