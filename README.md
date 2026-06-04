# Mini MOBA

一個用**原生 C++** 製作的簡化版 MOBA（靈感來自 *League of Legends*）。
**先做出本地能玩的單機版**（兵線、塔、主堡、中立資源、技能、經濟、勝負），**之後再加多人連線**。
學習專案，刻意貼近 LoL 真正的做法：C++ 引擎 + Lua 內容腳本 + 原生客戶端。

> 重點是學習與「能玩」，不是功能完整。

---

## 技術棧

| 用途 | 選擇 |
|---|---|
| 語言 | C++17 |
| 繪圖 / 視窗 / 輸入 / 音效 | [raylib](https://www.raylib.com/) 5.5（CMake FetchContent，靜態連結） |
| 美術 / 音效 | 程序生成為主（程式碼畫）＋ 免費 CC0（Kenney / itch.io）＋ 選配本機 AI 生圖 |
| 資料 | JSON（nlohmann/json） |
| 腳本層（中後期） | Lua + [sol2](https://github.com/ThePhD/sol2) |
| 網路（最後階段） | [ENet](http://enet.bespin.org/)（為遊戲設計的可靠 UDP） |
| 建置 | CMake + vcpkg |
| 編譯器 | MSVC（Visual Studio 2022 / 2026） |

## 架構原則

- **邏輯與渲染分離**：模擬放在 `shared` 純邏輯庫（**不碰 raylib、不碰網路**），`client` 只負責
  渲染 / 輸入 / 音效 / 資產。**邏輯只寫一次**——單機現在用它，之後多人連線時伺服器原封不動重用。
- **物件導向實體階層**：`Entity` 基底類別 → `Hero` / `Minion` / `Tower` / `Nexus` / `NeutralMonster`
  / `Projectile`；`World` 持有實體集合並逐一更新。
- **模組化 client**：`Game` / `AssetManager` / `Renderer` / `GameCamera` / `Input` / `Audio`。
- **固定時間步長**：模擬以固定 dt 推進（決定性，之後可無痛搬到伺服器）。
- **美術以程序生成為主**：畫面用程式碼畫出（形狀/漸層/生成貼圖），可再用 CC0/AI 升級。
- **（多人階段才適用）伺服器權威 + 快照插值**：屆時把 `shared` 接到權威伺服器，client 改薄。

## 專案結構

```
.
├─ shared/                  純模擬庫（無 raylib／無網路）
│  ├─ include/shared/       Vec2.h, Team.h, Entity.h, World.h, entities/Hero.h
│  └─ src/                  World.cpp, entities/Hero.cpp
├─ client/                  客戶端（raylib）：模組化
│  └─ src/                  main.cpp, Game, AssetManager, Renderer, GameCamera, Input, Audio
├─ CMakeLists.txt           頂層建置（FetchContent 取得 raylib 5.5）
├─ CMakePresets.json        建置設定檔（vcpkg 工具鏈）
├─ vcpkg.json               相依套件清單
└─ PROJECT.md               詳細開發計畫與進度
                            （server/ 會在多人連線階段才新增）
```

## 建置與執行

**需求**：Windows + Visual Studio 2022/2026（勾選「使用 C++ 的桌面開發」工作負載，內含 CMake）、
vcpkg（設好 `VCPKG_ROOT` 環境變數）。第一次設定會用 FetchContent 下載並編譯 raylib 5.5，需等幾分鐘。

```powershell
# 在專案根目錄（cmake 可用 VS 內建的，或加入 PATH）
cmake --preset default
cmake --build build --config Debug

# 執行：右鍵移動、攝影機跟隨，ESC 離開
.\build\client\Debug\client.exe
```

## 開發進度

- [x] **階段 0**：工具鏈 + 視窗
- [x] **階段 1**：`shared` 模擬 + 右鍵移動
- [x] **階段 2**：OO 重構 + 資產管線 + 攝影機
- [ ] 階段 3：戰鬥核心（HP / 普攻 / 技能 / 特效音效）
- [ ] 階段 4：兵線 + 塔 + 主堡（PvE 推塔破堡）
- [ ] 階段 5：中立資源 + 經濟成長
- [ ] 階段 6：完整一場 + 打磨
- [ ] 階段 7：敵方英雄 AI
- [ ] 階段 8：多人連線

完整的開發計畫、架構說明與每階段驗收標準見 **[PROJECT.md](PROJECT.md)**。

## 授權與致謝

個人學習專案。使用 [raylib](https://github.com/raysan5/raylib)（zlib 授權）等開源函式庫，各自授權
以其專案為準。日後若加入下載的素材，來源與授權會記在 `assets/CREDITS.md`。
