// Copyright 2025 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AI_Skills.generated.h"

USTRUCT(BlueprintType)
struct FSkillInfo
{
	GENERATED_BODY();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float FarmingSkill;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float GatheringSkill;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BuildingSkill;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ResearchSkill;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float HealingSkill;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BreedingSkill;

	FSkillInfo()
	{
		FarmingSkill = 0.f;
		GatheringSkill = 0.f;
		BuildingSkill = 0.f;
		ResearchSkill = 0.f;
		HealingSkill = 0.f;
		BreedingSkill = 0.f;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VV_API UAI_Skills : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAI_Skills();

	float GetSkillLevel(FName SkillToGet);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
