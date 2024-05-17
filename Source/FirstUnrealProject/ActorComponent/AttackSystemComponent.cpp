// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackSystemComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Character/CustomCharacter.h"
#include "Character/PlayerCharacter.h"
#include "Anim/CharacterAnimInstance.h"
#include "Anim/PlayerAnimInstance.h"
#include "Item/WeaponInterface.h"
#include "ActorComponent/CharacterStateComponent.h"
#include "ActorComponent/Damage/DamageType_FIre.h"
#include "ActorComponent/Damage/DamageType_Physical.h"
#include "ActorComponent/Damage/DamageType_Critical.h"
#include "ProjectileActor_Arrow.h"
#include "Item/Weapon_Bow.h"
#include "Item/Weapon_Arrow.h"



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
		AnimInstance->AttackStartTrace.AddUObject(this, &UAttackSystemComponent::TraceStart);
		AnimInstance->OnDrawArrow.AddUObject(this,& UAttackSystemComponent::SpawnArrow);
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

	if (AnimInstance)
	{
		if (AttackWeapon)
		{
			if(AttackWeapon->WeaponEnum == EWeaponEnum::E_Weapon_RangedWeapon && AnimInstance->IsZoom)
				AnimInstance->SetBowPower(LastStrength);
		}

		if (AttackArrow)
		{
			if (!AnimInstance->IsZoom || AnimInstance->IsFalling)
			{
				AttackArrow->StaticMesh->SetVisibility(false);
				CurrentStrength = 0.f;
			}
			if (Cast<APlayerCharacter>(Character))
			{
				if (AnimInstance->IsPull)
					Cast<AWeapon_Arrow>(AttackArrow)->ShowProjectilePath(CurrentStrength);
				else
					Cast<AWeapon_Arrow>(AttackArrow)->DeleteSpline();
			}
		}
		if (IWeaponInterface* interface = Cast<IWeaponInterface>(AttackWeapon))
		{
			interface->Execute_SetStringPosition(AttackWeapon, AnimInstance->IsPull, Character->GetMesh()->GetSocketLocation("WeaponSocket_R"));
		}
	}


	// ...
}

void UAttackSystemComponent::OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (!AttackWeapon)
		return;

	switch (AttackWeapon->WeaponEnum)
	{
	case EWeaponEnum::E_Weapon_RangedWeapon:
		RangedAttack();
		break;
	default:
		ComboAttack();
		break;
	}
	AttackWeapon->WeaponAction();
	Character->MainStateComponent->UseStamina(UseStamaina);
	UseStamaina = 0.f;
	CurrentStrength = 0.f;
	GetWorld()->GetTimerManager().ClearTimer(AttackTraceLoop);
}

bool UAttackSystemComponent::PlayMontage(UAnimMontage* Montage)
{
	if (Montage)
	{
		const float PlayRate = 1.0f;
		bool bPlayedSuccessfully = Character->PlayAnimMontage(Montage, PlayRate) > 0.0f;
		return bPlayedSuccessfully;
	}
	else
		return false;
}

void UAttackSystemComponent::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	AnimInstance->IsAttack = false;
	Character->IsAttacking = false;
}

void UAttackSystemComponent::SpawnArrow()
{
	if (AttackWeapon)
	{
		if (!AttackArrow)
		{
			if (AttackWeapon->WeaponEnum == EWeaponEnum::E_Weapon_RangedWeapon)
			{
				AWeapon_Bow* BowWeapon = Cast<AWeapon_Bow>(AttackWeapon);
				if (BowWeapon->ArrowBlueprint != nullptr)
				{
					FVector ActorLocation = GetOwner()->GetActorLocation();
					FTransform WeaponSocketTransform = Character->GetMesh()->GetSocketTransform("WeaponSocket_R");
					AttackArrow = GetWorld()->SpawnActor<AWeapon>(BowWeapon->ArrowBlueprint, WeaponSocketTransform);
					AttackArrow->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "WeaponSocket_R");
					Cast<AWeapon_Arrow>(AttackArrow)->SetOwnerWeapon(BowWeapon);
				}
			}
		}
		else
		{
			AttackArrow->StaticMesh->SetVisibility(true);
		}
	}
}

bool UAttackSystemComponent::PlayHitReactMontage()
{
	return false;
}

bool UAttackSystemComponent::Attack()
{
	if (!IsValid(AttackWeapon) || WeaponAttackMontage == nullptr)
		return false;
	if (Character->MainStateComponent->CurrentStamina < AttackWeapon->StaminaCost + UseStamaina)
	{
		AttackIndex = 0;
		return false;
	}
	UseStamaina = AttackWeapon->StaminaCost;

	if (AnimInstance != nullptr)
	{
		if (!AnimInstance->Montage_IsPlaying(WeaponAttackMontage))
		{
			switch (AttackWeapon->WeaponEnum)
			{
			case EWeaponEnum::E_Weapon_RangedWeapon:
				if (AnimInstance->IsZoom && !AnimInstance->IsFalling && AnimInstance->IsPull)
				{
					AnimInstance->IsAttack = true;
					AnimInstance->SetBowPower(LastStrength);
					PlayMontage(WeaponAttackMontage);
				}
				else
					return false;
				break;
			default:
				AttackIndex = 0;
				AnimInstance->IsAttack = true;
				PlayMontage(WeaponAttackMontage);
				break;
			}
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
	if (AttackWeapon)
	{
		FVector StartPoint = AttackWeapon->StartPoint->GetComponentLocation();
		FVector EndPoint = AttackWeapon->EndPoint->GetComponentLocation();

		ActorsToIgnore.Add(GetOwner());

		FHitResult HitResult;
		bool Result;
		
		if (!ShowDebug)
		{
			Result = UKismetSystemLibrary::SphereTraceSingle(
				GetWorld(),
				StartPoint,
				EndPoint,
				5.0f,
				ETraceTypeQuery::TraceTypeQuery3,
				false,
				ActorsToIgnore,
				EDrawDebugTrace::None,
				HitResult,
				true,
				FLinearColor::Red,
				FLinearColor::Green,
				5.0f
			);
		}
		else
		{
			Result = UKismetSystemLibrary::SphereTraceSingle(
				GetWorld(),
				StartPoint,
				EndPoint,
				5.0f,
				ETraceTypeQuery::TraceTypeQuery3,
				false,
				ActorsToIgnore,
				EDrawDebugTrace::ForDuration,
				HitResult,
				true,
				FLinearColor::Red,
				FLinearColor::Green,
				5.0f
			);
		}
		if (Result)
		{
			HitActorArray.AddUnique(HitResult.GetActor());
		}

	}
}

void UAttackSystemComponent::SetAttackWeapon()
{
	if (AttackArrow)
	{
		AttackArrow->Destroy();
		AttackArrow = nullptr;
	}
	if (Character->MainStateComponent->AttachedWeapon)
	{
		AttackWeapon = Character->MainStateComponent->AttachedWeapon;
		SetAttackMontage();
		SetEquipMontage();
		AttackWeapon->WeaponInitialize(AttackWeapon->EquipItem);
		PlayMontage(WeaponEquipMontage);
	}
	else
	{
		WeaponAttackMontage = nullptr;
		WeaponEquipMontage = nullptr;
	}
}

void UAttackSystemComponent::SetAttackMontage()
{
	if (AttackWeapon)
	{
		FWeaponAnim* TempMontage = GetWeaponMontage(AttackMontageTable,AttackWeapon->WeaponEnum);
		if (TempMontage != nullptr)
			WeaponAttackMontage = TempMontage->AnimMontage;
		else
			WeaponAttackMontage = nullptr;
	}
}

void UAttackSystemComponent::SetEquipMontage()
{
	if (AttackWeapon)
	{
		FWeaponAnim* TempMontage = GetWeaponMontage(EquipMontageTable,AttackWeapon->WeaponEnum);
		if (TempMontage != nullptr)
			WeaponEquipMontage = TempMontage->AnimMontage;
		else
			WeaponEquipMontage = nullptr;
	}
}

void UAttackSystemComponent::TraceStart()
{
	GetWorld()->GetTimerManager().SetTimer(AttackTraceLoop, this, &UAttackSystemComponent::Trace, 0.01f, true);
}

void UAttackSystemComponent::StopAttack()
{
	AnimInstance->Montage_Stop(0.f, WeaponAttackMontage);
	AnimInstance->IsAttack = false;
	Character->IsAttacking = false;
	GetWorld()->GetTimerManager().ClearTimer(AttackTraceLoop);
}


void UAttackSystemComponent::AttackDamage()
{
	for (AActor* HitActor : HitActorArray)
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
	}
	HitActorArray.Empty();
}

void UAttackSystemComponent::AttackDamage(AActor* TargetActor, float Value)
{
	if (TargetActor != nullptr)
	{
		float RandomChance = FMath::RandRange(0.f, 100000.f);
		float Power = FMath::Clamp(Value / 100.f, 0.f, 1.f);
		Power += 0.2f;
		if (Character->MainStateComponent->FinalState.CriticalChance > RandomChance / 100000.f)
		{
			TSubclassOf<UDamageType_Critical> DamageTypeClass = UDamageType_Critical::StaticClass();
			UGameplayStatics::ApplyDamage(TargetActor, Character->MainStateComponent->GetPhysicalDamage() * Character->MainStateComponent->FinalState.CriticalDamage * Power, Character->GetController(), Character, DamageTypeClass);
		}
		else {
			TSubclassOf<UDamageType_Physical> DamageTypeClass = UDamageType_Physical::StaticClass();
			UGameplayStatics::ApplyDamage(TargetActor, Character->MainStateComponent->GetPhysicalDamage() * Power, Character->GetController(), Character, DamageTypeClass);
		}
	}
}

void UAttackSystemComponent::StrengthUp(float Value)
{
	CurrentStrength = FMath::Clamp(FMath::Lerp(CurrentStrength, CurrentStrength + 1, Value),0.f, 100.f);
	LastStrength = CurrentStrength;
}

void UAttackSystemComponent::ComboAttack()
{
	AttackDamage();
	AttackIndex--;
	if (AttackIndex < 0)
	{
		AnimInstance->Montage_Stop(0.5f, WeaponAttackMontage);
		AttackIndex = 0;
	}
}

void UAttackSystemComponent::RangedAttack()
{
	if (!AttackArrow || !AttackWeapon)
		return;

	AWeapon_Arrow* ArrowWeapon = Cast<AWeapon_Arrow>(AttackArrow);
	if (ArrowWeapon->ProjectileBlueprint != nullptr)
	{
		ArrowWeapon->StaticMesh->SetVisibility(false);
		FTransform WeaponSocketTransform = Character->GetMesh()->GetSocketTransform("WeaponSocket_R");

		FVector BowLocation = AttackWeapon->SceneComponent->GetComponentLocation();
		BowLocation.Z += 2.f;
		FVector ArrowLocation = AttackArrow->SceneComponent->GetComponentLocation();
		BowLocation -= ArrowLocation;
		FRotator Rot = UKismetMathLibrary::MakeRotFromX(BowLocation.GetSafeNormal());

		SpawnArrowActor = GetWorld()->SpawnActor<AProjectileActor_Arrow>(ArrowWeapon->ProjectileBlueprint, WeaponSocketTransform.GetLocation(), Rot);
		SpawnArrowActor->SetOwnerCharacter(Character);
		SpawnArrowActor->SetHitBoxIgnore(Character);
		SpawnArrowActor->SetArrow(Rot, LastStrength);
		SpawnArrowActor->OnArrowHit.BindUObject(this, &UAttackSystemComponent::AttackDamage);

	}
}

FWeaponAnim* UAttackSystemComponent::GetWeaponMontage(UDataTable* DataTable, EWeaponEnum Enum)
{
	if (IsValid(DataTable))
	{
		FName RowName;
		switch (Enum)
		{
		case EWeaponEnum::E_Weapon_None:
			RowName = FName(TEXT("None"));
			break;
		case EWeaponEnum::E_Weapon_OneHandedWeapon:
			RowName = FName(TEXT("OneHandedWeapon"));
			break;
		case EWeaponEnum::E_Weapon_TwoHandedWeapon:
			RowName = FName(TEXT("TwoHandedWeapon"));
			break;
		case EWeaponEnum::E_Weapon_RangedWeapon:
			RowName = FName(TEXT("RangedWeapon"));
			break;
		case EWeaponEnum::E_Weapon_MagicWeapon:
			RowName = FName(TEXT("MagicWeapon"));
			break;
		case EWeaponEnum::E_Weapon_MiscellaneousWeapon:
			RowName = FName(TEXT("MiscellaneousWeapon"));
			break;
		default:
			break;
		}
		return DataTable->FindRow<FWeaponAnim>(RowName, RowName.ToString());
	}
	else
		return nullptr;
}
