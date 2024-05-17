// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomCameraMode_ThirdPerson.h"
#include "CameraMode_ThirdPerson_ZoomIn.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class FIRSTUNREALPROJECT_API UCameraMode_ThirdPerson_ZoomIn : public UCustomCameraMode_ThirdPerson
{
	GENERATED_BODY()
public:
	UCameraMode_ThirdPerson_ZoomIn();
protected:

	virtual void UpdateView(float DeltaTime) override;
	
};
