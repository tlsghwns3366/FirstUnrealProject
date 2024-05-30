// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyInventoryComponent.h"
#include "ItemActor.h"


UEnemyInventoryComponent::UEnemyInventoryComponent()
{
	InventorySize = 10;
}


void UEnemyInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	for (int32 Index = 0; Index < DefaultInventory.Num(); Index++)
	{
		AddDropItem(DefaultInventory[Index]);
	}
}

void UEnemyInventoryComponent::AddDropItem(UItemObject* Item)
{
	if (Item == nullptr)
		return ;
	float RandomDrop = FMath::RandRange(0.f, 100000.f);
	if (Item->DropChance > RandomDrop / 100000.f)
	{
		Item->World = GetWorld();
		Item->Inventory = this;
		DropItemInventory.Add(Item);
		OnInventoryUpdated.Broadcast();
	}
}

void UEnemyInventoryComponent::DropItem()
{
	if (GetOwner())
	{
		FVector ActorLocation = GetOwner()->GetActorLocation();
		for (auto Enemyitem : DropItemInventory)
		{
			ActorLocation += FVector(0.f, 0.f, 100.f);
			AItemActor* EItem = GetWorld()->SpawnActor<AItemActor>(EItem->StaticClass(), ActorLocation, FRotator::ZeroRotator);
			EItem->Iteminitialization(Enemyitem);
		}
		DropItemInventory.Empty();
	}
}

void UEnemyInventoryComponent::SetEnemyEquipItem(enum ECustomCharacterState EnemyState)
{
	switch (EnemyState)
	{
	case ECustomCharacterState::E_Peace:
		UnEquipItem(EItemEnum::E_Equip_Weapons_1);
		break;
	case ECustomCharacterState::E_Alert:
		break;
	case ECustomCharacterState::E_Attack:
		EquipItem(EItemEnum::E_Equip_Weapons_1);
		break;
	case ECustomCharacterState::E_Defense:
		break;
	case ECustomCharacterState::E_Runaway:
		break;
	case ECustomCharacterState::E_Rest:
		break;
	case ECustomCharacterState::E_Sleep:
		break;
	default:
		break;
	}
}

void UEnemyInventoryComponent::EquipItem(EItemEnum ItemEnum)
{
	if (GetOwner())
	{
		//End ItemInventory size == 0 warning
		for (auto Enemyitem : ItemInventory)
		{
			if (Enemyitem->ItemEnum == ItemEnum)
			{
				Enemyitem->OnUse_Implementation(Cast<ACustomCharacter>(GetOwner()));
				break;
			}
		}
	}
}

void UEnemyInventoryComponent::UnEquipItem(EItemEnum ItemEnum)
{
	if (GetOwner())
	{
		for (auto Enemyitem : EquipInventory)
		{
			if (Enemyitem->ItemEnum == ItemEnum)
			{
				Enemyitem->OnUse_Implementation(Cast<ACustomCharacter>(GetOwner()));
				break;
			}
		}
	}
}
