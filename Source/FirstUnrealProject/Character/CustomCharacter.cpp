// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomCharacter.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

#include "Item/InventoryComponent.h"
#include "Item/ItemActor.h"
#include "ActorComponent/CoolDownComponent.h"
#include "ActorComponent/CharacterStateComponent.h"
#include "ActorComponent/AttackSystemComponent.h"
#include "ActorComponent/Damage/DamageComponent.h"
#include "ActorComponent/Damage/DamageInterface.h"
#include "ActorComponent/Damage/DamageTypeBase.h"

// Sets default values
ACustomCharacter::ACustomCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DamageComponent = CreateDefaultSubobject<UDamageComponent>(TEXT("DamageComponent"));
	AttackSystemComponent = CreateDefaultSubobject< UAttackSystemComponent>(TEXT("AttackSystemComponent"));
	CoolDownComponent = CreateDefaultSubobject<UCoolDownComponent>(TEXT("CoolDownComponent"));
	MainStateComponent = CreateDefaultSubobject<UCharacterStateComponent>(TEXT("MainStateComponent"));

}

// Called when the game starts or when spawned
void ACustomCharacter::BeginPlay()
{
	Super::BeginPlay();
	MainStateComponent->EquipWeapon.AddDynamic(AttackSystemComponent, &UAttackSystemComponent::SetAttackWeapon);
	
}

// Called every frame
void ACustomCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACustomCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ACustomCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	//UE_LOG(LogTemp, Log, TEXT("TakeDamage"));
	if (IDamageInterface* DamageInterface = Cast<IDamageInterface>(DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>()))
	{
		DamageInterface->SetAttackType(DamageComponent, Damage, DamageCauser);
	}
	return 0.0f;
}

void ACustomCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
}

void ACustomCharacter::Attack()
{
}

void ACustomCharacter::OnHitActor()
{
}

void ACustomCharacter::Interaction()
{
}

void ACustomCharacter::DodgeAction()
{
}

UObject* ACustomCharacter::GetCharacterEquipItem(EItemEnum ItemEnum)
{
	switch (ItemEnum)
	{
	case EItemEnum::E_None:
		break;
	case EItemEnum::E_Item_Consumable:
		break;
	case EItemEnum::E_Item_QuestItem:
		break;
	case EItemEnum::E_Item_Miscellaneous:
		break;
	case EItemEnum::E_Equip_Helmet:
		return MainStateComponent->Helmat;
		break;
	case EItemEnum::E_Equip_Weapons_1:
		return MainStateComponent->AttachedWeapon;
		break;
	case EItemEnum::E_Equip_Weapons_2:
		break;
	case EItemEnum::E_Equip_TopArmor:
		return MainStateComponent->TopArmor;
		break;
	case EItemEnum::E_Equip_BottomArmor:
		return MainStateComponent->BottomArmor;
		break;
	case EItemEnum::E_Equip_Boots:
		return MainStateComponent->Boots;
		break;
	case EItemEnum::E_Equip_Gloves:
		return MainStateComponent->Gloves;
		break;
	case EItemEnum::E_Equip_Ring:
		return MainStateComponent->Ring_1;
		break;
	default:
		break;
	}

	return nullptr;
}

void ACustomCharacter::SetWeaponEnum(EWeaponEnum WeaponEnum)
{
}
