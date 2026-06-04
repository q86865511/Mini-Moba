# PROJECT.md — 簡化版多人 MOBA（原生 C++）

> 這份文件是整個專案的指南。每次開啟 Claude Code，第一件事就是叫它讀這份文件：
> 「請先閱讀 PROJECT.md，理解專案目標與目前進度，然後告訴我我們現在應該做哪一階段。」

---

## 1. 專案目標

做一個**原生桌面程式**（先 Windows）、能跟朋友連線對戰的簡化版 MOBA。
靈感來自 League of Legends，刻意簡化，重點是「能跑、能連線、能跟朋友玩」。
這條路線刻意貼近 LoL 真正的做法：**C++ 引擎 + Lua 內容腳本 + 原生客戶端**。

**核心理念：循序漸進，每一階段都要做出能實際執行、能測試的東西，才進下一階段。不要跳階段。**

---

## 2. 技術棧（Tech Stack）

- **語言**：C++（C++17）
- **繪圖 / 視窗 / 輸入 / 音效**：Raylib
- **網路**：ENet（為遊戲設計的可靠 UDP）
- **腳本層**：Lua + sol2（中後期才導入，學習目標，貼近 LoL 內容層）
- **資料**：JSON（`nlohmann/json`）放英雄 / 技能 / 道具 / 地圖數值，前後端共讀
- **建置**：CMake + vcpkg（manifest 模式）
- **編譯器 / IDE**：Visual Studio 2026 Community（MSVC）
- **版本控制**：Git

### 這台機器的工具鏈位置（VS 2026 全內建，不需另裝）
- cmake：`E:\VSstudio\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe`（4.2.3）
- ninja：`E:\VSstudio\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe`
- vcpkg 工具鏈：`E:\VSstudio\VC\vcpkg\scripts\buildsystems\vcpkg.cmake`
- git：`E:\Git\cmd\git.exe`
- CMake 預設產生器：`Visual Studio 18 2026`（用它就不必先進 Developer 環境）

---

## 3. 架構原則（非常重要，連線階段必須遵守）

- **伺服器權威（Server-authoritative）**：所有遊戲狀態的「真相」都在伺服器端。
  客戶端只負責「輸入」與「顯示」，不可以自己決定血量、傷害、位置等結果。
- **客戶端只送輸入、不送結果**：例如送「我想往這座標移動」，而不是「我現在在這座標」。
- **前後端都是 C++ → 模擬邏輯做成共用庫 `shared`**：`shared` 是純模擬靜態庫（實體、移動、
  傷害、`Tick(dt)`、快照），**不依賴 Raylib、不依賴 ENet**。伺服器拿它當權威；客戶端也
  連結它（之後若想做預測可直接重用）。**邏輯只寫一次。**
- **依賴衛生：Raylib 只在 client**。`shared` 純邏輯、`server` = shared + ENet（+ 之後 Lua）、
  `client` = shared + Raylib + ENet。如此伺服器無圖形依賴、可在 Linux VPS 編譯部署。
- **固定 tick + 快照插值**：伺服器固定頻率（先 20Hz）模擬並廣播快照；客戶端 60fps 繪圖，
  在最近兩個快照之間插值（顯示約 100ms 前的狀態），讓他人移動平滑。**不做**客戶端預測（之後可選）。
- **技能可腳本化**：引擎能力做成乾淨的「動詞」介面（SpawnProjectile / DealDamage…），
  早期由 C++ 技能原型呼叫，中後期用 sol2 原封不動開放給 Lua。

---

## 4. 開發階段（Phases）

### 階段 0：工具鏈 + 一個視窗（暖身）
**目標**：把「能編譯、能跑、能裝依賴、能除錯」這條路打通（原生最大的門檻）。
- vcpkg + CMake 拉 Raylib，開出視窗，畫一個跟著滑鼠的方塊；git init
- **驗收**：`cmake --build` 成功、跑出視窗、方塊會動、能下中斷點除錯。

### 階段 1：`shared` 模擬 + 點擊移動（本機，先學遊戲迴圈）
**目標**：螢幕上有一個能動的角色。
- 建 `shared` 庫：World + 一個 Hero + `Tick(dt)`（朝目標移動）
- client 暫時本機跑 `shared`：右鍵設目標、Raylib 畫地圖（格線）+ 角色、固定時間步長
- 說明：本機跑模擬只是為了先學遊戲迴圈；下一階段伺服器**原封不動重用這份 `shared`**
- **驗收**：右鍵讓角色在地圖上順暢移動。

### 階段 2：接上 ENet，模擬移到伺服器（伺服器權威 + 插值）
**目標**：客戶端變薄，伺服器當權威。
- 建 server（無畫面）跑 `shared` 當權威，固定 20Hz tick + ENet 廣播快照
- client 改為：送「移動到 (x,y)」、收快照、插值畫出來（不再本機模擬）
- **驗收**：一個玩家連本機 server，右鍵移動由伺服器算、client 插值顯示；開兩個 client 同連。

### 階段 3：核心 MOBA 機制（全在 `shared` / server）
**目標**：有 MOBA 的基本骨架。
- HP + 血條；技能 Q（經引擎動詞介面，含 CD）；小兵沿 waypoint；防禦塔自動攻擊；金錢 + 極簡商店
- **驗收**：用 Q 擊殺小兵得錢、買裝、被塔攻擊掉血。

### 階段 4：多人 + 房間 + 隊伍（擴到 3v3 多路）
**目標**：多人同場對戰。
- ENet 連線管理 + 房間 / 分隊（3v3）；多英雄互看、多路小兵與塔
- **驗收**：多名玩家（先 2 人測）進同一場，即時互看移動與互打。

### 階段 5：連線打磨（插值，非預測）
**目標**：連線手感順、無法輕易作弊。
- 強化插值 / 快照緩衝 / 掉包容錯；確認所有傷害 / 死亡只在伺服器；用 clumsy 等工具加人工延遲測試
- **驗收**：延遲下仍順；改 client 數值無法影響結果。

### 階段 6：內容與平衡 + Lua 腳本層
**目標**：變得像一款遊戲，並導入 Lua。
- 嵌入 Lua + sol2：技能改 / 新增為 `.lua` 腳本（學 Lua 落點）
- 多英雄（部分用 Lua）；勝利條件（摧毀敵方 Nexus）；UI（血量 / 金錢 / CD / 計分）；音效與打磨；平衡
- **驗收**：能跟朋友打完整一場分勝負；新增英雄只需加資料 + Lua 腳本，不必改伺服器核心。

---

## 5. 給 Claude Code 的工作規則

1. **一次只做一個階段**，做完並通過驗收標準才進下一步。
2. **每完成一個可運作的功能就提醒我 git commit**。
3. **動手前先簡短解釋你打算怎麼做**，我同意後再寫。
4. 寫完一段就告訴我**如何在本機測試**（要跑什麼指令）。
5. 遇到我貼上的錯誤訊息，先解釋成因再修。
6. 不確定的設計決策，**先問我**，不要自己假設規模或方向。
7. 維持「伺服器權威」「邏輯與渲染分離」「Raylib 只在 client」這三個架構原則。
8. C++ 經驗：我「寫過一點」，碰到指標 / 記憶體 / CMake 等實務點請適度解釋。

---

## 6. 目前進度

- [ ] 階段 0：工具鏈 + 視窗
- [ ] 階段 1：shared 模擬 + 點擊移動
- [ ] 階段 2：ENet + 模擬移到伺服器
- [ ] 階段 3：核心 MOBA 機制
- [ ] 階段 4：多人 + 房間 + 隊伍
- [ ] 階段 5：連線打磨
- [ ] 階段 6：內容與平衡 + Lua

> 每完成一階段，把對應的 [ ] 改成 [x]，並在下面記一句當天做了什麼，方便下次接續。

### 開發日誌
- 2026-06-04：完成技術選型討論（從原本 web/JS 改為原生 C++）。確認 VS2026 內建工具鏈可用。開始階段 0。

---

## 7. 如何建置與執行

```powershell
# 設定（第一次會用 vcpkg 下載並編譯 raylib，需等幾分鐘）
$cmake = 'E:\VSstudio\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe'
& $cmake --preset default          # 用 CMakePresets.json 的 default 設定檔

# 建置
& $cmake --build build --config Debug

# 執行（階段 0：客戶端視窗）
.\build\client\Debug\client.exe
```

> 之後有了伺服器：先跑 `server.exe`，再開一個或多個 `client.exe` 連上。
> 跟朋友連線：用 Tailscale 把 server 開出去，或部署到 VPS。
