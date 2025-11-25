// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "VV/Framework/InteractInterface.h"
#include "TP_ThirdPersonCharacter.generated.h"

class AVVGameState;

/** Default base class for all AI Characters in the game */
UENUM(BlueprintType)
enum class ECharStages : uint8 
{
	ECS_Unborn			UMETA(DisplayName="UnbornChild"),
	ECS_BabyGirl		UMETA(DisplayName="BabyGirl"),
	ECS_BabyBoy			UMETA(DisplayName="BabyBoy"),
	ECS_ChildFemale		UMETA(DisplayName="FemaleChild"),
	ECS_ChildMale		UMETA(DisplayName="MaleChild"),
	ECS_YoungFemale		UMETA(DisplayName="FemaleYoungAdult"),
	ECS_YoungMale		UMETA(DisplayName="MaleYoungAdult"),
	ECS_AdultFemale		UMETA(DisplayName="AdultFemale"),
	ECS_AdultMale		UMETA(DisplayName="AdultMale"),
	ECS_ElderFemale		UMETA(DisplayName="ElderFemale"),
	ECS_ElderMale		UMETA(DisplayName="ElderMale"),
	ECS_Dead			UMETA(DisplayName="Dead")
};

USTRUCT(BlueprintType)
struct FCharInfo : public  FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ECharStages CurrentStage;

	// Skeletal Mesh to show the character on screen
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMesh* MeshToUse;

	//  The number of days that must pass before this character ages
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 DaysToNextStage;

	// The next stage this character goes to when "aging"
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ECharStages NextStage;

	// Amount of food drain per second
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float FoodDrain;

	// Amount of water drain per second
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float WaterDrain;

	// Amount of healing per second
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float HealAmount;
};

USTRUCT(BlueprintType)
struct FCharDetails
{
	GENERATED_BODY()

	// ID of this character.  Used to keep track of marriages, children etc.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Details")
	int32 CharID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Details")
	FText FirstName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Details")
	FText LastName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Details")
	int32 Age;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Details")
	int32 SpouseID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Details")
	TArray<int32> ChildrenID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Details")
	int32 Birthday;
};

class USpringArmComponent;
class UCameraComponent;
	
UCLASS(config=Game)
class ATP_ThirdPersonCharacter : public ACharacter, public IInteractInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** Scene component for the render camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USceneComponent* RenderSceneComp;

	/** Render camera component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USceneCaptureComponent2D* RenderCameraComp; 

protected:
	// Called at game start
	virtual void BeginPlay() override;
	
	// Health this character has at the start of the game and the maximum it can have
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI Stats")
	int32 StartingHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Char")
	class UAI_Needs* AINeedsComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Char")
	FCharDetails CharDetails;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Char")
	ECharStages LocalStage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Char")
	bool bCanBeDragged;

	// Information widget to display for this AI Character
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Char")
	TSubclassOf<class UAI_DetailsWidget> DetailsWidget;
public:
	ATP_ThirdPersonCharacter();

	/** Returns CameraBoom subobject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	//void AddWidget(class AVVPlayerController* ControllerIn);
	
	UPROPERTY()
	AVVGameState* GameStateRef;
	void SetGameStateRef(AVVGameState* GameStateIn);

	UFUNCTION()
	void CheckForDayChange(FText DayAsText, int32 DayAsInt);

	void DragHasStarted();
	void UpdateDraggedPosition();
	void DragHasEnded();

	
	FCharDetails GetCharDetails() const;
	void DetailsWidgetClosed();
private:
	UPROPERTY()
	UDataTable* CharacterDataTable;
	int32 DaysBeforeAging;

	void UpdateStageInfo();

	bool bCanBePickedUp;
	bool bIsBeingDragged;

	virtual AActor* OnOverlapBegin_Implementation() override;
	virtual void OnOverlapEnd_Implementation() override;
	virtual void GetItemInfo_Implementation(FText& ItemNameOut, FText& ItemDescOut) override;
	virtual bool GetCanBeDragged_Implementation(ATP_ThirdPersonCharacter*& Character) override;
};

