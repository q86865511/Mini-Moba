# PROJECT.md — 簡化版 MOBA（原生 C++，單機優先）

> 這份文件是整個專案的指南。每次開啟 Claude Code，第一件事就是叫它讀這份文件：
> 「請先閱讀 PROJECT.md，理解專案目標與目前進度，然後告訴我我們現在應該做哪一階段。」

---

## 1. 專案目標

做一個**原生桌面程式**（先 Windows）的簡化版 MOBA，靈感來自 League of Legends。
**先做出功能完整、本地能玩的單機版**（兵線、塔、主堡、中立資源、技能、經濟、勝負），
**之後再加多人連線**。刻意貼近 LoL 真正的做法：C++ 引擎 + Lua 內容腳本 + 原生客戶端。

**核心理念：循序漸進，每一階段都要做出能實際執行、能測試的東西，才進下一階段。不要跳階段。**

---

## 2. 技術棧（Tech Stack）

- **語言**：C++（C++17）
- **繪圖 / 視窗 / 輸入 / 音效**：Raylib **5.5**（CMake FetchContent 靜態連結；vcpkg 的 6.0 在此機器 `EndDrawing` 失效＝白畫面，故固定 5.5）
- **美術 / 音效**：使用**本機生成的原創 MOBA 素材包**（`assets/moba_asset_pack`：2048² 三線地圖 + 英雄；`assets/moba_interaction_pack`：動畫 sprite sheet，含英雄/小兵/野怪/塔/主堡，附 `_actions.json` 影格與碰撞資料）；音效程序合成。日後可再用程序生成特效或本機 AI 升級。
- **資料**：JSON（`nlohmann/json`）放英雄 / 技能 / 道具 / 地圖數值
- **腳本層**：Lua + sol2（中後期才導入，學習目標，貼近 LoL 內容層）
- **網路**：ENet（為遊戲設計的可靠 UDP）— **最後一階段才做**
- **建置**：CMake；raylib 用 FetchContent（固定 5.5），其餘函式庫用 vcpkg（manifest 模式）
- **編譯器 / IDE**：Visual Studio 2026 Community（MSVC）
- **版本控制**：Git（GitHub: q86865511/Mini-Moba）

### 這台機器的工具鏈位置（VS 2026 全內建，不需另裝）
- cmake：`E:\VSstudio\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe`（4.2.3）
- vcpkg 工具鏈：`E:\VSstudio\VC\vcpkg\scripts\buildsystems\vcpkg.cmake`（`VCPKG_ROOT=E:\vcpkg`）
- git：`E:\Git\cmd\git.exe`
- CMake 預設產生器：`Visual Studio 18 2026`
- 注意：Smart App Control 已關閉（否則每次新編譯的未簽署 exe 會被擋）。

---

## 3. 架構原則（重要）

- **邏輯與渲染分離**：遊戲模擬放在 `shared` 純邏輯靜態庫（實體、移動、戰鬥、`Tick(dt)`），
  **不依賴 Raylib、不依賴網路**。`client` 只負責輸入、渲染、音效、資產。**邏輯只寫一次**——
  單機現在用它，之後多人連線時伺服器原封不動重用它。
- **物件導向實體階層**：`Entity` 基底類別（id, team, pos, hp, `Update()`…）→ `Hero` / `Minion`
  / `Tower` / `Nexus` / `NeutralMonster` / `Projectile` 繼承。`World` 持有實體集合並逐一更新。
- **模組化 client**：`Game` / `AssetManager` / `Renderer` / `Camera` / `Input` / `Hud` / `Audio`
  各司其職；`main.cpp` 只負責啟動。`shared::Vec2` ↔ raylib `Vector2` 只在 client 邊界轉換。
- **固定時間步長**：模擬以固定 dt 推進（決定性、之後可無痛搬到伺服器）。
- **技能可腳本化**：引擎能力做成「動詞」介面（SpawnProjectile / DealDamage / ApplyEffect…），
  早期由 C++ 技能呼叫，中後期用 sol2 開放給 Lua。
- **（多人階段才適用）伺服器權威 + 快照插值**：屆時把 `shared` 接到權威伺服器，client 改薄、
  只送輸入收快照並插值。

---

## 4. 開發階段（Phases）— 單機優先

> 一次一階段、做完通過驗收才前進。每階段都要本地能跑、能測。

### 階段 0：工具鏈 + 視窗 ✅（已完成）
### 階段 1：`shared` 模擬 + 右鍵移動 ✅（已完成）

### 階段 2：OO 重構 + 資產管線 + 攝影機
- `shared` 重構為 `Entity` 基底 + `Hero`；`World` 持有實體集合。
- `client` 拆成模組（`Game` / `AssetManager` / `Renderer` / `Camera` / `Input`）。
- 首批資產（程序生成為主）：地圖、英雄、UI 字型、移動音效、背景音樂。
- Camera2D 跟隨英雄、地圖比畫面大。
- **驗收**：英雄在有美術的較大地圖上右鍵移動、攝影機跟隨、有背景音樂與音效；程式碼 OO/模組化。

### 階段 3：戰鬥核心 — HP、普攻、技能、特效、音效
- `Entity` 的 HP/血條；左鍵普攻、技能 Q（skillshot）含 CD/傷害；`Projectile`。
- 命中粒子、傷害飄字、攻擊/命中音效；放一個攻擊假人。
- **驗收**：用普攻與 Q 打掉假人血量、有打擊特效與音效。

### 階段 4：兵線 + 防禦塔 + 主堡（PvE 推塔破堡）
- `Minion`（沿 lane waypoint 走、自動攻擊）成波生成；`Tower`；`Nexus`（主堡，被毀＝該方輸）。
  紅藍兩方兵線交戰，玩家在藍方推進。
- **驗收**：紅藍小兵交戰、塔會攻擊、跟兵推塔、摧毀紅方主堡＝勝利。

### 階段 5：中立資源 + 經濟成長
- `NeutralMonster`（野怪，給金錢/經驗）；擊殺得金錢/經驗、等級提升、技能升級；商店買裝、
  道具影響數值；HUD（金錢/等級/CD/物品欄/小地圖）。
- **驗收**：補兵/打野賺錢、買裝變強、升級。
- （選配學習目標）此處或階段 6 導入 Lua(sol2) 把技能改成腳本。

### 階段 6：完整一場 + 打磨
- 遊戲狀態機（選單/遊玩/勝負畫面）、完整 HUD/小地圖/計分；美術/音效/視覺打磨；平衡。
- **驗收**：能從頭打完一場功能性單機 MOBA 並分出勝負。

### 階段 7（之後）：敵方英雄 AI
- 敵方英雄 bot（補兵、推線、用技能、撤退），狀態機/行為樹。

### 階段 8（最後）：多人連線
- 把 `shared` 接到權威伺服器 + ENet + 快照插值；client 改薄。因 `shared` 一直純邏輯，這是
  「換驅動者」而非重寫。

---

## 5. 給 Claude Code 的工作規則

1. **一次只做一個階段**，做完並通過驗收標準才進下一步。
2. **每完成一個可運作的功能就提醒我 git commit**。
3. **動手前先簡短解釋你打算怎麼做**，我同意後再寫。
4. 寫完一段就告訴我**如何在本機測試**（要跑什麼指令）。
5. 遇到我貼上的錯誤訊息，先解釋成因再修。
6. 不確定的設計決策，**先問我**，不要自己假設規模或方向。
7. 維持「邏輯與渲染分離（`shared` 純邏輯）」「物件導向 + 模組化」這兩個架構原則。
8. C++ 經驗：我「寫過一點」，碰到指標 / 記憶體 / CMake 等實務點請適度解釋。

---

## 6. 目前進度

- [x] 階段 0：工具鏈 + 視窗
- [x] 階段 1：shared 模擬 + 右鍵移動
- [x] 階段 2：OO 重構 + 資產管線 + 攝影機
- [x] 階段 3：戰鬥核心（HP / 普攻 / 技能 / 特效音效）
- [ ] 階段 4：兵線 + 塔 + 主堡（PvE 推塔）
- [ ] 階段 5：中立資源 + 經濟成長
- [ ] 階段 6：完整一場 + 打磨
- [ ] 階段 7：敵方英雄 AI
- [ ] 階段 8：多人連線

> 每完成一階段，把對應的 [ ] 改成 [x]，並在下面記一句當天做了什麼，方便下次接續。

### 開發日誌
- 2026-06-04：完成技術選型討論（從原本 web/JS 改為原生 C++）。確認 VS2026 內建工具鏈可用。開始階段 0。
- 2026-06-04：完成階段 0。建立 CMake 專案骨架，client 用 raylib 開出視窗（方塊跟隨滑鼠）。踩到 raylib 6.0 在本機 `EndDrawing` 失效（白畫面＋無回應＋40 萬 FPS 空轉），改用 FetchContent 固定 raylib 5.5 後正常。
- 2026-06-04：完成階段 1。建立 `shared` 純模擬庫（`Vec2` / `Hero` / `World::Tick`，零 raylib／零網路）；client 固定時間步長、右鍵移動、格線地圖。期間關閉 Smart App Control 解決 exe 被擋。
- 2026-06-04：**改方向為「單機優先」**——先做出能玩的單機 MOBA 再連線，並織入美術/音效、要求 OO + 模組化。重排 roadmap（階段 2 起為單機內容，多人連線移到階段 8）。現有 `shared`/`client` 架構正好支援，不需重來。
- 2026-06-04：完成階段 2。`shared` 重構為 OO（`Entity` 基底 + `Hero`，`World` 持有 `vector<unique_ptr<Entity>>`）；client 拆成 `Game`/`AssetManager`/`Renderer`/`GameCamera`/`Input`/`Audio` 模組；程序生成英雄與草地貼圖、移動音效與環境背景音樂；Camera2D 跟隨英雄、右鍵移動。
- 2026-06-04：改用使用者自製的**原創素材包**（移除先前 Kenney 試用素材與下載）。接入 `moba_asset_pack` 三線地圖當世界背景（2048²）、`moba_interaction_pack` 的 `ember_vanguard` 動畫英雄（idle/move）；新增 `AnimatedSprite` 模組（sprite sheet 動畫）、`AssetManager` 改載入新包、CMake 增量複製 `assets/`。英雄起點在藍方基地。截圖驗證地圖與英雄渲染正常。
- 2026-06-04：完成階段 3（戰鬥核心）。資料驅動動畫（讀 `_actions.json` → `AnimationSet`/`AnimationPlayer` + per-entity `ViewRegistry`，載入全部 18 sprite sheet）；`Entity` 戰鬥屬性、`World::DealDamage`/`SpawnProjectile`、`Projectile`；`Hero` 範圍自動普攻 + Q skillshot（CD）；傷害飄字 + 命中環 + 命中/施法音效 + 受擊/死亡動畫；紅方假人會反擊。新增無 GUI 戰鬥單元測試 `tests/combat_test.cpp`（全通過）。

---

## 7. 如何建置與執行

```powershell
# 設定（第一次會用 FetchContent 下載並編譯 raylib 5.5，需等幾分鐘）
$cmake = 'E:\VSstudio\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe'
& $cmake --preset default          # 用 CMakePresets.json 的 default 設定檔

# 建置
& $cmake --build build --config Debug

# 執行
.\build\client\Debug\client.exe
```
