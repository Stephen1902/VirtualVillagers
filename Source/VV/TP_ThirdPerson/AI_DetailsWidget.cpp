// Copyright 2025 DME Games


#include "AI_DetailsWidget.h"

#include "AI_Needs.h"
#include "TP_ThirdPersonCharacter.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "VV/Player/VVPlayerController.h"

void UAI_DetailsWidget::SetControllerRef(AVVPlayerController* ControllerIn)
{
	if (ControllerIn && !PlayerControllerRef)
	{
		PlayerControllerRef = ControllerIn;
		CloseButton->OnClicked.AddDynamic(this, &UAI_DetailsWidget::OnCloseButtonClicked);
	}
}

void UAI_DetailsWidget::SetActorBeenHit(AActor* ActorIn)
{
	FocusedCharacter = Cast<ATP_ThirdPersonCharacter>(ActorIn);
	if (FocusedCharacter)
	{
		const FCharDetails CharDetails = FocusedCharacter->GetCharDetails();

		FirstNameText->SetText(CharDetails.FirstName);
		LastNameText->SetText(CharDetails.LastName);

		if (FocusedCharacter->GetAINeeds())
		{
			FocusedCharacter->GetAINeeds()->OnNeedsUpdated.AddDynamic(this, &UAI_DetailsWidget::OnNeedsUpdated);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FocusedActor is not valid."));
	}
}

void UAI_DetailsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//CloseButton->OnClicked.AddDynamic(this, &UAI_DetailsWidget::OnCloseButtonClicked);
}

void UAI_DetailsWidget::OnCloseButtonClicked()
{
	if (FocusedCharacter)
	{
		FocusedCharacter->DetailsWidgetClosed();
	}
	
	if (PlayerControllerRef)
	{
		PlayerControllerRef->ShowDetailsWidget();
	}

	FocusedCharacter->GetAINeeds()->OnNeedsUpdated.RemoveDynamic(this, &UAI_DetailsWidget::OnNeedsUpdated);
}

void UAI_DetailsWidget::OnNeedsUpdated(float HealthIn, float FoodIn, float WaterIn)
{
	HealthProgressBar->SetPercent(HealthIn);
	FoodProgressBar->SetPercent(FoodIn);
	WaterProgressBar->SetPercent(WaterIn);
}
