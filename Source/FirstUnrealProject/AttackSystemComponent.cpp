// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackSystemComponent.h"
#include "CustomCharacter.h"
#include "CharacterStateComponent.h"
#include "Weapon.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DamageType_FIre.h"
#include "DamageType_Physical.h"
#include "DamageType_Critical.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "CharacterAnimInstance.h"

// Sets default values for this component's properties
UAttackSystemComponent::UAttackSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}
// Called when the game starts
void UAttackSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<ACustomCharacter>(GetOwner());
	AnimInstance = Cast<UCharacterAnimInstance>(Character->GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnAttackHit.AddUObject(this, &UAttackSystemComponent::Trace);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UAttackSystemComponent::OnNotifyBeginReceived);
		AnimInstance->OnMontageEnded.AddDynamic(this, &UAttackSystemComponent::OnAttackMontageEnded);
	}
}


// Called every frame
void UAttackSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAttackSystemComponent::OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	AttackWeapon->WeaponAction();
	Character->MainStateComponent->UseStamina(AttackWeapon->StaminaCost);
	Character->MainStateComponent->StaminaUseDelay = 1.5f;
	UseStamaina = 0.f;
	AttackIndex--;
	if (AttackIndex < 0)
	{
		Character->StopAnimMontage(WeaponAttackMontage);
		AnimInstance->IsAttack = false;
		AttackIndex = 0;
	}
}

bool UAttackSystemComponent::PlayAttackMontage(UAnimMontage* AttackMontage)
{
	AttackWeapon->WeaponInitialize();
	const float PlayRate = 1.0f;
	bool bPlayedSuccessfully = Character->PlayAnimMontage(AttackMontage, PlayRate) > 0.0f;
	if (bPlayedSuccessfully)
	{
		return bPlayedSuccessfully;
	}
	return bPlayedSuccessfully;
}

void UAttackSystemComponent::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	AnimInstance->IsAttack = false;
}

bool UAttackSystemComponent::PlayHitReactMontage()
{
	return false;
}

bool UAttackSystemComponent::Attack()
{
	SetWeaponAttackMontage();
	if (!IsValid(AttackWeapon))
		return false;
	if (Character->MainStateComponent->CurrentStamina < AttackWeapon->StaminaCost + UseStamaina)
	{
		AttackIndex = 0;
		return false;
	}
	UseStamaina = AttackWeapon->StaminaCost;

	if (AnimInstance != nullptr)
	{
		AnimInstance->IsAttack = true;
		if (!AnimInstance->Montage_IsPlaying(WeaponAttackMontage))
		{
			PlayAttackMontage(WeaponAttackMontage);
			AttackIndex = 0;
		}
		else
		{
			AttackIndex = 1;
		}
	}
	return true;
}

void UAttackSystemComponent::Trace()
{
	TArray<AActor*> AttachedActors;
	Character->GetAttachedActors(AttachedActors);
	for (auto GetCharacterActor : AttachedActors)
	{
		AActor* AttachedWeapon = GetCharacterActor;
		if (AWeapon* Weapon = Cast<AWeapon>(AttachedWeapon))
		{
			FVector StartPoint = Weapon->StartPoint->GetComponentLocation();
			FVector EndPoint = Weapon->EndPoint->GetComponentLocation();

			ActorsToIgnore.Add(GetOwner());

			FHitResult HitResult;
			bool Result = UKismetSystemLibrary::SphereTraceSingle(
				GetWorld(),
				StartPoint,
				EndPoint,
				15.0f,
				UEngineTypes::ConvertToTraceType(ECC_Pawn),
				false,
				ActorsToIgnore,
				EDrawDebugTrace::ForDuration,
				HitResult,
				true,
				FLinearColor::Red,
				FLinearColor::Green,
				5.0f
			);
			if (Result)
			{
				AActor* HitActor = HitResult.GetActor();
				float RandomChance = FMath::RandRange(0.f, 100000.f);
				if (Character->MainStateComponent->FinalState.CriticalChance > RandomChance / 100000.f)
				{
					TSubclassOf<UDamageType_Critical> DamageTypeClass = UDamageType_Critical::StaticClass();
					UGameplayStatics::ApplyDamage(HitActor, Character->MainStateComponent->GetPhysicalDamage() * Character->MainStateComponent->FinalState.CriticalDamage, Character->GetController(), Character, DamageTypeClass);
				}
				else {
					TSubclassOf<UDamageType_Physical> DamageTypeClass = UDamageType_Physical::StaticClass();
					UGameplayStatics::ApplyDamage(HitActor, Character->MainStateComponent->GetPhysicalDamage(), Character->GetController(), Character, DamageTypeClass);
				}
			}
		}
	}
}

void UAttackSystemComponent::SetWeaponAttackMontage()
{
	TArray<AActor*> AttachedActors;
	Character->GetAttachedActors(AttachedActors);
	for (auto GetCharacterActor : AttachedActors)
	{
		AttackWeapon = Cast<AWeapon>(GetCharacterActor);
		AttackWeapon->AttachMesh = Character->GetMesh();
		if (AttackWeapon != nullptr && AttackWeapon->CharacterAttackMontage.Num() > 0)
			WeaponAttackMontage = AttackWeapon->CharacterAttackMontage[0];
	}
}