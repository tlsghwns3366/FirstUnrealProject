// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipItemObject.h"
#include "CharacterStateComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHpMpUpdated);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExpUpdated);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaminaUpdated);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHpbarUpdated);

USTRUCT(BlueprintType)
struct FCharacterState
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 Level = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MaxHp = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MaxMp = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float HpRegen = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MpRegen = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MaxStamina = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float StaminaRegen = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Speed = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float AttackDamage = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Shild = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float CriticalChance = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float CriticalDamage = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float DodgeChance = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxExp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float WalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RunSpeed;


	FCharacterState operator+(const FEquipItemInfo& Other) const
	{
		FCharacterState Result;
		Result.AttackDamage = AttackDamage + Other.AddDamage;
		Result.Shild = Shild + Other.AddShild;
		Result.MaxHp = MaxHp + Other.AddHP;
		Result.MaxMp = MaxMp + Other.AddMP;
		Result.MaxStamina = MaxStamina + Other.AddStamina;
		Result.CriticalChance = CriticalChance + Other.AddCriticalChance;
		Result.CriticalDamage = CriticalDamage + Other.AddCriticalDamage;
		Result.DodgeChance = DodgeChance + Other.AddDodgeChance;
		Result.Level = Level;
		Result.HpRegen = HpRegen;
		Result.MpRegen = MpRegen;
		Result.StaminaRegen = StaminaRegen;
		Result.Speed = Speed;
		Result.MaxExp = MaxExp;
		Result.WalkSpeed = WalkSpeed;
		Result.RunSpeed = RunSpeed;
		return Result;
	}
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRSTUNREALPROJECT_API UCharacterStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStateComponent();
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Final")
		FCharacterState FinalState;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default")
		FCharacterState CharacterState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsDie = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float StaminaUseDelay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Current")
		float CurrentHp = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Current")
		float CurrentMp = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Current")
		float CurrentStamina = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
		float CurrentExp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
		float CurrentSpeed;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EquipItem")
		class UEquipItemObject* Helmat;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EquipItem")
		class UEquipItemObject* TopArmor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EquipItem")
		class UEquipItemObject* BottomArmor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EquipItem")
		class UEquipItemObject* Boots;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EquipItem")
		class UEquipItemObject* Gloves;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EquipItem")
		class UEquipItemObject* Weapons_1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EquipItem")
		class UEquipItemObject* Weapons_2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EquipItem")
		class UEquipItemObject* Ring_1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EquipItem")
		class UEquipItemObject* Ring_2;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EquipItem")
		FEquipItemInfo CharacterEquipItemState;


	UPROPERTY(BlueprintAssignable)
		FOnHpMpUpdated OnHpMpUpdated;

	UPROPERTY(BlueprintAssignable)
		FOnExpUpdated OnExpUpdated;

	UPROPERTY(BlueprintAssignable)
		FOnStaminaUpdated OnStaminaUpdated;

	UPROPERTY(BlueprintAssignable)
		FOnHpbarUpdated OnHpbarUpdated;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual void SetState();
	virtual void SetHp(float NewHp);
	virtual bool UseStamina(float Amount);
	virtual float GetPhysicalDamage();
	virtual UEquipItemObject* GetEquip(UEquipItemObject* Item);
	virtual bool SetEquip(UEquipItemObject* Item, EItemEnum ItemEnum);
	virtual void SetEquipState();
	virtual void HpMpRegen(float DeltaTime);
	virtual void StaminaRegen(float DeltaTime);
};
