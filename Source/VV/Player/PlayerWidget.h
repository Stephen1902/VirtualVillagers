// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"

class UTextBlock;
class UButton;

/**
 * 
 */
UCLASS()
class VV_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// Called on Game State
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Widget", META=(BindWidget))
	UTextBlock* DayText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Widget", META=(BindWidget))
	UTextBlock* HourText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Widget", META=(BindWidget))
	UTextBlock* MinuteText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Widget", META=(BindWidget))
	UButton* PauseButton;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Widget", META=(BindWidget))
	UButton* SpeedDownButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Widget", META=(BindWidget))
	UButton* SpeedUpButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Widget", META=(BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Widget", META=(BindWidget))
	UTextBlock* ItemDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Widget", META=(BindWidget))
	UButton* DetailsButton;
private:
	UPROPERTY()
	class AVVGameState* GameStateRef;

	UFUNCTION()
	void OnTimeChangeReceived(FText Minutes, FText Hours, float Clockwork);

	UFUNCTION()
	void OnDayChangeReceived(FText DayAtText, int32 DayAsInt);

	UFUNCTION()
	void OnDetailsButtonClicked();


public:
	void SetItemInfo(FText NameTextIn, FText DescriptionTextIn);

	void SetDetailsButtonStatus(const bool StatusIn);
};
