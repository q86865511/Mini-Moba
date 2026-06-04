# Mini MOBA

一個用**原生 C++** 製作的簡化版多人 MOBA（靈感來自 *League of Legends*）。
這是一個循序漸進的學習專案，刻意貼近 LoL 真正的做法：**C++ 引擎 + Lua 內容腳本 + 原生客戶端**。

> 目標：能跑、能連線、能跟朋友玩——重點是學習，不是功能完整。

---

## 技術棧

| 用途 | 選擇 |
|---|---|
| 語言 | C++17 |
| 繪圖 / 視窗 / 輸入 / 音效 | [raylib](https://www.raylib.com/) 5.5（CMake FetchContent，靜態連結） |
| 網路（規劃中） | [ENet](http://enet.bespin.org/)（為遊戲設計的可靠 UDP） |
| 腳本層（規劃中） | Lua + [sol2](https://github.com/ThePhD/sol2) |
| 資料 | JSON（nlohmann/json） |
| 建置 | CMake + vcpkg |
| 編譯器 | MSVC（Visual Studio 2022 / 2026） |

## 架構原則

- **伺服器權威**：所有遊戲狀態的「真相」都在伺服器；客戶端只送輸入、不送結果。
- **共用模擬庫 `shared`**：純邏輯（不碰繪圖 / 網路），伺服器與客戶端共用——**邏輯只寫一次**。
- **依賴衛生**：raylib 只在 client；伺服器無圖形依賴，可在 Linux VPS 編譯部署。
- **固定 tick + 快照插值**：伺服器固定頻率模擬並廣播快照，客戶端插值繪圖。

## 專案結構

```
.
├─ client/           客戶端：raylib 繪圖 + 輸入（目前：階段 0 視窗）
├─ shared/           （規劃）純模擬庫，前後端共用
├─ server/           （規劃）權威伺服器（ENet）
├─ CMakeLists.txt    頂層建置
├─ CMakePresets.json 建置設定檔
├─ vcpkg.json        相依套件清單
└─ PROJECT.md        詳細開發計畫與進度
```

## 建置與執行

**需求**：Windows + Visual Studio 2022/2026（勾選「使用 C++ 的桌面開發」工作負載，內含 CMake）、
vcpkg（設好 `VCPKG_ROOT` 環境變數）。第一次設定會用 FetchContent 下載並編譯 raylib 5.5，需等幾分鐘。

```powershell
# 在專案根目錄（cmake 可用 VS 內建的，或加入 PATH）
cmake --preset default
cmake --build build --config Debug

# 執行（階段 0：跟隨滑鼠的方塊視窗，ESC 離開）
.\build\client\Debug\client.exe
```

## 開發進度

- [x] **階段 0**：工具鏈 + 視窗
- [ ] 階段 1：`shared` 模擬 + 點擊移動
- [ ] 階段 2：ENet + 模擬移到伺服器
- [ ] 階段 3：核心 MOBA 機制
- [ ] 階段 4：多人 + 房間 + 隊伍（3v3 多路）
- [ ] 階段 5：連線打磨
- [ ] 階段 6：內容與平衡 + Lua 腳本層

完整的開發計畫、架構說明與每階段驗收標準見 **[PROJECT.md](PROJECT.md)**。

## 授權與致謝

個人學習專案。使用 [raylib](https://github.com/raysan5/raylib)（zlib 授權）等開源函式庫，各自的授權以其專案為準。
