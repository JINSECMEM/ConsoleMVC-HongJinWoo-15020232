# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## 프로젝트 개요

반도체 시료 생산 관리 시스템의 Console App MVC PoC.  
주문 등록 → 재고 확인 → 출고 / 생산 요청 흐름을 C++20으로 검증한다.

- **언어/표준**: C++20, MSVC v145 (Visual Studio 2022)
- **플랫폼**: Win32 / x64 Console Application
- **솔루션**: `ConsoleMVC/ConsoleMVC.slnx`

## 빌드

```powershell
# Visual Studio GUI: ConsoleMVC/ConsoleMVC.slnx 열기 후 빌드
# MSBuild CLI
msbuild ConsoleMVC\ConsoleMVC.slnx /p:Configuration=Debug /p:Platform=x64
msbuild ConsoleMVC\ConsoleMVC.slnx /p:Configuration=Release /p:Platform=x64
```

현재 테스트 프레임워크는 없다. PoC 범위에서는 `main.cpp` 실행으로 직접 흐름을 검증한다.

## 아키텍처 — 5-레이어 MVC

```
View → Controller → Service → Repository → Model
```

| 레이어 | 위치 | 역할 |
|--------|------|------|
| Model | `Models/` | 순수 도메인 데이터 구조체 (의존 없음) |
| Database | `Database/` | `IRepository<T>` 기반 인메모리 `std::map` 저장소 |
| Service | `Services/` | 비즈니스 로직 캡슐화, Repository 중개 |
| Controller | `Controllers/` | 사용자 액션 → Service 호출 → View 갱신 |
| View | `Views/` | 콘솔 입출력 전담, Model 읽기 전용 참조 |

**의존 방향 규칙 (위반 금지)**
- Model은 다른 레이어를 참조하지 않는다
- View는 Controller를 직접 호출하지 않는다 (Controller가 View를 호출)
- Service는 View를 참조하지 않는다
- Repository는 Service/Controller를 참조하지 않는다

## 핵심 흐름 — 생산 시뮬레이션

`AppController::Run()`이 메인 루프를 소유한다. 루프마다 `ProductionController::Tick()`을 호출해 각 `ProductionLine`의 `progressPct`를 증가시키고, 100% 도달 시 `ProductionService::CompleteJob()` → `InventoryService::Replenish()` → `ShipmentService::RetryPending()` 순으로 연쇄 처리한다.

생산 완료 이벤트는 `ProductionService::SetOnCompleteCallback()`으로 주입된 람다를 통해 Controller로 역전달된다 (Service가 Controller를 직접 참조하지 않기 위한 콜백 패턴).

## 도메인 식별자 규칙

| 유형 | 형식 |
|------|------|
| 주문 | `ORD-YYYYMMDD-NNNN` |
| 생산 작업 | `JOB-YYYYMMDD-NNNN` |
| 출고 | `SHP-YYYYMMDD-NNNN` |
| 시료 | `S-001`, `S-002`, `S-003` |
| 생산라인 | `LINE-A`, `LINE-B`, `LINE-C` |

각 시료는 전담 생산라인과 1:1 매핑된다 (S-001↔LINE-A, S-002↔LINE-B, S-003↔LINE-C).

## 참고 문서

- `docs/REQUIREMENTS.md` — 기능/비기능 요구사항, 상태 정의
- `docs/ARCHITECTURE.md` — 레이어 상세 책임, 확장 고려사항
- `docs/FLOW.md` — 업무 플로우 다이어그램, 주문 상태 전이도, 콘솔 화면 예시
- `docs/CLASS_DESIGN.md` — 전체 클래스/메서드 시그니처, 파일 구조
