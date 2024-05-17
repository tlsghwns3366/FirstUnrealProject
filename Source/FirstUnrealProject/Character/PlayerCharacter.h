// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomCharacter.h"

#include "Item/Weapon.h"
#include "Camera/CustomCameraMode.h"
#include "PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class ETempActionType : uint8
{
	E_None UMETA(DisplayName = "None"),
	E_NpcTalk UMETA(DisplayName = "NpcTalk"),
};

UCLASS()
class FIRSTUNREALPROJECT_API APlayerCharacter : public ACustomCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
public:

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AActor* TalkActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsTalk = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction");
	float TraceDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera");
	float CameraDistance = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera");
	float CurrentCameraDistance = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ETempActionType TempActionType;

	
	// Default camera mode used by player controlled pawns.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCustomCameraMode> DefaultCameraMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCustomCameraMode> ZoomCameraMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsZoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool TargetLook;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACustomCharacter* TargetActor;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction");
	float TargetDistance = 1500.f;

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

	//Attack
	virtual void Attack() override;
	void AttackCharage();

	//Dodge
	void DodgeAction() override;

	//Interaction
	virtual void Interaction() override;
	void ForwardTrace();

	//Run
	bool SetIsRunTrue();
	void SetIsRunFalse();
	void Running(float DeltaTime);

	//Jump
	void JumpStart();
	void JumpEnd();
	
	//Crouch
	void CrouchAction();
	void SetTargetActor();

	//Zoom
	void ZoomAction(bool SetZoom);

	void StartFocus(AActor* TraceActor);
	void EndFocus(AActor* TraceActor);

	void SlotUse(float Num);

	void SetTraceDistance(float Value);

	void SetTalkActor(AActor* Actor);

	void TempAction();
	void SetTempAction(int32 Index);

	virtual void SetWeaponEnum(EWeaponEnum WeaponEnum) override;
	TSubclassOf<UCustomCameraMode>  DetermineCameraMode() const;
};
