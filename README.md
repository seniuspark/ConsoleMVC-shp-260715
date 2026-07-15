# ConsoleMVC-shp-260715

반도체 시료 생산주문관리 시스템의 **MVC 스켈레톤 PoC**다. 최종 시스템(`Main/`)에
통합하기 전, Model / Controller / View 계층이 명확히 분리되고 그 경계가 테스트로
증명되는지를 검증하는 것이 목적이며, 도메인 로직(주문 승인, 생산 큐 계산 등)의
완성도는 이 PoC의 평가 기준이 아니다. 상위 저장소의 공통 가이드는
`../CLAUDE.md`, 이 PoC의 목표/계층 책임은 [`CLAUDE.md`](CLAUDE.md), phase별
진행 계획과 완료 기록은 [`docs/Plan.md`](docs/Plan.md)를 참고한다.

## 디렉토리 구조

```
MVC/
├── CLAUDE.md                     # 이 PoC의 목표/계층 책임/TDD 진행 방식
├── docs/
│   ├── Plan.md                   # phase 0~5 전체 계획 및 완료 기록
│   ├── phase0-harness.md ~ phase5-boundary-review.md
├── Project1/
│   ├── Project1.slnx              # Visual Studio 솔루션 (Project1, Project1Tests)
│   ├── Project1/                  # 실행 파일 프로젝트
│   │   ├── main.cpp                # SampleRepository/ConsoleSampleView/SampleController 조립
│   │   ├── Model/
│   │   │   ├── Sample.h            # 시료 엔티티
│   │   │   ├── SampleRepository.h/.cpp  # 메모리 기반 시료 저장소
│   │   ├── Controller/
│   │   │   ├── SampleController.h/.cpp  # 메뉴 흐름 제어 (콘솔 I/O 없음)
│   │   ├── View/
│   │   │   ├── ISampleView.h        # View 추상 인터페이스 + MenuOption enum
│   │   │   ├── ConsoleSampleView.h/.cpp  # 콘솔 입출력 구현체
│   │   └── Project1.vcxproj / .vcxproj.filters
│   ├── Project1Tests/              # gtest 테스트 프로젝트 (Project1 소스 공유)
│   │   ├── HarnessSanityTest.cpp
│   │   ├── ModelTests/SampleTest.cpp, SampleRepositoryTest.cpp
│   │   ├── View/FakeSampleView.h    # 테스트 전용 View 스텁(테스트 더블)
│   │   ├── ViewTests/FakeSampleViewTest.cpp, ConsoleSampleViewTest.cpp
│   │   ├── ControllerTests/SampleControllerTest.cpp
│   │   ├── BoundaryTests/LayerBoundaryTest.cpp  # 계층 경계 자동 검증
│   │   ├── packages.config         # GoogleTest NuGet 패키지 지정
│   │   └── Project1Tests.vcxproj
│   └── packages/                   # NuGet으로 설치된 GoogleTest 패키지 (gitignore 대상 아님, 저장소에 포함)
└── README.md
```

`.gitignore`로 `x64/`, `.vs/`, `*.obj/.pdb/.ilk` 등 빌드 산출물은 저장소에서
제외된다.

## 계층 구조와 책임

### Model — `Project1/Project1/Model/`

- `Sample`: 시료 엔티티. `SampleId`, `Name`, `AvgProductionTime`, `Yield`,
  `Stock`을 불변 값으로 보관하는 순수 데이터 클래스. 콘솔 I/O나 흐름 제어를
  전혀 갖지 않는다.
- `SampleRepository`: `std::vector<Sample>` 기반 메모리 저장소.
  `Add`(중복 ID 거부), `GetAll`, `FindById`, `Count`만 제공한다. JSON 등
  영속화는 하지 않는다(범위 밖, `Json/` PoC의 책임).

### View — `Project1/Project1/View/`

- `ISampleView`: Controller가 의존하는 추상 인터페이스. `ReadMenuChoice`,
  `ReadNewSampleInput`, `ShowSampleList`, `ShowMessage` 4개 순수 가상 함수와,
  메뉴 번호를 상수화한 `MenuOption` enum class(`Exit=0`, `RegisterSample=1`,
  `ListSamples=2`)를 함께 정의한다.
- `ConsoleSampleView`: `ISampleView`의 실제 구현체. `std::istream&`/
  `std::ostream&`를 생성자로 주입받아(기본값 `std::cin`/`std::cout`) 메뉴
  출력, 입력 파싱, 시료 목록/메시지 출력을 담당한다. 재고 비교나 수율 계산 등
  판단 로직은 포함하지 않는다.
- `Project1Tests/View/FakeSampleView.h`: 테스트 전용 `ISampleView` 스텁.
  메뉴 선택값과 시료 입력을 큐에 미리 채워두고(`EnqueueMenuChoice`,
  `EnqueueSampleInput`), Controller가 호출한 마지막 출력(`LastShownSamples`,
  `LastMessage`)을 검사할 수 있게 한다.

### Controller — `Project1/Project1/Controller/`

- `SampleController`: 생성자에서 `SampleRepository&`와 `ISampleView&`를
  참조로 주입받는다(콘크리트 타입이 아닌 추상 인터페이스에 의존).
  `RunOnce()`가 `ISampleView::ReadMenuChoice()`로 받은 값을 `MenuOption`으로
  분기해 `RegisterSample()`/`ListSamples()`/종료를 처리하고, `Run()`은
  `RunOnce()`를 종료 시까지 반복 호출한다. `std::cin`/`std::cout`을 직접
  호출하는 코드가 없다 — 모든 입출력은 `m_view`를 통해서만 이루어진다.

### 계층 간 의존 방향

```
main.cpp
  └─ SampleRepository (Model, concrete)
  └─ ConsoleSampleView (View, concrete) ──implements──> ISampleView
  └─ SampleController(repository, view)
        └─ SampleRepository&      (Model에 직접 의존 — 구체 타입)
        └─ ISampleView&           (View는 추상 인터페이스로만 의존)
```

Controller는 `ConsoleSampleView`라는 구체 타입을 알지 못하고 `ISampleView&`만
참조한다. 실제 콘솔 구현과 조립(`ConsoleSampleView` ↔ `SampleController`
연결)은 `main()`에서만 이루어지므로, 테스트에서는 `ConsoleSampleView` 대신
`FakeSampleView`를 주입해 콘솔 없이 Controller 로직을 검증할 수 있다(의존성
역전). Model(`Sample`, `SampleRepository`)은 Controller/View 어느 쪽에도
의존하지 않는 순수 계층이다.

## 빌드 방법

Visual Studio 2022(이상, v145 툴셋) + Windows SDK 환경 기준.

1. NuGet 패키지 복원(최초 1회, GoogleTest):
   ```
   nuget restore Project1/Project1.slnx
   ```
   또는 Visual Studio에서 솔루션을 열면 자동 복원된다.
2. `Project1/Project1.slnx`를 Visual Studio에서 열고 `Project1`(실행 파일)
   또는 `Project1Tests`(테스트)를 시작 프로젝트로 선택해 빌드/실행한다.
3. 커맨드라인(MSBuild) 빌드 예:
   ```
   msbuild Project1/Project1.slnx /p:Configuration=Debug /p:Platform=x64
   ```

두 프로젝트 모두 C++20(`LanguageStandard=stdcpp20`), x64, Debug/Release 구성을
사용한다. `Project1Tests`는 `Project1` 소스(`Model/SampleRepository.cpp`,
`Controller/SampleController.cpp`, `View/ConsoleSampleView.cpp`)를
`AdditionalIncludeDirectories`(`..\Project1`)와 `ClCompile` 상대 경로로 직접
공유하며, 별도의 라이브러리로 분리하지 않았다.

## 테스트 실행 방법

1. `Project1Tests`를 빌드하면 `Project1Tests.exe`(GoogleTest 실행 파일)가
   생성된다.
2. Visual Studio의 테스트 탐색기(Test Explorer)로 실행하거나, 빌드 산출물
   경로에서 직접 실행한다:
   ```
   Project1/Project1Tests/x64/Debug/Project1Tests.exe
   ```
3. 전체 테스트는 다음과 같이 구성되어 있고 총 25개, 전부 통과한다.

   | 테스트 스위트 | 개수 | 대상 |
   |---|---|---|
   | `HarnessSanityTest` | 1 | gtest 하네스 자체 동작 확인(Phase 0) |
   | `ModelTests`(`SampleTest`, `SampleRepositoryTest`) | 6 | `Sample`, `SampleRepository` 순수 로직 |
   | `ViewTests`(`FakeSampleViewTest`, `ConsoleSampleViewTest`) | 7 | `FakeSampleView` 자기 검증 + `ConsoleSampleView` 입출력 |
   | `ControllerTests`(`SampleControllerTest`) | 6 | `FakeSampleView`를 주입한 `SampleController` 메뉴 흐름 |
   | `ContainsAnyPatternHelperTest` | 2 | 계층 경계 검사용 헬퍼 함수 자체 테스트 |
   | `LayerBoundaryTest` | 3 | Controller/Model에 콘솔 I/O 부재, View에 재고/수율 비교 부재 검증 |

## 동작하는 기능 범위

- **실제로 동작하는 흐름**: `Project1.exe` 실행 후 "1. 시료 등록 → 2. 시료
  목록 조회 → 0. 종료"가 메뉴 선택부터 콘솔 결과 출력까지 실제로 동작한다
  (`main.cpp`가 `SampleRepository` + `ConsoleSampleView` + `SampleController`를
  조립해 `controller.Run()`을 호출).
- **범위 밖(다른 PoC/최종 통합의 책임)**:
  - Order(주문) 엔티티, 주문 승인/거절, 생산 큐(FIFO) 계산 — 이 PoC에는 구현되어
    있지 않다.
  - 데이터 영속성(JSON 파일 저장/로딩)은 `Json/` PoC의 책임이며, 이 PoC의
    `SampleRepository`는 메모리 컬렉션만 사용한다(재실행 시 데이터가 사라짐).
  - 실시간 모니터링은 `Monitor/`, 더미 데이터 생성은 `Dummy/`의 책임이다.
  - 이 PoC에서 검증한 폴더 구조/`ISampleView` 기반 의존성 역전/`FakeSampleView`
    테스트 패턴은 사람이 읽고 `Main/`에 재구현하거나 파일을 복사해 옮긴다.

## TDD 진행 과정 요약 (Phase 0~5)

Red → Green → Refactor 사이클로 아래 순서대로 진행했고(자세한 내용은
`docs/Plan.md` 및 `docs/phase*.md` 참고), 각 단계는 작은 단위 커밋으로
나뉘어 있다.

1. **Phase 0 — 하네스 구성**: GoogleTest NuGet 설치, `Project1Tests`가
   `Project1` 소스를 공유하도록 vcxproj 구성, `HarnessSanityTest`로 실행
   환경 확인.
2. **Phase 1 — Model**: `Sample`/`SampleRepository`의 실패 테스트를 먼저
   작성한 뒤 최소 구현.
3. **Phase 2 — View 인터페이스**: `FakeSampleView` 자기 검증 테스트 작성 →
   `ISampleView`/`FakeSampleView` 구현.
4. **Phase 3 — Controller (콘솔 없이)**: `FakeSampleView`를 주입한
   `SampleControllerTest`로 메뉴 흐름(등록/조회/종료) 실패 테스트를 먼저
   작성한 뒤 `SampleController` 최소 구현.
5. **Phase 4 — 콘솔 View + End-to-End**: `ConsoleSampleView` 구현/테스트 후
   `main.cpp`에서 `SampleRepository`/`ConsoleSampleView`/`SampleController`를
   연결해 실제 콘솔 실행 흐름을 완성.
6. **Phase 5 — 계층 경계 검증/리팩터링**: `LayerBoundaryTest`로 Controller/
   Model 소스에 콘솔 I/O 코드가 없고 View에 재고/수율 비교 로직이 없음을
   자동 테스트로 고정하고, 메뉴 번호 매직 넘버를 `MenuOption` enum class로
   통합하는 리팩터링을 수행했다.

PoC는 Phase 0~5가 모두 끝난 종료 상태이며, 총 25개 테스트가 전부 통과하고
빌드 경고/오류는 0건이다.
