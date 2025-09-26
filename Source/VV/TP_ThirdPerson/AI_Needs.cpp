// Copyright 2025 DME Games

#include "AI_Needs.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UAI_Needs::UAI_Needs()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Set up default amount of time for timers to be called
	FoodDrainFrequency = 0.1f; // 10 times per second
	WaterDrainFrequency = 0.1f; // 10 times per second
	HealingFrequency = 0.1f;  // 10 times per second

	LocalFoodDrain = 0.f;
	LocalWaterDrain = 0.f;
	LocalHealing = 0.f;

	// Set maximum amount items can go up with eating, drinking etc.
	MaximumFood = 120.f;
	MaximumWater = 110.f;
	MaximumHealth = 100.f;

	CurrentFood = 100.f;
	CurrentWater = 100.f;
	CurrentHealth = 100.f;
	bIsDead = false;
}


// Called when the game starts
void UAI_Needs::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAI_Needs::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAI_Needs::CharacterNeedsUpdated(const bool IsDead, const float NewFoodDrain, const float NewWaterDrain, const float NewHealingRate)
{
	FTimerManager& TimerManager = GetOwner()->GetWorldTimerManager();
	// Check if the timers are running, clear them if they are
	if (TimerManager.IsTimerActive(FoodDrainTimer))
	{
		TimerManager.ClearTimer(FoodDrainTimer);	
	}

	if (TimerManager.IsTimerActive(WaterDrainTimer))
	{
		TimerManager.ClearTimer(WaterDrainTimer);	
	}
	
	if (TimerManager.IsTimerActive(HealthTimer))
	{
		TimerManager.ClearTimer(HealthTimer);	
	}

	// Check if the character has died
	if (IsDead)
	{
		bIsDead = true;
	}
	else
	{
		// The character is not dead.  Update the amount lost / gained and set the timers
		LocalFoodDrain = NewFoodDrain;
		LocalWaterDrain = NewWaterDrain;
		LocalHealing = NewHealingRate;
		
		TimerManager.SetTimer(FoodDrainTimer, this, &UAI_Needs::FoodDrainTimerEnded, FoodDrainFrequency, true, FoodDrainFrequency);
		TimerManager.SetTimer(WaterDrainTimer, this, &UAI_Needs::WaterDrainTimerEnded, WaterDrainFrequency, true, WaterDrainFrequency);
		TimerManager.SetTimer(HealthTimer, this, &UAI_Needs::HealingTimerEnded, HealingFrequency, true, HealingFrequency);
	}
}

void UAI_Needs::FoodDrainTimerEnded()
{
	CurrentFood = FMath::Clamp((CurrentFood - (LocalFoodDrain * FoodDrainFrequency)), 0.f, MaximumFood);

	//  TODO What to do if food gets too low
}

void UAI_Needs::WaterDrainTimerEnded()
{
	CurrentWater = FMath::Clamp(CurrentWater - (LocalWaterDrain * WaterDrainFrequency), 0.f, MaximumWater);

	// TODO What to do if water gets too low
}

void UAI_Needs::HealingTimerEnded()
{
	CurrentHealth = FMath::Clamp(CurrentHealth + (LocalHealing * HealingFrequency), 0.f, 100.f);
		
	// Check if the character's health is at maximum, clear the timer if it is
	if (CurrentHealth >= MaximumHealth)
	{
		CurrentHealth = MaximumHealth;
		if (GetWorld()->GetTimerManager().IsTimerActive(HealthTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(HealthTimer);
		}
	}
}
