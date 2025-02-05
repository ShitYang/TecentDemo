// Fill out your copyright notice in the Description page of Project Settings.


#include "Flock.h"
#include "Math/MathFwd.h"
#include "FlockAgent.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AFlock::AFlock()
	: m_flockRadius(1000)
	, m_agentCount(30)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_motionRange = FBox(FVector(GetActorLocation().X - m_flockRadius, GetActorLocation().Y - m_flockRadius, GetActorLocation().Z - m_flockRadius)
	, FVector(GetActorLocation().X + m_flockRadius, GetActorLocation().Y + m_flockRadius, GetActorLocation().Z + m_flockRadius));
	// UE_LOG(LogTemp, Warning, TEXT("FVector: %s"), *GetActorLocation().ToString());
	m_pStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = m_pStaticMesh;
}

const FBox& AFlock::GetMotionRange() const
{
	return m_motionRange;
}

// Called when the game starts or when spawned
void AFlock::BeginPlay()
{
	Super::BeginPlay();
	
	DrawDebugBox(GetWorld(), GetActorLocation(), FVector(m_flockRadius, m_flockRadius, m_flockRadius), FColor::Red, true);
	for (int i = 0; i < m_agentCount; ++i) {
	 	AFlockAgent *flockAgent = GetWorld()->SpawnActor<AFlockAgent>(m_flockAgentClass, FMath::RandPointInBox(m_motionRange), FRotator::ZeroRotator);
		flockAgent->SetUpFlock(this);
		m_flockAgentArray.Add(flockAgent);
	}
}

// Called every frame
void AFlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}