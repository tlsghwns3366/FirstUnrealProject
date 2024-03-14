// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemObject.h"
#include "CharacterStateComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHpMpUpdated);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExpUpdated);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaminaUpdated);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHpbarUpdated);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStateUpdated);

USTRUCT(BlueprintType)
struct FCharacterState
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterState)
		int32 Level = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterState)
		float MaxHp = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterState)
		float MaxMp = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterState)
		float HpRegen = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterState)
		float MpRegen = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterState)
		float MaxStamina = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterState)
		float StaminaRegen = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterState)
		float Speed = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterState)
		float AttackDamage = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterState)
		float Shild = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterState)
		float CriticalChance = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterState)
		float CriticalDamage = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterState)
		float DodgeChance = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterState)
		float MaxExp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterState)
		float WalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterState)
		float AlertSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterState)
		float RunSpeed;


	FCharacterState operator+(const FAddItemInfo& Other) const
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
		Result.AlertSpeed = AlertSpeed;
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

	//Current State
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Current")
		float CurrentHp = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Current")
		float CurrentMp = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Current")
		float CurrentStamina = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
		float CurrentExp = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
		float CurrentSpeed;

	//Equip Item slot
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
		class AWeapon* AttachedWeapon;

	//Info
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EquipItem")
		FAddItemInfo CharacterInfo;



	//Aggro
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aggro, meta = (ClampMin = 0.0f, ClampMax = 100.0f))
		float AggroCount = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aggro)
		bool IsAggro = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aggro)
		float AggroSensitivity = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aggro)
		float AggroTime = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aggro)
		float AggroCurrentTime = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aggro)
		float AggroDelayTime = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aggro)
		float AggroCurrentDelayTime = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aggro)
		float AggroExp = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aggro)
		bool AggroT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aggro)
		bool AggroTAdd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
		TArray<class UObject*> CharacterAddStateInfo;


	//DELEGATE
	UPROPERTY(BlueprintAssignable)
		FOnHpMpUpdated OnHpMpUpdated;

	UPROPERTY(BlueprintAssignable)
		FOnExpUpdated OnExpUpdated;

	UPROPERTY(BlueprintAssignable)
		FOnStaminaUpdated OnStaminaUpdated;

	//EnemyHpBar
	UPROPERTY(BlueprintAssignable)
		FOnHpbarUpdated OnHpbarUpdated;

	UPROPERTY(BlueprintAssignable)
		FOnStateUpdated OnStateUpdated;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual void SetState();
	virtual void SetAddState();
	virtual FCharacterState GetFinalState();

	// AddState
	virtual bool CharacterAddState(UObject* Object);
	virtual bool CharacterRemoveState(UObject* Object);

	//Use
	virtual bool UseHp(float NewHp);
	virtual bool UseStamina(float Amount);

	// Regen
	virtual void HpMpRegen(float DeltaTime);
	virtual void StaminaRegen(float DeltaTime);

	virtual float GetPhysicalDamage();
	virtual void SetEquip(UEquipItemObject* Item, EItemEnum ItemEnum);
	virtual void EquipItemSpawn(UEquipItemObject* Item);

	//Aggro
	virtual void TickAggroCount(float DeltaTime);
	virtual void AddAggro(float Value);

};
