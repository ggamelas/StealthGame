// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSBlackHole.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	InnerSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("InnerSphereComp"));
	InnerSphereComp->SetSphereRadius(100);
	InnerSphereComp->SetupAttachment(MeshComp);

	InnerSphereComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::OverlapInnerSphere);

	AttractiveSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("AttractiveSphereComp"));
	AttractiveSphereComp->SetSphereRadius(1000);
	AttractiveSphereComp->SetupAttachment(MeshComp);
}

// Called when the game starts or when spawned
void AFPSBlackHole::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSBlackHole::OverlapInnerSphere(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
		OtherActor->Destroy();
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float strengthRadialForce = -2000;

	TArray<UPrimitiveComponent*> OverlappingComps;
	AttractiveSphereComp->GetOverlappingComponents(OverlappingComps);

	for (UPrimitiveComponent* PrimComp : OverlappingComps)
		if (PrimComp && PrimComp->IsSimulatingPhysics())
			PrimComp->AddRadialForce(GetActorLocation(), AttractiveSphereComp->GetScaledSphereRadius(), strengthRadialForce, ERadialImpulseFalloff::RIF_Constant, true);
}

