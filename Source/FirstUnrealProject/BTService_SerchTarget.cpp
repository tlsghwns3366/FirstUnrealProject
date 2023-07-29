// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SerchTarget.h"
#include "EnemyAIController.h"
#include "PlayerCharacter.h"
#include "DrawDebugHelpers.h"	   
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyCharacter.h"
#include "EnemyAnimInstance.h"

UBTService_SerchTarget::UBTService_SerchTarget()
{
	NodeName = TEXT("SearchTarget");
	Interval = 1.f;
}

void UBTService_SerchTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn != nullptr)
	{
		UWorld* World = CurrentPawn->GetWorld();
		FVector Center = CurrentPawn->GetActorLocation();
		float SearchDistance = 1500.f;

		if (World != nullptr)
		{
			TArray<FOverlapResult> OverlapResults;
			FCollisionQueryParams QueryParams(NAME_None, false, CurrentPawn);

			bool Result = World->OverlapMultiByChannel(
				OverlapResults,
				Center,
				FQuat::Identity,
				ECollisionChannel::ECC_EngineTraceChannel2,
				FCollisionShape::MakeSphere(SearchDistance),
				QueryParams);
			if (Result)
			{
				for (auto& OverlapResult : OverlapResults)
				{
					APlayerCharacter* Player = Cast<APlayerCharacter>(OverlapResult.GetActor());
					if (Player)
					{

						float DistancePlayer = FVector::Distance(CurrentPawn->GetActorLocation(), Player->GetActorLocation());
						OwnerComp.GetBlackboardComponent()->SetValueAsFloat(FName("Distance"), DistancePlayer);
						if (DistancePlayer < 1000.f)
						{
							OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("Target"), Player);
							Cast<AEnemyCharacter>(CurrentPawn)->SetIsRun(true);
							DrawDebugSphere(World, Center, SearchDistance, 10, FColor::Green, false, 0.5f);
						}
						else
						{
							Cast<AEnemyCharacter>(CurrentPawn)->SetIsRun(false);
							OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("Target"), NULL);
							UE_LOG(LogTemp, Log, TEXT("%f"), DistancePlayer);

						}
						return;
					}
				}
				//Cast<AEnemyCharacter>(CurrentPawn)->SetIsRun(false);
				DrawDebugSphere(World, Center, SearchDistance, 10, FColor::Red, false, 0.5f);
			}
			else
			{
				DrawDebugSphere(World, Center, SearchDistance, 10, FColor::Red, false, 0.5f);
			}

		}
	}
}
