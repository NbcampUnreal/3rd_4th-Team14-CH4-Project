// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"

namespace ITItemGameplayTags
{
	/* 아이템 FX: 이펙트, 사운드 */
	IMITATIONTRIGGER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_FX);
	/* 사용 아이템: 포션, 수류탄 */
	IMITATIONTRIGGER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Use_Potion);
	IMITATIONTRIGGER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Use_Grenade);
	/* 장착 아이템: 헬멧, 쉴드, 가방 */
	IMITATIONTRIGGER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Equip_Helmet);
	IMITATIONTRIGGER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Equip_Shield);
	IMITATIONTRIGGER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Equip_Bag);
	/* 기타 아이템: 탄약, 무기 부착물 */
	IMITATIONTRIGGER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Etc_Ammo);
	IMITATIONTRIGGER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Etc_Attachment);
	/* 무기 아이템: 라이플, 샷건 등 */
	IMITATIONTRIGGER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Weapon_Rifle);
	IMITATIONTRIGGER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_Weapon_Shotgun);
}
