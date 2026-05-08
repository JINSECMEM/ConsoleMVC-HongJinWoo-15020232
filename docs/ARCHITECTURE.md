# 아키텍처 설계 (Architecture)

## 1. 레이어 구조

```
┌─────────────────────────────────────────────────────────┐
│                        View Layer                        │
│   ConsoleView / OrderView / InventoryView / MonitorView  │
│            (콘솔 출력·입력 담당, 상태 표시)                │
└────────────────────────┬────────────────────────────────┘
                         │  사용자 이벤트 전달
                         ▼
┌─────────────────────────────────────────────────────────┐
│                    Controller Layer                      │
│  OrderController / InventoryController                   │
│  ProductionController / ShipmentController               │
│            (유스케이스 오케스트레이션)                     │
└──────────┬──────────────────────────┬───────────────────┘
           │ 비즈니스 로직 위임         │ 모델 읽기/쓰기
           ▼                          ▼
┌──────────────────┐       ┌──────────────────────────────┐
│  Service Layer   │       │         Model Layer           │
│  OrderService    │       │  Order / Sample / Inventory   │
│  InventoryService│       │  ProductionLine / Shipment    │
│  ProductionService       │  (도메인 데이터 + 상태)        │
└──────────┬───────┘       └──────────────────────────────┘
           │ CRUD
           ▼
┌─────────────────────────────────────────────────────────┐
│                    Database Layer                        │
│        OrderDB / InventoryDB / ProductionDB              │
│           (인메모리 컨테이너, 추후 실DB 교체 가능)          │
└─────────────────────────────────────────────────────────┘
```

---

## 2. 레이어별 책임

### 2.1 Model Layer

- **역할**: 도메인 데이터 구조와 상태 보유. 비즈니스 규칙에 무관한 순수 데이터 클래스.
- **의존**: 없음 (다른 레이어를 참조하지 않는다)
- **주요 클래스**:

| 클래스 | 책임 |
|--------|------|
| `Order` | 주문 ID, 시료 ID, 수량, 상태, 납기일 |
| `Sample` | 시료 ID, 시료명, 생산라인 ID, 단위 생산시간 |
| `Inventory` | 시료별 재고 수량, 안전 재고 임계값 |
| `ProductionLine` | 라인 ID, 상태, 현재 작업(Job), 진행률 |
| `ProductionJob` | 작업 ID, 연결 주문 ID, 목표 수량, 완료 수량 |
| `Shipment` | 출고 ID, 주문 ID, 출고 수량, 출고 일시 |

### 2.2 Database Layer

- **역할**: 인메모리 CRUD 저장소. `std::map` 기반 단순 구현으로 실제 DB 인터페이스 모사.
- **의존**: Model Layer만 의존

```cpp
// 인터페이스 예시
class IOrderRepository {
public:
    virtual void Save(const Order&) = 0;
    virtual Order FindById(const std::string& id) = 0;
    virtual std::vector<Order> FindAll() = 0;
    virtual void Update(const Order&) = 0;
};
```

### 2.3 Service Layer

- **역할**: 도메인 비즈니스 로직 캡슐화. Controller가 직접 DB를 건드리지 않도록 중개.
- **의존**: Model Layer, Database Layer

| 서비스 | 핵심 메서드 |
|--------|-----------|
| `OrderService` | `RegisterOrder()`, `CancelOrder()`, `UpdateStatus()` |
| `InventoryService` | `CheckStock()`, `Deduct()`, `Replenish()` |
| `ProductionService` | `RequestProduction()`, `TickProgress()`, `CompleteJob()` |
| `ShipmentService` | `ProcessShipment()`, `RetryPending()` |

### 2.4 Controller Layer

- **역할**: 사용자 액션을 받아 Service를 호출하고, 결과를 View에 전달.
- **의존**: Service Layer, View Layer

```
사용자 입력 → Controller.HandleXxx()
    → Service.Xxx()          (비즈니스 처리)
    → Model 상태 변경
    → View.Render()          (결과 출력)
```

### 2.5 View Layer

- **역할**: 콘솔 입출력 전담. Model 데이터를 포매팅하여 출력. 사용자 입력 수집.
- **의존**: Model Layer (읽기 전용), Controller Layer (이벤트 전달)

---

## 3. MVC 데이터 흐름

```
[사용자: 주문 등록 입력]
        │
        ▼
  OrderView.GetInput()
        │  입력 DTO 반환
        ▼
  OrderController.RegisterOrder(dto)
        │
        ├─► OrderService.RegisterOrder()   → OrderDB.Save()
        │
        ├─► InventoryService.CheckStock()
        │         │
        │    [재고 충분]──► ShipmentService.ProcessShipment()
        │    [재고 부족]──► ProductionService.RequestProduction()
        │
        └─► OrderView.RenderOrderResult()
              InventoryView.RenderStock()
```

---

## 4. 생산라인 시뮬레이션 구조

PoC에서는 실제 생산 지연을 타이머로 시뮬레이션한다.

```
AppLoop (메인 루프, 1초 tick)
    │
    └─► ProductionController.Tick()
              │
              └─► 각 ProductionLine 진행률 +N%
                        │ 100% 도달 시
                        └─► ProductionService.CompleteJob()
                                  ├─► InventoryService.Replenish()
                                  └─► ShipmentService.RetryPending()
```

---

## 5. 확장 고려사항

| 항목 | PoC | 실제 시스템 |
|------|-----|-----------|
| DB | `std::map` 인메모리 | SQLite / Oracle |
| 생산 시뮬레이션 | 타이머 루프 | PLC/MES 연동 |
| 동시성 | 싱글 스레드 루프 | `std::thread` + 뮤텍스 |
| 알림 | 콘솔 출력 | 이메일 / SMS / 대시보드 |
| UI | Text Menu | Web UI / WPF |
