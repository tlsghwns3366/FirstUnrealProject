// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdateEnemy.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "EnemyAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MainGameState.h"
#include "EnemyStateActorComponent.h"

UBTService_UpdateEnemy::UBTService_UpdateEnemy()
{
	NodeName = TEXT("UpdateEnemy");
	Interval =0.5f;
}
	

void UBTService_UpdateEnemy::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	CurrentPawn = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("IsRest"), CurrentPawn->Anim->IsRest);
	int32 Hours = Cast<AMainGameState>(GetWorld()->GetGameState())->Hours;
	if (Hours > 22 || Hours < 6)
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("IsNight"), true);
	else
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("IsNight"), false);

	if (CurrentPawn->EnemyStateComponent->RestTime > 150 && !OwnerComp.GetBlackboardComponent()->GetValueAsBool(FName("FindTarget")))
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("IsRest"), true);


	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(FName("FindTarget")))
	{
		if(CurrentPawn->EnemyStateComponent->RestTime < 200.f)
			CurrentPawn->EnemyStateComponent->RestTime += 1;
	}

}
