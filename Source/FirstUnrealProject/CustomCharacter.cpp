// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomCharacter.h"
#include "CharacterStateComponent.h"
#include "DamageComponent.h"
#include "DamageInterface.h"
#include "Engine/DamageEvents.h"
#include "DamageType_FIre.h"
#include "DamageType_Physical.h"
#include "DamageType_Critical.h"
#include "Kismet/GameplayStatics.h"
#include "AttackSystemComponent.h"
#include "InventoryComponent.h"
#include "ItemActor.h"

// Sets default values
ACustomCharacter::ACustomCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DamageComponent = CreateDefaultSubobject<UDamageComponent>(TEXT("DamageComponent"));
	MainStateComponent = CreateDefaultSubobject<UCharacterStateComponent>(TEXT("MainStateComponent"));
	AttackSystemComponent = CreateDefaultSubobject< UAttackSystemComponent>(TEXT("AttackSystemComponent"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	TraceDistance = 2000;
}

// Called when the game starts or when spawned
void ACustomCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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


