// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomCharacter.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class FIRSTUNREALPROJECT_API APlayerCharacter : public ACustomCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
public:


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UPlayerAnimInstance* Anim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UPlayerActorComponent* PlayerComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UPlayerInventoryComponent* PlayerInventoryComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSourceComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class USceneComponent* Scene;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 AttackIndex;

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
		virtual void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
		void OnNotifyBeginRecieved(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

public:

	virtual void Attack() override;
	virtual void OnHitActor() override;
	void Interaction();

	void KeyUpDown(float value);
	void KeyLeftRight(float value);
	void LookLeftRight(float value);
	void LookUpDown(float value);

	void SetIsRunTrue();
	void SetIsRunFalse();

};
