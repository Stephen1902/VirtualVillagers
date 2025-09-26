// Copyright 2025 DME Games

#include "InteractiveActorBase.h"
#include "Components/SphereComponent.h"

// Sets default values
AInteractiveActorBase::AInteractiveActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	SetRootComponent(RootComp);

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Comp"));
	CollisionComp->SetSphereRadius(56.f);
	CollisionComp->SetupAttachment(RootComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetupAttachment(CollisionComp);
	MeshComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);

	ItemName = FText::FromString("Default Name");
	ItemDescription = FText::FromString("Default Description");

	bCanBeDragged = false;

}

// Called when the game starts or when spawned
void AInteractiveActorBase::BeginPlay()
{
	Super::BeginPlay();

}

AActor* AInteractiveActorBase::OnOverlapBegin_Implementation()
{
	IInteractInterface::OnOverlapBegin_Implementation();
	UE_LOG(LogTemp, Warning, TEXT("OnOverlapBegin called."));
	MeshComp->SetRenderCustomDepth(true);
	
	return this;
}

void AInteractiveActorBase::OnOverlapEnd_Implementation()
{
	IInteractInterface::OnOverlapEnd_Implementation();
	
	UE_LOG(LogTemp, Warning, TEXT("OnOverlapEnd called."));
	MeshComp->SetRenderCustomDepth(false);
}

void AInteractiveActorBase::GetItemInfo_Implementation(FText& ItemNameOut, FText& ItemDescOut)
{
	IInteractInterface::GetItemInfo_Implementation(ItemNameOut, ItemDescOut);

	ItemNameOut = ItemName;
	ItemDescOut = ItemDescription;
}

bool AInteractiveActorBase::GetCanBeDragged_Implementation()
{
	return bCanBeDragged;
}
