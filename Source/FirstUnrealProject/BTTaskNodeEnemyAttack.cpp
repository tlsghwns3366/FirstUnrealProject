// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNodeEnemyAttack.h"
#include "EnemyCharacter.h"
#include "EnemyAIController.h"

UBTTaskNodeEnemyAttack::UBTTaskNodeEnemyAttack()
{
	bNotifyTick = true;
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTaskNodeEnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	Enemy = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	
	if (Enemy == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	Enemy->Attack();

	return Result;
}

void UBTTaskNodeEnemyAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (Enemy->IsAttacking == false)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
