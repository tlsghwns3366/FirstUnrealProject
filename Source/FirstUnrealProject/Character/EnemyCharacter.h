// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomCharacter.h"
#include "EnemyCharacter.generated.h"

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	E_AttackingMonster UMETA(DisplayName = "AttackingMonster"),
	E_DefendingMonster UMETA(DisplayName = "DefendingMonster"),
	E_PeaceMonster UMETA(DisplayName = "PeaceMonster"),
	E_RunAwayMonster UMETA(DisplayName = "RunAwayMonster"),
};
UCLASS()
class FIRSTUNREALPROJECT_API AEnemyCharacter : public ACustomCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DestroyTime = 60.f;// Default 60
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aggro)
		float AlertAggro = 40.f;// Default 40
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aggro)
		float AttackAggro = 80.f;// Default 80

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
		EEnemyType EnemyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
		FString EnemyName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component)
		class UEnemyInventoryComponent* EnemyInventoryComponent;
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component)
		class UAggroComponent* AggroComponent;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim)
		class UEnemyAnimInstance* Anim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
		class UWidgetComponent* WidgetComponent;

	UPROPERTY(EditAnywhere, Category = Enemy, meta = (ClampMin = 0.0, ClampMax = 200.0))
		float DropExp;

	UPROPERTY(VisibleAnywhere)
		class AMainGameState* MainState;
public:
	FTimerHandle TimerHandle;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
public:
		virtual void Attack() override;
		void AttackAiming();

	UFUNCTION(BlueprintImplementableEvent)
		void SetEnemyInventory();

public:
	void EnemyDie();
	void DropItem();
	virtual void SetState();

	UFUNCTION(BlueprintCallable)
	void SetMovementSpeed(ECharacterMovementSpeedState State);

	virtual void SetWeaponEnum(EWeaponEnum WeaponEnum) override;

	void SetAiming(bool Value);
};
