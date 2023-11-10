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
		class UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSourceComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class USceneComponent* Scene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsRun = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UQuickSlotComponent* QuickSlotComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UPlayerMessageComponent* PlayerMessageComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		AActor* FocusedActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction");
		float TraceDistance;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:

public:

	virtual void Attack() override;
	void DodgeAction() override;
	virtual void Interaction() override;

	bool SetIsRunTrue();
	void SetIsRunFalse();
	void Running(float DeltaTime);
	void JumpStart();
	void JumpEnd();
	void CrouchAction();
	void ForwardTrace();
	void StartFocus(AActor* TraceActor);
	void EndFocus(AActor* TraceActor);
	void SlotUse(float Num);
	void SetTraceDistance(float Value);
};
