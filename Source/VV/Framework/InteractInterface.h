// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VV_API IInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	AActor* OnOverlapBegin();
	UFUNCTION(BlueprintNativeEvent)
	void OnOverlapEnd();
	UFUNCTION(BlueprintNativeEvent)
	void OnInteractBegin();
	UFUNCTION(BlueprintNativeEvent)
	void GetItemInfo(FText& ItemNameOut, FText& ItemDescOut); 
	UFUNCTION(BlueprintNativeEvent)
	bool GetCanBeDragged();
};
