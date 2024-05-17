// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"

#include "Character/EnemyCharacter.h"
#include "Controller/EnemyAIController.h"


UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
	NodeName = TEXT("Attack");
}


EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	Enemy = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("SightTarget"));
	RotationThreshold = 15.0f;
	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (TargetActor != nullptr)
	{
		FRotator CurrentRotation = Enemy->GetActorRotation();
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Enemy->GetActorLocation(), TargetActor->GetActorLocation());
		FRotator InterpRotation = UKismetMathLibrary::RInterpTo(Enemy->GetActorRotation(), LookAtRotation, DeltaSeconds, 5.f);
		float YawDiff = FMath::FindDeltaAngleDegrees(CurrentRotation.Yaw, LookAtRotation.Yaw);
		InterpRotation.Pitch = 0.0f;
		Enemy->SetActorRotation(InterpRotation);
		if (FMath::Abs(YawDiff) <= RotationThreshold)
		{
			Enemy->Attack();
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
	if (Enemy->IsAttacking == false && TargetActor == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
