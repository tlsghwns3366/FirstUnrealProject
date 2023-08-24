// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageComponent.h"
#include "DamageActor.h"
#include "CustomCharacter.h"
#include "CharacterStateComponent.h"


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
		float TempHp = MainStateComponent->CurrentHp - Damage;
		MainStateComponent->SetHp(TempHp);
		MainStateComponent->OnhpUpdated.Broadcast();
	}
}

void UDamageComponent::DamageTaken(float Damage, FColor Color)
{
	FVector Location = GetOwner()->GetActorLocation();
	ADamageActor* DamageActor = GetWorld()->SpawnActor<ADamageActor>(DamageActor->StaticClass(), Location, FRotator::ZeroRotator);
	DamageActor->SetDamage(Damage);
	DamageActor->SetColor(Color);
}

void UDamageComponent::OnPhysical(float Damage)
{
	UE_LOG(LogTemp, Log, TEXT("Physical"));
	OnDamaged(Damage);
	DamageTaken(Damage,FColor::White);
}

void UDamageComponent::OnFire(float Damage)
{
	UE_LOG(LogTemp, Log, TEXT("Fire"));
	OnDamaged(Damage);
	DamageTaken(Damage,FColor::Orange);
}

void UDamageComponent::OnCritical(float Damage)
{
	UE_LOG(LogTemp, Log, TEXT("Critical"));
	OnDamaged(Damage);
	DamageTaken(Damage,FColor::Red);
}

