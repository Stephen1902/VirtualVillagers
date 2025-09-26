// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Framework/InteractInterface.h"
#include "GameFramework/Actor.h"
#include "InteractiveActorBase.generated.h"

UCLASS()
class VV_API AInteractiveActorBase : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
protected:	
	// Sets default values for this actor's properties
	AInteractiveActorBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Actor")
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Actor")
	class USphereComponent* CollisionComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Actor")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Actor")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Actor")
	FText ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive Actor")
	bool bCanBeDragged;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Character that's working this actor
	UPROPERTY()
	class ATP_ThirdPersonCharacter* WorkingAIChar;

	virtual AActor* OnOverlapBegin_Implementation() override;
	virtual void OnOverlapEnd_Implementation() override;
	virtual void GetItemInfo_Implementation(FText& ItemNameOut, FText& ItemDescOut) override;
	virtual bool GetCanBeDragged_Implementation() override;
};
