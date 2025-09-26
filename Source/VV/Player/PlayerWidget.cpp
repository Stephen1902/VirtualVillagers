// Copyright 2025 DME Games

#include "PlayerWidget.h"
#include "Components/TextBlock.h"
#include "../Framework/VVGameState.h"

void UPlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!GameStateRef)
	{
		GameStateRef = Cast<AVVGameState>(GetWorld()->GetGameState());

		if (GameStateRef)
		{
			GameStateRef->OnTimeChanged.AddDynamic(this, &UPlayerWidget::OnTimeChangeReceived);
			GameStateRef->OnDayChanged.AddDynamic(this, &UPlayerWidget::OnDayChangeReceived);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to get game state in player widget."));
		}
	}
}

void UPlayerWidget::OnTimeChangeReceived(FText Minutes, FText Hours, float Clockwork)
{
	MinuteText->SetText(Minutes);
	HourText->SetText(Hours);
}

void UPlayerWidget::OnDayChangeReceived(FText DayAtText, int32 DayAsInt)
{
	DayText->SetText(DayAtText);
}

void UPlayerWidget::SetItemInfo(FText NameTextIn, FText DescriptionTextIn)
{
	ItemName->SetText(NameTextIn);
	ItemDescription->SetText(DescriptionTextIn);
}
