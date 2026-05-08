# 반도체 시료 생산 관리 시스템 — Console MVC PoC

C++20 / Visual Studio 2022 기반의 Console Application.  
MVC 패턴 스켈레톤으로 반도체 시료의 주문 → 재고 → 출고 → 생산 흐름을 검증한다.

## 목적

| 항목 | 내용 |
|------|------|
| 언어 | C++20 (MSVC v145) |
| 플랫폼 | Win32 / x64 Console |
| 패턴 | MVC (Model – View – Controller) |
| 범위 | PoC (기능 흐름 검증, 실DB 연동 제외) |

---

## 핵심 기능 흐름

```
주문 등록
  └─► 재고 체크
        ├─ 재고 충분 ──► 즉시 출고
        └─ 재고 부족 ──► 생산라인 생산 요청
                              └─► 생산 완료 ──► 출고
```

자세한 업무 플로우는 [docs/FLOW.md](docs/FLOW.md) 참고.

---

## 문서 목록

| 파일 | 설명 |
|------|------|
| [docs/REQUIREMENTS.md](docs/REQUIREMENTS.md) | 기능 요구사항 및 비기능 요구사항 |
| [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) | MVC 아키텍처 설계 및 레이어 책임 |
| [docs/FLOW.md](docs/FLOW.md) | 업무 플로우 및 상태 전이 다이어그램 |
| [docs/CLASS_DESIGN.md](docs/CLASS_DESIGN.md) | 클래스 설계, 파일 구조, 의존 관계 |

---

## 프로젝트 구조 (예정)

```
ConsoleMVC/
├── ConsoleMVC.slnx
└── ConsoleMVC/
    ├── main.cpp
    ├── Models/          # 도메인 데이터 구조
    ├── Views/           # 콘솔 출력 레이어
    ├── Controllers/     # 유스케이스 오케스트레이션
    ├── Services/        # 비즈니스 로직
    └── Database/        # 인메모리 DB 시뮬레이션
```

---

## 빌드 및 실행

```powershell
# Visual Studio 2022에서 ConsoleMVC.slnx 열기 후 빌드
# 또는 MSBuild CLI
msbuild ConsoleMVC\ConsoleMVC.slnx /p:Configuration=Debug /p:Platform=x64
```
