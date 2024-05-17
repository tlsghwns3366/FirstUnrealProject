// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraMode_ThirdPerson_ZoomIn.h"
#include "Curves/CurveVector.h"

#include "Character/PlayerCharacter.h"

UCameraMode_ThirdPerson_ZoomIn::UCameraMode_ThirdPerson_ZoomIn()
{
}

void UCameraMode_ThirdPerson_ZoomIn::UpdateView(float DeltaTime)
{
	UpdateForTarget(DeltaTime);
	UpdateCrouchOffset(DeltaTime);

	FVector PivotLocation = GetPivotLocation() + CurrentCrouchOffset;
	FRotator PivotRotation = GetPivotRotation();
	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	View.Location = PivotLocation;
	View.Rotation = PivotRotation;
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;
	// Apply third person offset using pitch.
	if (!bUseRuntimeFloatCurves)
	{
		if (TargetOffsetCurve)
		{
			FVector TargetOffset = TargetOffsetCurve->GetVectorValue(PivotRotation.Pitch);
			View.Location = PivotLocation + PivotRotation.RotateVector(TargetOffset);
			if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetTargetActor()))
			{
				TargetOffset.Y = 0.f;
				TargetOffset.Z = 0.f;
				FVector TargetRotation = PivotRotation.RotateVector(TargetOffset);
				View.Location += TargetRotation.GetSafeNormal() * CameraDistance;
			}
		}
	}
	else
	{
		FVector TargetOffset(0.0f);
		TargetOffset.X = TargetOffsetX.GetRichCurveConst()->Eval(PivotRotation.Pitch);
		TargetOffset.Y = TargetOffsetY.GetRichCurveConst()->Eval(PivotRotation.Pitch);
		TargetOffset.Z = TargetOffsetZ.GetRichCurveConst()->Eval(PivotRotation.Pitch);
		View.Location = PivotLocation + PivotRotation.RotateVector(TargetOffset);
	}
	// Adjust final desired camera location to prevent any penetration
	UpdatePreventPenetration(DeltaTime);
}
