# Phase 1 — Model 엔티티 설계

[← 전체 계획](Plan.md) | 이전: [Phase 0](phase0-harness.md) | 다음: [Phase 2](phase2-view-interface.md)

## 목표

콘솔 입출력이나 메뉴 흐름과 무관한 순수 도메인 모델(Sample, 필요 시 Order)을
정의하고, 메모리 컬렉션 기반 저장소(Repository) 동작을 단위 테스트로 증명한다.
이 PoC는 계층 구조 검증이 목적이므로 Order/생산큐/승인 로직은 최소화하고,
"시료 등록 → 목록 조회" 흐름에 필요한 최소 범위(Sample과 SampleRepository)만
우선 구현한다.

## 작성할 테스트 목록 (Red 단계)

`Project1Tests`에 `ModelTests` 네임스페이스/필터로 추가:

1. `SampleTest.ConstructsWithGivenFields` — SampleId/Name/AvgProductionTime/
   Yield/Stock을 넣어 생성 시 각 getter가 그대로 값을 반환한다.
2. `SampleRepositoryTest.AddIncreasesCount` — 빈 저장소에 시료 1개를 추가하면
   `Count()`(또는 `GetAll().size()`)가 1이 된다.
3. `SampleRepositoryTest.AddDuplicateIdThrows` (또는 bool 반환) — 이미 존재하는
   SampleId로 다시 추가 시 실패를 알리는 방식(예외 또는 bool) 확인.
4. `SampleRepositoryTest.GetAllReturnsInsertedSamplesInOrder` — 여러 개 추가 후
   목록 조회 시 추가한 순서/개수가 일치한다.
5. `SampleRepositoryTest.FindByIdReturnsNulloptWhenMissing` — 존재하지 않는
   ID 조회 시 실패를 나타내는 값(`std::optional` 등)을 반환한다.

## 구현할 클래스/파일

- `Project1/Project1/Model/Sample.h` — `Sample` 값 타입(구조체 또는 클래스).
  필드: `SampleId`(string), `Name`(string), `AvgProductionTime`(double,
  min/ea), `Yield`(double, 0.0~1.0), `Stock`(int).
- `Project1/Project1/Model/SampleRepository.h` (+ `.cpp`) — `std::vector<Sample>`
  기반 저장소. 최소 API:
  - `bool Add(Sample sample)`
  - `const std::vector<Sample>& GetAll() const`
  - `std::optional<Sample> FindById(const std::string& id) const`
  - `std::size_t Count() const`

## 완료 기준

- 위 5개 테스트가 모두 작성되고, 최소 구현 후 모두 통과(Green)한다.
- `Model/` 폴더 내 파일에 `<iostream>` include나 `std::cin`/`std::cout` 사용이
  없다(grep으로 확인 가능).
- Sample/SampleRepository는 Controller/View 어떤 헤더도 include하지 않는다
  (역방향 의존 없음).

## 다음 phase와의 연결점

Phase 2에서 정의할 View 인터페이스와 FakeView는 이 Model 타입(`Sample`)을
데이터 전달 형태로 사용한다. Phase 3의 Controller는 `SampleRepository`를 멤버로
가지고 View로부터 받은 입력을 여기 반영한다.

## 완료 확인 (Phase 5 재검증)

✅ 완료. `LayerBoundaryTest.ModelSourcesContainNoIostreamInclude`가
`Model/Sample.h`, `Model/SampleRepository.h/.cpp`에 `<iostream>`/`std::cin`/
`std::cout`/`std::cerr`가 없음을 자동으로 검증한다. Model은 Controller/View
헤더를 include하지 않는다(역방향 의존 없음, 코드 리뷰로 확인).
