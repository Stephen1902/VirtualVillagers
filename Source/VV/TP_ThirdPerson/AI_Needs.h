// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AI_Needs.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VV_API UAI_Needs : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAI_Needs();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// How often the food drain timer should be called
	UPROPERTY(EditDefaultsOnly, Category = "Character Needs")
	float FoodDrainFrequency;

	UPROPERTY(EditDefaultsOnly, Category = "Character Needs")
	float WaterDrainFrequency;

	UPROPERTY(EditDefaultsOnly, Category = "Character Needs")
	float HealingFrequency;

	UPROPERTY(EditDefaultsOnly, Category = "Character Needs")
	float MaximumFood;

	UPROPERTY(EditDefaultsOnly, Category = "Character Needs")
	float MaximumWater;

	UPROPERTY(EditDefaultsOnly, Category = "Character Needs")
	float MaximumHealth;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void CharacterNeedsUpdated(const bool IsDead, const float NewFoodDrain, const float NewWaterDrain, const float NewHealingRate);
private:
	float CurrentFood;
	float CurrentWater;
	float CurrentHealth;

	float LocalFoodDrain;
	float LocalWaterDrain;
	float LocalHealing;

	FTimerHandle FoodDrainTimer;
	UFUNCTION()
	void FoodDrainTimerEnded();
	
	FTimerHandle WaterDrainTimer;
	UFUNCTION()
	void WaterDrainTimerEnded();

	FTimerHandle HealthTimer;
	UFUNCTION()
	void HealingTimerEnded();
			
	bool bIsDead;
};
