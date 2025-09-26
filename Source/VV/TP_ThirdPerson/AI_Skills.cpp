// Copyright 2025 DME Games


#include "AI_Skills.h"

template<typename T>
T* GetPropertyValueWithinStruct(UObject* Target, const FName& StructPropertyName, const FName& PropertyNameWithinStruct)
{
	// Get the reflected struct property
	FStructProperty* StructProp = (FStructProperty*)Target->GetClass()->FindPropertyByName(StructPropertyName);

	// Get a pointer to the struct instance
	void* StructPtr = StructProp->ContainerPtrToValuePtr<void>(Target);

	// Get the struct reflection data
	UScriptStruct* StructReflectionData = StructProp->Struct;

	// Get the reflected property within struct
	FProperty* PropertyWithinStruct = StructReflectionData->FindPropertyByName(PropertyNameWithinStruct);

	// Get value of property within struct
	return PropertyWithinStruct->ContainerPtrToValuePtr<T>(StructPtr);
}

// Sets default values for this component's properties
UAI_Skills::UAI_Skills()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

float UAI_Skills::GetSkillLevel(FName SkillToGet)
{
	if (const int32* ValuePtr = GetPropertyValueWithinStruct<int32>(this, "FSkillInfo", SkillToGet))
	{
		UE_LOG(LogTemp, Log, TEXT("Result: %i"), *ValuePtr);
		return *ValuePtr;
	}

	return 0.f;
}


// Called when the game starts
void UAI_Skills::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAI_Skills::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

