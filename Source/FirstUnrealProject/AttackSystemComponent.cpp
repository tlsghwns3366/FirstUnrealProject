// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackSystemComponent.h"
#include "CustomCharacter.h"
#include "CharacterStateComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DamageType_FIre.h"
#include "DamageType_Physical.h"
#include "DamageType_Critical.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "CharacterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"


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
	if (IsValid(AnimInstance))
	{
		//AnimInstance->OnAttackHit.AddUObject(this, &UAttackSystemComponent::Trace);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UAttackSystemComponent::OnNotifyBeginReceived);
		AnimInstance->OnMontageEnded.AddDynamic(this, &UAttackSystemComponent::OnAttackMontageEnded);
	}
	if (IsValid(Character))
	{
		Character->StopAttack.AddUObject(this, &UAttackSystemComponent::StopAttack);
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
	Character->MainStateComponent->UseStamina(UseStamaina);
	AttackDamage();
	UseStamaina = 0.f;
	AttackIndex--;
	if (AttackIndex < 0)
	{
		Character->StopAnimMontage(WeaponAttackMontage);
		AttackIndex = 0;
	}
}

bool UAttackSystemComponent::PlayAttackMontage(UAnimMontage* AttackMontage)
{
	AttackWeapon->WeaponInitialize(AttackWeapon->EquipItem);
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
	Character->IsAttacking = false;
	GetWorld()->GetTimerManager().ClearTimer(AttackTraceLoop);
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
			if (WeaponAttackMontage != nullptr)
			{
				PlayAttackMontage(WeaponAttackMontage);
				GetWorld()->GetTimerManager().SetTimer(AttackTraceLoop, this, &UAttackSystemComponent::Trace, 0.03f, true);
			}

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
				10.0f,
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
				HitActor = HitResult.GetActor();
			}
		}
	}
}

void UAttackSystemComponent::SetWeaponAttackMontage()
{
	TArray<AActor*> AttachedActors;
	Character->GetAttachedActors(AttachedActors);
	if (AttachedActors.IsEmpty())
	{
		WeaponAttackMontage = nullptr;
		return;
	}
	for (auto GetCharacterActor : AttachedActors)
	{
		AttackWeapon = Cast<AWeapon>(GetCharacterActor);
		FWeaponAnim* TempMontage = GetAttackMontage(AttackWeapon->WeaponEnum);
		if (TempMontage != nullptr)
			WeaponAttackMontage = TempMontage->AnimMontage;
	}
}

void UAttackSystemComponent::StopAttack()
{
	AnimInstance->Montage_Stop(0.f, WeaponAttackMontage);
}

void UAttackSystemComponent::AttackDamage()
{
	if (HitActor != nullptr)
	{
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
		HitActor = nullptr;
	}
}

FWeaponAnim* UAttackSystemComponent::GetAttackMontage(EWeaponEnum Enum)
{
	if (IsValid(AttackMontageTable))
	{
		FName RowName;
		switch (Enum)
		{
		case EWeaponEnum::E_Weapon_None:
			RowName = FName(TEXT("0"));
			break;
		case EWeaponEnum::E_Weapon_OneHandedWeapon:
			RowName = FName(TEXT("1"));
			break;
		case EWeaponEnum::E_Weapon_TwoHandedWeapon:
			RowName = FName(TEXT("2"));
			break;
		case EWeaponEnum::E_Weapon_RangedWeapon:
			RowName = FName(TEXT("3"));
			break;
		case EWeaponEnum::E_Weapon_MagicWeapon:
			RowName = FName(TEXT("4"));
			break;
		case EWeaponEnum::E_Weapon_MiscellaneousWeapon:
			RowName = FName(TEXT("5"));
			break;
		default:
			break;
		}
		return AttackMontageTable->FindRow<FWeaponAnim>(RowName, RowName.ToString());
	}
	else
		return nullptr;
}
