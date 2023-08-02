// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class FIRSTUNREALPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsAttacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UPlayerAnimInstance* Animinstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UPlayerActorComponent* PlayerComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UPlayerInventoryComponent* PlayerInventoryComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSourceComponent;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	UFUNCTION()
		virtual void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

public:

	void Attack();
	void Interaction();
	void OnHit(); 

	void KeyUpDown(float value);
	void KeyLeftRight(float value);
	void LookLeftRight(float value);
	void LookUpDown(float value);

	void SetIsRunTrue();
	void SetIsRunFalse();

public:
};
