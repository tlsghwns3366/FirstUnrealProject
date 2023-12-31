// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Weapon.h"
#include "AttackSystemComponent.generated.h"

USTRUCT()
struct FWeaponAnim : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EWeaponEnum WeaponEnum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* AnimMontage;

};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FIRSTUNREALPROJECT_API UAttackSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAttackSystemComponent();
	UPROPERTY(EditAnywhere)
		class ACustomCharacter* Character;

	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* HitReactMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* WeaponAttackMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AWeapon* AttackWeapon;

	UPROPERTY(EditAnywhere, Category = "Animation")
		class UCharacterAnimInstance* AnimInstance;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 AttackIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float UseStamaina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDataTable* AttackMontageTable;

	FTimerHandle AttackTraceLoop;

	TArray<AActor*> ActorsToIgnore;

	AActor* HitActor;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	UFUNCTION()
		void OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
	UFUNCTION()
	virtual void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	bool PlayAttackMontage(UAnimMontage* AttackMontage);
	bool PlayHitReactMontage();
	bool Attack();
	void Trace();
	void SetWeaponAttackMontage();
	void StopAttack();
	void AttackDamage();

	FWeaponAnim* GetAttackMontage(EWeaponEnum Enum);

};
