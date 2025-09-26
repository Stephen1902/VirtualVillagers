// Copyright 2025 DME Games


#include "TestActor.h"

// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	SetRootComponent(RootComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetupAttachment(RootComp);

	FakeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Fake Mesh"));
	FakeMesh->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
		
	float LastRenderTimeOnScreen = FakeMesh->GetLastRenderTimeOnScreen();
	FString StringToDisplay = FString::SanitizeFloat(LastRenderTimeOnScreen);
	GEngine->AddOnScreenDebugMessage(0, 0.f, FColor::Green, *StringToDisplay);
	
	float TimeSinceRendered = GetWorld()->GetTimeSeconds() - LastRenderTimeOnScreen;

	
	
	if (LastRenderTimeOnScreen < 0.f || TimeSinceRendered >= 0.2f)
	{
		//if (MeshComp->bRenderCustomDepth == false)
     		{
     			MeshComp->SetRenderCustomDepth(false);
     			//MeshComp->SetRenderCustomDepth(true);
     		}
		//if (MeshComp->bRenderCustomDepth == true)
		{
			
		}
	}
	else
	{
		MeshComp->SetRenderCustomDepth(true);
	}
	
}

