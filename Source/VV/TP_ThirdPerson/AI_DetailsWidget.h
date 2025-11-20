// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AI_DetailsWidget.generated.h"

class UImage;
class UButton;
class UTextBlock;
class AVVPlayerController;
class ATP_ThirdPersonCharacter;

/**
 * 
 */
UCLASS()
class VV_API UAI_DetailsWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI Widget")
	UImage* CharRender;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI Widget", meta=(BindWidget))
	UButton* CloseButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI Widget", meta=(BindWidget))
	UTextBlock* FirstNameText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI Widget", meta=(BindWidget))
	UTextBlock* LastNameText;
public:
	UImage* GetCharRender() const { return CharRender; }

	void SetControllerRef(AVVPlayerController* ControllerIn);
	void SetActorBeenHit(AActor* ActorIn);

private:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnCloseButtonClicked();

	UPROPERTY()
	AVVPlayerController* PlayerControllerRef;

	UPROPERTY()
	ATP_ThirdPersonCharacter* FocusedCharacter;
};
