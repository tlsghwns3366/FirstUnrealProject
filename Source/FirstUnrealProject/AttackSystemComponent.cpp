// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackSystemComponent.h"
#include "CustomCharacter.h"
#include "Weapon.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UAttackSystemComponent::UAttackSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}
// Called when the game starts
void UAttackSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<ACustomCharacter>(GetOwner());

	// ...
	
}


// Called every frame
void UAttackSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAttackSystemComponent::OnNotifyBeginRecieved(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
}

bool UAttackSystemComponent::PlayAttackMontage(UAnimMontage* AttackMontage)
{
	const float PlayRate = 1.0f;
	bool bPlayedSuccessfully = Character->PlayAnimMontage(AttackMontage, PlayRate) > 0.0f;
	if (bPlayedSuccessfully)
	{
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (AttackMontage == CharacterAttackMontage)
		{
			AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UAttackSystemComponent::OnNotifyBeginRecieved);
		}
		return bPlayedSuccessfully;
	}
	return bPlayedSuccessfully;
}

bool UAttackSystemComponent::PlayHitReactMontage()
{
	return false;
}

void UAttackSystemComponent::Attack()
{
	IsAttacking = true;
	PlayAttackMontage(CharacterAttackMontage);
	//Trace();
}

void UAttackSystemComponent::Trace()
{
	TArray<AActor*> AttachedActors;
	Character->GetAttachedActors(AttachedActors);
	if (AttachedActors.Num() > 0)
	{
		AActor* AttachedWeapon = AttachedActors[0];
		AWeapon* Weapon = Cast<AWeapon>(AttachedWeapon);
		FVector StartPoint = Weapon->StartPoint->GetComponentLocation();
		FVector EndPoint = Weapon->EndPoint->GetComponentLocation();

		ActorsToIgnore.Add(GetOwner());

		FHitResult HitResult;
		bool Result = UKismetSystemLibrary::SphereTraceSingle(
			GetWorld(),
			StartPoint,
			EndPoint,
			15.0f,
			UEngineTypes::ConvertToTraceType(ECC_Visibility),
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			HitResult,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			1.0f
		);
		if (Result)
		{
			AActor* HitActor = HitResult.GetActor();
		}

	}
}

