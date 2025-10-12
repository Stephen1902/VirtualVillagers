// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AI_DetailsWidget.generated.h"

class UImage;

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

public:
	UImage* GetCharRender() const { return CharRender; }
};
