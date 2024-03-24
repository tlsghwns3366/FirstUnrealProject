// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CustomCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FStopAttack);

UENUM(BlueprintType)
enum class ECustomCharacterState : uint8
{
	E_Peace UMETA(DisplayName = "Peace"),
	E_Alert UMETA(DisplayName = "Alert"),
	E_Attack UMETA(DisplayName = "Attack"),
	E_Defense UMETA(DisplayName = "Defense"),
	E_Runaway UMETA(DisplayName = "Runaway"),
	E_Rest UMETA(DisplayName = "Rest"),
	E_Sleep UMETA(DisplayName = "Sleep")
}; 

UENUM(BlueprintType)
enum class ECharacterMovementSpeedState : uint8
{
	E_Walk UMETA(DisplayName = "Walk"),
	E_Run UMETA(DisplayName = "Run"),
	E_Alert UMETA(DisplayName = "Alert"),
};


UCLASS()
class FIRSTUNREALPROJECT_API ACustomCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACustomCharacter();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsAttacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component)
		class UDamageComponent* DamageComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component)
		class UAttackSystemComponent* AttackSystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component)
		class UCharacterStateComponent* MainStateComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component)
		class UInventoryComponent* InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component)
		class UCoolDownComponent* CoolDownComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aggro)
		AActor* AggroTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aggro)
		ECustomCharacterState MyCharacterState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MovementState)
		ECharacterMovementSpeedState MovementState;

	FStopAttack StopAttack;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION()
		virtual void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
public:

	virtual void Attack();
	virtual void OnHitActor();
	virtual void Interaction();
	virtual void DodgeAction();
};
