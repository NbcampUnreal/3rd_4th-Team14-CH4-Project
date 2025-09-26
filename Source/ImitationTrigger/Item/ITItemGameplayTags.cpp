// Fill out your copyright notice in the Description page of Project Settings.


#include "ITItemGameplayTags.h"

namespace ITItemGameplayTags
{
	// == 아이템 ==
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item, "Item", "아이템");
	/* 아이템 기타 프래그먼트 */
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Etc, "Item.Etc", "아이템 기타 프래그먼트");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Etc_FX, "Item.Etc.FX", "아이템 액터 이펙트, 사운드 효과");
	/* 인벤토리 아이템 */
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Inventory, "Item.Inventory", "인벤토리 아이템");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Inventory_Potion, "Item.Inventory.Potion", "포션");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Inventory_Grenade, "Item.Inventory.Grenade", "수류탄");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Inventory_Attachment, "Item.Inventory.Attachment", "무기 부착물");
	/* 장착 아이템 */
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Equipment, "Item.Equipment", "장비 아이템");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Equipment_Helmet, "Item.Equipment.Helmet", "헬멧");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Equipment_Shield, "Item.Equipment.Shield", "쉴드");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Equipment_Bag, "Item.Equipment.Bag", "가방");
	/* 탄약 아이템 */
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Ammo, "Item.Ammo", "탄약 아이템");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Ammo_Rifle, "Item.Ammo.Rifle", "소총 탄약");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Ammo_Shotgun, "Item.Ammo.Shotgun", "산탄총 탄약");
	/* 무기 아이템 */
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Weapon, "Item.Weapon", "무기 아이템");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Weapon_Rifle, "Item.Weapon.Rifle", "소총");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_Weapon_Shotgun, "Item.Weapon.Shotgun", "산탄총");

	/* Weapon Ability Input Config Tag */
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Weapon_Fire, "InputTag.Weapon.Fire", "발사 어빌리티 인풋");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Weapon_Reload, "InputTag.Weapon.Reload", "재장전 어빌리티 인풋");

	/* Weapon Ability Tag */
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Weapon_Cooldown, "Weapon.Cooldown", "총기 쿨타임");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Weapon_Damage, "Weapon.Damage", "총기 공격력");
}
