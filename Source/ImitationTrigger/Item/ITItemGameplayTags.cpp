// Fill out your copyright notice in the Description page of Project Settings.


#include "ITItemGameplayTags.h"

namespace ITItemGameplayTags
{
	/* 아이템 FX: 이펙트, 사운드 */
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_FX, "Item.FX", "아이템 효과");
	/* 사용 아이템: 포션, 수류탄 */
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Use_Potion, "Item.Use.Potion", "포션");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Use_Grenade, "Item.Use.Grenade", "수류탄");
	/* 장착 아이템: 헬멧, 쉴드, 가방 */
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Equip_Helmet, "Item.Equip.Helmet", "헬멧");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Equip_Shield, "Item.Equip.Shield", "쉴드");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Equip_Bag, "Item.Equip.Bag", "가방");
	/* 기타 아이템: 탄약, 무기 부착물 */
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Etc_Ammo, "Item.Etc.Ammo", "탄약");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Etc_Attachment, "Item.Etc.Attachment", "무기 부착물");
	/* 무기 아이템: 라이플, 샷건 등 */
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Weapon_Rifle, "Item.Weapon.Rifle", "라이플");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Weapon_Shotgun, "Item.Weapon.Shotgun", "산탄총");
}
