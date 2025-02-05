// Fill out your copyright notice in the Description page of Project Settings.


#include "FlockAgent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Flock.h"
#include "absl/container/internal/inlined_vector.h"

// Sets default values
AFlockAgent::AFlockAgent()
	: m_cohesionRadius(400)
	, m_avoidanceRadius(100)
	, m_rotateSpeed(10)
	, m_moveSpeed(100)
	, m_cohesionFactor(1)
 	, m_avoidanceFactor(1)
	, m_alignmentFactor(1)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	m_pBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	RootComponent = m_pBoxComponent;
	
	m_pStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	m_pStaticMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFlockAgent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFlockAgent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveTo();
}

void AFlockAgent::MoveTo()
{
	const FVector direction = CalculateDirection();
	this->SetActorRotation(FMath::Lerp(GetActorForwardVector(), direction, m_rotateSpeed * GetWorld()->GetDeltaSeconds()).Rotation());
	this->SetActorLocation(GetActorLocation() + GetActorForwardVector() * m_moveSpeed * GetWorld()->GetDeltaSeconds());
}

void AFlockAgent::SetUpFlock(AFlock* belongFlock)
{
	m_pFlock = belongFlock;
}


FVector AFlockAgent::CalculateCohesionDirection(const TArray<FHitResult> &traceResults) const
{
	if (traceResults.Num() == 0)
	{
		return FVector::Zero();
	}
	
	FVector movePosition = FVector::Zero();
	for (const auto &traceResult: traceResults)
	{
		movePosition += traceResult.GetActor()->GetActorLocation();
	}

	return movePosition / traceResults.Num() - GetActorLocation();
}

FVector AFlockAgent::CalculateAlignmentDirection(const TArray<FHitResult> &traceResults) const
{
	if (traceResults.Num() == 0)
	{
		return FVector::Zero();
	}
	
	FVector moveDirection = FVector::Zero();
	for (const auto &traceResult: traceResults)
	{
		moveDirection += traceResult.GetActor()->GetActorForwardVector();
	}
	
	return moveDirection / traceResults.Num();
}

FVector AFlockAgent::CalculateAvoidanceDirection(const TArray<FHitResult> &traceResults) const
{
	if (traceResults.Num() == 0)
	{
		return FVector::Zero();
	}

	int avoidCount = 0;
	FVector moveDirection = FVector::Zero();
	for (const auto &traceResult: traceResults)
	{
		if (GetDistanceTo(traceResult.GetActor()) <= m_avoidanceRadius) {
			avoidCount++;
			moveDirection += (GetActorLocation() - traceResult.GetActor()->GetActorLocation());
		}
	}

	if (avoidCount == 0)
	{
		return FVector::Zero();
	}
	
	return moveDirection / avoidCount;
}

FVector AFlockAgent::CalculateLimitedMotionDirection() const
{
	if (m_pFlock->GetMotionRange().IsInside(GetActorLocation())) {
		return FVector::Zero();
	}
	return m_pFlock->GetActorLocation() - GetActorLocation();
}

/*
 *	Return Normalised Vector As Direction
*/
FVector AFlockAgent::CalculateDirection() const
{
	TArray<FHitResult> traceResults;
	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation()
		, m_cohesionRadius, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, TArray<AActor*>(), EDrawDebugTrace::None, traceResults, true);
	FVector cohesionDirection = CalculateCohesionDirection(traceResults) * m_cohesionFactor,
				  alignmentDirection = CalculateAlignmentDirection(traceResults) * m_alignmentFactor,
				  avoidanceDirection = CalculateAvoidanceDirection(traceResults) * m_avoidanceFactor,
				  limitedMotionDirection = CalculateLimitedMotionDirection() * m_limitedMotionFactor;

	if (!cohesionDirection.IsNearlyZero() && cohesionDirection.Length() > m_cohesionFactor)
	{
		cohesionDirection = cohesionDirection.GetSafeNormal() * m_cohesionFactor;		
	}

	if (!alignmentDirection.IsNearlyZero() && alignmentDirection.Length() > m_alignmentFactor)
	{
		alignmentDirection = alignmentDirection.GetSafeNormal() * m_alignmentFactor;
	}

	if (!avoidanceDirection.IsNearlyZero() && avoidanceDirection.Length() > m_avoidanceFactor)
	{
		avoidanceDirection = avoidanceDirection.GetSafeNormal() * m_avoidanceFactor;
	}

	if (!limitedMotionDirection.IsNearlyZero() && limitedMotionDirection.Length() > m_limitedMotionFactor)
	{
		limitedMotionDirection = limitedMotionDirection.GetSafeNormal() * m_limitedMotionFactor;
	}
	
	return cohesionDirection + alignmentDirection + avoidanceDirection + limitedMotionDirection;
}


