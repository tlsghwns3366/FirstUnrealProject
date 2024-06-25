// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageComponent.h"
#include "DamageActor.h"
#include "Character/CustomCharacter.h"
#include "ActorComponent/CoolDownComponent.h"
#include "ActorComponent/CharacterStateComponent.h"


// Sets default values for this component's properties
UDamageComponent::UDamageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UDamageComponent::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<ACustomCharacter>(GetOwner());
	if (IsValid(Character))
	{
		MainStateComponent = Character->MainStateComponent;
		CoolDownComponent = Character->CoolDownComponent;
	}
	
}


// Called every frame
void UDamageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDamageComponent::OnDamaged(float Damage)
{
	if (MainStateComponent != nullptr)
	{
		MainStateComponent->UseHp(Damage);
		MainStateComponent->OnHpMpUpdated.Broadcast();
	}
}

float UDamageComponent::OnCriticalDamaged(float Damage, AActor* CauserActor)
{
	ACustomCharacter* CauserCharacter = Cast<ACustomCharacter>(CauserActor);
	if (CauserCharacter->MainStateComponent != nullptr)
	{
		OnDamaged(CauserCharacter->MainStateComponent->FinalState.CriticalDamage * Damage);
		return CauserCharacter->MainStateComponent->FinalState.CriticalDamage * Damage;
	}
	return 0.f;
}

void UDamageComponent::DamageTaken(float Damage, FColor Color)
{
	FVector Location = GetOwner()->GetActorLocation();
	ADamageActor* DamageActor = GetWorld()->SpawnActor<ADamageActor>(DamageActor->StaticClass(), Location, FRotator::ZeroRotator);
	DamageActor->SetDamage(Damage);
	DamageActor->SetColor(Color);
	DamageActor->SetRandomPosition();
}

const bool UDamageComponent::CheckCritical(AActor* CauserActor)
{
	float RandomChance = FMath::RandRange(0.f, 100000.f);

	ACustomCharacter*  CauserCharacter = Cast<ACustomCharacter>(CauserActor);
	if (CauserCharacter->MainStateComponent != nullptr)
	{
		if (CauserCharacter->MainStateComponent->FinalState.CriticalChance > RandomChance / 100000.f)
			return true;

	}
	return false;
	
}

const float UDamageComponent::GetCriticalDamage()
{
	if (MainStateComponent != nullptr)
	{
		return MainStateComponent->FinalState.CriticalDamage;
	}
	return false;
}

const float UDamageComponent::GetPhysicalDamage()
{
	if (MainStateComponent != nullptr)
		return FMath::FRandRange(MainStateComponent->FinalState.AttackDamage * 0.85, MainStateComponent->FinalState.AttackDamage * 1.15);
	else
		return false;
}

void UDamageComponent::SetDamageOverTime(UObject* DamageObject, float Damage, float DamageOverTim)
{
	CoolDownComponent->AddCoolDownObject(DamageObject);
	ICoolTimeInterface* Interface = Cast<ICoolTimeInterface>(DamageObject);
	if (Interface)
	{
		Interface->Execute_StartCooldown(DamageObject, DamageOverTim);
	}
}
