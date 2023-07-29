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
	auto MyEnemy = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	
	if (MyEnemy == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	MyEnemy->Attack();

	return Result;
}

void UBTTaskNodeEnemyAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}
