# 클래스 설계 (Class Design)

## 1. 파일 구조

```
ConsoleMVC/ConsoleMVC/
│
├── main.cpp                          # 진입점, AppLoop
│
├── Models/
│   ├── Order.h                       # 주문 도메인 모델
│   ├── Sample.h                      # 시료 도메인 모델
│   ├── Inventory.h                   # 재고 도메인 모델
│   ├── ProductionLine.h              # 생산라인 도메인 모델
│   ├── ProductionJob.h               # 생산 작업 도메인 모델
│   └── Shipment.h                    # 출고 도메인 모델
│
├── Database/
│   ├── IRepository.h                 # CRUD 인터페이스 (template)
│   ├── OrderRepository.h/.cpp
│   ├── SampleRepository.h/.cpp
│   ├── InventoryRepository.h/.cpp
│   ├── ProductionRepository.h/.cpp
│   └── ShipmentRepository.h/.cpp
│
├── Services/
│   ├── OrderService.h/.cpp
│   ├── InventoryService.h/.cpp
│   ├── ProductionService.h/.cpp
│   └── ShipmentService.h/.cpp
│
├── Controllers/
│   ├── AppController.h/.cpp          # 메뉴 라우팅, AppLoop 관리
│   ├── OrderController.h/.cpp
│   ├── InventoryController.h/.cpp
│   ├── ProductionController.h/.cpp
│   └── ShipmentController.h/.cpp
│
└── Views/
    ├── ConsoleHelper.h/.cpp          # 화면 지우기, 색상, 테이블 포매터
    ├── MenuView.h/.cpp               # 메인 메뉴 출력/입력
    ├── OrderView.h/.cpp
    ├── InventoryView.h/.cpp
    ├── ProductionView.h/.cpp
    ├── ShipmentView.h/.cpp
    └── MonitorView.h/.cpp            # 실시간 대시보드
```

---

## 2. Model 클래스

### Order.h

```cpp
enum class OrderStatus {
    PENDING,
    CHECKING,
    READY_TO_SHIP,
    AWAITING_PROD,
    IN_PRODUCTION,
    SHIPPED,
    CANCELLED
};

struct Order {
    std::string orderId;      // ORD-YYYYMMDD-XXXX
    std::string customerId;
    std::string sampleId;
    int         quantity;
    std::string dueDate;      // YYYY-MM-DD
    OrderStatus status;
    std::string createdAt;
    std::string updatedAt;
};
```

### Sample.h

```cpp
struct Sample {
    std::string sampleId;     // S-001
    std::string name;         // DRAM-8Gb
    std::string lineId;       // LINE-A
    int         cycleTimeSec; // 단위 생산 소요 시간(초)
};
```

### Inventory.h

```cpp
struct Inventory {
    std::string sampleId;
    int         stock;         // 현재 재고
    int         threshold;     // 안전 재고 임계값
    std::string lastUpdated;
};
```

### ProductionLine.h

```cpp
enum class LineStatus { IDLE, RUNNING, PAUSED, MAINTENANCE };

struct ProductionLine {
    std::string lineId;        // LINE-A
    std::string sampleId;      // 전담 시료
    LineStatus  status;
    std::string currentJobId;  // 빈 문자열이면 없음
    int         progressPct;   // 0~100
};
```

### ProductionJob.h

```cpp
enum class JobStatus { QUEUED, RUNNING, COMPLETED, FAILED };

struct ProductionJob {
    std::string jobId;         // JOB-YYYYMMDD-XXXX
    std::string orderId;
    std::string sampleId;
    std::string lineId;
    int         targetQty;
    int         completedQty;
    JobStatus   status;
    std::string startedAt;
    std::string completedAt;
};
```

### Shipment.h

```cpp
struct Shipment {
    std::string shipmentId;    // SHP-YYYYMMDD-XXXX
    std::string orderId;
    std::string sampleId;
    int         quantity;
    std::string shippedAt;
};
```

---

## 3. Repository (Database Layer)

### IRepository.h

```cpp
template<typename T, typename KeyType = std::string>
class IRepository {
public:
    virtual ~IRepository() = default;
    virtual void           Save(const T& entity) = 0;
    virtual T              FindById(const KeyType& id) = 0;
    virtual std::vector<T> FindAll() = 0;
    virtual void           Update(const T& entity) = 0;
    virtual void           Delete(const KeyType& id) = 0;
};
```

### OrderRepository.h

```cpp
class OrderRepository : public IRepository<Order> {
public:
    void           Save(const Order& order) override;
    Order          FindById(const std::string& id) override;
    std::vector<Order> FindAll() override;
    std::vector<Order> FindByStatus(OrderStatus status);
    void           Update(const Order& order) override;
    void           Delete(const std::string& id) override;
private:
    std::map<std::string, Order> store_;
};
```

---

## 4. Service Layer

### OrderService.h

```cpp
class OrderService {
public:
    explicit OrderService(OrderRepository& repo);

    Order       RegisterOrder(const std::string& customerId,
                              const std::string& sampleId,
                              int qty,
                              const std::string& dueDate);
    bool        CancelOrder(const std::string& orderId);
    void        UpdateStatus(const std::string& orderId, OrderStatus newStatus);
    std::vector<Order> GetAllOrders();
    Order       GetOrder(const std::string& orderId);

private:
    OrderRepository& repo_;
    std::string GenerateOrderId();
};
```

### InventoryService.h

```cpp
class InventoryService {
public:
    explicit InventoryService(InventoryRepository& repo);

    bool  CheckStock(const std::string& sampleId, int requiredQty);
    void  Deduct(const std::string& sampleId, int qty);
    void  Replenish(const std::string& sampleId, int qty);
    bool  IsBelowThreshold(const std::string& sampleId);
    int   GetStock(const std::string& sampleId);

private:
    InventoryRepository& repo_;
};
```

### ProductionService.h

```cpp
class ProductionService {
public:
    explicit ProductionService(ProductionRepository& repo);

    ProductionJob RequestProduction(const std::string& sampleId,
                                   int qty,
                                   const std::string& orderId);
    void          Tick();           // AppLoop에서 1초마다 호출
    void          CompleteJob(const std::string& jobId);
    ProductionLine GetLine(const std::string& lineId);
    std::vector<ProductionLine> GetAllLines();

    // 생산 완료 콜백 등록 (Controller가 주입)
    using OnCompleteCallback = std::function<void(const ProductionJob&)>;
    void SetOnCompleteCallback(OnCompleteCallback cb);

private:
    ProductionRepository& repo_;
    OnCompleteCallback    onComplete_;
    std::string GenerateJobId();
};
```

### ShipmentService.h

```cpp
class ShipmentService {
public:
    ShipmentService(ShipmentRepository& shipRepo,
                    InventoryService&   invSvc,
                    OrderService&       ordSvc);

    bool ProcessShipment(const std::string& orderId);
    void RetryPending(const std::string& orderId);
    std::vector<Shipment> GetHistory();

private:
    ShipmentRepository& shipRepo_;
    InventoryService&   invSvc_;
    OrderService&       ordSvc_;
    std::string GenerateShipmentId();
};
```

---

## 5. Controller Layer

### AppController.h

```cpp
class AppController {
public:
    AppController();
    void Run();   // 메인 루프 진입점

private:
    void HandleMenu(int choice);
    void Tick();  // 생산 진행률 갱신, 화면 갱신

    // 하위 컨트롤러 (의존성 주입)
    OrderController      orderCtrl_;
    InventoryController  invCtrl_;
    ProductionController prodCtrl_;
    ShipmentController   shipCtrl_;

    // 공유 서비스 (단일 인스턴스)
    OrderRepository      orderRepo_;
    InventoryRepository  invRepo_;
    ProductionRepository prodRepo_;
    ShipmentRepository   shipRepo_;

    OrderService      orderSvc_;
    InventoryService  invSvc_;
    ProductionService prodSvc_;
    ShipmentService   shipSvc_;

    MenuView    menuView_;
    MonitorView monitorView_;
    bool        running_ = true;
};
```

### OrderController.h

```cpp
class OrderController {
public:
    OrderController(OrderService& ordSvc,
                    InventoryService& invSvc,
                    ProductionService& prodSvc,
                    ShipmentService& shipSvc,
                    OrderView& view);

    void RegisterOrder();   // View에서 입력 받아 서비스 호출
    void ListOrders();
    void CancelOrder();

private:
    OrderService&      ordSvc_;
    InventoryService&  invSvc_;
    ProductionService& prodSvc_;
    ShipmentService&   shipSvc_;
    OrderView&         view_;
};
```

---

## 6. View Layer

### ConsoleHelper.h

```cpp
namespace ConsoleHelper {
    void ClearScreen();
    void SetColor(int colorCode);
    void ResetColor();
    void PrintTable(const std::vector<std::string>& headers,
                    const std::vector<std::vector<std::string>>& rows);
    void PrintProgressBar(int pct, int width = 20);
    void PrintDivider(char ch = '-', int width = 60);
}
```

### MonitorView.h

```cpp
class MonitorView {
public:
    void Render(const std::vector<Order>&         orders,
                const std::vector<Inventory>&     inventories,
                const std::vector<ProductionLine>& lines,
                const std::vector<std::string>&   eventLog);
private:
    void RenderOrderSummary(const std::vector<Order>& orders);
    void RenderInventoryTable(const std::vector<Inventory>& inventories);
    void RenderProductionLines(const std::vector<ProductionLine>& lines);
    void RenderEventLog(const std::vector<std::string>& logs);
};
```

---

## 7. 의존 관계 요약

```
main.cpp
  └─► AppController
        ├─► [Repositories] ──► [Models]
        ├─► [Services]     ──► [Repositories]
        ├─► [Controllers]  ──► [Services] + [Views]
        └─► [Views]        ──► [Models] (read-only)
```

**핵심 규칙**
- Model은 아무것도 참조하지 않는다
- View는 Controller를 직접 호출하지 않는다 (Controller가 View를 호출)
- Service는 View를 참조하지 않는다
- Repository는 Service/Controller를 참조하지 않는다

---

## 8. 식별자 생성 규칙

| 유형 | 형식 | 예시 |
|------|------|------|
| 주문 ID | `ORD-YYYYMMDD-NNNN` | ORD-20260508-0001 |
| 작업 ID | `JOB-YYYYMMDD-NNNN` | JOB-20260508-0003 |
| 출고 ID | `SHP-YYYYMMDD-NNNN` | SHP-20260508-0002 |
| 시료 ID | `S-NNN` | S-001, S-002, S-003 |
| 라인 ID | `LINE-X` | LINE-A, LINE-B, LINE-C |
