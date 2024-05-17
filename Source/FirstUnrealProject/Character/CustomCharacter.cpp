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
#include "ActorComponent/Damage/DamageType_FIre.h"
#include "ActorComponent/Damage/DamageType_Physical.h"
#include "ActorComponent/Damage/DamageType_Critical.h"

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
		DamageInterface->SetAttackType(DamageComponent, Damage);
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

void ACustomCharacter::SetWeaponEnum(EWeaponEnum WeaponEnum)
{
}
