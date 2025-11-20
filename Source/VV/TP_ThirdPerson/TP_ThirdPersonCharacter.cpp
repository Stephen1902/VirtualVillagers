// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_ThirdPersonCharacter.h"

#include "AI_DetailsWidget.h"
#include "AI_Needs.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "VV/Framework/VVGameState.h"
#include "VV/Player/VVPlayerController.h"

//////////////////////////////////////////////////////////////////////////
// ATP_ThirdPersonCharacter


ATP_ThirdPersonCharacter::ATP_ThirdPersonCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create the scene component for the render camera
	RenderSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Render Scene Comp"));
	RenderSceneComp->SetupAttachment(GetRootComponent());
	RenderSceneComp->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

	// Create the render component
	RenderCameraComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Render Camera Comp"));
	RenderCameraComp->SetupAttachment(RenderSceneComp);
	RenderCameraComp->SetRelativeLocation(FVector(-1000.f, 0.f, 0.f));
	RenderCameraComp->FOVAngle = 10.f;
	RenderCameraComp->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_RenderScenePrimitives;
	RenderCameraComp->SetAutoActivate(false);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	AINeedsComp = CreateDefaultSubobject<UAI_Needs>("Character Needs");
	
	static ConstructorHelpers::FObjectFinder<UDataTable> DT(TEXT("/Game/ThirdPersonCPP/Blueprints/DT_CharacterInfo"));
	if (DT.Succeeded())
	{
		CharacterDataTable = DT.Object;
	}

	LocalStage = ECharStages::ECS_Unborn;

	bCanBePickedUp = false;
	bCanBeDragged = true;
	bIsBeingDragged = false;
}

void ATP_ThirdPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	GameStateRef = Cast<AVVGameState>(GetWorld()->GetGameState());

	if (GameStateRef)
	{
		GameStateRef->OnDayChanged.AddDynamic(this, &ATP_ThirdPersonCharacter::CheckForDayChange);
		// If the character is created by the game state, it will be "Unborn"
		if (LocalStage == ECharStages::ECS_Unborn)
		{
			UpdateStageInfo();
		}
		else
		{
			// If not "unborn", will be an existing character in the world.  Set its info.
			if (CharacterDataTable)
			{
				FName RowName = FName(UEnum::GetDisplayValueAsText(LocalStage).ToString());
				FCharInfo* Row = CharacterDataTable->FindRow<FCharInfo>(RowName, "");

				if (Row)
				{
					DaysBeforeAging = Row->DaysToNextStage;
					AINeedsComp->CharacterNeedsUpdated(false, Row->FoodDrain, Row->WaterDrain, Row->HealAmount);
					
					GetMesh()->SetSkeletalMesh(Row->MeshToUse);
				}
			}
		}
	}
/*
	if (RenderCameraComp)
	{
		TArray<AActor*> ShownItems;
		ShownItems.AddUnique(this);
		//RenderCameraComp->
	}

	*/
}

void ATP_ThirdPersonCharacter::AddWidget(AVVPlayerController* ControllerIn)
{
	if (ControllerIn && DetailsWidget)
	{
		UAI_DetailsWidget* DW = CreateWidget<UAI_DetailsWidget>(GetWorld(), DetailsWidget);
		DW->AddToViewport();
		ControllerIn->SetDetailsWidgetRef(DW);
	}
}

void ATP_ThirdPersonCharacter::SetGameStateRef(AVVGameState* GameStateIn)
{
	/*if (GameStateIn)
	{
		GameStateRef = GameStateIn;
		GameStateRef->OnDayChanged.AddDynamic(this, &ATP_ThirdPersonCharacter::CheckForDayChange);
		// Set the character stage to unborn so that it goes to baby when UpdateStageInfo is called 
		LocalStage = ECharStages::ECS_Unborn;
		UpdateStageInfo();
	}*/
}

void ATP_ThirdPersonCharacter::CheckForDayChange(FText DayAsText, int32 DayAsInt)
{
	// A day has passed.  Check if the character needs to age.
	DaysBeforeAging -= 1;
	if (DaysBeforeAging == 0)
	{
		UpdateStageInfo();
	}
	
}

void ATP_ThirdPersonCharacter::DragHasStarted()
{
	bIsBeingDragged = true;
}

void ATP_ThirdPersonCharacter::UpdateDraggedPosition()
{
	if (bIsBeingDragged)
	{
		
	}
}

void ATP_ThirdPersonCharacter::DragHasEnded()
{
	bIsBeingDragged = false;
}

FCharDetails ATP_ThirdPersonCharacter::GetCharDetails() const
{
	if (RenderCameraComp)
	{
		RenderCameraComp->Activate();
	}

	return CharDetails;
}

void ATP_ThirdPersonCharacter::DetailsWidgetClosed()
{
	if (RenderCameraComp)
	{
		RenderCameraComp->Deactivate();
	}
}

void ATP_ThirdPersonCharacter::UpdateStageInfo()
{
	if (CharacterDataTable)
	{
		FName RowName = FName(UEnum::GetDisplayValueAsText(LocalStage).ToString());
		// Only used if the Character has been created by a Game State call
		if (LocalStage == ECharStages::ECS_Unborn)
		{
			// Randomly decide if the baby is to be male or female
			//  TODO Update so that there is a rough balance of male to female in the world
			float RandomFloat = FMath::RandRange(0.f, 1.f);
			if (RandomFloat < .5f)
			{
				RowName = FName("BabyGirl");
			}
			else
			{
				RowName = FName("BabyBoy");
			}
		}
		
		FCharInfo* Row = CharacterDataTable->FindRow<FCharInfo>(RowName, "");

		if (Row)
		{
			LocalStage = Row->NextStage;
			if (LocalStage != ECharStages::ECS_Dead)
			{
				RowName = FName(UEnum::GetDisplayValueAsText(LocalStage).ToString());
				Row = CharacterDataTable->FindRow<FCharInfo>(RowName, "");
				if (Row)
				{
					DaysBeforeAging = Row->DaysToNextStage;
					AINeedsComp->CharacterNeedsUpdated(false, Row->FoodDrain, Row->WaterDrain, Row->HealAmount);
					
					GetMesh()->SetSkeletalMesh(Row->MeshToUse);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Failed to update character info in TP_ThirdPersonCharacter."));
				}
			}
			else
			{
				// The character has died, deal with this
				GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				GetMesh()->SetSimulatePhysics(true);

				AINeedsComp->CharacterNeedsUpdated(true, 0.f, 0.f, 0.f);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to get existing info for a character in TP_ThirdPersonCharacter."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Tried to update item info, but there is no valid data table in TP_ThirdPersonCharacter."));
	}
}

AActor* ATP_ThirdPersonCharacter::OnOverlapBegin_Implementation()
{
	IInteractInterface::OnOverlapBegin_Implementation();

	bCanBePickedUp = true;
	GetMesh()->SetRenderCustomDepth(true);
	
	return this;
}

void ATP_ThirdPersonCharacter::OnOverlapEnd_Implementation()
{
	IInteractInterface::OnOverlapEnd_Implementation();

	GetMesh()->SetRenderCustomDepth(false);
	bCanBePickedUp = false;
}

void ATP_ThirdPersonCharacter::GetItemInfo_Implementation(FText& ItemNameOut, FText& ItemDescOut)
{
	IInteractInterface::GetItemInfo_Implementation(ItemNameOut, ItemDescOut);

	FString NameString = CharDetails.FirstName.ToString() + " " + CharDetails.LastName.ToString();
	ItemNameOut = FText::FromString(NameString);
	ItemDescOut = FText::FromString("");
	 
}

bool ATP_ThirdPersonCharacter::GetCanBeDragged_Implementation(ATP_ThirdPersonCharacter*& Character)
{
	Character = this;
	return bCanBeDragged;
}
