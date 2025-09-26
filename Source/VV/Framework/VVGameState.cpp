// Copyright 2025 DME Games


#include "VVGameState.h"
#include "Kismet/KismetMathLibrary.h"
#include "VV/TP_ThirdPerson/TP_ThirdPersonCharacter.h"

AVVGameState::AVVGameState()
{
	PrimaryActorTick.bCanEverTick = true;

	Seconds = 0;
	Minutes = 30;
	Hours = 0;

	Day = 1;

	// 150 Time unit is 1 day in 1 real hour
	TimeUnit = 75.0f;
	Clockwork = 0.f;
	DayTick = 0;
}

void AVVGameState::BeginPlay()
{
	Super::BeginPlay();

	Clockwork = Hours + (Minutes / 60.f);
	
	GameTime.Add(Seconds);
	GameTime.Add(Minutes);
	GameTime.Add(Hours);
}

void AVVGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Check if the day has ended
	const float ClockworkThisTick = (DeltaSeconds / TimeUnit) + Clockwork;
	DayTick = UKismetMathLibrary::FMod(ClockworkThisTick, 24.f, Clockwork);

	if (DayTick > 0)
	{
		Day += 1;
		OnDayChange();
	}

	SetClock();
	
}

void AVVGameState::SetClock()
{
	const int32 MinutesAtStart = Minutes;
	
	// Set the time; There are 3600 seconds in an hour.
	const float ClockworkPerHour = Clockwork * 3600.f;
	float SecondsAsFloat;
	int32 MinutesAsInt = UKismetMathLibrary::FMod(ClockworkPerHour, 60.f, SecondsAsFloat);
	Seconds = FMath::Floor(SecondsAsFloat);
	GameTime[0] = Seconds;

	float MinutesAsFloat;
	int32 HoursAsInt = UKismetMathLibrary::FMod(MinutesAsInt, 60.f, MinutesAsFloat);
	Minutes = FMath::Floor(MinutesAsFloat);
	GameTime[1] = Minutes;

	float HoursAsFloat;
	UKismetMathLibrary::FMod(HoursAsInt, 24.f, HoursAsFloat);
	Hours = FMath::Floor(HoursAsFloat);
	GameTime[2] = Hours;

	// Check if the minutes value has changed.  If it has, update the widget information
	if (Minutes != MinutesAtStart)
	{
		OnTimeChange();
	}
	
	//const FString Message = "Day: " + FString::FromInt(Day)  + FString::FromInt(Hours) + ":" + FString::FromInt(Minutes) + ":" + FString::FromInt(Seconds);
	//GEngine->AddOnScreenDebugMessage(0, 0.1f, FColor::Green, *Message);
}

void AVVGameState::OnTimeChange() const
{
	FString MinutesAsString = "0";
	if (Minutes < 10)
	{
		MinutesAsString += FString::FromInt(Minutes);
	}
	else
	{
		MinutesAsString = FString::FromInt(Minutes);
	}
	
	FString HoursAsString = "0";
	if (Hours < 10)
	{
		HoursAsString += FString::FromInt(Hours);
	}
	else
	{
		HoursAsString = FString::FromInt(Hours);
	}

	
	OnTimeChanged.Broadcast(FText::FromString(MinutesAsString), FText::FromString(HoursAsString), (((Clockwork * 4.167f) / 100.f) * 360.f) + 90.f);
}

void AVVGameState::OnDayChange() const
{
	FString DayAsString = FString::FromInt(Day);
	OnDayChanged.Broadcast(FText::FromString(DayAsString), Day);
	UE_LOG(LogTemp, Warning, TEXT("OnDayChange called by GameState"));
}

void AVVGameState::SpawnNewAI()
{
	if (AICharToSpawn)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(FVector(0.f));
		// TODO Sort Location for spawning
		ATP_ThirdPersonCharacter* NewAIChar = GetWorld()->SpawnActor<ATP_ThirdPersonCharacter>(AICharToSpawn, FVector(0.f), FRotator::ZeroRotator, SpawnParameters);
		
	}
}

