// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNodeEnemyAttack.h"
#include "EnemyCharacter.h"
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"

UBTTaskNodeEnemyAttack::UBTTaskNodeEnemyAttack()
{
	bNotifyTick = true;
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTaskNodeEnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	Enemy = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	TarGetActor =Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if (Enemy == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	if (TarGetActor != nullptr)
	{
	}
	return EBTNodeResult::InProgress;
}

void UBTTaskNodeEnemyAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (TarGetActor != nullptr)
	{
		FRotator CurrentRotation = Enemy->GetActorRotation();
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Enemy->GetActorLocation(), TarGetActor->GetActorLocation());
		FRotator InterpRotation = UKismetMathLibrary::RInterpTo(Enemy->GetActorRotation(), LookAtRotation, DeltaSeconds, 5.f);
		float YawDiff = FMath::FindDeltaAngleDegrees(CurrentRotation.Yaw, LookAtRotation.Yaw);
		InterpRotation.Pitch = 0.0f;
		Enemy->SetActorRotation(InterpRotation);
		float RotationThreshold = 15.0f;
		if (FMath::Abs(YawDiff) <= RotationThreshold)
		{
			Enemy->Attack();
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}

	}
	if (Enemy->IsAttacking == false && TarGetActor == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
