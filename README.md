# Team_14천리 / CH4_Project : ImitationTrigger

- **2025-09-08 ~ 2025-10-15**
- [팀 메인 노션](https://www.notion.so/teamsparta/14-2552dc3ef51480e2af92f2c3c1ed0aea) | [프로젝트 노션](https://www.notion.so/teamsparta/Team-Project-Main-page-2562dc3ef5148006b7d4d60fe1470d7c)

<img width="2838" height="1596" alt="헤더" src="https://github.com/user-attachments/assets/0009f4b5-120e-4ea7-ac41-4e6c71e0968c" />

 **"능력과 전장에 흩어진 아이템을 사용하여, 서서히 좁혀오는 전장 속에서 살아남는 최후의 1인이 되어 승리를 쟁취하세요!"**

- 멀티플레이 기반의 게임 개발 프로젝트
- "**Fate Trigger** (Saroasis Studio)" 모작
- TPS / 히어로 슈팅 / 배틀로얄

## 🧑‍💻팀원 구성

| **이름** | **역할** |
| :------: | :------ |
| **정찬혁** | `**팀장**, 서버` 로비 및 매칭 서버, 게임별 전용(데디케이티드) 서버 개발, 게임 모드 및 규칙 구현, 네트워크 관련 담당 |
| **최대근** | `**부팀장**, 리드` PawnData를 사용한 모듈형 프레임워크 개발, 레벨 디자인, PR 리뷰, 코드 병합 |
| **김기인** | `캐릭터, 셰이더` 카툰 랜더링(쉘 쉐이딩), 캐릭터 리타기팅, ALS 기반 애니메이션 제작, 무기 애님몽타주 제작 |
| **이정재** | `기획, UI` 게임 기획, HUD 및 게임 시작 화면 및 로비 UI 제작 |
| **장재근** | `아이템` 모듈형 아이템 프레임워크 개발, 드랍 아이템 구현, 총기/방어구 제작 및 로직 구현 |
| **정영기** | `카메라, 스킬` 게임 프레임워크 개발 보조, CameraStack을 이용한 ADS(조준모드) 구현, GAS를 활용한 스킬 구현 |

<br>

</div>

---

## 1. 개발 환경
**버전 및 이슈 관리**<br>
<img src="https://img.shields.io/badge/Github-181717?style=flat-square&logo=github&logoColor=white"/>

**협업 툴**<br>
<img src="https://img.shields.io/badge/Notion-000000?style=flat-square&logo=notion&logoColor=white"/> &nbsp;
<img src="https://img.shields.io/badge/Slack-4A154B?style=flat-square&logo=slack&logoColor=white"/>

**개발 툴**<br>
<img src="https://img.shields.io/badge/C++-00599C?style=flat-square&logo=cplusplus&logoColor=white"/> &nbsp;
<img src="https://img.shields.io/badge/UE 5.5.4-0E1128?style=flat-square&logo=unrealengine&logoColor=white"/>

[Coding Convention](https://www.notion.so/teamsparta/Coding-Convention-2562dc3ef51480ffa04cfde8c5f31371) |
[Git Convention](https://www.notion.so/teamsparta/Git-2562dc3ef514804b999ec9b2248cd39c)

<br>

## 2. 핵심 개발 전략
- **Lyra-Like 모듈형 아키텍처**
   - Lyra-Like Pawn Data, Cosmetic, Camera Mode, Fragment패턴, 입력 바인딩, Log Channel
     
- **GAS(GamePlayAbilitySystem)**
   - 무기 로직, 스킬 등 모든 플레이어의 행동은 GameplayAbility로 구현
   - 체력, 탄약, 궁극기 게이지 등, 플레이어가 소유할 수 있는 속성은 Attribute로 구현
   - 사운드, 이펙트 등 시청각 효과는 GameplayCue로 구현
     
- **데이터 주도 및 서버 권위 설계**
   - 핵심 로직은 서버 권위(Server-Authoritative) 구조로 멀티플레이 환경에서의 보안과 데이터 일관성 보장
   - 게임의 요소는 데이터 테이블과 데이터 에셋으로 관리

<br>

## 3. 프로젝트 구조

```
ImitationTrigger
├── AbilitySystem
│	├── Attributes
│	│	├── ITAttributeTableRow.h .cpp
│	│	├── ITAttributeSet.h .cpp
│	│	├── ITHealthSet.h .cpp
│	│	├── ITCombatSet.h .cpp
│	│	└── ITAmmoSet.h .cpp
│	├── ITAbilitySystemComponent.h .cpp
│	└── ITAbilitySet.h .cpp
├── Camera
│	├── ITCameraComponent.h .cpp
│	├── ITCameraMode.h .cpp
│	├── ITCameraMode_ThirdPerson.h .cpp
│	├── ITPlayerCameraManager.h .cpp
│	└── ITADSAbility.h .cpp
├── Character
│	├── ITCharacter.h .cpp
│	├── ITPawnData.h .cpp
│	├── ITPawnDataList.h .cpp
│	└── ITHeroComponent.h .cpp
├── Cosmetics
│	├── ITCharacterAnimComponent.h .cpp
│	├── ITCharacterPartComponent.h .cpp
│	└── ITCharacterPartType.h .cpp
├── GameModes
│	├── ITGameModeBase.h .cpp
│	├── ITGameStateBase.h .cpp
│	├── ITBattleGameMode.h .cpp
│	└── ITBattleGameState.h .cpp
├── Input
│	├── ITInputComponent.h .cpp
│	└── ITInputConfig.h .cpp
├── Item
│	├── Fragment
│	│	├── ITItemFragment.h .cpp
│	│	└── ITItemFragment_Equipment.h .cpp
│	├── Spawn
│	│	├── ITSpawnItemData.h .cpp
│	│	└── ITSpawnItemManagerActor.h .cpp
│	├── Weapon
│	│	├── ITItemDefinition_Weapon.h .cpp
│	│	├── ITWeaponManagerComponent.h .cpp
│	│	├── ITWeaponFireAbility.h .cpp
│	│	├── ITWeaponFireAbility_Hitscan.h .cpp
│	│	├── ITWeaponFireAbility_HitscanSG.h .cpp
│	│	└── ITReloadAbility.h .cpp
│	├── ITItemDefinition.h .cpp
│	├── ITItemInstance.h .cpp
│	├── ITItemActor.h .cpp
│	├── ITItemGameplayTags.h .cpp
│	└── ITItemManagerComponent.h .cpp
├── NetWork
│	├── ITGameInstance.h .cpp
│	├── ITMatchTypes.h .cpp
│	├── ITLobbyGameMode.h .cpp
│	├── ITLobbyGameState.h .cpp
│	├── ITLobbyWidget.h .cpp
│	├── ITTitleGameMode.h .cpp
│	├── ITTitlePlayerController.h .cpp
│	└── ITTitleWidget.h .cpp
├── Player
│	├── ITPlayerState.h .cpp
│	├── ITPlayerController.h .cpp
│	└── ITBattlePlayerController.h .cpp
├── System
│	├── ITAssetManager.h .cpp
│	├── ITForbiddenArea.h .cpp
│	├── ITGameplayTags.h .cpp
│	└── ITLogChannel.h .cpp
├── UI
│	├── Ammo
│	│	└── AmmoWidget.h .cpp
│	├── Area
│	│	└── AreaWidget.h .cpp
│	├── EquipmentIcon
│	│	└── EquipmentIconWidget.h .cpp
│	├── ItemSlot
│	│	└── ItemSlotWidget.h .cpp
│	├── KillLog
│	│	└── KillLogWidget.h .cpp
│	├── KillNotify
│	│	└── KillNotifyWidget.h .cpp
│	├── PlayerBar
│	│	└── PlayerBarWidget.h .cpp
│	├── PlayerKillCount
│	│	└── PlayerKillCountWidget.h .cpp
│	├── Result
│	│	└── ResultWidget.h .cpp
│	├── SkillSlot
│	│	└── SkillSlotWidget.h .cpp
│	├── UltimateGauge
│	│	└── UltimateGaugeWidget.h .cpp
│	└── WeaponSlot
│		├── WeaponSlotWidget.h .cpp
│		└── WeaponSlotNumberWidget.h .cpp
├── ImitationTrigger.h .cpp
└── ImitationTrigger.Build.cs

Config
├── ImitationTrigger.uproject
├── ImitationTrigger.Target.cs
├── ImitationTriggerEditor.Target.cs
└── ImitationTriggerServer.Target.cs
```

<br>

## 4. 인게임 소개

### 캐릭터와 스킬, 장비
<img width="1964" height="851" alt="스크린샷 2025-10-15 221425" src="https://github.com/user-attachments/assets/435d36a8-a4e4-4504-8dd4-6f6c2a8b07c8" />
<br>

### HUD 및 조작법
<img width="1973" height="869" alt="스크린샷 2025-10-15 221403" src="https://github.com/user-attachments/assets/417753e5-4f50-48c1-9252-bbae43a7c448" />
<br>

### 맵 소개

- **L_Title** : 메인 타이틀
<img width="1795" height="859" alt="스크린샷 2025-10-15 222551" src="https://github.com/user-attachments/assets/35fbbee9-eb3a-40e2-937c-38ef4d35be6e" />
<img width="1828" height="889" alt="스크린샷 2025-10-15 222559" src="https://github.com/user-attachments/assets/b0d04a47-9c02-4995-9005-a1672836125f" />

- **L_Entry** : 캐릭터 선택 및 게임 매칭
<img width="1877" height="927" alt="스크린샷 2025-10-15 222638" src="https://github.com/user-attachments/assets/186d230e-3dd2-409c-847b-2fa91c0af467" />

- **L_TransitionMap** : 레벨 전환 시 로딩 UI 출력
<img width="1104" height="616" alt="스크린샷 2025-10-15 223807" src="https://github.com/user-attachments/assets/72bda9e4-8d34-481f-8ef3-6665c2c3e311" />

- **L_BattleMap** : 인게임 레벨


<br>

## 5. 플레이 영상
https://www.youtube.com/watch?v=SBkoYiFFHDg

