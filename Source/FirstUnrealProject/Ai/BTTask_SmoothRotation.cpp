// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/BTTask_SmoothRotation.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Rotator.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Controller/EnemyAIController.h"

UBTTask_SmoothRotation::UBTTask_SmoothRotation()
{
	NodeName = TEXT("SmoothRotation");
	// accept only actors and vectors
	bNotifyTick = true;
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_SmoothRotation, BlackboardKey), AActor::StaticClass());
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_SmoothRotation, BlackboardKey));
	BlackboardKey.AddRotatorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_SmoothRotation, BlackboardKey));
}

void UBTTask_SmoothRotation::PostInitProperties()
{
	Super::PostInitProperties();

	PrecisionDot = FMath::Cos(FMath::DegreesToRadians(Precision));
}

void UBTTask_SmoothRotation::PostLoad()
{
	Super::PostLoad();

	PrecisionDot = FMath::Cos(FMath::DegreesToRadians(Precision));
}

EBTNodeResult::Type UBTTask_SmoothRotation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (AIController == NULL || AIController->GetPawn() == NULL)
	{
		return EBTNodeResult::Failed;
	}
	EBTNodeResult::Type Result = EBTNodeResult::InProgress;
	return Result;
}
namespace
{
	FORCEINLINE_DEBUGGABLE FVector::FReal CalculateAngleDifferenceDot(const FVector& VectorA, const FVector& VectorB)
	{
		return (VectorA.IsNearlyZero() || VectorB.IsNearlyZero())
			? 1.f
			: VectorA.CosineAngle2D(VectorB);
	}
}


void UBTTask_SmoothRotation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	if (AIController == NULL || AIController->GetPawn() == NULL)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	else
	{
		EBTNodeResult::Type Result = EBTNodeResult::Failed;
		APawn* Pawn = AIController->GetPawn();
		const FVector PawnLocation = Pawn->GetActorLocation();
		const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
		FRotator TargetRotator;
		if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
		{
			UObject* KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
			AActor* ActorValue = Cast<AActor>(KeyValue);
			
			if (ActorValue != NULL)
			{
				TargetRotator = (ActorValue->GetActorLocation() - PawnLocation).Rotation();
				TargetRotator.Pitch = 0.f;
				TargetRotator.Roll = 0.f;
				TargetRotator.Normalize();
				if (TargetRotator.Yaw < 0.0f)
					TargetRotator.Yaw += 360.0f;
				Pawn->SetActorRotation(FMath::RInterpConstantTo(Pawn->GetActorRotation(), TargetRotator, DeltaSeconds, 300.f));
			}
			else
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			}
		}
		else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
		{
			const FVector KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());
			TargetRotator = (KeyValue - PawnLocation).Rotation();
			TargetRotator.Pitch = 0.f;
			TargetRotator.Roll = 0.f;
			TargetRotator.Normalize();
			if (TargetRotator.Yaw < 0.0f)
				TargetRotator.Yaw += 360.0f;
			Pawn->SetActorRotation(FMath::RInterpConstantTo(Pawn->GetActorRotation(), TargetRotator, DeltaSeconds, 300.f));
		}
		else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Rotator::StaticClass())
		{
			const FRotator KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Rotator>(BlackboardKey.GetSelectedKeyID());
			TargetRotator = KeyValue;
			TargetRotator.Pitch = 0.f;
			TargetRotator.Roll = 0.f;
			TargetRotator.Normalize();
			if (TargetRotator.Yaw < 0.0f)
				TargetRotator.Yaw += 360.0f;
			Pawn->SetActorRotation(FMath::RInterpConstantTo(Pawn->GetActorRotation(), TargetRotator, DeltaSeconds, 300.f));
		}
		const FVector PawnDirection = Pawn->GetActorForwardVector();
		const FVector FocalPoint = TargetRotator.Vector();
		if (CalculateAngleDifferenceDot(PawnDirection, FocalPoint) >= PrecisionDot)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
		FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
	}
}