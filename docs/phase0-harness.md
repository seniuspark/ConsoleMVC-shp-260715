# Phase 0 — 프로젝트/테스트 하네스 구성

[← 전체 계획](Plan.md)

## 목표

Project1(콘솔 실행 파일 프로젝트)과 소스를 공유하는 GoogleTest 기반 테스트
프로젝트를 구성해, Phase 1 이후의 모든 TDD 사이클이 곧바로 Red → Green을
반복할 수 있는 상태를 만든다. 이 phase 자체는 테스트 대상 로직이 없으므로
"테스트가 통과한다"는 gtest가 정상적으로 빌드/실행된다는 뜻으로 사용한다.

## 작성할 테스트 목록 (Red 단계)

- `HarnessSanityTest.AlwaysPasses` — gtest 실행 파일이 빌드되고 돌아가는지
  확인하는 자명한 테스트 1개(`EXPECT_EQ(1, 1)` 수준). 이후 실제 로직 테스트가
  추가되면 유지할 필요 없이 삭제 가능.

## 구현/설정해야 할 것

- `MVC/Project1/Project1Tests/` (가칭) 테스트 프로젝트 vcxproj 신설.
  - NuGet: `Microsoft.googletest.v140.windesktop.msvcstdlib.static` 설치.
  - `Project1` 프로젝트의 소스 폴더(Model/Controller/View, 아래 Phase 1~4에서
    생성될 예정)를 참조 추가(Additional Include Directories + 공유 소스 항목
    추가, 단 `main()`이 있는 진입점 파일은 테스트 프로젝트에서 제외).
  - `Project1.slnx`에 테스트 프로젝트를 추가하고 두 프로젝트가 같은 솔루션에서
    빌드되게 한다.
- 소스 트리에 계층별 폴더를 먼저 만들어 둔다 (파일 없이 폴더 구조만 예약):
  - `Project1/Project1/Model/`
  - `Project1/Project1/Controller/`
  - `Project1/Project1/View/`
- `.gitignore`에 `.vs/`, `x64/`, `Debug/`, `Release/`, `packages/`(NuGet 복원
  캐시) 등 빌드 산출물 제외 규칙이 있는지 확인하고 없으면 추가.

## 완료 기준

- `Project1Tests` 프로젝트가 x64/Debug 구성으로 빌드된다.
- `HarnessSanityTest.AlwaysPasses`가 실행되어 통과한다(Visual Studio 테스트
  탐색기 또는 `vstest.console.exe` 기준).
- 솔루션 빌드 시 `Project1`(콘솔 앱)과 `Project1Tests`(테스트) 두 산출물이 모두
  생성된다.

## 다음 phase와의 연결점

Phase 1부터는 이 테스트 프로젝트에 실제 Model 테스트를 추가하기만 하면 되므로,
매 phase마다 새 프로젝트를 만들 필요 없이 동일한 하네스 위에서 Red → Green →
Refactor를 반복한다.

## 완료 확인 (Phase 5 재검증)

✅ 완료. `Project1.slnx` 빌드 시 `Project1.exe`/`Project1Tests.exe` 모두
생성되며, `Project1Tests.exe`가 25개 테스트(HarnessSanityTest 포함) 모두
통과함을 재확인했다.
