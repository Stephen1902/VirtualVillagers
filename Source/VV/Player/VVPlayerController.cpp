// Copyright 2025 DME Games

#include "VVPlayerController.h"
#include "VVPlayerPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerWidget.h"
#include "Kismet/KismetSystemLibrary.h"

AVVPlayerController::AVVPlayerController()
{
	// Set this controller to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerPawnRef = nullptr;
	PlayerWidgetRef = nullptr;
	ActorBeenHit = nullptr;
	bIsDragging = false;
	TimeLeftHeld = 0.f;
	TimeBeforeDragging = 0.5f;

	bIsHolding = false;
}

void AVVPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerPawnRef = Cast<AVVPlayerPawn>(GetPawn());

	if (!PlayerPawnRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get player pawn reference in VVPlayerController"));
	}

	if (PlayerWidget)
	{
		PlayerWidgetRef = CreateWidget<UPlayerWidget>(this, PlayerWidget);
		PlayerWidgetRef->AddToViewport();
		SetShowMouseCursor(true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No player widget has been set in VVPlayerController."))
	}

	// Call to ensure we're not trying to pan on game start
	PanCameraReleased();

	// Begin the timer to check for items in the world that are interactive, running 10 times per second
	GetWorld()->GetTimerManager().SetTimer(CheckForInteractiveTimer, this, &AVVPlayerController::CheckForInteractive, 0.1f, true, 0.1f);
}

void AVVPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (bIsPanning)
	{
		DealWithPanning();
	}
	else
	{
		CheckForEdgeMovement();
	}

	if (bIsHolding)
	{
		TimeLeftHeld += DeltaSeconds;
		FString StringToDisplay = FString::SanitizeFloat(TimeLeftHeld);
		GEngine->AddOnScreenDebugMessage(0, 0.f, FColor::Green, *StringToDisplay);
		// TODO Deal with what happens when the threshold is reached
		if (TimeLeftHeld >= TimeBeforeDragging)
		{
			
		}
	}
}

void AVVPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Movement actions
	InputComponent->BindAxis("Forward", this, &AVVPlayerController::MoveForward);
	InputComponent->BindAxis("Right", this, &AVVPlayerController::MoveRight);
	InputComponent->BindAction("Pan", IE_Pressed, this, &AVVPlayerController::PanCameraPressed);
	InputComponent->BindAction("Pan", IE_Released, this, &AVVPlayerController::PanCameraReleased);
	
	InputComponent->BindAction("ToggleFastMove", IE_Pressed, this, &AVVPlayerController::FastMoveMultiplierPressed);
	InputComponent->BindAction("ToggleFastMove", IE_Released, this, &AVVPlayerController::FastMoveMultiplierReleased);

	InputComponent->BindAxis("Zoom", this, &AVVPlayerController::Zoom);

	InputComponent->BindAxis("MOUSEX");
	InputComponent->BindAxis("MOUSEY");

	InputComponent->BindAction("LeftMouseUse", IE_Pressed, this, &AVVPlayerController::DealWithLeftMousePressed);
	InputComponent->BindAction(TEXT("LeftMouseUse"), IE_Released, this, &AVVPlayerController::DealWithLeftMouseReleased);
}

float AVVPlayerController::MovementSpeedCalculation() const
{
	if (PlayerPawnRef)
	{
		const float CurrentArmLength = PlayerPawnRef->GetSpringArmCompRef()->TargetArmLength / 100.f;
		return FMath::ClampAngle(CurrentArmLength, 5.0f, 20.f);
	}

	return 10.f;
}

void AVVPlayerController::MoveForward(float Value)
{
	if (Value != 0.f && !bDisableCamNav && PlayerPawnRef)
	{
		// Calculate how much to move this time, based on zoom level and whether player is asking for fast move
		const float MoveThisTime = (MovementSpeedCalculation() * FastMoveMultiplier) * Value;
		// Create a vector that only allows forward and backward movement
		const FVector NewVector = FVector(MoveThisTime, 0.f, 0.f);
		// Get the player location and rotation, set the relative forward movement
		const FTransform PlayerTransform = PlayerPawnRef->GetActorTransform();
		const FVector ForwardTransform = UKismetMathLibrary::TransformDirection(PlayerTransform, NewVector);
		// Add the new location to the existing location
		const FVector NewLocation = PlayerTransform.GetLocation() + ForwardTransform;
		// Make a new transform based on calculated and existing values
		const FTransform NewTransform = FTransform(PlayerTransform.GetRotation(), NewLocation, PlayerTransform.GetScale3D());
		// Set the player to the new location
		PlayerPawnRef->SetActorTransform(NewTransform);

		// When rotating, Z can be altered.  Correct this
		PlayerPawnRef->SetActorLocation(FVector(PlayerPawnRef->GetActorLocation().X, PlayerPawnRef->GetActorLocation().Y, 100.f));
	}
}

void AVVPlayerController::MoveRight(float Value)
{
	if (Value != 0.f && !bDisableCamNav && PlayerPawnRef)
	{
		// Calculate how much to move this time, based on zoom level and whether player is asking for fast move
		const float MoveThisTime = (MovementSpeedCalculation() * FastMoveMultiplier) * Value;
		// Create a vector that only allows forward and backward movement
		const FVector NewVector = FVector(0.f, MoveThisTime, 0.f);
		// Get the player location and rotation, set the relative forward movement
		const FTransform PlayerTransform = PlayerPawnRef->GetActorTransform();
		const FVector ForwardTransform = UKismetMathLibrary::TransformDirection(PlayerTransform, NewVector);
		// Add the new location to the existing location
		const FVector NewLocation = PlayerTransform.GetLocation() + ForwardTransform;
		// Make a new transform based on calculated and existing values
		const FTransform NewTransform = FTransform(PlayerTransform.GetRotation(), NewLocation, PlayerTransform.GetScale3D());
		// Set the player to the new location
		PlayerPawnRef->SetActorTransform(NewTransform);

		// When rotating, Z can be altered.  Correct this
		PlayerPawnRef->SetActorLocation(FVector(PlayerPawnRef->GetActorLocation().X, PlayerPawnRef->GetActorLocation().Y, 100.f));
	}
}

void AVVPlayerController::PanCameraPressed()
{
	bDisableCamNav = true;
	bIsPanning = true;

}
void AVVPlayerController::PanCameraReleased()
{
	bDisableCamNav = false;
	bIsPanning = false;

	// When the mouse in on screen, it will only track to the edges if in Game mode only
	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(true);
	SetInputMode(InputMode);
}

void AVVPlayerController::DealWithPanning()
{
	if (PlayerPawnRef)
	{
		// Get the value the Mouse Axis and multiply by the pan sensitivity set in BP
		const float MouseXFloat = InputComponent->GetAxisValue(TEXT("MOUSEX")) * PanSensitivity;
		const float MouseYFloat = InputComponent->GetAxisValue(TEXT("MOUSEY")) * PanSensitivity;
		// Get the current player rotation
		FRotator PlayerRot = PlayerPawnRef->GetActorRotation();
		// X runs on the Yaw.  Adjust this one only
		const float NewYaw = PlayerRot.Yaw + MouseXFloat;
		// Y runs on the Pitch.  Adjust this, but clamp it to avoid issues with clipping through the scenery
		const float NewPitch = FMath::Clamp(PlayerRot.Pitch + MouseYFloat, -15.f, 45.f);
		
		// Set the new actor Transform, based on the calculations above
		PlayerPawnRef->SetActorRotation(FRotator(NewPitch, NewYaw, PlayerRot.Roll));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No PlayerPawnRef in VVPlayerController."));
	}
}

void AVVPlayerController::Zoom(float Value)
{
	if (Value != 0.f && PlayerPawnRef)
	{
		// Get the current arm length
		float NewTargetLength = PlayerPawnRef->GetSpringArmCompRef()->TargetArmLength;
		// Adjust the length depending on which way the mouse wheel has been moved
		if (Value > 0.f)
		{
			NewTargetLength -= ZoomStep;
		}
		else
		{
			NewTargetLength += ZoomStep;
		}
		// Clamp the value, based on the min and max zoom distances
		NewTargetLength = FMath::Clamp(NewTargetLength, MinZoomDistance, MaxZoomDistance);
		// Set the arm length
		PlayerPawnRef->GetSpringArmCompRef()->TargetArmLength = NewTargetLength;
	}
}

void AVVPlayerController::CheckForEdgeMovement()
{
	if (PlayerPawnRef)
	{
		const float MouseXFloat = InputComponent->GetAxisValue(TEXT("MOUSEX"));
		const float MouseYFloat = InputComponent->GetAxisValue(TEXT("MOUSEY"));
		if (MouseXFloat != 0.f || MouseYFloat != 0.f)
		{
			int32 ViewportX, ViewportY;
			GetViewportSize(ViewportX, ViewportY);
			float MouseLocX, MouseLocY;
			GetMousePosition(MouseLocX, MouseLocY);

			if (MouseXFloat != 0.f)
			{
				if ((MouseLocX / ViewportX) > .975f)
				{
					EdgeMoveSpeedX = 5.0f * FastMoveMultiplier;
				}
				else if ((MouseLocX / ViewportX) < .025f)
				{
					EdgeMoveSpeedX = -5.0f * FastMoveMultiplier;
				}
				else
				{
					EdgeMoveSpeedX = 0.f;
				}
			}

			if (MouseYFloat != 0.f)
			{
				if ((MouseLocY / ViewportY) > .975f)
				{
					EdgeMoveSpeedY = -5.0f * FastMoveMultiplier;
				}
				else if ((MouseLocY / ViewportY) < .025f)
				{
					EdgeMoveSpeedY = 5.0f * FastMoveMultiplier;
				}
				else
				{
					EdgeMoveSpeedY = 0.f;
				}
			}
		}

		PlayerPawnRef->AddActorLocalOffset(FVector(EdgeMoveSpeedY, EdgeMoveSpeedX, 0.f));
	}
}

void AVVPlayerController::FastMoveMultiplierPressed()
{
	FastMoveMultiplier = 2.0f;
}

void AVVPlayerController::FastMoveMultiplierReleased()
{
	FastMoveMultiplier = 1.0f;
}

void AVVPlayerController::DealWithLeftMousePressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Pressed"));
	// We only want to be able to drag things that can be
	if (ActorBeenHit && UKismetSystemLibrary::DoesImplementInterface(ActorBeenHit, UInteractInterface::StaticClass()))
	{
		bIsHolding = Execute_GetCanBeDragged(ActorBeenHit);
	}
}

void AVVPlayerController::DealWithLeftMouseReleased()
{
	bIsHolding = false;
	TimeLeftHeld = 0.f;

	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(true);
	SetInputMode(InputMode);
	
	if (PlayerPawnRef)
	{
		if (ActorBeenHit && UKismetSystemLibrary::DoesImplementInterface(ActorBeenHit, UInteractInterface::StaticClass()))
		{
			FText ItemNameInfo;
			FText ItemDescInfo;
			Execute_GetItemInfo(ActorBeenHit, ItemNameInfo, ItemDescInfo);
		
			PlayerWidgetRef->SetItemInfo(ItemNameInfo, ItemDescInfo);
		}
		else
		{
			const FText BlankText = FText::FromString("");
			PlayerWidgetRef->SetItemInfo(BlankText, BlankText);
		}
	}
}

void AVVPlayerController::CheckForInteractive()
{
	FHitResult HitResult;
	if (GetHitResultUnderCursor(ECC_GameTraceChannel1, false, HitResult))
	{
		if (ActorBeenHit != HitResult.GetActor())
		{
			if (UKismetSystemLibrary::DoesImplementInterface(HitResult.GetActor(), UInteractInterface::StaticClass()))
			{
				// Check if there is an actor being referenced which is now different to the actor being hit now
				if (ActorBeenHit && UKismetSystemLibrary::DoesImplementInterface(ActorBeenHit, UInteractInterface::StaticClass()))
				{
					Execute_OnOverlapEnd(ActorBeenHit);
				}
				
				ActorBeenHit = Execute_OnOverlapBegin(HitResult.GetActor());
				UE_LOG(LogTemp, Warning, TEXT("ActorBeenHit set to %s"), *ActorBeenHit->GetName());
			}
		}
	}
	else
	{
		if (ActorBeenHit != nullptr)
		{
			/*
			// Clear the information in the player widget so old info doesn't stay on screen
			if (PlayerWidgetRef)
			{
				PlayerWidgetRef->SetItemInfo(FText::FromString(""), FText::FromString(""));
			}
			*/
			Execute_OnOverlapEnd(ActorBeenHit);
			ActorBeenHit = nullptr;
		}
	}
}
