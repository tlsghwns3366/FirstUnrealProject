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
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIRSTUNREALPROJECT_API UPlayerMessageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerMessageComponent();

public:

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UInventoryComponent* InventoryComponent;


	UPROPERTY(BlueprintAssignable)
		FMessageUpdated MessageUpdated;

	UPROPERTY(BlueprintAssignable)
		FQuestUpdated QuestUpdated;

	UPROPERTY(BlueprintAssignable)
		FWidgetUpdated MenuUpdated;

	UPROPERTY(BlueprintAssignable)
		FEnemyKillUpdated EnemyKillUpdated;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
	bool AddMessage(struct FSystemMessage Message);
	void RemoveMessage();
	void RemoveMenu();

	UFUNCTION(BlueprintCallable)
	bool AddQuest(FQuestData Data);

	UFUNCTION(BlueprintCallable)
	bool RemoveQuest(/*FString String, FQuestData Data*/);

	void SetNpcMenuInfo(TArray<struct FPlayerSelect> *Menu);
	void NextSelectNumber();

	bool CurrentCheck(FString String);
	bool ClearCheck(FString String);
	bool FinishQuest(FString String);

	void ShowQuestWidget();


	UFUNCTION(BlueprintCallable)
	void InventoryFind();

	UFUNCTION(BlueprintCallable)
	void EnemyKill(FString String);
};
