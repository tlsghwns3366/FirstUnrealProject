// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestComponent.h"
#include "PlayerMessageComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMessageUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FQuestUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWidgetUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyKillUpdated);

USTRUCT(BlueprintType)
struct FSystemMessage
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName SystemName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString SystemMessage;

};
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FIRSTUNREALPROJECT_API UPlayerMessageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerMessageComponent();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UInventoryComponent* InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<struct FSystemMessage> SystemMessageArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<struct FPlayerSelect> PlayerSelect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 PlayerSelectNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> ClearQuest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<struct FQuestData> CurrentQuest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString QuestString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 LastSelectQuest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D LastQuestListPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsTalk;


	UPROPERTY(BlueprintAssignable)
		FMessageUpdated MessageUpdated;

	UPROPERTY(BlueprintAssignable)
		FQuestUpdated QuestUpdated;

	UPROPERTY(BlueprintAssignable)
		FWidgetUpdated MenuUpdated;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	//------------------------Message
	UFUNCTION(BlueprintCallable)
		bool AddMessage(struct FSystemMessage Message);
	void RemoveMessage();
	void RemoveMenu();


	//------------------------Quest
	bool AddQuest(FQuestData* Data);
	bool RemoveQuest(FQuestData* Data);
	bool QuestClear(FQuestData* Data);

	void SetNpcMenuInfo(TArray<struct FPlayerSelect>* Menu);
	void NextSelectNumber();
	void SetIsTalk(bool NpcTalk);


	bool CurrentCheck(FString String);
	bool ClearCheck(FString String);
	FQuestData* GetCurrentQuest(FString String);

	void EnemyKillCount(FString String);

	UFUNCTION(BlueprintCallable)
	void InventoryCheck();
};
