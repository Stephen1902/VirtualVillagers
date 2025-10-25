// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VV/Framework/InteractInterface.h"
#include "VVPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class VV_API AVVPlayerController : public APlayerController, public IInteractInterface
{
	GENERATED_BODY()

	AVVPlayerController();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Controller")
	class AVVPlayerPawn* PlayerPawnRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Controller")
	TSubclassOf<class UPlayerWidget> PlayerWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Controller")
	TSubclassOf<class UAI_DetailsWidget> AI_DetailsWidget;
	
	// Multiplier for the Mouse Axis when panning.  Higher is faster.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controller")
	float PanSensitivity = 5.0f; 

	// The amount per mouse wheel roll camera zooms in or out
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controller")
	float ZoomStep = 50.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controller")
	float MinZoomDistance = 500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controller")
	float MaxZoomDistance = 2500.f;

	// Time in seconds before the player will attempt to pick up and drag a suitable actor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Controller")
	float TimeBeforeDragging;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
	
	// Sets up the player input
	virtual void SetupInputComponent() override;

	

private:
	// function to adjust movement speed, depending on how far the camera is zoomed
	float MovementSpeedCalculation() const;
	
	void MoveForward(float Value);
	void MoveRight(float Value);
	void FastMoveMultiplierPressed();
	void FastMoveMultiplierReleased();

	void PanCameraPressed();
	void PanCameraReleased();
	bool bIsPanning = false;
	void DealWithPanning();

	void Zoom(float Value);
	void CheckForEdgeMovement();
	float EdgeMoveSpeedX = 0.f;
	float EdgeMoveSpeedY = 0.f;
	
	bool bDisableCamNav = false;
	// Value that can be changed to move faster in the world
	float FastMoveMultiplier = 1.0f;

	void DealWithLeftMousePressed();
	void DealWithLeftMouseReleased();
	
	UPROPERTY()
	UPlayerWidget* PlayerWidgetRef;

	UPROPERTY()
	AActor* ActorBeenHit;

	FTimerHandle CheckForInteractiveTimer;
	UFUNCTION()
	void CheckForInteractive();

	// Used when the player has grabbed something that can be moved
	float TimeLeftHeld;
	bool bIsHolding;
	bool bIsDragging;
	UPROPERTY()
	ATP_ThirdPersonCharacter* CharacterToDrag;
	void SeeIfDragging();

	void TempActionPressed();
	void TempActionReleased();
	UPROPERTY()
	class UAI_DetailsWidget* DetailsWidgetRef;

public:
	void SetDetailsWidgetRef(UAI_DetailsWidget* WidgetIn);

	void ShowDetailsWidget();
};

