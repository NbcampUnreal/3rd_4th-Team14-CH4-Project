# Team_14천리 / CH4_Project : ImitationTrigger

- 개발 기간: **2025.09.08 - 2025.10.15**
- [팀 메인 노션](https://www.notion.so/teamsparta/14-2552dc3ef51480e2af92f2c3c1ed0aea) | [프로젝트 노션](https://www.notion.so/teamsparta/Team-Project-Main-page-2562dc3ef5148006b7d4d60fe1470d7c)
- 시연 영상: https://www.youtube.com/watch?v=SBkoYiFFHDg

<img width="2838" height="1596" alt="헤더" src="https://github.com/user-attachments/assets/0009f4b5-120e-4ea7-ac41-4e6c71e0968c" />

 **"스킬과 전장에 흩어진 아이템을 사용하여, 서서히 좁혀오는 전장 속에서 살아남는 최후의 1인이 되어 승리를 쟁취하세요!"**

- 멀티플레이 기반의 게임 개발 프로젝트
- "**Fate Trigger** (Saroasis Studio)" 모작
- TPS / 히어로 슈팅 / 배틀로얄

## 🧑‍💻팀원 구성

| **이름** | **역할** |
| :------: | :------ |
| **정찬혁** | `팀장`, `서버` 로비 및 매칭 서버, 게임별 전용(데디케이티드) 서버 개발, 게임 모드 및 규칙 구현, 네트워크 관련 담당 |
| **최대근** | `부팀장`, `리드` PawnData를 사용한 모듈형 프레임워크 개발, 레벨 디자인, PR 리뷰, 코드 병합 |
| **김기인** | `캐릭터`, `셰이더` 카툰 랜더링(쉘 쉐이딩), 캐릭터 리타기팅, ALS 기반 애니메이션 제작, 무기 애님몽타주 제작 |
| **이정재** | `기획`, `UI` 게임 기획, HUD 및 게임 시작 화면 및 로비 UI 제작 |
| **장재근** | `아이템` 모듈형 아이템 프레임워크 개발, 드랍 아이템 구현, 총기/방어구 제작 및 로직 구현 |
| **정영기** | `카메라`, `스킬` 게임 프레임워크 개발 보조, CameraStack을 이용한 ADS(조준모드) 구현, GAS를 활용한 스킬 구현 |

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

## 3. Editor 테스트 방법

1. 에디터 개인 설정에서, 멀티플레이어 설정을 변경합니다.
    - Launch Separate Server: True
    - Play Net Mode: Play Standalone
    - Run Under One Process: False
2. Title 맵을 엽니다. 위치는 All/Content/ImitationTrigger/Maps 입니다.
3. Number of Players 개수를 적당히 설정합니다. 여기에서는 2으로 하겠습니다.
4. 플레이 버튼을 누릅니다.
5. 타이틀 가운데에 있는 에디트 박스를 눌러, `127.0.0.1:17777` 을 입력합니다. 실행된 모든 클라이언트에서 같은 작업을 합니다.
6. 로비 맵으로 이동한 것을 확인합니다.
7. 캐릭터를 선택하고, 닉네임을 입력합니다.
8. 게임 시작 버튼을 누르면, 매칭이 시작됩니다.
9. 에디터 환경에서는, 잠시 뒤 모든 클라이언트가 하나의 게임 서버로 이동합니다.
10. 테스트를 진행하면 됩니다.


<br>

## 4. 프로젝트 구조

### Source

- ImitationTrigger
   * AbilitySystem: GAS 관련 클래스
   * Camera: Lyra-like 카메라 제어 관련 클래스
   * Character: 캐릭터 핵심 클래스
   * Cosmetics: 캐릭터의 외형을 담당하는 클래스
   * GameModes: 핵심 게임 모드
   * Input: 입력 제어 클래스
   * Item: 아이템 관련 클래스
   * NetWork: 네트워크, 서버 관련 클래스
   * Player: 플레이어 컨트롤러와 스테이트 클래스
   * System: 게임 시스템 관련 클래스
   * UI: Widget 클래스

```
ImitationTrigger/
├── AbilitySystem/                           # GAS 관련 클래스
│	├── Attributes/                               # [AttributeSet] 모음. 생명력, 보호막, 가한 피해량, 처치 수, 남은 총알 수와 같은 속성을 처리함
│	├── ITAbilitySystemComponent.cpp              # [ASC] 커스텀 ASC
│	└── ITAbilitySet.cpp                          # [DataAsset] Ability, Effect, AttributeSet 묶음
├── Camera/                                  # Lyra-like 카메라 제어 관련 클래스
├── Character/                               # 캐릭터 핵심 클래스
│	├── ITCharacter.cpp                          # [Character] 커스텀 캐릭터 클래스
│	├── ITPawnData.cpp                           # [DataAsset] 캐릭터의 외형, 애니메이션, Ability, 입력과 같은 설정 정보를 가진 데이터
│	├── ITPawnDataList.cpp                       # [DataAsset] 게임에 사용되는 PawnData 목록
│	└── ITHeroComponent.cpp                      # [ActorComponent] 캐릭터의 입력과 카메라를 제어하는 컴포넌트
├── Cosmetics/                               # 캐릭터의 외형을 담당하는 클래스
│	├── ITCharacterAnimComponent.cpp             # [ActorComponent] 상황에 맞는 애니메이션 재생을 제어하는 컴포넌트 (ex. 빈손, 권총, 라이플)
│	└── ITCharacterPartComponent.cpp             # [ActorComponent] 캐릭터의 외형을 제어하는 컴포넌트 (ex. Skeletal Mesh)
├── GameModes/                               # 핵심 게임 모드
│	└── ITBattleGameMode.cpp                     # [GameMode] 배틀로얄 게임 맵에서 사용하는, 핵심 게임 모드
├── Input/                                   # 입력 제어 클래스
│	├── ITInputComponent.cpp                     # [InputComponent] 커스텀 입력 컴포넌트 
│	└── ITInputConfig.cpp                        # [DataAsset] Input Action, Input Mapping Context, Gameplay Tag를 매핑하는 설정 데이터
├── Item/                                    # 아이템 관련 클래스
│	├── Fragment/                                # 아이템의 기능(ex. 장착 가능)을 정의하는 Fragment 클래스 모음
│	├── Spawn/                                   # 아이템 스포너 관련 클래스 모음
│	├── Weapon/                                  # 무기 클래스
│	│	├── ITItemDefinition_Weapon.cpp              # [DataAsset, ITItemDefinition] 무기 정의 데이터
│	│	├── ITWeaponManagerComponent.cpp             # [ActorComponent] PlayerState에 붙어서, 무기 제어를 담당하는 컴포넌트
│	│	├── ITWeaponFireAbility.cpp                  # [GameplayAbility] 무기 어빌리티 베이스 클래스
│	│	├── ITWeaponFireAbility_Hitscan.cpp          # [GameplayAbility] 히트스캔 무기
│	│	├── ITWeaponFireAbility_HitscanSG.cpp        # [GameplayAbility] 히트스캔 무기, 샷건 (여러 개의 탄환)
│	│	└── ITReloadAbility.cpp                      # [GameplayAbility] 재장전
│	├── ITItemDefinition.cpp                      # [DataAsset] 아이템 정의 데이터 
│	├── ITItemInstance.cpp                        # [UObject] 아이템 인스턴스
│	├── ITItemActor.cpp                           # [Actor] 레벨에 배치되는 아이템 액터
│	└── ITItemManagerComponent.cpp                # [ActorComponent] PlayerState에 붙어서, 아이템 제어를 담당하는 컴포넌트        
├── NetWork/                                 # 네트워크, 서버 관련 클래스
│	├── ITGameInstance.cpp                        # [GameInstance] 서버와 클라이언트에서 사용하는 게임 인스턴스      
│	├── ITLobbyGameMode.cpp                       # [GameMode] 로비 전용 게임 모드. 매차 대기열, 게임 서버 fork 등의 제어를 담당
│	├── ITLobbyGameState.cpp                      # [GameState] 로비 전용 게임 스테이트
│	├── ITLobbyWidget.cpp                         # [UserWidget] 로비 전용 위젯
│	├── ITTitleGameMode.cpp                       # [GameMode] 타이틀 전용 게임 모드
│	├── ITTitlePlayerController.cpp               # [PlayerController] 타이틀 전용 플레이어 컨트롤러
│	└── ITTitleWidget.cpp                         # [UserWidget] 타이틀 전용 위젯
├── Player/                                  # 플레이어 컨트롤러와 스테이트 
│	├── ITPlayerState.cpp                         # [PlayerState] ASC를 가지고 있는, 게임 전용 플레이어 스테이트
│	└── ITBattlePlayerController.cpp              # [PlayerController] 배틀로얄 게임에 사용되는 게임 전용 플레이어 컨트롤러. HUD 제어 담당
├── System/                                  # 게임 시스템 관련
│	├── ITForbiddenArea.cpp                       # [Actor] 금지 구역
│	└── ITLogChannel.cpp                          # 로그 관리
├── UI/                                      # Widget 클래스
│	├── Ammo/                                     # 탄약 관련 Widget
│	├── Area/                                     # 금지구역 정보 관련 Widget
│	├── EquipmentIcon/                            # 장비 등급 Widget
│	├── ItemSlot/                                 # 아이템 슬롯 Widget
│	├── KillLog/                                  # 킬 로그 Widget
│	├── KillNotify/                               # 킬 안내 Widget
│	├── PlayerBar/                                # 플레이어 생명력, 보호막, 아이콘, 이름 Widget
│	├── PlayerKillCount/                          # 남은 인원과 킬 카운트 Widget
│	├── Result/                                   # 결과 Widget
│	├── SkillSlot/                                # 스킬 슬롯 Widget
│	├── UltimateGauge/                            # 궁극기 충전량 Widget
│	├── WeaponSlot/                               # 무기 및 탄약 정보 Widget
│	└── HUDWidget.cpp                             # [UserWidget] 게임 플레이에 사용하는 Head Up Display 위젯
├── ImitationTrigger.cpp
└── ImitationTrigger.Build.cs
```

### Contents

- ImitationTrigger
   * Animation: 애니메이션 관련 에셋
   * Blueprint: 블루프린트 클래스 및 관련 에셋. 구조는 Sources의 그것과 비슷합니다.
   * Input: 입력 관련 에셋
   * Maps: 맵
   * PawnData: 캐릭터의 속성과 기능을 정의하는 DataAsset 모음
   * Resources: 리소스 파일 모음

```
ImitationTrigger/
├── Animation/                        # 애니메이션 관련 에셋
│	├── Interfaces/                       # 애니메이션 레이어 인터페이스
│	├── IT_Character_C/                   # 파란 머리 캐릭터 애니메이션
│	├── IT_Character_D/                   # 분홍 토끼 캐릭터 애니메이션
│	├── Magician/                         # 마법사 캐릭터 애니메이션
│	└── Weapon/                           # 무기 애니메이션
├── Blueprint/                        # 블루프린트 클래스 및 관련 에셋. 구조는 Sources의 그것과 비슷합니다.
│	├── Character/                        # 캐릭터 블루프린트 클래스: BP_ITCharacterBase
│	├── GameModes/                        # 게임에 사용하는 게임 모드 및 게임 스테이트 클래스
│	├── Item/                             # 아이템, 장비 관련 블루프린트 클래스 및 에셋
│	├── Player/                           # 플레이어 컨트롤러, 플레이어 스테이트 클래스
│	├── UI/                               # User Widget 블루프린트 클래스
│	└── Weapon/                           # 무기 관련 블루프린트 클래스 및 에셋
├── Input/                            # 입력 관련 에셋
├── Maps/                             # 맵
├── PawnData/                         # 캐릭터의 속성과 기능을 정의하는 DataAsset 모음
└── Resources/                        # 리소스 파일 모음
```

<br>

## 4. 인게임 소개

### 🔸캐릭터 스킬 및 장비
<img width="1964" height="851" alt="스크린샷 2025-10-15 221425" src="https://github.com/user-attachments/assets/435d36a8-a4e4-4504-8dd4-6f6c2a8b07c8" />
<br><br>

### 🔸HUD 및 조작법
<img width="1973" height="869" alt="스크린샷 2025-10-15 221403" src="https://github.com/user-attachments/assets/417753e5-4f50-48c1-9252-bbae43a7c448" />
<br><br>

### 🔸Maps, UI

**▼ Title** : 메인 타이틀 화면 맵
<img width="1795" height="859" alt="스크린샷 2025-10-15 222551" src="https://github.com/user-attachments/assets/35fbbee9-eb3a-40e2-937c-38ef4d35be6e" />
<img width="1828" height="889" alt="스크린샷 2025-10-15 222559" src="https://github.com/user-attachments/assets/b0d04a47-9c02-4995-9005-a1672836125f" />
<br>

**▼ IT_TestEntry** : 캐릭터 선택 및 게임 매칭 로비 맵
<img width="1877" height="927" alt="스크린샷 2025-10-15 222638" src="https://github.com/user-attachments/assets/186d230e-3dd2-409c-847b-2fa91c0af467" />
<br>


**▼ L_BattleMap** : 인게임 맵
<img width="1569" height="775" alt="스크린샷 2025-10-16 090109" src="https://github.com/user-attachments/assets/b9f728a0-394b-47fb-ae2d-14717068bd88" />
<img width="1777" height="806" alt="스크린샷 2025-10-16 090217" src="https://github.com/user-attachments/assets/c7622dbf-451a-441a-996c-a5f642979fbc" />
<img width="1526" height="711" alt="스크린샷 2025-10-16 090247" src="https://github.com/user-attachments/assets/9f5f2f4d-b243-4f06-b86d-e04de6b93182" />
<img width="1875" height="919" alt="스크린샷 2025-10-15 222924" src="https://github.com/user-attachments/assets/9a562c2c-a5e3-4cdd-8518-4510ad458cdc" />




