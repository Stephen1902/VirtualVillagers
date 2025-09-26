// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestActor.generated.h"

UCLASS()
class VV_API ATestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Test Actor")
	USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Test Actor")
	UStaticMeshComponent* MeshComp;

	// Required to see if this is being rendered on screen for the purposes of highlight not happening behind walls etc.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Test Actor")
	UStaticMeshComponent* FakeMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
