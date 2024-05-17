// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "CustomCameraComponent.generated.h"

class UCustomCameraMode;
class UCustomCameraModeStack;
class UObject;
struct FFrame;
struct FGameplayTag;
struct FMinimalViewInfo;
template <class TClass> class TSubclassOf;

DECLARE_DELEGATE_RetVal(TSubclassOf<UCustomCameraMode>, FCustomCameraModeDelegate);
/**
 * 
 */

UCLASS()
class FIRSTUNREALPROJECT_API UCustomCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
public:

	UCustomCameraComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, Category = "Custom|Camera")
		static UCustomCameraComponent* FindCameraComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UCustomCameraComponent>() : nullptr); }

	virtual class AActor* GetTargetActor()const { return GetOwner(); }

	FCustomCameraModeDelegate DetermineCameraModeDelegate;

	void AddFieldOfViewOffset(float FovOffset) { FieldOfViewOffset += FovOffset; }

	// Gets the tag associated with the top layer and the blend weight of it
	void GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTag& OutTagOfTopLayer) const;


protected:
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;

	virtual void OnRegister() override;
	virtual void UpdateCameraModes();

	UPROPERTY()
		TObjectPtr<UCustomCameraModeStack> CameraModeStack;

	float FieldOfViewOffset;
};
