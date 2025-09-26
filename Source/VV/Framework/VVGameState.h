// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "VVGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTimeChange, FText, NewMinute, FText, NewHour, float, RemaingClockwork);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDayChange, FText, NewDayAsText, int32, NewDayAsInt);


/**
 * 
 */
UCLASS()
class VV_API AVVGameState : public AGameStateBase
{
	GENERATED_BODY()

	AVVGameState();
protected:
	// Called at Game Start
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
	
	// Minute for the game to be at on Begin Play
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Date Time")
	int32 Minutes;

	// Hour for the game to be at on Begin Play
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Date Time")
	int32 Hours;

	// Day for game to be at on Begin Play
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Date Time")
	int32 Day;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI Details")
	TSubclassOf<class ATP_ThirdPersonCharacter> AICharToSpawn;

public:
	UPROPERTY(BlueprintAssignable)
	FOnTimeChange OnTimeChanged;

	UPROPERTY(BlueprintAssignable)
	FOnDayChange OnDayChanged;
private:
	float TimeUnit;
	float Clockwork;
	int32 Seconds;
	TArray<int32> GameTime;
	int32 DayTick;

	void SetClock();
	void OnTimeChange() const;
	void OnDayChange() const;

	void SpawnNewAI();
};
