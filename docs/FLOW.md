# 업무 플로우 (Business Flow)

## 1. 전체 흐름 개요

```
고객 주문 입력
     │
     ▼
[주문 등록] ──────────────────────────────────── OrderDB 저장
     │                                            상태: PENDING
     ▼
[재고 확인] ──────────────────────────────────── InventoryDB 조회
     │
     ├── 재고 >= 주문 수량 ─────────────────────► [즉시 출고 처리]
     │                                                  │
     │                                            재고 차감
     │                                            Shipment 생성
     │                                            상태: SHIPPED
     │
     └── 재고 < 주문 수량 ──────────────────────► [생산 요청]
                                                       │
                                                 생산라인 Job 등록
                                                 상태: AWAITING_PROD
                                                       │
                                                 [생산라인 가동]
                                                 상태: IN_PRODUCTION
                                                       │
                                                 생산 진행 (타이머)
                                                       │
                                                 생산 완료
                                                 재고 증가
                                                       │
                                                 [출고 재시도]
                                                 재고 차감
                                                 Shipment 생성
                                                 상태: SHIPPED
```

---

## 2. 주문 등록 플로우 (FR-01)

```
시작
  │
  ▼
콘솔 메뉴: [1] 주문 등록 선택
  │
  ▼
입력 수집
  ├─ 고객 ID
  ├─ 시료 ID (S-001 / S-002 / S-003)
  ├─ 주문 수량
  └─ 납기일
  │
  ▼
입력 유효성 검사
  ├─ 실패 → 오류 메시지 출력 → 재입력
  └─ 성공 ↓
  │
  ▼
OrderService.RegisterOrder()
  │
  ├─ 주문 ID 생성 (ORD-YYYYMMDD-XXXX)
  ├─ OrderDB.Save()
  └─ 상태 = PENDING
  │
  ▼
InventoryService.CheckStock(시료ID, 수량)
  │
  ├─ 재고 충분 → ShipmentController.ProcessShipment()
  └─ 재고 부족 → ProductionController.RequestProduction()
  │
  ▼
OrderView.RenderResult() — 등록 결과 출력
끝
```

---

## 3. 재고 확인 및 출고 플로우 (FR-02, FR-03)

```
InventoryService.CheckStock(sampleId, qty)
  │
  ├─ currentStock >= qty ?
  │      │
  │     [YES] → ShipmentService.ProcessShipment()
  │               ├─ InventoryDB.Deduct(sampleId, qty)
  │               ├─ ShipmentDB.Save(Shipment)
  │               └─ OrderDB.UpdateStatus(SHIPPED)
  │
  └─ [NO] → ProductionService.RequestProduction()
               ├─ ProductionLineDB.AddJob(lineId, qty, orderId)
               └─ OrderDB.UpdateStatus(AWAITING_PROD)
```

### 재고 임계값 경고

```
출고 처리 후:
  remainingStock <= threshold ?
    → View: "[경고] S-001 재고 임계값 이하: 현재 {n}개"
```

---

## 4. 생산라인 플로우 (FR-04)

```
ProductionService.RequestProduction(sampleId, qty, orderId)
  │
  ▼
생산라인 상태 확인
  ├─ MAINTENANCE → 오류 반환 (생산 불가 알림)
  └─ IDLE / RUNNING → Job 큐에 추가
                         │
                         ▼
                    ProductionLine.status = RUNNING
                    OrderDB.UpdateStatus(IN_PRODUCTION)

[AppLoop Tick — 1초마다]
  │
  ▼
ProductionController.Tick()
  │
  └─ 각 RUNNING 생산라인:
       progress += progressPerTick
         │
         ├─ progress < 100% → 계속
         └─ progress >= 100% → CompleteJob()
                                  ├─ InventoryService.Replenish(sampleId, qty)
                                  ├─ ProductionLine.status = IDLE
                                  └─ ShipmentService.RetryPending(orderId)
```

---

## 5. 주문 상태 전이 다이어그램

```
                     취소 요청
   ┌──────────────────────────────────────────────────────────────┐
   │                                                              ▼
[PENDING] ──재고확인중──► [CHECKING]                        [CANCELLED]
                              │
               ┌──────────────┴──────────────┐
               │ 재고 충분                    │ 재고 부족
               ▼                             ▼
        [READY_TO_SHIP]             [AWAITING_PROD]
               │                             │
          출고 처리                    생산 요청됨
               │                             │
               ▼                      생산 시작
          [SHIPPED]◄──────────────[IN_PRODUCTION]
                        생산완료
                        + 출고처리
```

---

## 6. 모니터링 대시보드 갱신 주기

| 항목 | 갱신 주기 |
|------|---------|
| 주문 현황 (상태별 건수) | 이벤트 발생 시 |
| 재고 현황 테이블 | 이벤트 발생 시 |
| 생산라인 진행률 | AppLoop Tick (1초) |
| 이벤트 로그 | 이벤트 발생 시 (최근 20줄) |

---

## 7. 콘솔 메뉴 구조

```
============================================
  반도체 시료 생산 관리 시스템 v1.0 (PoC)
============================================
[1] 주문 등록
[2] 주문 목록 조회
[3] 주문 취소
[4] 재고 현황 조회
[5] 생산라인 현황 조회
[6] 출고 이력 조회
[7] 모니터링 대시보드
[0] 종료
--------------------------------------------
선택 >
```

### 모니터링 대시보드 화면 예시

```
========== MONITORING DASHBOARD ==========
[주문 현황]
  PENDING: 2  |  IN_PRODUCTION: 1  |  SHIPPED: 5  |  CANCELLED: 0

[재고 현황]
  시료 ID   시료명          재고   임계값  상태
  S-001     DRAM-8Gb        1200   500     OK
  S-002     NAND-128Gb       320   500    [!] LOW
  S-003     Logic-SOC        800   300     OK

[생산라인]
  라인     상태       현재 작업         진행률
  LINE-A   IDLE       -                 -
  LINE-B   RUNNING    JOB-20260508-003  [████████░░] 78%
  LINE-C   IDLE       -                 -

[최근 이벤트]
  08:32:10  주문 등록   ORD-20260508-012  S-002  200개
  08:32:11  재고 부족   S-002  현재320 < 주문200 (안전재고: 500)
  08:32:12  생산 요청   LINE-B  JOB-20260508-003  200개
  08:33:05  출고 완료   ORD-20260508-011  S-001  150개
==========================================
```
