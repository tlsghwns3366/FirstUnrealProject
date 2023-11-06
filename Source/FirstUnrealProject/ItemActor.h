	// Fill out your copyright notice in the Description page of Project Settings.

	#pragma once

	#include "CoreMinimal.h"
	#include "GameFramework/Actor.h"
	#include "InteractionInterface.h"
	#include "ItemActor.generated.h"

	UCLASS()
	class FIRSTUNREALPROJECT_API AItemActor : public AActor, public IInteractionInterface
	{
		GENERATED_BODY()
	
	public:	
		// Sets default values for this actor's properties
		AItemActor();

	public:


		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			class UWidgetComponent* WidgetComponent;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			TSubclassOf<class UUserWidget> Widget;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			class UInteractionUserWidget* InteractionWidget;

		UPROPERTY(EditAnywhere,BlueprintReadWrite, Instanced)
			class UItemObject* Item;
		UPROPERTY(EditAnywhere)
			UStaticMeshComponent* StaticMesh;

		UPROPERTY(EditAnywhere)
			class UMaterialInstanceDynamic* OverlayMaterialInstance;

	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

	public:	
		// Called every frame
		virtual void Tick(float DeltaTime) override;
	public:
		bool AddInventory(class UInventoryComponent * Inventory);

	public:
		UFUNCTION(BlueprintCallable)
		void Iteminitialization(UItemObject* _Item);

		FLinearColor GetItemColor();



		UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
			void OnInteract(AActor* Caller);
		virtual void OnInteract_Implementation(AActor* Caller);

		UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
			void StartFocus();
		virtual void StartFocus_Implementation();

		UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
			void EndFocus();
		virtual void EndFocus_Implementation();
	};
